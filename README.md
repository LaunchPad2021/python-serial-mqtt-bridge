# Serial Port to MQTT Bridge

## Can't run this on docker due to Serial Port forwarding limitations on docker.

### 1. [__bridge.py__](./bridge.py) 
Contains python program to bridge Serial Port and MQTT Server.
- Data received on serial port will be published to topics based on data type.

    Data Type | Topic
    --- | ---
    Container Telemetry | `cansat/container`
    SP1 Telemetry | `cansat/sp1`
    SP2 Telemetry | `cansat/sp2`


- Any data published to topic `cansat/sendData` will be sent to serial port.

### 2. [__serial_demo.cpp__](./serial_demo/src/serial_demo.cpp)
Contains a sample code to be written to Arduino (data format to be received on __GS__ xbee).
- Complex file structure due to PlatformIO.
- Check [__serial_demo.cpp__](./serial_demo/src/serial_demo.cpp) for more details.