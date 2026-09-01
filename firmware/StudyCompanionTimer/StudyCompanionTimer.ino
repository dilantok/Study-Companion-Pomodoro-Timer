#include "Alarm.h"
#include "AppScreen.h"
#include "BleTimerService.h"
#include "Button.h"
#include "Config.h"
#include "DisplayView.h"
#include "FortuneGenerator.h"
#include "TimerController.h"

DisplayView displayView;
TimerController timer;
BleTimerService bleTimerService;
FortuneGenerator fortuneGenerator;
Alarm alarm;
Button leftButton(config::LeftButtonPin);
Button okButton(config::OkButtonPin);

AppScreen currentScreen = AppScreen::Menu;
uint8_t menuIndex = 0;
const char* currentFortune = "";

void setup() {
  Serial.begin(115200);

  if (!displayView.begin()) {
    Serial.println("OLED display not found.");
    while (true) {
      delay(100);
    }
  }

  leftButton.begin();
  okButton.begin();
  alarm.begin();
  fortuneGenerator.begin();
  bleTimerService.begin();

  Serial.println("Companion Study Timer ready.");
}

void loop() {
  const uint32_t nowMs = millis();

  const uint8_t requestedMinutes = bleTimerService.takeRequestedMinutes();
  if (requestedMinutes > 0) {
    alarm.silence();
    timer.startForMinutes(requestedMinutes, nowMs);
    currentScreen = AppScreen::Timer;
  }

  const bool leftPressed = leftButton.wasPressed(nowMs);
  const bool okPressed = okButton.wasPressed(nowMs);

  switch (currentScreen) {
    case AppScreen::Menu:
      if (leftPressed) {
        menuIndex = (menuIndex + 1) % 2;
      }

      if (okPressed) {
        if (menuIndex == 0) {
          currentScreen = AppScreen::Timer;
        } else {
          currentFortune = fortuneGenerator.next();
          currentScreen = AppScreen::Fortune;
        }
      }

      displayView.drawMenu(menuIndex);
      break;

    case AppScreen::Timer:
      if (leftPressed) {
        timer.reset();
        alarm.silence();
        currentScreen = AppScreen::Menu;
        break;
      }

      if (okPressed) {
        alarm.silence();
        timer.toggle(nowMs);
      }

      if (timer.update(nowMs)) {
        alarm.playCompletion();
      }

      displayView.drawTimer(timer.remainingMs(nowMs), timer.status());
      break;

    case AppScreen::Fortune:
      if (leftPressed) {
        currentScreen = AppScreen::Menu;
        break;
      }

      if (okPressed) {
        currentFortune = fortuneGenerator.next();
      }

      displayView.drawFortune(currentFortune);
      break;
  }
}

