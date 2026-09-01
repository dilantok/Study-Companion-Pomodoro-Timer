#include "BleTimerService.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include "Config.h"

namespace {

class TimerWriteCallbacks : public BLECharacteristicCallbacks {
 public:
  explicit TimerWriteCallbacks(BleTimerService& timerService)
      : timerService_(timerService) {}

  void onWrite(BLECharacteristic* characteristic) override {
    const String value = characteristic->getValue();
    const int minutes = value.toInt();

    if (minutes >= config::MinimumBleMinutes &&
        minutes <= config::MaximumBleMinutes) {
      timerService_.queueRequestedMinutes(static_cast<uint8_t>(minutes));
    }
  }

 private:
  BleTimerService& timerService_;
};

}  // namespace

void BleTimerService::begin() {
  BLEDevice::init(config::BleDeviceName);
  BLEServer* server = BLEDevice::createServer();
  BLEService* service = server->createService(config::BleServiceUuid);
  BLECharacteristic* characteristic = service->createCharacteristic(
      config::BleCharacteristicUuid,
      BLECharacteristic::PROPERTY_WRITE);

  characteristic->setCallbacks(new TimerWriteCallbacks(*this));
  service->start();
  BLEDevice::getAdvertising()->start();
}

uint8_t BleTimerService::takeRequestedMinutes() {
  return requestedMinutes_.exchange(0);
}

void BleTimerService::queueRequestedMinutes(uint8_t minutes) {
  requestedMinutes_.store(minutes);
}

