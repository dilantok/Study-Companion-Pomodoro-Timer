#include "DisplayView.h"

#include <Adafruit_GFX.h>
#include <Wire.h>

#include "Config.h"

DisplayView::DisplayView()
    : display_(config::ScreenWidth, config::ScreenHeight, &Wire, -1) {}

bool DisplayView::begin() {
  Wire.begin(config::SdaPin, config::SclPin);

  if (!display_.begin(config::ScreenAddress, true)) {
    return false;
  }

  display_.clearDisplay();
  display_.setTextColor(SH110X_WHITE);
  display_.setTextWrap(false);
  display_.display();
  return true;
}

void DisplayView::drawMenu(uint8_t selectedItem) {
  display_.clearDisplay();
  display_.setTextSize(2);

  display_.setCursor(8, 12);
  display_.print(selectedItem == 0 ? ">STUDY" : " STUDY");

  display_.setCursor(8, 38);
  display_.print(selectedItem == 1 ? ">FORTUNE" : " FORTUNE");
  display_.display();
}

void DisplayView::drawTimer(uint32_t remainingMs, TimerStatus status) {
  const uint16_t totalSeconds = remainingMs / 1000UL;
  const uint8_t minutes = totalSeconds / 60;
  const uint8_t seconds = totalSeconds % 60;

  display_.clearDisplay();
  display_.setTextSize(1);
  display_.setCursor(44, 12);

  switch (status) {
    case TimerStatus::Running:
      display_.print("STUDY");
      break;
    case TimerStatus::Finished:
      display_.print("DONE!");
      break;
    case TimerStatus::Paused:
      display_.print("PAUSED");
      break;
  }

  display_.setTextSize(2);
  display_.setCursor(22, 24);

  if (minutes < 10) {
    display_.print('0');
  }
  display_.print(minutes);
  display_.print(':');
  if (seconds < 10) {
    display_.print('0');
  }
  display_.print(seconds);
  display_.display();
}

void DisplayView::drawFortune(const char* message) {
  display_.clearDisplay();
  display_.setTextSize(1);
  display_.setCursor(0, 18);
  display_.print("Will I pass");
  display_.setCursor(0, 30);
  display_.print("the exam?");
  display_.setCursor(0, 48);
  display_.print(message);
  display_.display();
}

