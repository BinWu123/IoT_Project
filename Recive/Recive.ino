#include <Arduino.h>

// Pin definitions for sounds
#define SNARE_PIN 5
#define HIHAT_PIN 18
#define BASS_PIN 19
#define CYMBAL_PIN 21

void setup() {
  Serial.begin(115200);

  // Initialize pins as outputs
  pinMode(SNARE_PIN, OUTPUT);
  pinMode(HIHAT_PIN, OUTPUT);
  pinMode(BASS_PIN, OUTPUT);
  pinMode(CYMBAL_PIN, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    String zone = Serial.readStringUntil('\n');
    zone.trim(); // Remove newline characters
    Serial.println("Received Zone: " + zone);

    // Trigger the appropriate sound
    if (zone == "Snare") {
      triggerSound(SNARE_PIN);
    } else if (zone == "Hi-hat") {
      triggerSound(HIHAT_PIN);
    } else if (zone == "Bass Drum") {
      triggerSound(BASS_PIN);
    } else if (zone == "Cymbal") {
      triggerSound(CYMBAL_PIN);
    }
  }
}

void triggerSound(int pin) {
  digitalWrite(pin, HIGH);
  delay(100); // Simulate sound duration
  digitalWrite(pin, LOW);
}
