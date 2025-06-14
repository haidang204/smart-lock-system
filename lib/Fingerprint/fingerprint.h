#ifndef FINGERPRINT_H
#define FINGERPRINT_H

#include <Arduino.h>

// Khai báo extern cho các biến toàn cục
extern void (*unlockCallback)();     // Hàm callback mở khóa
extern int failedCount;               // Đếm số lần thất bại
extern bool enrollMode;               // Biến kiểm tra chế độ đăng ký vân tay
extern uint8_t idToEnroll;            // ID vân tay cần thêm

// Các hàm cần thiết cho việc xử lý vân tay
void Fingerprint_setup();
bool Fingerprint_detected();
bool Fingerprint_verify();
bool Fingerprint_enroll();
void Fingerprint_clearAll();
void Fingerprint_setUnlockCallback(void (*callback)());
bool Fingerprint_isInEnrollMode();
void Fingerprint_setEnrollMode(bool mode);
int Fingerprint_getFailedCount();
void Fingerprint_resetFailedCount();
bool enrollFingerprint(uint8_t id);

#endif
