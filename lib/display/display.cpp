#include "display.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Địa chỉ I2C thông thường là 0x27 hoặc 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);

void Display_setup() {
  lcd.init();       // Khởi tạo LCD
  lcd.backlight();  // Bật đèn nền
  Display_show("Smart Door", "Dang khoi dong...");
  delay(10000);
}

void Display_show(const String &line1, const String &line2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

void Display_clear() {
  lcd.clear();
}
