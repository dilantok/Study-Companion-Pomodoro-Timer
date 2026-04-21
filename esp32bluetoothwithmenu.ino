#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define SDA_PIN 21
#define SCL_PIN 22

// Buttons
#define BTN_LEFT  32
#define BTN_OK    33
#define BTN_RIGHT 25

// Timer
unsigned long startTime = 0;
unsigned long elapsedTime = 0;
unsigned long duration = 1500000;

bool running = false;

// MENU
int screen = 0;     // 0=menu,1=timer
int menuIndex = 0;  // 0=study,1=quit

// BLE
BLECharacteristic *pCharacteristic;

class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {

    String value = pCharacteristic->getValue();

    if (value.length() > 0) {
      int minutes = value.toInt();

      if (minutes > 0 && minutes <= 60) {
        duration = minutes * 60000;

        startTime = millis();
        elapsedTime = 0;
        running = true;
        screen = 1;

        Serial.println("Yeni sure: " + String(minutes));
      }
    }
  }
};

void setup() {
  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(0x3C, true)) {
    while (1);
  }

  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);

  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_OK, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);

  // BLE
  BLEDevice::init("PomodoroESP32");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService("1234");

  pCharacteristic = pService->createCharacteristic(
                      "5678",
                      BLECharacteristic::PROPERTY_WRITE
                    );

  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  BLEDevice::getAdvertising()->start();
}

// -------- MENU --------
void drawMenu() {
  display.clearDisplay();

  display.setCursor(10, 10);
  if (menuIndex == 0) display.print("> ");
  else display.print("  ");
  display.println("STUDY");

  if (menuIndex == 1) display.print("> ");
  else display.print("  ");
  display.println("QUIT");

  display.display();
}

// -------- TIMER --------
void drawTimer(int minutes, int seconds) {
  display.clearDisplay();

  display.setCursor(10, 20);
  display.print("Time:");

  display.setCursor(10, 40);
  if (minutes < 10) display.print("0");
  display.print(minutes);
  display.print(":");
  if (seconds < 10) display.print("0");
  display.print(seconds);

  display.display();
}

void loop() {

  // -------- MENU --------
  if (screen == 0) {

    if (digitalRead(BTN_LEFT) == LOW) {
      menuIndex--;
      if (menuIndex < 0) menuIndex = 1;
      delay(200);
    }

    if (digitalRead(BTN_RIGHT) == LOW) {
      menuIndex++;
      if (menuIndex > 1) menuIndex = 0;
      delay(200);
    }

    if (digitalRead(BTN_OK) == LOW) {
      if (menuIndex == 0) {
        screen = 1; // STUDY
      }
      else if (menuIndex == 1) {
        ESP.restart(); // QUIT
      }
      delay(250);
    }

    drawMenu();
  }

  // -------- TIMER --------
  else if (screen == 1) {

    if (digitalRead(BTN_OK) == LOW) {
      if (!running) {
        startTime = millis() - elapsedTime;
        running = true;
      } else {
        elapsedTime = millis() - startTime;
        running = false;
      }
      delay(250);
    }

    if (digitalRead(BTN_LEFT) == LOW) {
      running = false;
      elapsedTime = 0;
      screen = 0; // geri menu
      delay(250);
    }

    unsigned long currentTime = running ? (millis() - startTime) : elapsedTime;
    unsigned long remaining = (duration > currentTime) ? (duration - currentTime) : 0;

    int minutes = remaining / 60000;
    int seconds = (remaining % 60000) / 1000;

    drawTimer(minutes, seconds);
  }
}