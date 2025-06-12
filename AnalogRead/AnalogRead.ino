#define analog_pin 34

void setup() {
  Serial.begin(115200);
  pinMode(analog_pin, INPUT);
}

void loop() {
  int lightvalue = analogRead(analog_pin);
  float light_percent = (1 - (lightvalue / 4095.0)) * 100;


  Serial.print("Light value: ");
  Serial.print(lightvalue);
  Serial.print(" | Percent: ");
  Serial.print(light_percent);
  Serial.println("%");

  delay(1000);
}
