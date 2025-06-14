#ifndef RFID_H
#define RFID_H

#include <Arduino.h>

// Khai báo extern cho các biến toàn cục (nếu có)
extern void (*unlockCallback)();  // Để sử dụng callback đã được định nghĩa ở nơi khác
extern int failedCount;           // Biến toàn cục failedCount từ RFID

// Các hàm cần thiết
void RFID_setup();                    // Khởi tạo RFID
String RFID_readCard();               // Đọc thẻ RFID và trả về UID
bool RFID_checkValid(String cardID);  // Kiểm tra thẻ có hợp lệ hay không
bool RFID_enroll();                   // Thêm thẻ mới vào bộ nhớ
void RFID_clearAll();                 // Xóa tất cả các thẻ đã lưu

#endif
