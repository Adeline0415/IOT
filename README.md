# Smart Energy Saving and IoT Application - 2nd Place in 2024 Mobile Communications Practice Competition

## Project Overview
This project won **2nd place in the Smart Energy Saving and IoT Application category** at the **2024 Mobile Communications Practice Competition**. It was a collaborative team project that combines IoT technologies to implement smart energy monitoring, featuring environmental data sensing, wireless communication, and intelligent energy-saving controls.

The device is designed as a **smart persimmon drying rack** equipped with a **waterproof rolling curtain**. The curtain is controlled by a motor and can switch between a breathable mesh and a waterproof fabric, helping persimmon farmers reduce labor and respond swiftly to sudden rainfall.

## Features
1. **Environmental Data Sensing**: Collects temperature, humidity, light intensity, and rain status using a DHT22 sensor, a photoresistor, and a rain sensor.
2. **Data Transmission**: Sends sensor data to the competition-provided IoT platform via the MQTT protocol.
3. **Platform Integration**: The IoT platform supports both **conditional automation** and **manual control**, allowing users to send control commands back to the device.
4. **Dynamic Control**: Executes GPIO control based on MQTT messages, with support for timer functionalities.
5. **High Expandability**: Modular design allows easy integration of additional sensors or control logic.

## Technology Stack
- **Hardware**: ESP32 / Arduino
- **Sensors**: DHT22, Photoresistor, Rain Sensor
- **Communication Protocol**: MQTT
- **Programming Language**: C++

## How It Works
1. The **smart persimmon drying rack** monitors environmental conditions and sends sensor data to the competition-provided IoT platform.
2. The IoT platform processes data and allows users to:
   - Set **automated control conditions**, such as closing the waterproof curtain when rain is detected.
   - Send **manual control commands**, such as opening or closing the curtain.
3. The device receives control commands from the platform and adjusts the motor to switch the curtain between breathable mesh and waterproof fabric as needed.

## Setup Instructions
1. Install [Arduino IDE](https://www.arduino.cc/en/software) or a compatible development environment.
2. Install the required Arduino libraries:
   - `PubSubClient`
   - `DHT`
3. Configure and upload the code to the development board:
   - Edit `Config.h` to set your WiFi and MQTT server configurations:
     ```cpp
     const char* WIFI_SSID = "YourWiFiSSID";
     const char* WIFI_PASSWORD = "YourWiFiPassword";
     const char* MQTT_SERVER = "YourMQTTServer";
     ```
   - Upload all files to the ESP32 / Arduino board.

## Project Structure
```plaintext
├── Config.cpp / Config.h        # Configuration files for WiFi and MQTT parameters
├── MQTTControl.cpp / MQTTControl.h  # Handles MQTT message parsing and GPIO control
├── SensorHandler.cpp / SensorHandler.h # Processes sensor data
├── Timer.cpp / Timer.h          # Timer module
├── Utility.h                    # Debugging and utility functions
├── main.ino                     # Main entry point


## JSON Message Format

### Sensor Data Upload
```json
[
  {
    "macAddr": "00000000aaBB0D838",
    "data": "0267xxxx"
  }
]
```

**Parameters:**
- `macAddr`: Device MAC address
- `data`: Sensor data in HEX format

### Control Message Downlink
```json
{
  "type": "1F",
  "pin": 33,
  "isOn": true,
  "delaySeconds": 30
}
```

**Parameters:**
- `type`: Message type
- `pin`: GPIO pin number
- `isOn`: State (on/off)
- `delaySeconds`: Delay time in seconds

## Results
This project successfully demonstrated the potential of IoT technologies in smart environmental monitoring and automation. Specifically, it provides a practical solution for persimmon farmers to reduce labor costs and improve responsiveness to sudden rainfall.

## Video Demonstration
Watch our project in action:

## Credits
## Credits
This project was collaboratively developed by (in alphabetical order based on English names):
- **Chia-Hsuan Yu**: Assisted with code development  
- **Ni-Hsuan Tsai**: Device design and construction  
- **Sheng-Mei Kao**: Project management and market research  
- **Ting Li**: Device design, construction, and hardware-software integration  
- **Tsun-Han Chang**: Arduino code development  
- **Tzu-Yun Lai**: Arduino code development and program architecture design  

Special thanks to the competition organizers for providing the IoT platform and resources.

