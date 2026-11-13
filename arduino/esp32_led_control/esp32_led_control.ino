/*
 * Project: ESP32 Real-Time Object Detection System Using YOLO and USB Serial
 * Course: Microprocessor Systems / Machine Learning (Activity 4)
 * Target: ESP32 Development Board
 * 
 * Hardware Pins (Active HIGH):
 *   - GPIO 25 -> 220 ohm resistor -> Blue LED  -> GND  ('P' = Person)
 *   - GPIO 26 -> 220 ohm resistor -> Green LED -> GND  ('B' = Bottle)
 *   - GPIO 27 -> 220 ohm resistor -> Red LED   -> GND  ('C' = Cell phone)
 * 
 * Serial Protocol Contract:
 *   - Baud Rate: 115200
 *   - Format: 1 ASCII byte per message (Python -> ESP32)
 *   - Commands:
 *       'P' : Turn on Blue LED (Person), all other LEDs off
 *       'B' : Turn on Green LED (Bottle), all other LEDs off
 *       'C' : Turn on Red LED (Cell phone), all other LEDs off
 *       'N' : Turn all LEDs off (No object)
 *       Others: Ignored (e.g. \r, \n)
 */

// Pin Definitions according to README Contract
const int PIN_LED_BLUE  = 25;  // Person ('P')
const int PIN_LED_GREEN = 26;  // Bottle ('B')
const int PIN_LED_RED   = 27;  // Cell phone ('C')

// Turn off all LEDs
void turnAllOff() {
  digitalWrite(PIN_LED_BLUE, LOW);
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_LED_RED, LOW);
}

// Quick visual self-test on boot to verify LED wiring
void runSelfTest() {
  turnAllOff();
  
  digitalWrite(PIN_LED_BLUE, HIGH);
  delay(150);
  digitalWrite(PIN_LED_BLUE, LOW);
  
  digitalWrite(PIN_LED_GREEN, HIGH);
  delay(150);
  digitalWrite(PIN_LED_GREEN, LOW);
  
  digitalWrite(PIN_LED_RED, HIGH);
  delay(150);
  digitalWrite(PIN_LED_RED, LOW);
}

void setup() {
  // Initialize USB Serial communication at 115200 baud
  Serial.begin(115200);

  // Configure GPIO pins as outputs
  pinMode(PIN_LED_BLUE, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);

  // Initial state: all LEDs off
  turnAllOff();

  // Run hardware verification self-test (completes in ~450ms)
  runSelfTest();
}

void loop() {
  // Check if serial data has arrived from Python or Serial Monitor
  if (Serial.available() > 0) {
    char command = Serial.read();

    switch (command) {
      case 'P':
        // Person detected -> Blue LED ON
        turnAllOff();
        digitalWrite(PIN_LED_BLUE, HIGH);
        break;

      case 'B':
        // Bottle detected -> Green LED ON
        turnAllOff();
        digitalWrite(PIN_LED_GREEN, HIGH);
        break;

      case 'C':
        // Cell phone detected -> Red LED ON
        turnAllOff();
        digitalWrite(PIN_LED_RED, HIGH);
        break;

      case 'N':
        // Nothing detected -> All LEDs OFF
        turnAllOff();
        break;

      default:
        // Ignore whitespace, newlines, carriage returns, or other characters
        break;
    }
  }
}
