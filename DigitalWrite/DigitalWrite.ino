#define led_pin 2

void setup() {
  Serial.begin(115200);
  pinMode(led_pin,OUTPUT);

}

void loop() {
 
 digitalWrite(led_pin,HIGH);
 Serial.println("Led upaljen");
 delay(2000);
 digitalWrite(led_pin,LOW);
 Serial.println("Led ugašen");
 delay(2000);

}
