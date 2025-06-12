#define Ventilator_DIR 4  // IN1 pin H-mosta (smjer)
#define PWM_PIN 2         // ENA pin H-mosta (pwm brzina)

const int pwmFreq = 5000;
const int pwmResolution = 12;

void setup() {
  Serial.begin(115200);
  pinMode(Ventilator_DIR, OUTPUT);
  digitalWrite(Ventilator_DIR, HIGH);  // Fiksni smjer

  ledcAttach(PWM_PIN, pwmFreq, pwmResolution);
}

void loop() {
  // Ide od 30% do 100%
  for (int p = 30; p <= 100; p += 10) {
    int duty = p * 4095 / 100;
    ledcWrite(PWM_PIN, duty);
    Serial.print("PWM: ");
    Serial.print(p);
    Serial.println("%");
    delay(1000);
  }

  // Nazad od 100% do 30%
  for (int p = 100; p >= 30; p -= 10) {
    int duty = p * 4095 / 100;
    ledcWrite(PWM_PIN, duty);
    Serial.print("PWM: ");
    Serial.print(p);
    Serial.println("%");
    delay(1000);
  }
}
