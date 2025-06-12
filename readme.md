# Embedded Workshop Projects: Arduino, ESP32 & Raspberry Pi

Welcome to the repository created for an **educational workshop** on embedded systems. As the **workshop educator**, I guide participants through the basics and intermediate-level development on platforms like **Arduino**, **ESP32**, and **Raspberry Pi**.

This repo contains a series of **short examples** and a **final mini project**, designed to help learners get hands-on experience with embedded hardware and programming.

---

## 📚 What This Workshop Covers

- Basics of microcontroller programming
- Sensor and actuator integration
- PWM control and digital input/output
- Introduction to IoT with **Blynk**
- Using **Raspberry Pi** for GPIO and simple automation
- Building and structuring real-world mini-projects

---

## 📁 Repository Structure & Projects

### `ESP32_Examples/`
A collection of simple examples created to introduce ESP32 development:

- `basic_input_output/`: Control LEDs, read buttons, and understand GPIOs.
- `pwm_control/`: Demonstrates PWM control of a fan or LED using analog output.
- `sensor_readings/`: Reads data from basic analog or digital sensors (e.g., photoresistors, PIR motion sensor).

Each sketch is designed to be short, readable, and easy to expand.

---

### `Final_Project/`

This folder contains the final demonstration project presented at the end of the workshop.

#### `Arduino_Without_Blynk/`
A fully functional standalone version of a fan and light controller. Features include:
- Automated control based on sensor input
- LED indicators for motion or brightness
- Fan control using PWM based on thresholds

#### `Arduino_With_Blynk/`
An IoT-enabled version of the project using the **Blynk platform**. Features:
- Real-time remote control of the fan
- Live monitoring of sensor states (motion, brightness)
- Manual/automatic toggling via the mobile app

Perfect for showing how simple embedded devices can be extended to **cloud-based control and monitoring**.

---

### `RPi_Examples/`
Though not the main focus of the workshop, a few **Raspberry Pi** examples are included:

- `gpio_control.py`: A Python script to toggle GPIO pins and read inputs.

These examples demonstrate the power of Raspberry Pi in home automation or rapid prototyping.

---

## 🙌 Explore and Learn

All code is provided for **educational purposes**. Feel free to:
- Explore each project folder
- Review or modify the code
- Use the examples in your own embedded projects

Pull requests and feedback are welcome!

---

## 🚀 Final Note

The goal of this workshop is to **demystify embedded development** and give participants the confidence to build on their own. Whether you're lighting an LED, controlling a fan, or building an IoT dashboard—every small project helps you grow as a maker.

Happy learning and building!
