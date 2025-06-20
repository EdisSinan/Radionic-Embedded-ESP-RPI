from flask import Flask, render_template, request, jsonify
from gpiozero import Servo
from time import sleep

app = Flask(__name__)

# Servo na GPIO 18 (PWM pin)
servo = Servo(18, min_pulse_width=0.0005, max_pulse_width=0.0025)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/set_servo', methods=['POST'])
def set_servo():
    angle = float(request.json['angle'])  # -1.0 to 1.0 za gpiozero Servo
    servo.value = angle
    return jsonify(status="ok", angle=angle)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
