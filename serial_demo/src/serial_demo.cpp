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
int GPS_SATS = 1; // Number of satallites
String SOFTWARE_STATE = "LAUNCH_WAIT";
/*LAUNCH_WAIT, ASCENT,
ROCKET_SEPARATION, DESCENT,
SP1_RELEASE, SP2_RELEASE,
LANDED*/
int SP1_PACKET_COUNT = 0;
int SP2_PACKET_COUNT = 0;
String CMD_ECHO = "SOME_COMMAND";

float SP_ALTITUDE = 0.0;
float SP_TEMP = 27.0;
int SP_ROTATION_RATE = 80;

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

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    ;
  }
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
    else if (command.substring(0,2) == "ST")
    {
      Serial.println("Setting Time");
    }
    else
    {
      Serial.println("Not a Command");
    }
  }
  timer();
}
void timer()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    // save the last time the code ran
    previousMillis = currentMillis;

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
      PACKET_COUNT + "," + "S1" + "," +
      SP_ALTITUDE + "," + SP_TEMP +
      "," + SP_ROTATION_RATE);
}
void sendPayload2Telemetry()
{
  Serial.println(
      TEAM_ID + "," +
      MISSION_TIME + "," + PACKET_COUNT +
      "," + "S2" + "," + SP_ALTITUDE +
      "," + SP_TEMP + "," + SP_ROTATION_RATE);
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
  SP_ALTITUDE = random(1000);
  SP_TEMP = random(28, 30);
  SP_ROTATION_RATE = random(50, 80);

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