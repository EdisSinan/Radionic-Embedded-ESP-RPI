from gpiozero import DigitalInputDevice, Servo
from time import sleep, time

# Digitalni ulaz - temperaturni senzor (HIGH ili LOW)
temp_sensor = DigitalInputDevice(17)

# Servo motor na GPIO 18
servo = Servo(18, min_pulse_width=0.0005, max_pulse_width=0.0025)

def lepeza(sekundi=5):
    start = time()
    while time() - start < sekundi:
        servo.value = -1  # -90 stepeni
        sleep(0.5)
        servo.value = 1   # +90 stepeni
        sleep(0.5)
    servo.value = 0       # Sredina (0 stepeni)
    servo.value = None    # Isključi PWM signal (ako želiš)

print("Cekam da senzor bude HIGH...")

while True:
    if temp_sensor.value == 1:
        print("Senzor HIGH - pokrecem servo lepezu")
        lepeza(5)
        print("Servo u sredini, cekam dalje...")
    sleep(0.1)
