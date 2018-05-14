# Panneau domotique
Firmware for the home automation panel of the robotic cup 2018.

The panel includes :
* an Arduino Uno running this firmware
* an Adafruit 8x8 bicolor LED matrix (controlled with I2C)
* APA102 RGB LED strip with 20 LEDs (controlled with SPI)
* 4 analog voltmeters (galvanometers)

## Setup

First, install the Arduino IDE if you don't have it already.

Then you'll need the following libraries :
* [Adafruit GFX library](https://github.com/adafruit/Adafruit-GFX-Library)
* [Adafruit LED Bagpack library](https://github.com/adafruit/Adafruit_LED_Backpack)
* [Polulu APA102 library](https://github.com/pololu/apa102-arduino)

Finally, clone this repository in your Arduino folder.

## Usage

Open the project with the Arduino IDE and flash it to the Arduino Uno.
The panel will start whenever it is powered.
