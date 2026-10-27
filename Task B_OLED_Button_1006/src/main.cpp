#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Screen and pin configuration
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS  0x3C   // I2C address of the OLED
#define SDA_PIN       21
#define SCL_PIN       22
#define BUTTON_PIN    18     // Push button connected to GPIO18 and GND

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Variables used for counting and button checking
int pressCount = 0;                       
int lastReading = HIGH;                   
int stableState = HIGH;                   
unsigned long lastChangeTime = 0;         
const unsigned long debounceDelay = 50;   

// Function to show button status and count on OLED
void updateDisplay() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("ESP32 Button Counter");

  display.setCursor(0, 20);
  if (stableState == LOW) {
    display.println("Button: PRESSED");
  } else {
    display.println("Button: RELEASED");
  }

  display.setTextSize(2);
  display.setCursor(0, 42);
  display.print("Count:");
  display.print(pressCount);

  display.display();   // Display the updated information
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);      
  Wire.begin(SDA_PIN, SCL_PIN);

  // Start the OLED screen
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("OLED not found! Check wiring.");
    while (true) { }                      
  }

  Serial.println("System started. Count = 0");
  updateDisplay();   // Display the initial count
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);

  // Check whether the button reading has changed
  if (reading != lastReading) {
    lastChangeTime = millis();
    lastReading = reading;
  }

  // Wait until the button signal remains stable for 50 ms
  if ((millis() - lastChangeTime) > debounceDelay) {

    // Compare the current reading with the previous stable state
    if (reading != stableState) {
      stableState = reading;

      // Increase the count when the button is pressed
      if (stableState == LOW) {
        pressCount++;
        Serial.print("Press accepted! Count = ");
        Serial.println(pressCount);
      }

      updateDisplay();   // Refresh the OLED screen
    }
  }
}