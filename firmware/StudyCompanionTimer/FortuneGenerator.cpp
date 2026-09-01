#include "FortuneGenerator.h"

#include <Arduino.h>

namespace {

const char* const Fortunes[] = {
    "Yes.",
    "Very likely.",
    "Probably bruh.",
    "Maybe, idk man.",
    "Keep studying.",
    "NOPE HAHAHA.",
    "Study harder."};

constexpr size_t FortuneCount = sizeof(Fortunes) / sizeof(Fortunes[0]);

}  // namespace

void FortuneGenerator::begin() {
  randomSeed(micros());
}

const char* FortuneGenerator::next() const {
  return Fortunes[random(0, FortuneCount)];
}

