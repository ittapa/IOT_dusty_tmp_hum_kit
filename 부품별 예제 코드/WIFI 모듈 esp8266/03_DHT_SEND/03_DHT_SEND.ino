l/**

   @
   dht11 온습도 센서
   온습도 측정
   시리얼 통신으로 출력.

**/

#include <ESP8266WiFi.h>  //esp8266wifi 모듈 전용 라이브러리 포함

#include <DHT.h>      // DHT.h 라이브러리를 포함

#define DHTPIN D3      // DHT핀을 2번으로 정의
#define DHTTYPE DHT11  // DHT타입을 DHT11로 정의

const char* server = "api.thingspeak.com"; 
String apiKey = "XOHULPGTFO2VN58X"; //thingspeak에서 apikeys에 있는걸 올려야 한다. 온습도센서이므로 writekey에 있는 코드를 사용한다.
const char* MY_SSID = "atoplanet-2.4G";  //wifi 아이디
const char* MY_PWD = "asdf1234"; //wifi 비번

DHT dht(DHTPIN, DHTTYPE);  // DHT설정 - dht (디지털2, dht11)

void setup() {

  dht.begin();
  Serial.begin(115200);    // 115200 속도로 시리얼 통신을 시작
  Serial.println("Humidity and temperature\n\n");
  delay(700);

  //WIFI 연결시작
  Serial.println();
  Serial.print("Connecting to " + *MY_SSID);

  WiFi.begin(MY_SSID, MY_PWD); //wifi 아아디 비번 설정

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected");
  Serial.println("");

}



void loop() {
  delay(15000); //15초의 텀
  
  float hum = dht.readHumidity();  // 변수 h에 습도 값을 저장
  float tmp = dht.readTemperature();  // 변수 t에 온도 값을 저장

  Serial.print("Humidity: ");  // 문자열 Humidity: 를 출력한다.

  Serial.print(hum);  // 변수 h(습도)를 출력한다.
  Serial.print("%\t");  // %를 출력한다

  Serial.print("Temperature: ");  // 이하생략
  Serial.print(tmp);

  Serial.println(" C");

  //   h: 습도, t: 온도
  // send code think speak
  WiFiClient client; //위모스보드가 클라이언트
  if (client.connect(server, 80)) { //서버가 thingspeak, 포트는 80번
    Serial.println("WiFi Client connected ");

    String postStr = "api_key="+apiKey; //post 방식
    postStr += "&field1=";
    postStr += String(tmp);
    postStr += "&field2=";
    postStr += String(hum);

    //  postStr
    //  [apikey]&field1=[온도]&field2=[습도]

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n"); //업로드 형식이다.
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr); //데이터가 바디에 붙는다는 것

    Serial.println(postStr);
    //https://api.thingspeak.com/update?api_key=SUVU2A8VI7Y323XR&field1=1.5&field2=7.0
  }
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      
      return;
    }
  }


  

}
