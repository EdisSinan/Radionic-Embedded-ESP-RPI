#define motion_pin 15
#define LED_PIN 2

void setup() {
  Serial.begin(115200);
  pinMode(motion_pin, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int ir_state = digitalRead(motion_pin);

  if (ir_state == 1) {
    Serial.print(ir_state);
    Serial.println(" | Detektujem nesto");
    digitalWrite(LED_PIN, HIGH);  // Upali LED
    Serial.println("LED upaljen");
  } else {
    Serial.print(ir_state);
    Serial.println(" | Nista ne detektujem");
    digitalWrite(LED_PIN, LOW);   // Ugasi LED
    Serial.println("LED ugasen");
  }

  delay(300);  // Mala pauza radi stabilnosti očitavanja
}
