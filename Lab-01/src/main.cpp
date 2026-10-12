

// #include <Arduino.h>

// #define LED_BUILTIN 2
// void setup() {
//   pinMode(LED_BUILTIN, OUTPUT);
// }

// void loop() {
//   digitalWrite(LED_BUILTIN, HIGH);
//   delay(1000);
//   digitalWrite(LED_BUILTIN, LOW);
//   delay(1000);
// }

#include <Arduino.h>

void setup() {
  pinMode(2, OUTPUT);
  
  // Initialize serial communication at 115200 baud
  Serial.begin(115200);
  Serial.println("ESP32 Simulation Started!");
}

void loop() {
  digitalWrite(2, HIGH);   
  Serial.println("LED State: ON");
  delay(1000);             
  
  digitalWrite(2, LOW);    
  Serial.println("LED State: OFF");
  delay(1000);             
}