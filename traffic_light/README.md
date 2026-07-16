# Arduino Traffic Light System 🚦

![Circuit](circuit.jpg)

## Demo Video
[Watch the project working](PASTE_YOUR_YOUTUBE_LINK_HERE)

## Project Description
This project simulates a real traffic intersection using an Arduino Uno. It automatically cycles through traffic lights and includes pedestrian crossing signals.

## Features
- Automatic traffic light cycle
- Red, yellow, and green vehicle lights
- Pedestrian walk/don't walk signals
- Countdown timer using the Serial Monitor
- Expandable design for a physical countdown display

## Components Used
- Arduino Uno
- Breadboard
- Red LED
- Yellow LED
- Green LED
- Pedestrian red LED
- Pedestrian green LED
- Push button
- 220Ω resistors
- Jumper wires
- USB cable

## How It Works
The traffic light cycles through green, yellow, and red phases automatically. During the red phase, the pedestrian signal changes to WALK while vehicles remain stopped. A countdown timer is displayed through the Serial Monitor.

## Arduino Concepts Learned
- Controlling multiple outputs
- Managing timing with `delay()`
- Organizing larger Arduino programs
- Working with multiple LEDs
- Building a state-based system

## Pin Setup

### Vehicle Lights

| Component | Arduino Pin |
|-----------|-------------|
| Green LED | Pin 8 |
| Yellow LED | Pin 9 |
| Red LED | Pin 10 |

### Pedestrian Lights

| Component | Arduino Pin |
|-----------|-------------|
| Walk LED | Pin 11 |
| Don't Walk LED | Pin 12 |

### Input

| Component | Arduino Pin |
|-----------|-------------|
| Push Button | Pin 2 |

## What I Learned
- How multiple outputs work together
- How timing controls automated systems
- How real traffic lights are programmed
- How to organize larger Arduino sketches

## Future Improvements
- Add a 4-digit countdown display
- Add a buzzer for pedestrian alerts
- Improve the pedestrian button logic
- Replace `delay()` with `millis()` for smoother operation
- Detect vehicles using sensors
