// === BLYNK i WiFi PODEŠAVANJE ===
#define BLYNK_TEMPLATE_ID "TMPL4F5HgCf3s"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "VBnKkGLOiU7W6fPhauxeELNEQwc9pygR"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

// === WiFi PODACI ===
char ssid[] = "radionica";
char pass[] = "12341234";

// === PIN DEFINICIJE ===
#define Tempsensor_PIN     15
#define Ventilator_PIN     4
#define FotoRezistor_PIN   34
#define PMW                2
#define PIR_SENSOR_PIN     27
#define LED_PIN            26
#define SERVO_PIN          25

// === PWM KONFIGURACIJA ===
const int pwmFreq = 5000;
const int pwmResolution = 12;

// === VARIJABLE ===
Servo servo;
bool manuelnaKontrola = false;
int manuelniPWM = 0;

// === BLYNK CALLBACK FUNKCIJE ===
BLYNK_WRITE(V4) {
  manuelnaKontrola = param.asInt();
}

BLYNK_WRITE(V5) {
  manuelniPWM = param.asInt();
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(Tempsensor_PIN, INPUT);
  pinMode(FotoRezistor_PIN, INPUT);
  pinMode(Ventilator_PIN, OUTPUT);
  pinMode(PIR_SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  ledcAttach(PMW, pwmFreq, pwmResolution);
  servo.attach(SERVO_PIN);
  servo.write(0);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  Blynk.run();

  // === DETEKCIJA POKRETA ===
  int motionDetected = digitalRead(PIR_SENSOR_PIN);
  Blynk.virtualWrite(V0, motionDetected);

  if (motionDetected) {
    Serial.println("Pokret detektovan!");
    digitalWrite(LED_PIN, HIGH);
    servo.write(90);
  } else {
    digitalWrite(LED_PIN, LOW);
    servo.write(0);
  }
  Blynk.virtualWrite(V1, digitalRead(LED_PIN));

  // === MJERENJE TEMPERATURE I SVIJETLA ===
  int tempValue = digitalRead(Tempsensor_PIN);
  int lightValue = analogRead(FotoRezistor_PIN);
  Blynk.virtualWrite(V2, lightValue);

  if (manuelnaKontrola) {
    digitalWrite(Ventilator_PIN, HIGH);
    int duty = manuelniPWM * 4095 / 100;
    ledcWrite(PMW, duty);
    Blynk.virtualWrite(V3, 1);
  } else {
    if (tempValue == 0) {
      digitalWrite(Ventilator_PIN, HIGH);

      int pwmPercent;
      if (lightValue > 2047) pwmPercent = 30;
      else if (lightValue > 1000) pwmPercent = 60;
      else if (lightValue > 100) pwmPercent = 80;
      else pwmPercent = 100;

      int pwmDuty = pwmPercent * 4095 / 100;
      ledcWrite(PMW, pwmDuty);
      Blynk.virtualWrite(V3, 1);
    } else {
      digitalWrite(Ventilator_PIN, LOW);
      ledcWrite(PMW, 0);
      Blynk.virtualWrite(V3, 0);
    }
  }

  delay(500);
}
