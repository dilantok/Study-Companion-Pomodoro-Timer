#pragma once

#include <Arduino.h>
#include <atomic>

class BleTimerService {
 public:
  void begin();

  // Returns zero when no new duration has been received.
  uint8_t takeRequestedMinutes();
  void queueRequestedMinutes(uint8_t minutes);

 private:
  std::atomic<uint8_t> requestedMinutes_{0};
};

