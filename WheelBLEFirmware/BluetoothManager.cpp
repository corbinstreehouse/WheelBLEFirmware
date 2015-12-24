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
#define LONG_NAME "LED Cyr Wheel"


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


void BluetoothManager::setup() {
    g_currentManager = this;
    
#if DEBUG
    int led2 = 13;
    pinMode(led2, OUTPUT);
    digitalWrite(led2, LOW);   // low turns it on? whatever.. it is on for debugging.
    
    DEBUG_PRINTLN("setup.....");
#endif
    
    
    m_ble.init();
    
    // Setup our handlers for connections
    m_ble.onConnection(connectionCallback);
    m_ble.gap().onDisconnection(disconnectionCallBack);
    
    // attribute server handlers
    m_ble.onDataWritten(onDataWrittenHandler);
    
    m_ble.gap().onTimeout(onTimeoutCallback);
    
    // setup adv_data and srp_data
    m_ble.accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED);
//    m_ble.accumulateAdvertisingPayload(GapAdvertisingData::SHORTENED_LOCAL_NAME, (const uint8_t *)SHORT_NAME, sizeof(SHORT_NAME) - 1);
//    m_ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME,                            (const uint8_t *)"LED Cyr Wheel", sizeof("LED Cyr Wheel") - 1);
    
    m_ble.accumulateAdvertisingPayload(GapAdvertisingData::INCOMPLETE_LIST_128BIT_SERVICE_IDS, WheelService::UUID_WHEEL_SERVICE, sizeof(WheelService::UUID_WHEEL_SERVICE));
    
//    m_ble.accumulateScanResponse(GapAdvertisingData::SHORTENED_LOCAL_NAME, (const uint8_t *)SHORT_NAME, sizeof(SHORT_NAME) - 1);
    m_ble.accumulateScanResponse(GapAdvertisingData::INCOMPLETE_LIST_128BIT_SERVICE_IDS, (const uint8_t *)WheelService::UUID_WHEEL_SERVICE, sizeof(WheelService::UUID_WHEEL_SERVICE));
    
    
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
    
    
    // set adv_timeout, in seconds
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