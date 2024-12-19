#include <esp_now.h>
#include <WiFi.h>
#include "AudioFileSourceSD_MMC.h"
#include "AudioOutputI2S.h"
#include "AudioGeneratorMP3.h"
#include "SD_MMC.h"
#include "FS.h"

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    char a[32];
    int b;
    float c;
    bool d;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Declare pointers for the MP3 generator, file source, and output.
AudioGeneratorMP3 *mp3;
AudioFileSourceSD_MMC *file;
AudioOutputI2S *out;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("String: ");
  Serial.println(myData.a);
  Serial.print("detection: ");
  Serial.println(myData.b);
  Serial.print("inference: ");
  Serial.println(myData.c);
  Serial.print("detected: ");
  Serial.println(myData.d);
  Serial.println();

  // Play audio when data is received
  if (myData.b == 2) {
    if (!mp3->isRunning()) {
      file->close();
      file->open("/snaredrum.mp3");
      mp3->begin(file, out);
    }
  }
  if (myData.b == 1) {
    if (!mp3->isRunning()) {
      file->close();
      file->open("/kickdrum.mp3");
      mp3->begin(file, out);
    }
  }
  
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  delay(1000);
  Serial.println("Beginning serial");

  // Initialize the SD card. If it fails, print an error message.
  if (!SD_MMC.begin()) {
    Serial.println("SD card mount failed!");
  } else {
    Serial.println("SD mounted!");
  }

  // Open the MP3 file from the SD card.
  file = new AudioFileSourceSD_MMC("/cymbal.mp3");

  // Set up the I2S output on ESP32's internal DAC.
  out = new AudioOutputI2S(0, 1);

  // Set the output to mono.
  out->SetOutputModeMono(true);

  // Initialize the MP3 generator with the file and output.
  mp3 = new AudioGeneratorMP3();
  mp3->begin(file, out);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // If the MP3 is running, loop it. Otherwise, stop it.
  
  if (mp3->isRunning()) {
    if (!mp3->loop()) {
      mp3->stop();
    }
  }
  
}