/**
 * 
 * @
 * dht11 온습도 센서
 * 온습도 측정
 * 시리얼 통신으로 출력.
 * 
**/



#include <DHT.h>      // DHT.h 라이브러리를 포함한다
#define DHTPIN D3      // DHT핀을 2번으로 정의한다(DATA핀)
#define DHTTYPE DHT11  // DHT타입을 DHT11로 정의한다

DHT dht(DHTPIN, DHTTYPE);  // DHT설정 - dht (디지털2, dht11)

void setup() {
  dht.begin();  
  Serial.begin(115200);    // 9600 속도로 시리얼 통신을 시작한다
  Serial.println("Humidity and temperature\n\n");
  delay(700);
}



void loop() {

  float h = dht.readHumidity();  // 변수 h에 습도 값을 저장
  float t = dht.readTemperature();  // 변수 t에 온도 값을 저장

  Serial.print("Humidity: ");  // 문자열 Humidity: 를 출력한다.

  Serial.print(h);  // 변수 h(습도)를 출력한다.
  Serial.print("%\t");  // %를 출력한다
  
  Serial.print("Temperature: ");  // 이하생략
  Serial.print(t);
  
  Serial.println(" C");

  delay(1000);

}
