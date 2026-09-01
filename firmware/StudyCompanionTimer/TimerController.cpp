#include "TimerController.h"

#include "Config.h"

TimerController::TimerController()
    : durationMs_(config::DefaultDurationMs) {}

void TimerController::toggle(uint32_t nowMs) {
  if (running_) {
    pausedElapsedMs_ = elapsedMs(nowMs);
    running_ = false;
    return;
  }

  if (pausedElapsedMs_ >= durationMs_) {
    pausedElapsedMs_ = 0;
  }

  startTimeMs_ = nowMs - pausedElapsedMs_;
  running_ = true;
}

void TimerController::startForMinutes(uint8_t minutes, uint32_t nowMs) {
  durationMs_ = static_cast<uint32_t>(minutes) * 60UL * 1000UL;
  pausedElapsedMs_ = 0;
  startTimeMs_ = nowMs;
  running_ = true;
}

void TimerController::reset() {
  pausedElapsedMs_ = 0;
  running_ = false;
}

bool TimerController::update(uint32_t nowMs) {
  if (!running_ || elapsedMs(nowMs) < durationMs_) {
    return false;
  }

  pausedElapsedMs_ = durationMs_;
  running_ = false;
  return true;
}

uint32_t TimerController::remainingMs(uint32_t nowMs) const {
  const uint32_t elapsed = elapsedMs(nowMs);
  return elapsed < durationMs_ ? durationMs_ - elapsed : 0;
}

TimerStatus TimerController::status() const {
  if (running_) {
    return TimerStatus::Running;
  }

  return pausedElapsedMs_ >= durationMs_
             ? TimerStatus::Finished
             : TimerStatus::Paused;
}

uint32_t TimerController::elapsedMs(uint32_t nowMs) const {
  return running_ ? nowMs - startTimeMs_ : pausedElapsedMs_;
}

