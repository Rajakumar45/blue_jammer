# ESP32 2.4GHz Bluetooth Jammer

A software-defined radio (SDR) project using the ESP32's Wi-Fi and BLE radios to generate targeted 2.4GHz interference. This project demonstrates FHSS disruption, channel sweeping, and RF noise injection for educational and laboratory testing purposes.

## 📡 How It Works
Bluetooth uses Frequency Hopping Spread Spectrum (FHSS), hopping across 79 channels (2402–2480 MHz) 1,600 times per second. This project uses the ESP32's Wi-Fi radio (which operates in the same 2.4GHz ISM band) to:
1.  **Channel Sweep:** Rapidly cycle through Wi-Fi channels 1–11 (covering the Bluetooth band).
2.  **Noise Injection:** Transmit high-power 802.11 management frames or raw noise to raise the noise floor.
3.  **BLE Flooding:** Use the BLE stack to inject malformed advertisements to disrupt BLE-specific protocols.

## 🛠️ Hardware Requirements
- **ESP32 DevKit V1** (or any ESP32 with Wi-Fi/BLE)
- **2.4GHz Antenna** (IPEX/U.FL connector recommended for gain)
- **5V Power Supply** (2A minimum, stable)
- **Target Device** (Bluetooth speaker, phone, or headset for testing)

## 🚀 Installation & Build

### Option 1: Arduino IDE
1.  Install the [ESP32 Arduino Core](https://docs.espressif.com/projects/arduino-esp32/).
2.  Select your ESP32 board (e.g., "ESP32 Dev Module").
3.  Set **Tools > USB CDC On Boot** to `Enabled`.
4.  Open `firmware/src/main.cpp`.
5.  Click **Upload**.

### Option 2: PlatformIO (Recommended)
1.  Install [PlatformIO](https://platformio.org/).
2.  Clone this repo.
3.  Open the `firmware/` directory in VS Code.
4.  Click **PlatformIO: Build**.
5.  Click **PlatformIO: Upload**.

## ⚙️ Configuration
Edit `firmware/src/config.h` to adjust:
- `SWEEP_INTERVAL_MS`: How often the jammer changes channels (default: 50ms).
- `TX_POWER_DBM`: Maximum transmit power (default: 19dBm).
- `JAM_MODE`: `SWEEP`, `FIXED_CHANNEL`, or `BLE_FLOOD`.

## 📊 Testing & Verification
1.  **Visual:** Use a Wi-Fi Analyzer app (Android/iOS) to observe the 2.4GHz band. You should see a "wall" of noise across channels 1–11.
2.  **Audio:** Play audio on a Bluetooth speaker within 3 meters. Expect crackling, stuttering, or dropouts.
3.  **Connection:** Attempt to pair a new Bluetooth device. Pairing should fail or time out.

## 📝 Legal & Safety Disclaimer
- **FCC/CE Compliance:** This device intentionally causes interference. Do not deploy in a way that disrupts emergency services, aviation, or critical infrastructure.
- **Heat:** The ESP32 will heat up significantly. Ensure proper ventilation.
- **Liability:** The author is not responsible for unintended disruption of third-party systems.

## 📄 License
[MIT](LICENSE)
