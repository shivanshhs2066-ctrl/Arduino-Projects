# Arduino Button-Controlled LED 🔘💡

![Circuit](circuit.jpg)

## Demo Video
[Watch the project working](PASTE_YOUR_YOUTUBE_LINK_HERE)

## Project Description
This project controls an LED using a push button. The LED turns on while the button is pressed and turns off when it is released.

## Features
- Push button input
- LED output
- Uses Arduino's internal pull-up resistor

## Components Used
- Arduino Uno
- Breadboard
- Push button
- LED
- 220Ω resistor
- Jumper wires
- USB cable

## How It Works
The Arduino continuously reads the button connected to pin 2. When the button is pressed, the Arduino turns on the LED connected to pin 13.

## Arduino Concepts Learned
- `digitalRead()`
- `digitalWrite()`
- `INPUT_PULLUP`
- `if` statements
- Reading digital inputs

## Pin Setup

| Component | Arduino Pin |
|-----------|-------------|
| Button | Pin 2 |
| LED | Pin 13 |

## What I Learned
- How to read user input
- How push buttons work
- How software can react to hardware events
- Using conditional statements in Arduino

## Future Improvements
- Toggle the LED on/off with each press
- Add multiple buttons
- Add different LED patterns
