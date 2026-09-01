#pragma once

#include <Arduino.h>

namespace config {

constexpr uint8_t ScreenWidth = 128;
constexpr uint8_t ScreenHeight = 64;
constexpr uint8_t ScreenAddress = 0x3C;

constexpr uint8_t SdaPin = 21;
constexpr uint8_t SclPin = 22;
constexpr uint8_t LeftButtonPin = 32;
constexpr uint8_t OkButtonPin = 33;
constexpr uint8_t BuzzerPin = 25;

constexpr uint32_t DefaultDurationMs = 25UL * 60UL * 1000UL;
constexpr uint16_t ButtonDebounceMs = 35;
constexpr uint8_t MinimumBleMinutes = 1;
constexpr uint8_t MaximumBleMinutes = 60;

constexpr char BleDeviceName[] = "PomodoroESP32";
constexpr char BleServiceUuid[] = "1234";
constexpr char BleCharacteristicUuid[] = "5678";

}  // namespace config

