#pragma once

#include <Arduino.h>  // Để dùng String và các hàm của Arduino

// Hàm khởi tạo và vòng lặp Blynk
void BlynkManager_setup();       
void BlynkManager_loop();        

// Gửi trạng thái về ứng dụng Blynk
void BlynkManager_sendStatus(const char* msg);  // Dùng const char* cho ổn định khi dùng với Blynk

// Các cờ điều khiển từ ứng dụng
bool BlynkManager_shouldEnrollFingerprint();      
bool BlynkManager_shouldEnrollRFID();             
bool BlynkManager_shouldDeleteAll();              

// Xóa tất cả cờ điều khiển
void BlynkManager_clearFlags();                   
