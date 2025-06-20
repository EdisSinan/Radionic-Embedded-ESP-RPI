from gpiozero import DigitalInputDevice, PWMOutputDevice, OutputDevice
from time import sleep

# === Ulazi: senzori ===
pir = DigitalInputDevice(17)        # PIR senzor pokret
ir = DigitalInputDevice(27)         # IR senzor
svjetlo = DigitalInputDevice(22)    # Senzor svjetla (digitalni)

# === Izlazi: motor ===
motor_pwm = PWMOutputDevice(18)     # PWM snaga (ENA pin na L298N)
motor_pin = OutputDevice(23)        # Smjer motora (IN1 pin na L298N)

while True:
    aktivni = 0

    # Provjera PIR
    if pir.value == 1:
        print("PIR senzor: Aktiviran")
        aktivni += 1
    else:
        print("PIR senzor: Neaktivan")

    # Provjera IR
    if ir.value == 1:
        print("IR senzor: Aktiviran")
        aktivni += 1
    else:
        print("IR senzor: Neaktivan")

    # Provjera svjetla
    if svjetlo.value == 1:
        print("Senzor svjetla: Aktiviran")
        aktivni += 1
    else:
        print("Senzor svjetla: Neaktivan")

    # Uključivanje motora u desnom smjeru ako bilo koji senzor radi
    if aktivni > 0:
        motor_pin.on()  # pokreni motor
    else:
        motor_pin.off()  # stani motor

    # Postavljanje snage motora
    if aktivni == 1:
        motor_pwm.value = 0.3
        print("Snaga motora: 30%")
    elif aktivni == 2:
        motor_pwm.value = 0.6
        print("Snaga motora: 60%")
    elif aktivni == 3:
        motor_pwm.value = 1.0
        print("Snaga motora: 100%")
    else:
        motor_pwm.value = 0.0
        print("Motor isključen")

    print("-" * 30)
    sleep(1)
