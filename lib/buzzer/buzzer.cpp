#include "buzzer.h"
#include <Arduino.h>

#define BUZZER_PIN 27          // Chân nối với buzzer
#define BEEP_TIME 100          // Thời gian bíp ngắn (ms)
#define ALERT_INTERVAL 200     // Tần suất cảnh báo (ms)
#define ERROR_BEEP_TIME 500    // Thời gian bíp dài cho lỗi (ms)

unsigned long lastBeepTime = 0;   // Lưu thời gian bíp gần nhất
bool isBeeping = false;           // Biến kiểm tra trạng thái bíp
bool alertMode = false;           // Biến kiểm tra chế độ cảnh báo

void Buzzer_setup() {
  pinMode(BUZZER_PIN, OUTPUT);    // Cài đặt chân BUZZER_PIN làm đầu ra
  digitalWrite(BUZZER_PIN, LOW);  // Đảm bảo buzzer tắt khi khởi động
  Serial.begin(115200);           // Khởi tạo Serial Monitor để theo dõi
}

void Buzzer_beep() {
  digitalWrite(BUZZER_PIN, HIGH);  // Bật buzzer
  delay(BEEP_TIME);                // Chờ thời gian bíp ngắn
  digitalWrite(BUZZER_PIN, LOW);   // Tắt buzzer
}

void Buzzer_alert(bool enable) {
  alertMode = enable;  // Cập nhật trạng thái cảnh báo
  if (!enable) {
    digitalWrite(BUZZER_PIN, LOW);  // Tắt buzzer khi tắt cảnh báo
    isBeeping = false;
    Serial.println("Buzzer đã tắt cảnh báo.");
  }
}

void Buzzer_beepError() {
  digitalWrite(BUZZER_PIN, HIGH);  // Bật buzzer
  delay(ERROR_BEEP_TIME);          // Chờ thời gian bíp dài (lỗi)
  digitalWrite(BUZZER_PIN, LOW);   // Tắt buzzer
  Serial.println("Buzzer báo lỗi.");
}

void Buzzer_loop() {
  if (alertMode) {
    unsigned long now = millis();
    if (!isBeeping && now - lastBeepTime > ALERT_INTERVAL) {
      digitalWrite(BUZZER_PIN, HIGH);  // Bật buzzer khi cảnh báo
      isBeeping = true;
      lastBeepTime = now;
      Serial.println("Buzzer cảnh báo bắt đầu.");
    } else if (isBeeping && now - lastBeepTime > BEEP_TIME) {
      digitalWrite(BUZZER_PIN, LOW);  // Tắt buzzer sau khi bíp
      isBeeping = false;
      lastBeepTime = now;
      Serial.println("Buzzer cảnh báo tắt.");
    }
  }
}
