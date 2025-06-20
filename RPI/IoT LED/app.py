from flask import Flask, render_template, request, jsonify
from gpiozero import PWMLED, DigitalInputDevice
from threading import Thread
from time import sleep

app = Flask(__name__)

led = PWMLED(18)  # PWM LED na GPIO 18
ir = DigitalInputDevice(17)  # IR senzor na GPIO 17

ir_status = False

def ir_monitor():
    global ir_status
    while True:
        ir_status = ir.value
        print(f"[IR SENSOR] Detekcija: {'DA' if ir_status else 'NE'}")
        sleep(2)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/set", methods=["POST"])
def set_led():
    data = request.json
    value = float(data.get("value", 0))
    led.value = value  # 0.0 - 1.0
    return jsonify(success=True)

@app.route("/status", methods=["GET"])
def get_status():
    return jsonify(brightness=round(led.value, 2), ir=ir_status)

Thread(target=ir_monitor, daemon=True).start()

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
