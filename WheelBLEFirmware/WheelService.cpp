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

uint32_t start;
int count = 0;

void WheelUARTService::handleDataWritten() {
    
    if (m_dataLeft <= 0) {
        DEBUG_PRINTLN("Starting to get data...");
        // start over!
        // read the size
        int32_t totalBytes = 0;
        // TODO: better checking
        char c1 = _getc();
        char c2 = _getc();
        char c3 = _getc();
        char c4 = _getc();
        // reassemble... TOOD: make a method to read an int32
        totalBytes = c1 | (c2 << 8) | (c3 << 16) | (c4 << 24);
        DEBUG_PRINTF("bytes: %x, %x, %x, %x\r\n", c1, c2, c3, c4);;
        DEBUG_PRINTF("total bytes: %d\r\n", totalBytes);
        m_dataLeft = totalBytes;
        start = millis();
        count = 0;
    }

    while (available()) {
        char c = _getc();
//        Serial.print(c);
        m_dataLeft--;
        count++;
    }
    if (m_dataLeft == 0) {
        // Done
        uint32_t time = millis() - start;
        DEBUG_PRINTF("  read %d, time took: %d ms, %g s\r\n", count, time, time /1000.0 );
    }
}

const uint8_t WheelService::UUID_WHEEL_SERVICE[UUID::LENGTH_OF_LONG_UUID] = kLEDWheelServiceUUID_BYTE_ARRAY;
const uint8_t WheelService::UUID_CHAR_SEND_COMMAND[UUID::LENGTH_OF_LONG_UUID] = kLEDWheelCharSendCommandUUID_BYTE_ARRAY;



void WheelService::onDataWritten(const GattWriteCallbackParams *params) {
//    DEBUG_PRINTLN("XX WheelService:OndataWritten");
    
}