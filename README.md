# Serial Port to MQTT Bridge

### 1. [__bridge.py__](./bridge.py) 
Contains python program to bridge Serial Port and MQTT Server.
- Data received on serial port will be published to `cansat/gotData` topic.
- Any data published to topic `cansat/sendData` will be sent to serial port.

### 2. [__serial_demo.cpp__](serial_demo\src\serial_demo.cpp)
Contains a sample code to be written to Arduino.
- (Complex file structure due to PlatformIO.)