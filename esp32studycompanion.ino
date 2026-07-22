#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

// ---------- OLED ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_SH1106G display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

// ---------- BUTTONS ----------
#define BTN_LEFT 32
#define BTN_OK   33

// ---------- BUZZER ----------
#define BUZZER_PIN 25

// ---------- TIMER ----------
unsigned long startTime = 0;
unsigned long elapsedTime = 0;

// Default: 25 minutes
unsigned long duration = 1500000UL;

bool running = false;
bool alarmPlayed = false;

// ---------- SCREENS ----------
// 0 = Menu
// 1 = Timer
// 2 = Fortune
int screen = 0;
int menuIndex = 0;

// ---------- BLE ----------
BLECharacteristic *pCharacteristic;

// ---------- FORTUNE ----------
const char* fortunes[] = {
  "Yes.",
  "Very likely.",
  "Probably bruh.",
  "Maybe, idk man.",
  "Keep studying.",
  "NOPE HAHAHA.",
  "Study harder."
};

String currentFortune = "";
bool fortuneGenerated = false;

// ---------- BLE CALLBACK ----------
class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    String value = pCharacteristic->getValue();

    if (value.length() > 0) {
      int minutes = value.toInt();

      if (minutes > 0 && minutes <= 60) {
        duration = (unsigned long)minutes * 60000UL;

        startTime = millis();
        elapsedTime = 0;

        running = true;
        alarmPlayed = false;

        digitalWrite(BUZZER_PIN, LOW);

        screen = 1;
      }
    }
  }
};

// ---------- SETUP ----------
void setup() {
  Serial.begin(115200);

  // OLED
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(0x3C, true)) {
    Serial.println("OLED bulunamadi!");

    while (true) {
      delay(100);
    }
  }

  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextWrap(false);
  display.display();

  // Buttons
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_OK, INPUT_PULLUP);

  // Buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Random fortune seed
  randomSeed(micros());

  // BLE
  BLEDevice::init("PomodoroESP32");

  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService =
    pServer->createService("1234");

  pCharacteristic =
    pService->createCharacteristic(
      "5678",
      BLECharacteristic::PROPERTY_WRITE
    );

  pCharacteristic->setCallbacks(new MyCallbacks());

  pService->start();

  BLEDevice::getAdvertising()->start();

  Serial.println("PomodoroESP32 hazir.");
}

// ---------- DRAW MENU ----------
void drawMenu() {
  display.clearDisplay();
  display.setTextSize(2);

  display.setCursor(8, 12);

  if (menuIndex == 0) {
    display.print(">STUDY");
  } else {
    display.print(" STUDY");
  }

  display.setCursor(8, 38);

  if (menuIndex == 1) {
    display.print(">FORTUNE");
  } else {
    display.print(" FORTUNE");
  }

  display.display();
}

// ---------- DRAW TIMER ----------
void drawTimer(int minutes, int seconds) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(44, 12);

  if (running) {
    display.print("STUDY");
  } else if (elapsedTime >= duration) {
    display.print("DONE!");
  } else {
    display.print("PAUSED");
  }

  display.setTextSize(2);
  display.setCursor(22, 24);

  if (minutes < 10) {
    display.print("0");
  }

  display.print(minutes);
  display.print(":");

  if (seconds < 10) {
    display.print("0");
  }

  display.print(seconds);

  display.display();
}

// ---------- DRAW FORTUNE ----------
void drawFortune(const String &msg) {
  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(0, 18);
  display.print("Will I pass");

  display.setCursor(0, 30);
  display.print("the exam?");

  display.setCursor(0, 48);
  display.print(msg);

  display.display();
}

// ---------- GENERATE FORTUNE ----------
void generateFortune() {
  int fortuneCount =
    sizeof(fortunes) / sizeof(fortunes[0]);

  int index = random(0, fortuneCount);

  currentFortune = fortunes[index];
  fortuneGenerated = true;
}

// ---------- PLAY ALARM ----------
void playAlarm() {
  if (alarmPlayed) {
    return;
  }

  alarmPlayed = true;

  // Three short beeps
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(250);

    digitalWrite(BUZZER_PIN, LOW);
    delay(150);
  }
}

// ---------- LOOP ----------
void loop() {

  // ========================================
  // MENU
  // ========================================
  if (screen == 0) {

    if (digitalRead(BTN_LEFT) == LOW) {
      menuIndex++;

      if (menuIndex > 1) {
        menuIndex = 0;
      }

      delay(220);
    }

    if (digitalRead(BTN_OK) == LOW) {

      if (menuIndex == 0) {
        screen = 1;
      } else {
        screen = 2;
        fortuneGenerated = false;
      }

      delay(250);
    }

    drawMenu();
  }

  // ========================================
  // TIMER
  // ========================================
  else if (screen == 1) {

    // Start, pause or restart timer
    if (digitalRead(BTN_OK) == LOW) {

      if (!running) {

        // If timer has finished, restart from beginning
        if (elapsedTime >= duration) {
          elapsedTime = 0;
        }

        startTime = millis() - elapsedTime;

        running = true;
        alarmPlayed = false;

        digitalWrite(BUZZER_PIN, LOW);

      } else {

        elapsedTime = millis() - startTime;
        running = false;
      }

      delay(250);
    }

    // Return to menu and reset timer
    if (digitalRead(BTN_LEFT) == LOW) {
      running = false;
      elapsedTime = 0;
      alarmPlayed = false;

      digitalWrite(BUZZER_PIN, LOW);

      screen = 0;

      delay(250);
    }

    unsigned long currentTime;

    if (running) {
      currentTime = millis() - startTime;
    } else {
      currentTime = elapsedTime;
    }

    unsigned long remaining;

    if (duration > currentTime) {
      remaining = duration - currentTime;
    } else {
      remaining = 0;
    }

    // Timer finished
    if (remaining == 0 && running) {
      running = false;
      elapsedTime = duration;

      playAlarm();
    }

    int minutes = remaining / 60000UL;
    int seconds = (remaining % 60000UL) / 1000UL;

    drawTimer(minutes, seconds);
  }

  // ========================================
  // FORTUNE
  // ========================================
  else if (screen == 2) {

    if (!fortuneGenerated) {
      generateFortune();
    }

    if (digitalRead(BTN_OK) == LOW) {
      generateFortune();
      delay(250);
    }

    if (digitalRead(BTN_LEFT) == LOW) {
      screen = 0;
      fortuneGenerated = false;
      delay(250);
    }

    drawFortune(currentFortune);
  }
}
