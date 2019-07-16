//라이브러리 IMPORT
#include <LiquidCrystal_I2C.h> //LCD
#include <Wire.h> //LCD 관련
#include <ESP8266WiFi.h> //wifi 모듈
#include "DHT.h"                // DHT11 라이브러리 포함
#include <BlynkSimpleEsp8266.h> // blynk

//LCD 객체선언
LiquidCrystal_I2C lcd(0x27, 16, 2);

//BLyNK Serial
#define BLYNK_PRINT Serial 

//Blynk 프로젝트 인증 토큰값 지정
char auth[] = "pwlV16RbZSKnYxlT1uP4DhJyQ5OLFkP3";
//BlynkTimer timer; // bliynk timer 선언

//WIFI모듈 아이디 비밀번호 설정
char ssid[] = "notebook-2.4G"; //사용 WiFi의 SSID입력
char pass[] = "make2407"; //사용 WiFi의 password입력

//DHT11 온습도센서 세팅
#define DHTPIN D2    // DHT2 연결핀
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//미세먼지 관련 변수 설정
int dustPin = A0;
float dustVal = 0;
float dustDensity = 0;
float dustDensityug=0;
float voMeasured = 0;
float calcVoltage = 0;
float SumUg = 0;
int ledPower = D13;
int delayTime = 280;
int delayTime2 = 40;
float offTime = 9680;

// RGB LED 핀 번호 설정
int r = D11; 
int g = D10;
int b = D9;

void setup(){

  //위모스 보드 시리얼 통신 설정
  Serial.begin(115200);

  //미세먼지 적외선LED 설정
  pinMode(ledPower,OUTPUT);

  //RGB 모듈 GPIO핀 설정
  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);

  //LCD I2c 모듈 설정
  lcd.begin();
  lcd.backlight();

  //blynk 연결 시작
  Blynk.begin(auth, ssid, pass); //auth:토큰, ssid: wifi 이름, pass: wifi 비밀번호
}

void loop(){

  //dht11 온습도센서 코드
  long h = dht.readHumidity();     // 습도 값 구하기
  long t = dht.readTemperature();  // 온도 값 구하기
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" C");

  // 미세먼지 측정(30회) 30회 측정 후 평균값 도출
  SumUg = 0;
  
  for(int i=1; i<=30; i++){
    digitalWrite(ledPower, LOW); // power on the LED
    delayMicroseconds(delayTime);
  
    dustVal=analogRead(dustPin); // read the dust value via pin 5 on the sensor
    calcVoltage = dustVal * (5.0 / 1024); 
    dustDensityug = (calcVoltage-0.5)/0.01; // 평균값 보정 필요
    SumUg = SumUg + dustDensityug;
    
    delayMicroseconds(delayTime2);
  
    digitalWrite(ledPower, HIGH); // turn the LED off
    delayMicroseconds(offTime);
    delay(100);
  }
  
  SumUg = SumUg/30;

  //미세먼지 층적 값 시리얼모니터 출력
  Serial.print("Dust Density [ug/m3]: ");
  Serial.println(SumUg);

  // 1602 I2C LCD 모듈 에 온도 습도 미세먼지 측정결과 출력
  lcd.setCursor(0, 0);
  lcd.print("H: ");
  lcd.print(h);
  lcd.print(" %  ");
  lcd.print("T: ");
  lcd.print(t);
  lcd.print(" C");
  lcd.setCursor(0, 1);
  lcd.print("Dust: ");
  lcd.print(SumUg);
  lcd.print("ug");
  
  //RGB LED 모듈 코드 미세먼지 값에 따라 
  if (SumUg < 30) { // ~30 미만일 때, 좋음
   RGB(LOW,LOW,HIGH); // 파란색 LED 점등
  }
  if (SumUg >= 30 & SumUg < 80) { // 30이상 80미만일 때, 보통
   RGB(LOW,HIGH,LOW); // 초록색 LED 점등
  }
  if (SumUg >= 80) { // 80이상일 때, 나쁨
   RGB(HIGH,LOW,LOW);// 빨간색 LED 점등
  }

  // Blynk APP으로 값 전달
  Blynk.virtualWrite(V0, SumUg); //V0 : 미세먼지 측정값
  Blynk.virtualWrite(V1, t); // V1: 온도
  Blynk.virtualWrite(V2, h); //V2: 습도
  // Blynk.notify("The air is so bad!!"); //Notifiation위젯에서 "The air is so bad!!" 알림
}

  void RGB(int x, int y, int z){ // RGB LED 함수
    digitalWrite(r,x); // r,g,b는 아두이노 핀 번호
    digitalWrite(g,y); // x,y,z는 위에서 받아오는 숫자
    digitalWrite(b,z);
  }
