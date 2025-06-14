#include "rfid.h"
#include <MFRC522.h>
#include <SPI.h>
#include <EEPROM.h>

#define RST_PIN  22
#define SS_PIN   21

MFRC522 mfrc522(SS_PIN, RST_PIN);
const int maxCards = 20;
const int cardSize = 4;  // UID có độ dài cố định 4 byte

extern int failedCount;  // Định nghĩa biến toàn cục
const int maxFails = 5;

bool isAddingCard = false;
extern void (*unlockCallback)();  // Định nghĩa biến toàn cục

void RFID_setup() {
  SPI.begin();
  mfrc522.PCD_Init();
  EEPROM.begin(512);
  Serial.println("[RFID] Khởi tạo xong.");
}

bool compareUID(uint8_t *uid, uint8_t *stored) {
  for (int i = 0; i < cardSize; i++) {
    if (uid[i] != stored[i]) return false;
  }
  return true;
}

bool isCardRegistered(uint8_t *uid) {
  for (int i = 0; i < maxCards; i++) {
    int base = i * cardSize;
    uint8_t stored[cardSize];
    for (int j = 0; j < cardSize; j++) {
      stored[j] = EEPROM.read(base + j);
    }
    if (stored[0] == 0xFF) continue; // ô trống
    if (compareUID(uid, stored)) return true;
  }
  return false;
}

bool RFID_addCard(uint8_t *uid, uint8_t len) {
  if (len != cardSize) return false;
  if (isCardRegistered(uid)) return false;

  for (int i = 0; i < maxCards; i++) {
    int base = i * cardSize;
    if (EEPROM.read(base) == 0xFF) {
      for (int j = 0; j < cardSize; j++) {
        EEPROM.write(base + j, uid[j]);
      }
      EEPROM.commit();
      Serial.println("[RFID] Thêm thẻ mới thành công.");
      return true;
    }
  }

  Serial.println("[RFID] Bộ nhớ đầy.");
  return false;
}

void RFID_clearAll() {
  for (int i = 0; i < maxCards * cardSize; i++) {
    EEPROM.write(i, 0xFF);
  }
  EEPROM.commit();
  Serial.println("[RFID] Đã xóa toàn bộ thẻ.");
}

void RFID_setAddMode(bool state) {
  isAddingCard = state;
  Serial.printf("[RFID] Chế độ thêm thẻ: %s\n", state ? "BẬT" : "TẮT");
}

bool RFID_isInAddMode() {
  return isAddingCard;
}

void RFID_setUnlockCallback(void (*callback)()) {
  unlockCallback = callback;
}

int RFID_getFailedCount() {
  return failedCount;
}

void RFID_resetFailedCount() {
  failedCount = 0;
}

void RFID_loop() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) return;

  uint8_t *uid = mfrc522.uid.uidByte;
  if (mfrc522.uid.size != cardSize) return;

  Serial.print("[RFID] UID: ");
  for (int i = 0; i < cardSize; i++) {
    Serial.printf("%02X ", uid[i]);
  }
  Serial.println();

  if (isAddingCard) {
    if (isCardRegistered(uid)) {
      Serial.println("[RFID] Thẻ đã tồn tại.");
    } else {
      if (RFID_addCard(uid, cardSize)) {
        Serial.println("[RFID] Đã thêm thẻ mới.");
      }
    }
    isAddingCard = false;
  } else {
    if (isCardRegistered(uid)) {
      Serial.println("[RFID] Thẻ hợp lệ - Mở cửa.");
      failedCount = 0;
      if (unlockCallback != nullptr) unlockCallback(); // Gọi callback mở cửa
    } else {
      Serial.println("[RFID] Thẻ không hợp lệ.");
      failedCount++;
      if (failedCount >= maxFails) {
        Serial.println("[RFID] Cảnh báo: Quét sai quá nhiều lần.");
        // Có thể thêm cảnh báo như beep hoặc thông báo qua Blynk
      }
    }
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

// Hàm sửa lại RFID_readCard
String RFID_readCard() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String uid = "";
    for (int i = 0; i < cardSize; i++) {
      uid += String(mfrc522.uid.uidByte[i], HEX);  // Chuyển UID thành chuỗi Hex
    }
    return uid;
  }
  return "";
}

// Hàm sửa lại RFID_checkValid
bool RFID_checkValid(String uid) {
  for (int i = 0; i < maxCards; i++) {
    int base = i * cardSize;
    uint8_t stored[cardSize];
    for (int j = 0; j < cardSize; j++) {
      stored[j] = EEPROM.read(base + j);
    }
    if (stored[0] == 0xFF) continue;  // Skip empty slots
    String storedUID = "";
    for (int j = 0; j < cardSize; j++) {
      storedUID += String(stored[j], HEX);
    }
    if (storedUID == uid) {
      return true;  // Thẻ hợp lệ
    }
  }
  return false;  // Thẻ không hợp lệ
}

// Hàm thêm RFID_enroll để thêm thẻ vào bộ nhớ (nếu bạn cần thêm chức năng này)
bool RFID_enroll() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    uint8_t *uid = mfrc522.uid.uidByte;
    if (RFID_addCard(uid, mfrc522.uid.size)) {
      Serial.println("[RFID] Thêm thẻ mới thành công.");
      return true;
    } else {
      Serial.println("[RFID] Không thể thêm thẻ.");
      return false;
    }
  }
  return false;
}
