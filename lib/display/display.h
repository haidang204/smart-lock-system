#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>


void Display_setup();                       // Khởi tạo LCD
void Display_show(const String &line1, const String &line2 = "");  // Hiển thị 2 dòng
void Display_clear();                       // Xóa màn hình

#endif
