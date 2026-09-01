#include "Alarm.h"

#include <Arduino.h>

#include "Config.h"

void Alarm::begin() {
  pinMode(config::BuzzerPin, OUTPUT);
  silence();
}

void Alarm::playCompletion() {
  const uint16_t notes[] = {1319, 1568, 1976, 2637, 1976, 2637};
  const uint16_t durations[] = {100, 100, 100, 220, 100, 300};

  for (size_t i = 0; i < sizeof(notes) / sizeof(notes[0]); ++i) {
    tone(config::BuzzerPin, notes[i]);
    delay(durations[i]);
    noTone(config::BuzzerPin);
    delay(35);
  }
}

void Alarm::silence() {
  noTone(config::BuzzerPin);
  digitalWrite(config::BuzzerPin, LOW);
}

