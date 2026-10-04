#pragma once

#include "config.h"
#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

class JammerCore {
private:
    int currentMode;
    int currentChannelIndex;
    unsigned long lastSwitchTime;
    BLEServer *pServer;
    BLECharacteristic *pCharacteristic;
    bool bleInitialized;

    // Wi-Fi Jamming Functions
    void initWiFiJamming() {
        WiFi.mode(WIFI_STA);
        WiFi.disconnect(true);
        
        // Set maximum TX power
        if (esp_wifi_set_max_tx_power(WIFI_TX_POWER_DBM) != ESP_OK) {
            Serial.println("[WARN] Could not set max TX power");
        } else {
            Serial.printf("[INFO] Wi-Fi TX Power set to %ddBm\n", WIFI_TX_POWER_DBM);
        }
        
        // Start on the first channel
        int startChannel = WIFI_CHANNEL_START;
        WiFi.setChannel(startChannel);
        currentChannelIndex = 0;
        Serial.printf("[INFO] Wi-Fi Jamming Active on Channel %d\n", startChannel);
    }

    void sweepWiFiChannels() {
        if (millis() - lastSwitchTime < SWEEP_INTERVAL_MS) return;
        lastSwitchTime = millis();

        // Cycle through channels 1-11
        int totalChannels = WIFI_CHANNEL_END - WIFI_CHANNEL_START + 1;
        currentChannelIndex = (currentChannelIndex + 1) % totalChannels;
        int targetChannel = WIFI_CHANNEL_START + currentChannelIndex;
        
        WiFi.setChannel(targetChannel);
        Serial.printf("[SWEEP] Jumped to Channel %d (%d MHz)\n", targetChannel, 2401 + (targetChannel * 5));
    }

    // BLE Jamming Functions
    void initBLEJamming() {
        if (bleInitialized) return;
        
        BLEDevice::init("ESP32_Jammer");
        BLEDevice::setPower(ESP_PWR_LVL_P9); // Max BLE power

        // Create Server
        pServer = new BLEServer();
        pServer->setCallbacks(new JammerServerCallbacks(this));

        // Create Service
        BLEService *pService = pServer->createService("00001800-0000-1000-8000-00805F9B34FB");
        
        // Create Characteristic
        pCharacteristic = pService->createCharacteristic(
            "00002A00-0000-1000-8000-00805F9B34FB",
            BLECharacteristic::PROPERTY_READ   |
            BLECharacteristic::PROPERTY_WRITE  |
            BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_INDICATE
        );
        
        pService->start();
        
        // Start Advertising with MAXIMUM interval (flood the air)
        BLEAdvertising *pAdvertising = pServer->getAdvertising();
        pAdvertising->start(BLE_ADVERT_INTERVAL_MS);
        pAdvertising->setScanResponse(true); // Also send scan responses
        
        bleInitialized = true;
        Serial.printf("[INFO] BLE Jamming Active (Interval: %dms)\n", BLE_ADVERT_INTERVAL_MS);
    }

    void floodBLE() {
        if (!bleInitialized || !pCharacteristic) return;
        
        static unsigned char noiseData[512];
        // Fill with random noise to maximize RF energy
        for (int i = 0; i < 512; i++) {
            noiseData[i] = random(0, 255);
        }
        // Force re-transmission by updating value
        pCharacteristic->setValue(noiseData, 512);
    }

public:
    JammerCore() : currentMode(JAM_MODE_SWEEP), currentChannelIndex(0), lastSwitchTime(0), pServer(nullptr), pCharacteristic(nullptr), bleInitialized(false) {}

    void begin() {
        Serial.begin(SERIAL_BAUD);
        Serial.println("\n=== ESP32 2.4GHz Jammer Starting ===");
        
        // Select mode based on config
        if (currentMode == JAM_MODE_SWEEP || currentMode == JAM_MODE_FIXED) {
            initWiFiJamming();
        } else if (currentMode == JAM_MODE_BLE_FLOOD) {
            initBLEJamming();
        }
    }

    void loop() {
        if (currentMode == JAM_MODE_SWEEP) {
            sweepWiFiChannels();
        } else if (currentMode == JAM_MODE_FIXED) {
            // Stay on the initial channel, do nothing
        } else if (currentMode == JAM_MODE_BLE_FLOOD) {
            floodBLE();
        }
    }

    void setMode(int mode) {
        if (mode >= JAM_MODE_SWEEP && mode <= JAM_MODE_BLE_FLOOD) {
            currentMode = mode;
            Serial.printf("[INFO] Mode changed to %d\n", mode);
        }
    }

    void status() {
        Serial.printf("\n[STATUS] Mode: %d | Channel: %d | BLE Active: %s\n", 
                      currentMode, 
                      currentMode == JAM_MODE_SWEEP ? WIFI_CHANNEL_START + currentChannelIndex : 0,
                      bleInitialized ? "YES" : "NO");
    }
};

// BLE Callbacks
class JammerServerCallbacks : public BLEServerCallbacks {
    JammerCore* core;
public:
    JammerServerCallbacks(JammerCore* c) : core(c) {}
    
    void onConnect(BLEServer* pServer, BLEConnInfo connInfo) {
        Serial.println("[BLE] Device Connected - Jamming Active");
    }
    
    void onDisconnect(BLEServer* pServer, BLEConnInfo connInfo) {
        Serial.println("[BLE] Device Disconnected - Re-advertising");
        pServer->startAdvertising();
    }
};
