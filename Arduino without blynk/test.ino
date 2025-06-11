#include <ESP32Servo.h>

#define Tempsensor_PIN    15  // Digitalni temperaturni senzor
#define Ventilator_PIN     4  // IN1 pin za H-most
#define FotoRezistor_PIN  34  // Analogni ulaz za foto-rezistor
#define PMW                2  // Enable pin na H-mostu (PWM)

#define PIR_SENSOR_PIN    27  // PIR senzor (digitalni ulaz)
#define LED_PIN           26  // LED lampica
#define SERVO_PIN         25  // Servo motor PWM pin

const int pwmFreq = 5000;
const int pwmResolution = 12;

Servo servo;

void setup() {
  Serial.begin(115200);

  pinMode(Tempsensor_PIN, INPUT);
  pinMode(FotoRezistor_PIN, INPUT);
  pinMode(Ventilator_PIN, OUTPUT);
  pinMode(PIR_SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  // PWM postavka za H-most
  ledcAttach(PMW, pwmFreq, pwmResolution);

  // Servo postavka
  servo.attach(SERVO_PIN);
  servo.write(0);  // Početna pozicija

  // LED OFF
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  // === 1. PIR DETEKCIJA ===
  int motionDetected = digitalRead(PIR_SENSOR_PIN);

  if (motionDetected) {
    Serial.println("Pokret detektovan!");
    digitalWrite(LED_PIN, HIGH);
    servo.write(90);
  } else {
    digitalWrite(LED_PIN, LOW);
    servo.write(0);
  }

  // === 2. TEMPERATURA I FOTO-REZISTOR ===
  int tempValue = digitalRead(Tempsensor_PIN);
  Serial.print("Temperaturni senzor: ");
  Serial.println(tempValue);

  if (tempValue == 0) {
    digitalWrite(Ventilator_PIN, HIGH);  // Smjer motora

    int lightValue = analogRead(FotoRezistor_PIN);
    Serial.print("Foto-rezistor: ");
    Serial.println(lightValue);

    int pwmPercent;

    if (lightValue > 2047) {
      pwmPercent = 30;
    } else if (lightValue > 1000) {
      pwmPercent = 60;
    } else if (lightValue > 100) {
      pwmPercent = 80;
    } else {
      pwmPercent = 100;
    }

    int pwmDuty = pwmPercent * 4095 / 100;
    ledcWrite(PMW, pwmDuty);

    Serial.print("PWM %: ");
    Serial.print(pwmPercent);
    Serial.print(" | Duty: ");
    Serial.println(pwmDuty);

  } else {
    digitalWrite(Ventilator_PIN, LOW);
    ledcWrite(PMW, 0);
  }
  Serial.println("ENDE");
  delay(500);
}
