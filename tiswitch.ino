#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include <HTTPClient.h>

// wifiのssidとパスワードを定義
const char *ssid = "***********";
const char *password = "*********";
const char html[] = "<!DOCTYPE html><html lang='ja'><head><meta charset='UTF-8'></head>\
<body>not Browser Access</body></html>";
HTTPClient http;
int threshold = 60;
bool touchdetected = false;

Servo servo;

WebServer server(80);

void setup()
{
  Serial.begin(115200);
  // sg90サーボをセット
  servo.attach(15, 500, 2400);
  delay(10);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  // ルートアクセス時の挙動を定義
  server.on("/", handleRoot);
  //　サーバ起動
  server.begin();
  // タッチセンサにハンドラを追加
  touchAttachInterrupt(T6, touchHandler, threshold);
}

void loop()
{

  server.handleClient();

  if (touchdetected)
  {
    touchdetected = false;
    moveServo();
    Serial.println(flag);
    delay(100);
  }
}
void touchHandler()
{
  touchdetected = true;
}

void handleRoot()
{
  server.send(200, "text/plain", html);
  moveServo();
}

void moveServo()
{
  servo.write(80);
  delay(300);
  servo.write(10);
  delay(300);
  Serial.println("On Servo Motor ");
  // ハンドラの呼ばれるタイミング調整用
  touchdetected = false;
}
