// corbin dunn, 12/22/15


#include "Arduino.h"
#include "BluetoothManager.h"
#include "SharedSettings.h"

static BluetoothManager g_btManager;

void setup() {
#if DEBUG
    Serial.begin(9600);
    Serial.println("Starting Bluetooth manager...");
#endif
    g_btManager.setup();
    g_btManager.startAdvertising();
}

void loop() {
    g_btManager.loop();
}


// corbin hacks to work around linker errors. Who is calling these?
extern "C"{
    int _getpid(){ return -1;}
    int _kill(int pid, int sig){ return -1; }
}

