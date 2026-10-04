#pragma once

// --- Jamming Parameters ---
#define SWEEP_INTERVAL_MS     50      // Time to dwell on each Wi-Fi channel (ms)
#define JAM_MODE_SWEEP        0       // Cycle through channels 1-11
#define JAM_MODE_FIXED        1       // Stay on a single channel
#define JAM_MODE_BLE_FLOOD    2       // Use BLE stack for active interference

// --- Wi-Fi Radio Settings ---
#define WIFI_TX_POWER_DBM     19      // Max ESP32 Wi-Fi TX power (0-19)
#define WIFI_CHANNEL_START    1       // Start channel (2412 MHz)
#define WIFI_CHANNEL_END      11      // End channel (2462 MHz)

// --- BLE Radio Settings ---
#define BLE_TX_POWER_DBM      9       // Max ESP32 BLE TX power (ESP_PWR_LVL_P9 = 9dBm)
#define BLE_ADVERT_INTERVAL_MS 10     // BLE advertising interval (ms)

// --- System Settings ---
#define SERIAL_BAUD           115200
#define STATUS_INTERVAL_MS    5000    // Log status every 5 seconds
