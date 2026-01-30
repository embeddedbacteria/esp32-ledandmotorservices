# ⚡ ESP32 LED & Motor Services (Bacteria-IoT)

An integrated IoT control system developed for the **ESP32**, providing high-concurrency services to manage LED arrays and DC/Servo motors via network protocols. This project serves as a bridge between high-level commands and low-level hardware execution.

## 🦠 Project Philosophy
This module is part of the **embeddedbacteria** ecosystem, focusing on creating seamless, low-latency "services" within microcontrollers to enable remote interaction with physical actuators.

## 🛠️ Features

- **Dual-Core Processing:** Leverages the ESP32's dual-core architecture to separate network handling from motor/LED timing logic.
- **Motor Control Services:** - PWM-based speed control for DC motors.
    - Precise pulse generation for Servos.
    - Logic for H-Bridge (L298N/DRV8833) integration.
- **LED Services:** - Support for standard RGB/Single-color LEDs.
    - (Optional) WS2812B Neopixel integration for advanced lighting patterns.
- **Connectivity:** - **Web Server:** RESTful API endpoints to trigger motor/LED actions.
    - **WebSocket Support:** Real-time, bi-directional communication for low-latency control.
- **Non-Blocking Architecture:** Uses `millis()` and hardware timers to ensure smooth motor movements without stalling the CPU.

## 📂 Project Structure

- `src/`: Core logic for Web/BT services.
- `include/`: Pin mappings and constant definitions.
- `lib/`: Motor control libraries and LED drivers.

## 🚀 Getting Started

### Prerequisites
- **Framework:** Arduino IDE or PlatformIO.
- **Hardware:** - ESP32 Development Board.
    - DC Motors / Servos.
    - LED Strips or high-intensity LEDs.
    - External Power Source (ESP32 pins alone cannot drive motors).

### Setup & Flash
1. Clone the repository:
   ```bash
   git clone [https://github.com/embeddedbacteria/esp32-ledandmotorservices.git](https://github.com/embeddedbacteria/esp32-ledandmotorservices.git)
Configure your WiFi credentials in the configuration file:

C++
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
Upload to your ESP32 board.

📡 API Example
Control your hardware with simple HTTP requests:

GET /motor/forward?speed=255 -> Moves the motor at full speed.

GET /led/on -> Toggles the LED service.

📄 License
MIT License.

Bridging the digital and physical world. Developed by embeddedbacteria
