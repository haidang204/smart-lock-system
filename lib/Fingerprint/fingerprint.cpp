#include "fingerprint.h"
#include <Adafruit_Fingerprint.h>



HardwareSerial mySerial(2);  // Sử dụng UART2: GPIO17 (RX2), GPIO16 (TX2)
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int failedCount = 0;          // Đếm số lần xác thực sai
const int maxFails = 5;      // Số lần tối đa cho phép thất bại trước khi khóa

bool enrollMode = false;
uint8_t idToEnroll = 1;

void (*unlockCallback)() = nullptr;  // Callback khi mở khóa thành công

// Khởi tạo cảm biến vân tay
void Fingerprint_setup() {
  mySerial.begin(115200);
  finger.begin(115200);

  if (finger.verifyPassword()) {
    Serial.println("[Fingerprint] Kết nối thành công.");
  } else {
    Serial.println("[Fingerprint] Lỗi kết nối cảm biến.");
    //while(1);  // Dừng lại nếu không kết nối được với cảm biến
  }
}

// Gán hàm callback khi mở khóa
void Fingerprint_setUnlockCallback(void (*callback)()) {
  unlockCallback = callback;
}

// Kiểm tra xem có đang ở chế độ đăng ký vân tay không
bool Fingerprint_isInEnrollMode() {
  return enrollMode;
}

// Thiết lập chế độ đăng ký vân tay
void Fingerprint_setEnrollMode(bool mode) {
  enrollMode = mode;
  Serial.print("[Fingerprint] Chế độ thêm vân tay: ");
  Serial.println(mode ? "BẬT" : "TẮT");
}

// Lấy số lần thất bại
int Fingerprint_getFailedCount() {
  return failedCount;
}

// Đặt lại số lần thất bại
void Fingerprint_resetFailedCount() {
  failedCount = 0;
}

// Kiểm tra xem có phát hiện vân tay không
bool Fingerprint_detected() {
  return finger.getImage() == FINGERPRINT_OK;
}

// Xác thực vân tay
bool Fingerprint_verify() {
  if (finger.image2Tz() != FINGERPRINT_OK) return false;
  if (finger.fingerFastSearch() != FINGERPRINT_OK) {
    Serial.println("[Fingerprint]  Vân tay không đúng.");
    failedCount++;
    if (failedCount >= maxFails) {
      // Xử lý khi số lần thất bại vượt quá giới hạn
      Serial.println("[Fingerprint] Quá nhiều lần thất bại, khóa bị vô hiệu hóa.");
      // Thực hiện hành động khóa nếu cần (ví dụ: khóa lại cửa)
    }
    return false;
  }

  Serial.printf("[Fingerprint] Mở cửa! ID: %d\n", finger.fingerID);
  failedCount = 0; // Reset số lần thất bại
  if (unlockCallback != nullptr) unlockCallback();  // Gọi callback khi mở cửa thành công
  return true;
}

// Đăng ký vân tay mới
bool Fingerprint_enroll() {
  bool success = enrollFingerprint(idToEnroll);
  if (success) {
    Serial.println("[Fingerprint] Thêm vân tay thành công.");
    idToEnroll++;
  } else {
    Serial.println("[Fingerprint] Thêm vân tay thất bại.");
  }
  return success;
}

// Hàm xử lý việc đăng ký vân tay
bool enrollFingerprint(uint8_t id) {
  Serial.println("[Fingerprint] Đặt ngón tay lần 1...");
  while (finger.getImage() != FINGERPRINT_OK);

  if (finger.image2Tz(1) != FINGERPRINT_OK) return false;

  Serial.println("[Fingerprint] Nhấc tay ra...");
  delay(2000);

  Serial.println("[Fingerprint] Đặt lại ngón tay...");
  while (finger.getImage() != FINGERPRINT_OK);

  if (finger.image2Tz(2) != FINGERPRINT_OK) return false;
  if (finger.createModel() != FINGERPRINT_OK) return false;
  if (finger.storeModel(id) != FINGERPRINT_OK) return false;

  Serial.println("[Fingerprint] Đã lưu vân tay.");
  return true;
}

// Xóa toàn bộ vân tay đã lưu
void Fingerprint_clearAll() {
  Serial.println("[Fingerprint] Đang xóa toàn bộ vân tay...");
  for (int id = 1; id < 127; id++) {
    finger.deleteModel(id);
  }
  Serial.println("[Fingerprint] Đã xóa xong.");
}

void Fingerprint_loop() {
  // Không cần xử lý nữa vì main sẽ xử lý
}
