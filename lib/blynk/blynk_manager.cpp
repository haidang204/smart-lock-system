#define BLYNK_TEMPLATE_ID "TMPL6ckZ1P8Ag"
#define BLYNK_TEMPLATE_NAME "smart_lock"
#define BLYNK_AUTH_TOKEN "nhx-k2X_t0SPD4L0ToN2Yl2JpYb1_gWb"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "YEN LY";        // Thay bằng tên WiFi thật
char pass[] = "06052001";    // Thay bằng mật khẩu WiFi thật

#define VPIN_UNLOCK         V0
#define VPIN_ENROLL_FP      V1
#define VPIN_ENROLL_RFID    V2
#define VPIN_DELETE_ALL     V3
#define VPIN_STATUS         V4

bool enrollFP = false;
bool enrollRFID = false;
bool deleteAll = false;

void BlynkManager_sendStatus(const char* status);

void BlynkManager_setup() {
  Serial.begin(115200);  // Khởi tạo Serial Monitor
  WiFi.begin(ssid, pass);

  // Kết nối Wi-Fi và Blynk
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Đang kết nối WiFi...");
  }
  Serial.println("Đã kết nối WiFi!");
  Blynk.begin(auth, ssid, pass);
}

void BlynkManager_loop() {
  Blynk.run();
}

// Cập nhật lại hàm này với kiểu tham số là const char*
void BlynkManager_sendStatus(const char* status) {
  if (WiFi.status() == WL_CONNECTED) {  // Kiểm tra kết nối Wi-Fi
    Blynk.virtualWrite(VPIN_STATUS, status);  // Gửi dữ liệu đến Blynk App
    Serial.println("[Blynk] Gửi về app: " + String(status));
  } else {
    Serial.println("[Blynk] Lỗi kết nối Wi-Fi, không thể gửi dữ liệu.");
  }
}

bool BlynkManager_shouldEnrollFingerprint() {
  return enrollFP;
}

bool BlynkManager_shouldEnrollRFID() {
  return enrollRFID;
}

bool BlynkManager_shouldDeleteAll() {
  return deleteAll;
}

void BlynkManager_clearFlags() {
  enrollFP = false;
  enrollRFID = false;
  deleteAll = false;
}

BLYNK_WRITE(VPIN_ENROLL_FP) {
  enrollFP = param.asInt() == 1;
}

BLYNK_WRITE(VPIN_ENROLL_RFID) {
  enrollRFID = param.asInt() == 1;
}

BLYNK_WRITE(VPIN_DELETE_ALL) {
  deleteAll = param.asInt() == 1;
}

BLYNK_WRITE(VPIN_UNLOCK) {
  if (param.asInt() == 1) {
    BlynkManager_sendStatus("Mở cửa thủ công");
    // Gọi hàm mở cửa từ main.cpp nếu cần
  }
}
