# 🚗 Arduino Parking Sensor

## Overview

This project is an Arduino-based parking sensor that measures distance using an HC-SR04 ultrasonic sensor and displays the distance on a 4-digit 5641AS seven-segment display.

The system uses LEDs and a buzzer to warn the user as an object gets closer.

## Features

- Ultrasonic distance measurement
- 4-digit distance display
- Green/yellow/red warning system
- Increasing buzzer alerts as distance decreases
- Real-time distance updates

## Hardware Used

- Arduino Uno
- HC-SR04 Ultrasonic Sensor
- 5641AS 4-Digit 7-Segment Display
- 3 LEDs
- Buzzer
- Breadboard
- Jumper wires
- 220Ω resistors

## How It Works

The HC-SR04 sends ultrasonic waves and measures the time it takes for the echo to return.

The Arduino converts this time into distance in centimeters.

The display shows the current distance while the LEDs and buzzer provide warnings:

| Distance | Warning |
|---|---|
| >50 cm | Green LED |
| 20-50 cm | Yellow LED + slow beep |
| <20 cm | Red LED + fast beep |

## Wiring

### Ultrasonic Sensor

| Component | Arduino Pin |
|---|---|
| TRIG | 9 |
| ECHO | 10 |

### LEDs

| LED | Arduino Pin |
|---|---|
| Green | 1 |
| Yellow | 2 |
| Red | 3 |

### Buzzer

| Component | Arduino Pin |
|---|---|
| Buzzer | 8 |

### Display

| Segment | Pin |
|---|---|
| A | 11 |
| B | 13 |
| C | A3 |
| D | A4 |
| E | A5 |
| F | 12 |
| G | A2 |

## Demo

(Add video link here)

## What I Learned

- How ultrasonic sensors work
- How multiplexed displays are controlled
- Arduino input/output programming
- Debugging electronics circuits
- Combining multiple sensors and outputs into one system
