//
//  SharedSettings.h
//  WheelBLEFirmware
//
//  Created by Corbin Dunn on 12/23/15 .
//  Copyright © 2015 Corbin Dunn. All rights reserved.
//

#ifndef SharedSettings_h
#define SharedSettings_h


#define DEBUG 1

#if DEBUG
#warning "DEBUG CODE IS ON!!!! LEDCommon.h"
    #define DEBUG_PRINTLN(a) Serial.println(a)
    #define DEBUG_PRINTF(a, ...) char buffer[1024]; sprintf(buffer, a, ##__VA_ARGS__); Serial.print(buffer);
#else
    #define DEBUG_PRINTLN(a)
    #define DEBUG_PRINTF(a, ...)
#endif


#endif /* SharedSettings_h */
