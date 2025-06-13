#include <ESP32Servo.h>

Servo servo;

int servoPin =2;     // gdje je spojen servo
int buttonPin = 0;     // dugme na ESP32
int angle = 0;         // trenutni ugao
bool pritisnuto = false;

void setup() {
  Serial.begin(115200);           // pokreni serijsku komunikaciju
  servo.attach(servoPin);         // spoji servo
  pinMode(buttonPin, INPUT_PULLUP); // dugme koristi pullup
  servo.write(angle);             // postavi početni ugao

  Serial.println("Pokrenuto. Servo na 0°");
}

void loop() {
  if (digitalRead(buttonPin) == LOW && !pritisnuto) {
    pritisnuto = true;

    // ako je ugao 0, postavi na 90, inače vrati na 0
    if (angle == 0) {
      angle = 90;
    } else {
      angle = 0;
    }

    servo.write(angle);
    Serial.print("Dugme pritisnuto! Servo ide na ");
    Serial.print(angle);
    Serial.println("°");

    delay(300); // čekanje da se servo pomjeri i debounce
  }

  if (digitalRead(buttonPin) == HIGH) {
    pritisnuto = false;
  }
}
