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

void WheelUARTService::handleDataWritten() {
    
//    DEBUG_PRINTLN("     WheelUARTService..for TX char");
    while (available()) {
        char c = _getc();
        Serial.print(c);
    }
}

const uint8_t WheelService::UUID_WHEEL_SERVICE[UUID::LENGTH_OF_LONG_UUID] = kLEDWheelServiceUUID_BYTE_ARRAY;
const uint8_t WheelService::UUID_CHAR_SEND_COMMAND[UUID::LENGTH_OF_LONG_UUID] = kLEDWheelCharSendCommandUUID_BYTE_ARRAY;



void WheelService::onDataWritten(const GattWriteCallbackParams *params) {
//    DEBUG_PRINTLN("XX WheelService:OndataWritten");
}