```언어
#include <NewPing.h>

#define ledPin 13  // LED 디지털 핀 13번에 연결
#define LINE_DETECT_WHITE 0  // 변경된 부분

#define TRIGGER_PIN 12  // 트리거 핀을 12번 핀으로 설정
#define ECHO_PIN 11     // 에코 핀을 11번 핀으로 설정
#define MAX_DISTANCE 100  // 최대 측정 거리를 100cm로 설정

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

int linesensor_data[5] = {0, 0, 0, 0, 0};  // 읽은 값을 저장할 변수
int linesensor_pin[5] = {2, 3, 4, 5, 6};   // int형 배열

void setup() {
  pinMode(ledPin, OUTPUT);    

  for (int i = 0; i < 5; i++) {
    pinMode(linesensor_pin[i], INPUT_PULLUP); // 내부 풀업 사용으로 변경     
  }

  Serial.begin(9600);
}

void loop() {
  // Read digital line sensor data
  int sum = 0;
  for (int i = 0; i < 5; i++) {
    if (LINE_DETECT_WHITE == 0) {
      linesensor_data[i] = 1 - digitalRead(linesensor_pin[i]);
    } else {
      linesensor_data[i] = digitalRead(linesensor_pin[i]);
    }
    sum += linesensor_data[i];
  }
  
  int lineSensorValue = -5;
  if (sum == 5) {
    lineSensorValue = 5;
  } else if (sum == 2) {
    if ((linesensor_data[3] == 1) && (linesensor_data[4] == 1)) lineSensorValue = 3;
    if ((linesensor_data[2] == 1) && (linesensor_data[3] == 1)) lineSensorValue = 1;
    if ((linesensor_data[1] == 1) && (linesensor_data[2] == 1)) lineSensorValue = -1;
    if ((linesensor_data[0] == 1) && (linesensor_data[1] == 1)) lineSensorValue = -3;
  } else if (sum == 1) {
    if (linesensor_data[0] == 1) lineSensorValue = -4;
    if (linesensor_data[1] == 1) lineSensorValue = -2;
    if (linesensor_data[2] == 1) lineSensorValue =  0;
    if (linesensor_data[3] == 1) lineSensorValue =  2;
    if (linesensor_data[4] == 1) lineSensorValue =  4;
  } else if (sum == 3) {
    lineSensorValue = -10;
  }

  // Read distance sensor data
  float distance = 0.0;
  delay(50);
  distance = sonar.ping_cm();
  if (distance == 0) distance = MAX_DISTANCE;  

  // Output both sensor data
  Serial.print("Line sensor data: ");
  for (int i = 0; i < 5; i++) {
    Serial.print(linesensor_data[i]);
    Serial.print(" ");
  }
  Serial.print(" | Ping distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(100); // 임의의 딜레이
}
```
