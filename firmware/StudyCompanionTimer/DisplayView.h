#pragma once

#include <Adafruit_SH110X.h>
#include <Arduino.h>

#include "TimerController.h"

class DisplayView {
 public:
  DisplayView();

  bool begin();
  void drawMenu(uint8_t selectedItem);
  void drawTimer(uint32_t remainingMs, TimerStatus status);
  void drawFortune(const char* message);

 private:
  Adafruit_SH1106G display_;
};

