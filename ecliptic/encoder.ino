/*#define ENCODER_CLK 4
#define ENCODER_DT 5
#define ENCODER_SW 6

int lastCLKState;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 20; // 5ms debounce time

void setup() {
  Serial.begin(115200);

  // Set encoder pins to input with pull-ups
  pinMode(ENCODER_CLK, INPUT_PULLUP);
  pinMode(ENCODER_DT, INPUT_PULLUP);
  pinMode(ENCODER_SW, INPUT_PULLUP);

  // Read initial state of CLK pin
  lastCLKState = digitalRead(ENCODER_CLK);

  Serial.println("Rotary Encoder Initialized");
}

void loop() {
  int currentCLKState = digitalRead(ENCODER_CLK);

  // Only act on a stable falling edge of the CLK signal
  if (lastCLKState == HIGH && currentCLKState == LOW && (millis() - lastDebounceTime) > debounceDelay) {
    lastDebounceTime = millis(); // Update debounce timer

    // Determine rotation direction based on DT pin state
    if (digitalRead(ENCODER_DT) == HIGH) {
      Serial.println("Rotated Clockwise");
    } else {
      Serial.println("Rotated Counterclockwise");
    }
  }

  // Update last state
  lastCLKState = currentCLKState;

  // Check button press
  if (digitalRead(ENCODER_SW) == LOW) {
    Serial.println("Button Pressed");
    delay(200);  // Debounce delay for button
  }
}
*/