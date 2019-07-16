#define BLYNK_PRINT Serial //시리얼 모니터 활성화
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = "Olfw3PB41c_zJ32FGOLzArSosjpQ2dzG"; //인증 토큰 입력
char ssid[] = "notebook-2.4G"; //사용 WiFi의 SSID입력
char pass[] = "make2407"; //사용 WiFi의 password입력

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass); //아두이노가 Blynk Cloud에 연결.
}

void loop() {
  Blynk.run();
}
