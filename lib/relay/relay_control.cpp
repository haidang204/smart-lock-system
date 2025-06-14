#include "relay_control.h"
#include <Arduino.h>

#define RELAY_PIN 26        // Chân điều khiển relay
#define RELAY_TIME 5000     // Thời gian mở cửa (ms)

unsigned long unlockTime = 0;
bool isUnlocked = false;

void Relay_setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Mặc định tắt relay
}

void Relay_unlock() {
  Serial.println("[Relay]  Mở khóa");
  digitalWrite(RELAY_PIN, HIGH);
  unlockTime = millis();
  isUnlocked = true;
}

void Relay_loop() {
  if (isUnlocked && millis() - unlockTime > RELAY_TIME) {
    digitalWrite(RELAY_PIN, LOW);
    isUnlocked = false;
    Serial.println("[Relay]  Đóng khóa lại");
  }
}
