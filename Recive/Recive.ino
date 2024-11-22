#include <Arduino.h>

// Pin definitions for outputs (e.g., buzzers, LEDs)
#define SNARE_PIN 5
#define HIHAT_PIN 18
#define BASS_PIN 19
#define CYMBAL_PIN 21

void setup() {
  Serial.begin(115200);  // Initialize UART communication

  // Initialize pins as outputs
  pinMode(SNARE_PIN, OUTPUT);
  pinMode(HIHAT_PIN, OUTPUT);
  pinMode(BASS_PIN, OUTPUT);
  pinMode(CYMBAL_PIN, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    String zone = Serial.readStringUntil('\n');
    zone.trim();  // Remove any extra whitespace or newline characters
    Serial.println("Received Zone: " + zone);

    // Trigger the appropriate output
    if (zone == "Snare") {
      triggerOutput(SNARE_PIN);
    } else if (zone == "Hi-hat") {
      triggerOutput(HIHAT_PIN);
    } else if (zone == "Bass Drum") {
      triggerOutput(BASS_PIN);
    } else if (zone == "Cymbal") {
      triggerOutput(CYMBAL_PIN);
    }
  }
}

void triggerOutput(int pin) {
  digitalWrite(pin, HIGH);
  delay(100);  // Simulate output duration
  digitalWrite(pin, LOW);
}
