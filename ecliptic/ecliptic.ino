#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display width and height
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// I2C pins
#define OLED_SDA 18
#define OLED_SCL 19
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Rotary encoder pins
#define ENCODER_CLK 4
#define ENCODER_DT 5
#define ENCODER_SW 6

int lastCLKState;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 20; // 20ms debounce time

int timerMinutes = 0; // Timer value in minutes

void setup() {
  Serial.begin(115200);

  // Initialize I2C communication for OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED initialization failed!");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Rotary Timer");
  display.display();

  // Set encoder pins to input with pull-ups
  pinMode(ENCODER_CLK, INPUT_PULLUP);
  pinMode(ENCODER_DT, INPUT_PULLUP);
  pinMode(ENCODER_SW, INPUT_PULLUP);

  // Read initial state of CLK pin
  lastCLKState = digitalRead(ENCODER_CLK);

  Serial.println("Rotary Encoder Timer Initialized");
}

void loop() {
  int currentCLKState = digitalRead(ENCODER_CLK);

  // Only act on a stable falling edge of the CLK signal
  if (lastCLKState == HIGH && currentCLKState == LOW && (millis() - lastDebounceTime) > debounceDelay) {
    lastDebounceTime = millis(); // Update debounce timer

    // Determine rotation direction based on DT pin state
    if (digitalRead(ENCODER_DT) == HIGH) {
      timerMinutes++;
    } else {
      timerMinutes--;
    }

    // Ensure timer value stays non-negative
    if (timerMinutes < 0) {
      timerMinutes = 0;
    }

    // Update display
    updateDisplay();
  }

  // Update last state
  lastCLKState = currentCLKState;

  // Check button press
  if (digitalRead(ENCODER_SW) == LOW) {
    Serial.println("Timer Started");
    startTimer(timerMinutes);
  }
}

void updateDisplay() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Set Timer:");
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print(timerMinutes);
  display.println(" min");
  display.display();
}

void startTimer(int minutes) {
  unsigned long startTime = millis();
  unsigned long targetTime = minutes * 60 * 1000; // Convert minutes to milliseconds

  while (millis() - startTime < targetTime) {
    unsigned long remainingTime = targetTime - (millis() - startTime);
    int remainingMinutes = remainingTime / (60 * 1000);
    int remainingSeconds = (remainingTime / 1000) % 60;

    // Update display
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("TR:");
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.print(remainingMinutes);
    display.print(":");
    if (remainingSeconds < 10) {
      display.print("0");
    }
    display.print(remainingSeconds);
    display.display();

    delay(200); // Slight delay for smoother updates
  }

  // Timer finished
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Timer Done!");
  display.display();

  Serial.println("Timer Finished");
}
