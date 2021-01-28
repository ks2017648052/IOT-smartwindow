#include <Servo.h> // servo.h 라이브러리
#include <SoftwareSerial.h> // 서보모터를 위한 라이브러리
#include "DHT.h"	// 온습도를 위한 라이브러리
#define DHTPIN 4       // SDA 핀의 설정
#define DHTTYPE DHT22   // DHT22 (AM2302) 센서종류 설정
Servo myservo;		// 서보모터 선언
DHT dht(DHTPIN, DHTTYPE);	
SoftwareSerial btSerial(12,13);	// 블루투스 통신 핀 설정
float temperature;	// temperature 변수 선언
int state = 0;      // 불꽃감지 센서의 상태 값 저장 변수
int led = 9;       // LED 핀 번호 9
int flame = 2;      // 불꽃감지 센서 핀 번호 2
int data;		// data 변수 선언
int angle = 0; // servo position in degrees 


void setup(){

   pinMode(led, OUTPUT);    // LED를 출력으로 설정
   pinMode(flame, INPUT);   // 불꽃 감지 센서를 입력으로 설정
   myservo.attach(11); 	// 서보모터 핀 11번 설정
   Serial.begin(9600); 	// 직렬 포트를 열고 데이터 속도를 9600bps로 설정
   dht.begin();
   btSerial.begin(9600); // 블루투스 직렬 포트를 열고 데이터 속도를 9600bps로 설정
  delay(100);	 // 100ms delay
}

void loop()
{
   state = digitalRead(flame);   // 불꽃 감지 센서값 입력받음
  digitalWrite(led, LOW);       // 초기 LED값을 0FF로 설정해주기
   float h = dht.readHumidity();//변수 h 에 온습도계에서 측정된 습도 받아오기
   float t = dht.readTemperature(); //변수 t 에 온습도계에서 측정된 온도 받아오기
   int reading = analogRead(A2);
  delay(10);				// 10ms delay
  reading = analogRead(A2);
  delay(10);				// 10ms delay
  temperature = 5.0*reading/1023.0*100;		// 온도 계산
  delay(1000); 				// 1000ms delay

 
    if(analogRead(0)<500 ||  temperature <21 )	// 빗물 감지 센서가 500보다 작게 되거나

    {
      for(angle = 0; angle < 150; angle++) // 0도 부터 150도까지 angle 설정
     { 
       myservo.write(angle);  // angle을 서보모터에 입력
      } 

       if( state == 0) 	//불꽃이 감지 되었을때
    {

    digitalWrite(led, HIGH);    // LED ON
     for(angle = 150; angle >0; angle--) // 150도 부터 0도까지 angle 설정
     { 
       myservo.write(angle);    // angle을 서보모터에 입력
      }        
      } 
        btSerial.print("Fire");// 블루투스 통신하여 어플리케이션에 Fire출력
    }

 // 불꽃감지 센서의 값이 1일때 (불꽃이 감지 되지 않았을 때) 
   else if( state ==1){ 
   
     digitalWrite(led, LOW);     // LED OFF
   
     for(angle = 0; angle < 150; angle++) // 0도 부터 150도 까지 angle 설정
     { 
       myservo.write(angle); // angle을 서보모터에 입력
      }    
      btSerial.println("rain");// 블루투스 통신하여 어플리케이션에 rain 출력
    }
    else if( temperature > 26)
    {
       for(angle = 150; angle >0; angle--) 
     { 
       myservo.write(angle); 	// 서브 모터 값을 angle로 
      } 
      btSerial.print("sunny");// 블루투스 통신하여 어플리케이션에 sunny 출력
    }


    btSerial.println("Fire Off");      // 시리얼 통신에 센서값 출력해 주기.
  }

   btSerial.print("Humidity: ");  //  Humidity: 출력
    btSerial.print(h);		// 습도값 출력
    btSerial.print(" %\t");	//  %출력
    btSerial.print("Temperature: "); 	// Temperature:  출력
    btSerial.print(t);		// 온도값 출력
    btSerial.println(" *C");	// *C출력
    delay(1000);
   }

