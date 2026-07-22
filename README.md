# ⏱️ Companion Study Timer

## 🌟 Summary

Companion Study Timer is a compact ESP32-based Pomodoro device built with a 1.3-inch SH1106 OLED display. It combines a study countdown, a random exam fortune screen, Bluetooth Low Energy timer control, and an audible alarm in a simple two-button interface.

This project is part of my personal robotics development journey towards building an AI-based robotic companion.

## ✨ Features

- 25-minute Pomodoro timer by default
- Start, pause, resume, and restart controls
- `STUDY`, `PAUSED`, and `DONE!` timer states
- Three-beep alarm when a study session finishes
- Random exam fortune generator
- BLE timer configuration from 1 to 60 minutes
- Two-button menu designed for a 128x64 OLED display

## 🛠️ Hardware Used

- ESP32 development board
- 1.3-inch SH1106 OLED display (128x64, I2C)
- 2 push buttons
- Active buzzer
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
| OK | GPIO 33 | Confirm / start / pause / generate fortune |

Connect each push button between its GPIO pin and **GND**. The sketch uses `INPUT_PULLUP`, so an external pull-up resistor is not required.

> On a four-pin tactile button, the two pins on each side are already connected internally. Use pins from opposite sides of the switch. An unpressed button should read `HIGH`, and a pressed button should read `LOW`.

### Active Buzzer

| Buzzer pin | ESP32 pin |
| --- | --- |
| Positive (`+`) | GPIO 25 |
| Negative (`-`) | GND |

The current sketch is designed for a small active buzzer. Use a transistor driver if the buzzer requires more current or a higher voltage than an ESP32 GPIO can safely provide.

## 📚 Required Libraries

Install these libraries using the Arduino IDE Library Manager:

- Adafruit GFX Library
- Adafruit SH110X
- ESP32 BLE Arduino (included with the ESP32 Arduino core)

Also install **ESP32 by Espressif Systems** using the Arduino IDE Boards Manager.

## 🚀 Installation

1. Assemble the circuit according to the wiring tables.
2. Open the Arduino sketch in Arduino IDE.
3. Select the correct ESP32 board under **Tools > Board**.
4. Select the correct serial port.
5. Compile and upload the sketch.
6. The `STUDY` and `FORTUNE` menu will appear on the OLED.

The display uses the I2C address `0x3C`. If the OLED does not turn on, run an I2C scanner and update the address in the sketch if necessary.

## 🎮 Controls

### Main Menu

- Press **Left** to switch between `STUDY` and `FORTUNE`.
- Press **OK** to open the selected screen.

### Study Timer

- Press **OK** to start, pause, or resume the countdown.
- Press **Left** to reset the current session and return to the menu.
- When the countdown reaches `00:00`, the display shows `DONE!` and the buzzer sounds three times.
- Press **OK** after completion to restart the timer from the beginning.

### Fortune Screen

- Press **OK** to generate a new random fortune.
- Press **Left** to return to the main menu.

## 📶 BLE Timer Control

The ESP32 advertises as:

```text
PomodoroESP32
```

| Type | UUID |
| --- | --- |
| Service | `1234` |
| Write characteristic | `5678` |

Write a number between `1` and `60` to characteristic `5678`. The value represents the study duration in minutes. After receiving a valid value, the ESP32 opens the timer screen and starts the countdown automatically.

The BLE feature can be tested using an application such as nRF Connect.

## 🖥️ Display Modes

- **Menu:** Selects the study timer or fortune generator.
- **Study:** Shows the remaining time in `MM:SS` format.
- **Paused:** Keeps the current remaining time until the session is resumed.
- **Done:** Shows that the session has finished and triggers the buzzer.
- **Fortune:** Displays a randomly selected answer to “Will I pass the exam?”

## 💡 Possible Improvements

- Add automatic study and break cycles
- Save the selected duration in non-volatile memory
- Add debounced, non-blocking button handling
- Add buzzer enable/disable and volume controls
- Build a companion mobile application for BLE control
