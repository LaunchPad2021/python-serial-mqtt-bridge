/*
CMD,2176,CX,ON                  IMPLEMENTED
CMD,2176,CX,OFF                 IMPLEMENTED
CMD,2176,SP1X,ON                IMPLEMENTED
CMD,2176,SP1X,OFF               IMPLEMENTED
CMD,2176,SP2X,ON                IMPLEMENTED
CMD,2176,SP2X,OFF               IMPLEMENTED
CMD,2176,ST,<UTC_TIME>          IMPLEMENTED             LOGIC NEEDED
CMD,2176,SIM,<MODE>             IMPLEMENTED             LOGIC NEEDED
CMD,2176,SIMP,<PRESSURE>        IMPLEMENTED             LOGIC NEEDED
*/

#include <Arduino.h>
bool VALUE = true;
String TEAM_ID = "2176";
int MISSION_TIME = 0;
int PACKET_COUNT = 0;
String PACKET_TYPE = "C"; // C, S1 or S2
char MODE = 'F';
char SP1_RELEASED = 'N'; // N for not released, R for released
char SP2_RELEASED = 'N';
float ALTITUDE = 0.0;
float TEMP = 28.0;
float VOLTAGE = 4.48;
float GPS_TIME = 0.0001;
float GPS_LATITUDE = 0.0001;
float GPS_LONGITUDE = 0.0001;
float GPS_ALTITUDE = 0.1;
int GPS_SATS = 1;                      // Number of satallites
String SOFTWARE_STATE = "LAUNCH_WAIT"; // LAUNCH_WAIT, ASCENT, ROCKET_SEPARATION, DESCENT, SP1_RELEASE, SP2_RELEASE, LANDED
int SP1_PACKET_COUNT = 0;
int SP2_PACKET_COUNT = 0;
String CMD_ECHO = "SOME_COMMAND";
float SP1_ALTITUDE = 0;
float SP1_TEMP = 0;
int SP1_ROTATION_RATE = 0;
float SP2_ALTITUDE = 0;
float SP2_TEMP = 0;
int SP2_ROTATION_RATE = 0;

// for timer (1Hz)
unsigned long previousMillis = 0;
const long interval = 1000;

bool CONTAINER_TELEMETRY = false;
bool SP1_TELEMETRY = false;
bool SP2_TELEMETRY = false;

void sendContainerTelemetry();
void sendPayload1Telemetry();
void sendPayload2Telemetry();
void demo();
void timer();
void setTime();
void setSimulationMode();
void precessPressure();

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    if (Serial.available())
    {
        String command = Serial.readString().substring(9);
        if (command == "CX,ON")
        {
            CONTAINER_TELEMETRY = true;
        }
        else if (command == "CX,OFF")
        {
            CONTAINER_TELEMETRY = false;
        }
        else if (command == "SP1X,ON")
        {
            SP1_TELEMETRY = true;
        }
        else if (command == "SP1X,OFF")
        {
            SP1_TELEMETRY = false;
        }
        else if (command == "SP2X,ON")
        {
            SP2_TELEMETRY = true;
        }
        else if (command == "SP2X,OFF")
        {
            SP2_TELEMETRY = false;
        }
        else if (command.substring(0, 2) == "ST")
        {
            setTime();
        }
        else if (command.substring(0, 3) == "SIM")
        {
            setSimulationMode();
        }
        else if (command.substring(0, 4) == "SIMP")
        {
            precessPressure();
        }
        else
        {
            Serial.println("NOT_A_COMMAND");
        }
    }
    timer();
}

void timer()
{
    // RUNS EVERY SECOND
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
        // save the last time the code ran
        previousMillis = currentMillis;

        // UNCOMMENT AND ADD YOUR LOGIC HERE.
        demo();
        // Send Container Telemetry
        if (CONTAINER_TELEMETRY)
        {
            sendContainerTelemetry();
        }
        // Send Payload 1 Telemetry
        if (SP1_TELEMETRY)
        {
            sendPayload1Telemetry();
        }
        // Send Payload 2 Telemetry
        if (SP2_TELEMETRY)
        {
            sendPayload2Telemetry();
        }
    }
}

void sendContainerTelemetry()
{
    Serial.println(
        TEAM_ID + "," + MISSION_TIME + "," + PACKET_COUNT + "," + "C" +
        "," + MODE + "," + SP1_RELEASED + "," + SP2_RELEASED + "," + ALTITUDE +
        "," + TEMP + "," + VOLTAGE + "," + GPS_TIME + "," + GPS_LATITUDE + "," +
        GPS_LONGITUDE + "," + GPS_ALTITUDE + "," + GPS_SATS + "," + SOFTWARE_STATE +
        "," + SP1_PACKET_COUNT + "," + SP2_PACKET_COUNT + "," + CMD_ECHO);
}
void sendPayload1Telemetry()
{
    Serial.println(
        TEAM_ID + "," + MISSION_TIME + "," +
        SP1_PACKET_COUNT + "," + "S1" + "," +
        SP1_ALTITUDE + "," + SP1_TEMP +
        "," + SP1_ROTATION_RATE);
}
void sendPayload2Telemetry()
{
    Serial.println(
        TEAM_ID + "," +
        MISSION_TIME + "," + SP2_PACKET_COUNT +
        "," + "S2" + "," + SP2_ALTITUDE +
        "," + SP2_TEMP + "," + SP2_ROTATION_RATE);
}

void setTime()
{
    // Fill in the logics
    ;
}

void setSimulationMode()
{
    // Add your logic here
    ;
}

void precessPressure()
{
    // fill in your code here
    ;
}

void demo()
{
    SP1_PACKET_COUNT++;
    SP2_PACKET_COUNT++;
    PACKET_COUNT++;
    MISSION_TIME = millis() / 1000;
    ALTITUDE = random(1000);
    TEMP = random(30);
    VOLTAGE = float(random(300, 500)) / 100;
    GPS_LATITUDE = random(73, 74);
    GPS_LONGITUDE = random(12, 13);
    GPS_ALTITUDE = random(1000);
    GPS_SATS = random(1, 7);
    SP1_ALTITUDE = random(1000);
    SP1_TEMP = random(28, 30);
    SP1_ROTATION_RATE = random(50, 80);
    SP2_ALTITUDE = random(1000);
    SP2_TEMP = random(28, 30);
    SP2_ROTATION_RATE = random(50, 80);

    switch (PACKET_COUNT)
    {
    case 5:
        SOFTWARE_STATE = "ASCENT";
        break;

    case 20:
        SOFTWARE_STATE = "DESCENT";
        SP1_RELEASED = 'R';
        break;

    case 25:
        SP2_RELEASED = 'R';
        break;

    default:
        break;
    }
}