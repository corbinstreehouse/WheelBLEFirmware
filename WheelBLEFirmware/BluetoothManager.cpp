//
//  BluetoothManager.cpp
//  ArudinoBLETest
//
//  Created by Corbin Dunn on 12/22/15 .
//  Copyright © 2015 Corbin Dunn. All rights reserved.
//

#include "BluetoothManager.h"
#include "SharedSettings.h"

#define SHORT_NAME "CyrWhl"
#define LONG_NAME "LED Cyr Wheel RB"


BluetoothManager *g_currentManager = NULL;



void connectionCallback(const Gap::ConnectionCallbackParams_t *params) {
    Serial.println("connected ");

}

void disconnectionCallBack(Gap::Handle_t handle, Gap::DisconnectionReason_t reason)
{
    Serial.println("Disconnected ");
    Serial.println("Restart advertising ");
    g_currentManager->startAdvertising();
}

// GATT call back handle
void onDataWrittenHandler(const GattWriteCallbackParams *handler)
{
//    uint8_t buf[TXRX_BUF_LEN];
//    uint16_t bytesRead, index;
    
//    if (handler->handle == characteristic1.getValueAttribute().getHandle())
//    {
//        ble.readCharacteristicValue(characteristic1.getValueAttribute().getHandle(), buf, &bytesRead);
//        for(byte index=0; index<bytesRead; index++)
//        {
//            Serial.print(buf[index], HEX);
//        }
//        Serial.println(" ");
//    }
}

void onTimeoutCallback(Gap::TimeoutSource_t source) {
    DEBUG_PRINTF("timeout, source: %d\r\n", source);
//    char buffer[1024];
//    sprintf(buffer, "timeout, source: %d\r\n", source);
//    Serial.print(buffer);
}

BluetoothManager::BluetoothManager() :
    m_wheelService(NULL)
{
    // DON't put logs here! it will kill stuff..
}

static inline uint32_t msec_to_1_25msec(uint32_t interval_ms)
{
    return (interval_ms * 4) / 5;
}


void BluetoothManager::setup() {
    g_currentManager = this;
    
#if DEBUG
    int led2 = 13;
    pinMode(led2, OUTPUT);
    digitalWrite(led2, LOW);   // low turns it on? whatever.. it is on for debugging.
    
    DEBUG_PRINTLN("setup.....");
#endif
    
    
    m_ble.init();

    // High performance params (these are wrong..I need to figure them out better!)
    Gap::ConnectionParams_t connectionParams;
    // NOTE: these connection params are MUCH faster!! 577ms to transfer 1k, vs 10s
    connectionParams.minConnectionInterval = msec_to_1_25msec(20); // in 1.25 ms units
    connectionParams.maxConnectionInterval = msec_to_1_25msec(40); // in 1.25 ms units
    connectionParams.connectionSupervisionTimeout = 500 / 10; // 500ms to 10ms units
    connectionParams.slaveLatency = 0;
    m_ble.gap().setPreferredConnectionParams(&connectionParams);

    // Setup our handlers for connections
    m_ble.onConnection(connectionCallback);
    m_ble.gap().onDisconnection(disconnectionCallBack);
    
    // attribute server handlers
    m_ble.onDataWritten(onDataWrittenHandler);
    
    m_ble.gap().onTimeout(onTimeoutCallback);
    
    // setup adv_data and srp_data

    // NOTE: limited space! 29 bytes..I think. The response can have more data
    
    m_ble.accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED);
    
    // 16 bytes
    m_ble.accumulateAdvertisingPayload(GapAdvertisingData::INCOMPLETE_LIST_128BIT_SERVICE_IDS, (const uint8_t *)WheelService::UUID_WHEEL_SERVICE_REVERSED, sizeof(WheelService::UUID_WHEEL_SERVICE_REVERSED));

    // 13 bytes (default).. if it is longer, i'm not sure what i should do or deal with
    m_ble.accumulateScanResponse(GapAdvertisingData::COMPLETE_LOCAL_NAME,                            (const uint8_t *)LONG_NAME, sizeof(LONG_NAME) - 1);

    // 6 bytes
//    m_ble.accumulateAdvertisingPayload(GapAdvertisingData::SHORTENED_LOCAL_NAME, (const uint8_t *)SHORT_NAME, sizeof(SHORT_NAME) - 1);
    
    
    // NOTE: longest length: BLE_GAP_DEVNAME_MAX_LEN // 31
    m_ble.setDeviceName((uint8_t*)LONG_NAME);
    
    // set adv_type
    m_ble.setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    
    DEBUG_PRINTLN(" creating wheel service: ");
    
    // create our services
    m_wheelService = new WheelService(m_ble); // TODO: free?
    
    // set tx power,valid values are -40, -20, -16, -12, -8, -4, 0, 4
    m_ble.setTxPower(4);
    
    // set adv_interval, 100ms in multiples of 0.625ms.
//    m_ble.setAdvertisingInterval(160); // 20ms is min. 160 was "default".
    m_ble.gap().setAdvertisingInterval(20); // 20 is the min!
    
    // slave latency to 0
    
    
    // set adv_timeout, in seconds; we want to timeout really fast so I can re-connect later (I think..)
    m_ble.setAdvertisingTimeout(0);

    
    // ger BLE stack version
#if DEBUG
    DEBUG_PRINTLN(" BLE version: ");
    DEBUG_PRINTLN( m_ble.getVersion() );
#endif
}

void BluetoothManager::loop() {
    m_ble.waitForEvent();
}