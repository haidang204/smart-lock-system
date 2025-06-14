#include <Arduino.h>

// ========== BLYNK CONFIG ==========
#define BLYNK_TEMPLATE_ID "TMPL6ckZ1P8Ag"
#define BLYNK_TEMPLATE_NAME "smart_lock"
#define BLYNK_AUTH_TOKEN "nhx-k2X_t0SPD4L0ToN2Yl2JpYb1_gWb"

// ========== MODULE LIBRARIES ==========
#include "display.h"
#include "buzzer.h"
#include "relay_control.h"
#include "blynk_manager.h"
#include "fingerprint.h"
#include "rfid.h"
#include "smartlocksystem.h"

// ========== SYSTEM VARIABLES ==========
const int MAX_FAILED_ATTEMPTS = 5;
int failedAttempts = 0;

// ========== DOOR CONTROL ==========
void unlockDoor() {
  Display_show("Mo cua", "Thanh cong");
  Buzzer_beep();
  Relay_unlock();
  BlynkManager_sendStatus("Mo cua thanh cong");
  failedAttempts = 0;
}

void handleFailed() {
  failedAttempts++;
  Buzzer_beepError();

  if (failedAttempts >= MAX_FAILED_ATTEMPTS) {
    Display_show("Canh bao!", "Sai qua nhieu");
    Buzzer_alert(true);
    BlynkManager_sendStatus("Sai qua nhieu lan!");
  }
}

// ========== INIT ==========
void smartHomeSystemInit() {
  Serial.begin(115200);

  Display_setup();
  Display_show("Smart Door", "Khoi dong...");

  Fingerprint_setup();
  RFID_setup();
  Relay_setup();
  Buzzer_setup();
  BlynkManager_setup();

  delay(1000);
  Display_show("San sang", "Quet de vao");
}

// ========== LOOP ==========
void smartHomeSystemUpdate() {
  BlynkManager_loop();
  Buzzer_loop();

  // Kiểm tra vân tay
  if (Fingerprint_detected()) {
    if (Fingerprint_verify()) {
      unlockDoor();
    } else {
      Display_show("Sai van tay!", "");
      BlynkManager_sendStatus("Sai van tay");
      handleFailed();
    }
  }

  // Kiểm tra thẻ RFID
  String cardID = RFID_readCard();
  if (cardID != "") {
    if (RFID_checkValid(cardID)) {
      unlockDoor();
    } else {
      Display_show("The khong hop le", "");
      BlynkManager_sendStatus("The RFID khong hop le");
      handleFailed();
    }
  }

  // Thêm vân tay
  if (BlynkManager_shouldEnrollFingerprint()) {
    Display_show("Them van tay", "");
    if (Fingerprint_enroll()) {
      Display_show("Them thanh cong", "");
      BlynkManager_sendStatus("Them van tay OK");
    } else {
      Display_show("Them that bai", "");
      BlynkManager_sendStatus("Them van tay that bai");
    }
    BlynkManager_clearFlags();
  }

  // Thêm thẻ RFID
  if (BlynkManager_shouldEnrollRFID()) {
    Display_show("Cho quet the", "");
    if (RFID_enroll()) {
      Display_show("The da luu", "");
      BlynkManager_sendStatus("Them the RFID OK");
    } else {
      Display_show("Loi hoac huy", "");
      BlynkManager_sendStatus("Them the RFID that bai");
    }
    BlynkManager_clearFlags();
  }

  // Xóa dữ liệu
  if (BlynkManager_shouldDeleteAll()) {
    Display_show("Xoa du lieu", "");
    Fingerprint_clearAll();
    RFID_clearAll();
    BlynkManager_sendStatus("Da xoa tat ca du lieu");
    Display_show("Da xoa xong", "");
    delay(1500);
    Display_show("San sang", "Quet de vao");
    BlynkManager_clearFlags();
  }
}
