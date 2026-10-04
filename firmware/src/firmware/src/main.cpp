#include "jammer_core.h"

JammerCore jammer;

void setup() {
    jammer.begin();
}

void loop() {
    jammer.loop();
    
    // Periodic status log
    static unsigned long lastStatus = 0;
    if (millis() - lastStatus > STATUS_INTERVAL_MS) {
        lastStatus = millis();
        jammer.status();
    }
    
    // Allow serial command input to change modes
    if (Serial.available()) {
        char cmd = Serial.read();
        switch (cmd) {
            case '1': jammer.setMode(JAM_MODE_SWEEP); break;
            case '2': jammer.setMode(JAM_MODE_FIXED); break;
            case '3': jammer.setMode(JAM_MODE_BLE_FLOOD); break;
            default: break;
        }
    }
}
