# ⏱️ Companion Study Timer

## 🌟 Summary

Companion Study Timer is an ESP32-based Pomodoro timer built with a 1.3-inch SH1106 OLED display. It includes a simple two-button menu, a study countdown, a random exam fortune screen, and Bluetooth Low Energy (BLE) support for setting the timer duration from another device.

This project is part of my personal robotics development journey towards building an AI-based robotic companion.

## ✨ Features

- Pomodoro-style study countdown
- Pause and resume support
- Menu controlled with two push buttons
- Random exam fortune generator
- BLE timer configuration from 1 to 60 minutes
- Compact 128x64 OLED interface

## 🛠️ Hardware Used

- ESP32 development board
- 1.3-inch SH1106 OLED display (128x64, I2C)
- 2 push buttons
- Breadboard
- Jumper wires
- USB cable

## 🔌 Wiring

### OLED Display

| OLED pin | ESP32 pin |
| --- | --- |
| VDD / VCC | 3.3V |
| GND | GND |
| SCK / SCL | GPIO 22 |
| SDA | GPIO 21 |

### Push Buttons

| Button | ESP32 pin | Function |
| --- | --- | --- |
| Left | GPIO 32 | Change selection / return to menu |
| OK | GPIO 33 | Confirm / pause / generate fortune |

Each button must be connected between its GPIO pin and **GND**. The sketch uses the ESP32's internal pull-up resistors with `INPUT_PULLUP`, so no external resistor is required.

> On a four-pin tactile button, the two pins on each side are internally connected. Use pins from opposite sides of the switch. When the button is not pressed, the GPIO should read `HIGH`; when pressed, it should read `LOW`.

## 📚 Required Libraries

Install the following libraries from the Arduino IDE Library Manager:

- Adafruit GFX Library
- Adafruit SH110X
- ESP32 BLE Arduino (included with the ESP32 Arduino core)

Also install the **ESP32 by Espressif Systems** board package from Arduino IDE's Boards Manager.

## 🚀 Uploading the Project

1. Connect the components according to the wiring tables above.
2. Open the Arduino sketch in Arduino IDE.
3. Select your ESP32 board from **Tools > Board**.
4. Select the correct serial port.
5. Compile and upload the sketch.
6. The main menu will appear on the OLED display.

The display uses I2C address `0x3C`. If your display does not turn on, scan the I2C bus and update the address in the sketch if necessary.

## 🎮 Controls

### Main Menu

- Press **Left** to switch between `STUDY` and `FORTUNE`.
- Press **OK** to open the selected screen.

### Study Timer

- Press **OK** to start, pause, or resume the countdown.
- Press **Left** to reset the timer and return to the main menu.
- The default duration is 25 minutes.

### Fortune Screen

- Press **OK** to generate a new fortune.
- Press **Left** to return to the main menu.

## 📶 BLE Timer Control

The ESP32 advertises with the name:

```text
PomodoroESP32
```

BLE identifiers:

| Type | UUID |
| --- | --- |
| Service | `1234` |
| Write characteristic | `5678` |

Write a number from `1` to `60` to characteristic `5678`. The number represents minutes. After receiving a valid value, the ESP32 switches to the timer screen and starts the countdown automatically.

You can test this feature with a BLE utility such as nRF Connect.

## 🖥️ Screens

- **Study:** Displays the remaining time in `MM:SS` format.
- **Fortune:** Displays a randomly selected answer to the question, “Will I pass the exam?”

## 💡 Possible Improvements

- Add a buzzer or vibration motor when the timer finishes
- Save the selected duration in non-volatile memory
- Add configurable study and break sessions
- Create a companion mobile application for BLE control
- Replace blocking button delays with debounced edge detection
