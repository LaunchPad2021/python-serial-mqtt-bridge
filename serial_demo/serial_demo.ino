bool VALUE = true;
void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }

}

void loop() {

  Serial.println("Printing " + String(VALUE));
  digitalWrite(LED_BUILTIN, VALUE);
  VALUE = !VALUE;
  delay(500);

 if (Serial.available()){
     Serial.print("Got Data: ");
     Serial.println(Serial.readString());
   }
}
