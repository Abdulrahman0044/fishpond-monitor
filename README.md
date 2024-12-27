# Pond Monitoring System with ESP32

This project is a pond measuring system built using the ESP32 microcontroller, which reads data from multiple sensors and displays the results on an LCD. The system connects to a WiFi network to send data to a remote server via HTTP requests.

## Features
- Measures temperature using a DallasTemperature sensor
- Measures pH levels using an analog pH sensor
- Measures turbidity (water quality) using a TDS sensor
- Displays sensor readings (Temperature, pH, Turbidity) on a 4-line LCD screen
- Connects to a WiFi network and sends data to a remote server

## Components Used
- **ESP32 Microcontroller**
- **DallasTemperature Sensor** (for temperature)
- **pH Sensor** (analog)
- **Turbidity Sensor (TDS Sensor)**
- **LCD Display** (20x4 I2C LCD)
- **WiFi Module** (Integrated in ESP32)
- **Wires and Breadboard** (for sensor connections)

## Installation

### 1. Hardware Setup
Connect the sensors and components to the ESP32 board as follows:
- **Temperature Sensor**: Connect to GPIO 4 (OneWire bus)
- **pH Sensor**: Connect to GPIO 34 (analog input)
- **TDS Sensor**: Connect to GPIO 35 (analog input)
- **LCD Display**: Connect to the I2C pins (SCL and SDA)

### 2. Software Setup
#### Requirements
- **Arduino IDE**: Install the Arduino IDE from [here](https://www.arduino.cc/en/software).
- **ESP32 Board**: Follow [this guide](https://docs.espressif.com/projects/arduino-esp32/en/latest/) to set up ESP32 in the Arduino IDE.
- **Libraries**: Install the following libraries:
  - `DallasTemperature`
  - `Wire`
  - `ESPAsyncWebServer`
  - `LiquidCrystal_I2C`
  - `SimpleTimer`
  - `HTTPClient`

#### Code
- Open the Arduino IDE and paste the provided code into a new sketch.
- Set your **WiFi credentials** (`ssid` and `password`).
- Set the **server URL** to the endpoint where the data will be sent (modify `String serverName`).

### 3. Upload and Run
- Select the correct board and port in the Arduino IDE.
- Click on the **Upload** button.
- Once uploaded, the ESP32 will connect to WiFi and begin reading data from the sensors, displaying it on the LCD, and sending it to the server.

## Configuration
- **WiFi Credentials**: Update the `ssid` and `password` with your WiFi credentials.
- **Server URL**: Update the `serverName` with the correct URL for your server that will receive the data.

## LCD Display
The LCD screen will display the following information:
- **Temperature**: Displays the current temperature in Celsius.
- **pH**: Displays the current pH level of the soil.
- **Turbidity**: Displays the turbidity (dissolved oxygen) value in ppm.

## Data Format
Data is sent to the server using the following GET request format:
```cp
https://your-server-url.com/data.php?
Temp=temperature_value&PH=ph_value&Tur=turbidity_value&Id=device_id
