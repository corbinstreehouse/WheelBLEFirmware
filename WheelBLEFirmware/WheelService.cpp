//
//  WheelService.cpp
//  ArudinoBLETest
//
//  Created by Corbin Dunn on 12/22/15 .
//  Copyright © 2015 Corbin Dunn. All rights reserved.
//

#include "WheelService.h"
#include "CDWheelBluetoothShared.h"
#include "SharedSettings.h"

const uint8_t WheelService::UUID_WHEEL_SERVICE[UUID::LENGTH_OF_LONG_UUID] = kLEDWheelServiceUUID_BYTE_ARRAY;
const uint8_t WheelService::UUID_CHAR_SEND_COMMAND[UUID::LENGTH_OF_LONG_UUID] = kLEDWheelCharSendCommandUUID_BYTE_ARRAY;


void WheelService::onDataWritten(const GattWriteCallbackParams *params) {
    DEBUG_PRINTLN("WheelService:OndataWritten");
    if (params->handle == m_uartService.getTXCharacteristicHandle()) {
        DEBUG_PRINTLN("     WheelService:OndataWritten..for TX char");
        while (m_uartService.available()) {
            char c = m_uartService._getc();
            Serial.print(c);
        }
    }
}