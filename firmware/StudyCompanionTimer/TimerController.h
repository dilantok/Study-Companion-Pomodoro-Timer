#pragma once

#include <Arduino.h>

enum class TimerStatus {
  Paused,
  Running,
  Finished
};

class TimerController {
 public:
  TimerController();

  void toggle(uint32_t nowMs);
  void startForMinutes(uint8_t minutes, uint32_t nowMs);
  void reset();

  // Returns true exactly once when a running timer reaches zero.
  bool update(uint32_t nowMs);

  uint32_t remainingMs(uint32_t nowMs) const;
  TimerStatus status() const;

 private:
  uint32_t elapsedMs(uint32_t nowMs) const;

  uint32_t durationMs_;
  uint32_t startTimeMs_ = 0;
  uint32_t pausedElapsedMs_ = 0;
  bool running_ = false;
};

