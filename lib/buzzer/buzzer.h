#ifndef BUZZER_H
#define BUZZER_H

void Buzzer_setup();                 // Khởi tạo chân
void Buzzer_beep();                  // Bíp ngắn (mở cửa thành công)
void Buzzer_alert(bool enable);     // Bật/tắt chế độ cảnh báo
void Buzzer_beepError();            // Bíp dài (lỗi xác thực)
void Buzzer_loop();                 // Gọi trong loop nếu có cảnh báo

#endif
