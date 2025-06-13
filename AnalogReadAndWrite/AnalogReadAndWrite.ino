#define Ventilator_DIR 4   // IN1 pin H-mosta (smjer)
#define PWM_PIN 2          // ENA pin H-mosta (PWM brzina)
#define LDR_PIN 34         // Analogni pin za LDR senzor (npr. GPIO34)

// PWM postavke
const int pwmFreq = 5000;
const int pwmResolution = 12; // 12-bitna rezolucija = max 4095

void setup() {
  Serial.begin(115200);

  pinMode(Ventilator_DIR, OUTPUT);
  digitalWrite(Ventilator_DIR, HIGH); // Fiksni smjer ventilatora

  // PWM konfiguracija
  ledcAttachPin(PWM_PIN, 0); // PWM_PIN spojen na kanal 0
  ledcSetup(0, pwmFreq, pwmResolution);
}

void loop() {
  int lightValue = analogRead(LDR_PIN);  // 0 - 4095
  float lightPercent = lightValue / 4095.0 * 100.0;

  // Pretvori svjetlo u PWM vrijednost — manja svjetlost = veći duty cycle
  int duty = map(lightValue, 0, 4095, 4095, 500); // Prilagodi raspon po potrebi
  ledcWrite(0, duty);

  Serial.print("Svjetlo: ");
  Serial.print(lightValue);
  Serial.print(" | ");
  Serial.print(lightPercent);
  Serial.print("% | PWM duty: ");
  Serial.println(duty);

  delay(500);
}
