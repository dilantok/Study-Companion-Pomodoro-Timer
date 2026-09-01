#include "Button.h"

#include "Config.h"

Button::Button(uint8_t pin) : pin_(pin) {}

void Button::begin() {
  pinMode(pin_, INPUT_PULLUP);
  stableState_ = digitalRead(pin_);
  lastReading_ = stableState_;
}

bool Button::wasPressed(uint32_t nowMs) {
  const bool reading = digitalRead(pin_);

  if (reading != lastReading_) {
    lastReading_ = reading;
    lastChangeMs_ = nowMs;
  }

  if ((nowMs - lastChangeMs_) < config::ButtonDebounceMs ||
      reading == stableState_) {
    return false;
  }

  stableState_ = reading;
  return stableState_ == LOW;
}

