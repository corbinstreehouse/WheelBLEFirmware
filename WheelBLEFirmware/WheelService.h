//
//  WheelService.hpp
//  ArudinoBLETest
//
//  Created by Corbin Dunn on 12/22/15 .
//  Copyright © 2015 Corbin Dunn. All rights reserved.
//

#ifndef WheelService_hpp
#define WheelService_hpp

#include "Arduino.h" // For Serial only! I should get rid of that use... need to figure out how to print to the console otherwise

#include <BLE_API.h>
#include <UARTService.h>


class WheelUARTService : public UARTService {
public:
    WheelUARTService(BLE &ble) : UARTService(ble) { m_dataLeft = 0; }
    int available() {
        if (numBytesReceived > 0) {
            return numBytesReceived - receiveBufferIndex;
        } else {
            return 0;
        }
    }
protected:
    virtual void handleDataWritten() override;
private:
    int m_dataLeft; // TODO: maybe a reset method in case we disconnect..

};


class WheelService {
public:
    static const uint8_t UUID_WHEEL_SERVICE[UUID::LENGTH_OF_LONG_UUID];
    static const uint8_t UUID_WHEEL_SERVICE_REVERSED[UUID::LENGTH_OF_LONG_UUID];
    
    static const uint8_t UUID_CHAR_SEND_COMMAND[UUID::LENGTH_OF_LONG_UUID];
public:
    WheelService(BLE &ble) :
        m_ble(ble),
        m_command(-1),
        m_sendCommandCharacteristic(UUID_CHAR_SEND_COMMAND, &m_command),
        m_uartService(ble)
    {
        GattCharacteristic *charTable[] = {&m_sendCommandCharacteristic};
        GattService wheelService(UUID_WHEEL_SERVICE, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
        m_ble.addService(wheelService);
        
        // registered after the UART service so we know it has the data
        m_ble.onDataWritten(this, &WheelService::onDataWritten);
    }
    
private:
    BLE &m_ble;
    
    WriteOnlyGattCharacteristic<uint16_t> m_sendCommandCharacteristic;
    uint16_t m_command;
    
    WheelUARTService m_uartService;
            
private:
    void onDataWritten(const GattWriteCallbackParams *params);
};




#endif /* WheelService_hpp */
