//
//  BluetoothManager.hpp
//  ArudinoBLETest
//
//  Created by Corbin Dunn on 12/22/15 .
//  Copyright © 2015 Corbin Dunn. All rights reserved.
//

#ifndef BluetoothManager_hpp
#define BluetoothManager_hpp

#include "Arduino.h"
#include <BLE_API.h>
#include "CDWheelBluetoothShared.h"
#include "WheelService.h"

class BluetoothManager {
private:
    BLE m_ble;
    WheelService *m_wheelService;
public:
    BluetoothManager();
    
    void loop();
    void setup();
    void startAdvertising() { m_ble.startAdvertising(); }
};



#endif /* BluetoothManager_hpp */
