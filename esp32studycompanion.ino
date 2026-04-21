#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define SDA_PIN 21
#define SCL_PIN 22

// Buttons
#define BTN_LEFT  32
#define BTN_OK    33

// Timer
unsigned long startTime = 0;
unsigned long elapsedTime = 0;
unsigned long duration = 1500000; // default 25 min
bool running = false;

// Screens
// 0 = menu
// 1 = timer
// 2 = fortune
int screen = 0;
int menuIndex = 0;   // 0=STUDY, 1=FORTUNE

// Fortune answers
const char* fortunes[] = {
  "Yes.",
  "Very likely.",
  "Probably.",
  "Maybe.",
  "Keep studying.",
  "Not yet.",
  "Study harder."
};

String currentFortune = "";
bool fortuneGenerated = false;

// SETUP
void setup() {
  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(0x3C, false)) {
    while (1);  // Stop here if display init fails
  }

  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextWrap(false);
  display.display();

  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_OK, INPUT_PULLUP);

  randomSeed(micros());
}

// DRAW MENU
void drawMenu() {
  display.clearDisplay();
  display.setTextSize(2);

  display.setCursor(8, 12);
  if (menuIndex == 0) display.print(">STUDY");
  else display.print(" STUDY");

  display.setCursor(8, 38);
  if (menuIndex == 1) display.print(">FORTUNE");
  else display.print(" FORTUNE");

  display.display();
}

//DRAW TIMER 
void drawTimer(int minutes, int seconds) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(28, 2);
  display.print("Study Timer");

  display.setTextSize(2);
  display.setCursor(22, 24);

  if (minutes < 10) display.print("0");
  display.print(minutes);
  display.print(":");
  if (seconds < 10) display.print("0");
  display.print(seconds);

  display.display();
}

// DRAW FORTUNE
void drawFortune(const String &msg) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(20, 2);
  display.print("Study Fortune");

  display.setCursor(0, 18);
  display.print("Will I pass");

  display.setCursor(0, 30);
  display.print("the exam?");

  display.setCursor(0, 48);
  display.print(msg);

  display.display();
}

//GENERATE FORTUNE
void generateFortune() {
  int index = random(0, 7);
  currentFortune = fortunes[index];
  fortuneGenerated = true;
}

// LOOP
void loop() {

  // MENU 
  if (screen == 0) {
    if (digitalRead(BTN_LEFT) == LOW) {
      menuIndex++;
      if (menuIndex > 1) menuIndex = 0;
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

  // TIMER 
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
      screen = 0;
      delay(250);
    }

    unsigned long currentTime = running ? (millis() - startTime) : elapsedTime;
    unsigned long remaining = (duration > currentTime) ? (duration - currentTime) : 0;

    int minutes = remaining / 60000;
    int seconds = (remaining % 60000) / 1000;

    drawTimer(minutes, seconds);
  }

  // FORTUNE
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
