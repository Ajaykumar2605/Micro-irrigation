# Micro Irrigation System using NodeMCU

This project is a Micro irrigation system that utilizes NodeMCU (ESP8266) to monitor and control water flow based on real-time sensor data. It provides a web-based dashboard for remote monitoring and control of soil moisture levels, temperature, humidity, and rain detection.

## Features
- **Web Dashboard:** Real-time monitoring of sensor data via a web interface.
- **WiFi Connectivity:** Connects to a WiFi network to host a web server.
- **LCD Display:** Displays real-time status updates including WiFi connection and system IP.
- **Sensor Integration:** Supports DHT11 (temperature & humidity), soil moisture, and rain detection sensors.
- **Automatic Pump Control:** The system automatically turns the pump on/off based on soil moisture levels and rain detection.
- **Valve Control:** Allows remote control of three irrigation valves through the web interface.
- **JSON API:** Provides real-time sensor data in JSON format.
- **Secure Login Page:** Initial login page for restricted access to the dashboard.

## Components Used
- **NodeMCU ESP8266** (WiFi microcontroller)
- **DHT11 Sensor** (Temperature & Humidity sensor)
- **Soil Moisture Sensor**
- **Rain Detection Sensor**
- **Relay Module** (For controlling the pump and valves)
- **Water Valves** (For irrigation control)
- **I2C LCD Display** (For displaying system information)

## Installation and Setup
### 1. Clone this repository:
```sh
git clone https://github.com/yourusername/smart-irrigation.git
cd smart-irrigation
```

### 2. Install Required Libraries:
Ensure you have the following Arduino libraries installed:
```sh
ESP8266WiFi
ESP8266WebServer
DHTesp
LiquidCrystal_PCF8574
```

### 3. Modify WiFi Credentials:
Update the following lines in `main.ino` with your WiFi SSID and password:
```cpp
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
```

### 4. Compile and Upload the Code:
Use the Arduino IDE to compile and upload the code to the NodeMCU.

### 5. Access the Web Dashboard:
- Once connected to WiFi, the NodeMCU will print its IP address in the Serial Monitor.
- Open a browser and enter:
  ```
  http://<your_nodeMCU_IP>/dashboard
  ```

## API Endpoints
```sh
/ - Loads the login page (index.html).
/index - Redirects to the index page.
/dashboard - Loads the web-based dashboard.
/readADC - Fetches real-time sensor data in JSON format.
/control-valve/1/on - Turns Valve 1 ON.
/control-valve/1/off - Turns Valve 1 OFF.
/control-valve/2/on - Turns Valve 2 ON.
/control-valve/2/off - Turns Valve 2 OFF.
/control-valve/3/on - Turns Valve 3 ON.
/control-valve/3/off - Turns Valve 3 OFF.
```

## Future Improvements
- Implement a mobile-friendly UI for better accessibility.
- Integrate IoT cloud support for remote monitoring.
- Add machine learning for predictive irrigation control.
- Introduce user authentication for secure access.
- Improve LCD-based interaction with system messages.

## License
This project is open-source and available under the MIT License.

## Author
Developed by **AJAY KUMAR M**.
