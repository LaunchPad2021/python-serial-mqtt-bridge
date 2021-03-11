#include <Arduino.h>
bool VALUE = true;
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

  Serial.print("Printing ");
  Serial.println(VALUE);
  digitalWrite(LED_BUILTIN, VALUE);
  VALUE = !VALUE;
  delay(1000);

  if (Serial.available())
  {
    Serial.println("Message Received " + Serial.readString());
  }
}
