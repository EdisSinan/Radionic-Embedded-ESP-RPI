// Uključujemo biblioteku za upravljanje servo motorom na ESP32, ESP32Servo by Kevin
#include <ESP32Servo.h>

// === DEFINICIJA PINOVA NA ESP32 ===
#define Tempsensor_PIN 15    // Digitalni izlaz temperaturnog senzora (npr. kada je vruće -> 0)
#define Ventilator_PIN 4     // IN1 pin za H-most – određuje smjer rada ventilatora
#define FotoRezistor_PIN 34  // Analogni ulaz za mjerenje svjetlosti (foto-rezistor)
#define PMW 2                // Enable pin na H-mostu – ovdje šaljemo PWM za brzinu motora

#define PIR_SENSOR_PIN 27  // PIR senzor za pokret – šalje 1 kada otkrije osobu
#define LED_PIN 26         // LED lampica koja svijetli kad je neko u blizini
#define SERVO_PIN 25       // Servo motor (npr. otvara vrata)


// === PWM KONFIGURACIJA ===
// PWM signal omogućava kontrolu brzine ventilatora (0% do 100%)
const int pwmFreq = 5000;      // Frekvencija PWM signala (Hz)
const int pwmResolution = 12;  // Rezolucija PWM-a: 12 bita → vrijednosti od 0 do 4095

// Inicijalizujemo servo objekt
Servo servo;

void setup() {
  Serial.begin(115200);  // Pokrećemo serijsku komunikaciju za ispis na monitor

  // Postavljamo ulaze i izlaze
  pinMode(Tempsensor_PIN, INPUT);    // Temperaturni senzor kao ulaz
  pinMode(FotoRezistor_PIN, INPUT);  // Foto-rezistor kao ulaz
  pinMode(Ventilator_PIN, OUTPUT);   // Kontrola smjera ventilatora
  pinMode(PIR_SENSOR_PIN, INPUT);    // PIR senzor kao ulaz
  pinMode(LED_PIN, OUTPUT);          // LED kao izlaz

  // Vežemo PWM signal na pin PMW (pin 2) za upravljanje brzinom motora
  ledcAttach(PMW, pwmFreq, pwmResolution);

  // Povezujemo servo motor sa njegovim pinom
  servo.attach(SERVO_PIN);
  servo.write(0);  // Postavljamo servo u početni položaj (zatvoreno)

  digitalWrite(LED_PIN, LOW);  // Isključujemo LED na početku
}

void loop() {
  // === 1. DETEKCIJA POKRETA (PIR SENZOR) ===
  // Ako PIR senzor otkrije pokret, uključujemo LED i otvaramo vrata (servo)
  int motionDetected = digitalRead(PIR_SENSOR_PIN);

  if (motionDetected) {
    Serial.println("Pokret detektovan!");
    digitalWrite(LED_PIN, HIGH);  // Upali LED
    servo.write(90);              // Pomjeri servo u otvoreni položaj
  } else {
    digitalWrite(LED_PIN, LOW);  // Ugasi LED
    servo.write(0);              // Zatvori vrata (servo u početni položaj)
  }

  // === 2. PROVJERA TEMPERATURE I SVIJETLA ===

  // Čitamo da li je temperatura visoka (senzor daje 0 kad je toplo)
  int tempValue = digitalRead(Tempsensor_PIN);
  Serial.print("Temperaturni senzor: ");
  Serial.println(tempValue);

  // Ako je temperatura visoka (0), pokrećemo ventilator
  if (tempValue == 0) {
    digitalWrite(Ventilator_PIN, HIGH);  // Uključujemo smjer motora (naprijed)

    // Čitamo jačinu svjetlosti sa foto-rezistora
    int lightValue = analogRead(FotoRezistor_PIN);
    Serial.print("Foto-rezistor: ");
    Serial.println(lightValue);

    int pwmPercent;  // Ovdje ćemo odlučiti kolika će biti brzina motora

    // Ako je jako svijetlo → ne mora jako da puše
    if (lightValue > 2047) {
      pwmPercent = 30;
    } else if (lightValue > 1000) {
      pwmPercent = 60;
    } else if (lightValue > 100) {
      pwmPercent = 80;
    } else {
      pwmPercent = 100;  // Ako je mračno i toplo → maksimalno hlađenje
    }

    // Pretvaramo procent u vrijednost PWM signala (0-4095)
    int pwmDuty = pwmPercent * 4095 / 100;
    ledcWrite(PMW, pwmDuty);  // Šaljemo tu vrijednost kao PWM signal

    // Ispisujemo informacije
    Serial.print("PWM %: ");
    Serial.print(pwmPercent);
    Serial.print(" | Duty: ");
    Serial.println(pwmDuty);

  } else {
    // Ako nije toplo, gasimo ventilator
    digitalWrite(Ventilator_PIN, LOW);
    ledcWrite(PMW, 0);  // PWM = 0 znači brzina = 0
  }
  delay(500);  // Pauza od pola sekunde prije sljedeće provjere
}
