
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
void setup() {

  Serial.begin(115200);
  pinMode(ledPower,OUTPUT);
  
}

void loop() {
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

  Serial.print("Dust Density [ug/m3]: ");
  Serial.println(SumUg);
}
