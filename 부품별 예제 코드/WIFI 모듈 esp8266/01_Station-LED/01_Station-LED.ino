//esp8266 라이브러리 추가
#include <ESP8266WiFi.h>

//WiFi Server 객체 생성 및 선언 (80포트)
WiFiServer server(80);

void setup() {
   //시리얼 통신 선언 보드레이트 115200
  Serial.begin(115200);
  delay(10);
  //LED핀모드 설정 및 off
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
   /*
  * WiFI 설정  및 지정
  */
  WiFi.mode(WIFI_STA); //station 모드
  WiFi.disconnect();
  delay(100);

  //연결할 WIFI 설정
  WiFi.begin("notebook-2.4G", "make2407");


  //wifi가 연결될때까지 기다림.
  //WiFi.status() wifi 현재 상태 호출
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");

  // 서버 시작
  server.begin();
  
  Serial.println("Server started");
  Serial.print("http://");
  
  //local IP , 공유기가 할당해준 IP 조회 출력
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() {
   //client 객체 선언 
  WiFiClient client = server.available();
    //client null 체크
  if (!client) {
    return;
  }
  
  //시리얼 모니터 출력
  Serial.println("new client");
  
  while (!client.available()) {
    delay(1);
  }
  
  String request = client.readStringUntil('\r'); // client가 보낸 값 받기
  
  Serial.println(request);
  
  client.flush();
  
  int value = LOW;
  if (request.indexOf("/ON") != -1) {
    digitalWrite(LED_BUILTIN, LOW);
    value = HIGH;
  }
  
  if (request.indexOf("/OFF") != -1) {
    digitalWrite(LED_BUILTIN, HIGH);
    value = LOW;
  }
   //html 문서 전송
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); // do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.print("Led pin is now: ");
  

  if (value == HIGH) {
    client.print("on");
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("<a href=\"/ON\"\"><button>Turn on </button></a>");
  client.println("<a href=\"/OFF\"\"><button>Turn Off </button></a><br />");
  client.println("</html>");
  delay(1);

  //client 
  Serial.println("Client disonnected");
  Serial.println("");
}
