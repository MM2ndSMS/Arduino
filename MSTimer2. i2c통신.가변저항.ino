// 아두이노1(송신기)
#include <Wire.h> // I2C 통신 Wire 라이브러리 
#include <NewPing.h> // 초음파 센서 NewPing라이브러리 
#include <MsTimer2.h> // MsTimer2 라이브러리 

#define SONAR_NUM 3      // 초음파 센서 수 정의
#define MAX_DISTANCE 100  // 최대 측정 거리 100cm

#define SIZE 5  // 재귀이동평균 계산을 할 때 데이터 배열 크기  

#define sensorPin 3  // 가변 저항 센서 연결된 핀 번호

#define Front 0 // 정면 초음파 
#define Left  1  // 왼쪽 초음파 
#define Right 2  // 오른쪼 초음파 

#define TRIG1 50 // 초음파 센서 1번 Trig 핀 번호
#define ECHO1 51 // 초음파 센서 1번 Echo 핀 번호

#define TRIG2 22 // 초음파 센서 2번 Trig 핀 번호
#define ECHO2 23 // 초음파 센서 2번 Echo 핀 번호

#define TRIG3 46 // 초음파 센서 3번 Trig 핀 번호
#define ECHO3 47 // 초음파 센서 3번 Echo 핀 번호

// 초음파 센서 배열 초기화
NewPing sonar[SONAR_NUM] = 
{   
  NewPing(TRIG0, ECHO0, MAX_DISTANCE),  // 초음파 센서 0 초기화
  NewPing(TRIG1, ECHO1, MAX_DISTANCE),  // 초음파 센서 1 초기화
  NewPing(TRIG2, ECHO2, MAX_DISTANCE)  // 초음파 센서 2 초기화
};

// 재귀이동평균 계산을 위해 배열 초기화
float front_sensorData[SIZE] = {0.0};  // 정면 초음파 센서 데이터 배열 초기화
float left_sensorData[SIZE] = {0.0}; // 왼쪽 초음파 센서 데이터 배열 초기화
float right_sensorData[SIZE] = {0.0}; // 오른쪽 초음파 센서 데이터 배열 초기화
float resistance_sensorData[SIZE] = {0.0};  // 가변 저항 데이터 배열 초기화

// 정면 센서의 재귀 이동 평균 계산 함수 선언
float recursive_moving_average_front(float ad_value)
{
  static float avg_front = 0.0;  // 정면 초음파 센서의 재귀이동평균 값

  // 데이터 배열을 한 칸씩 왼쪽으로 이동 
  for (int i = 0; i <= SIZE - 2; i++) 
  {
    front_sensorData[i] = front_sensorData[i + 1];  // 현재 데이터를 다음 데이터로 이동 
  }
    
  front_sensorData[SIZE - 1] = ad_value;  //마지막 데이터에 새로운 값을 추가
     
  avg_front = avg_front + (front_sensorData[SIZE - 1] - front_sensorData[0]) / (float)SIZE;  // 재귀이동평균 계산식 

  return avg_front;  // 정면 초음파 센서 avg 반환
}

// 왼쪽 센서의 재귀 이동 평균 계산 함수 선언
float recursive_moving_average_left(float ad_value) 
{
  static float avg_left = 0.0;   // 왼쪽 초음파 센서의 재귀이동평균 값
  // 데이터 배열을 한 칸씩 왼쪽으로 이동
  for (int i = 0; i <= SIZE - 2; i++)  
  {
    left_sensorData[i] = left_sensorData[i + 1];  // 현재 데이터를 다음 데이터로 이동
  }
    
  left_sensorData[SIZE - 1] = ad_value;  //마지막 데이터에 새로운 값을 추가
     
  avg_left = avg_left + (left_sensorData[SIZE - 1] - left_sensorData[0]) / (float)SIZE;  // 재귀이동평균 계산식

  return avg_left;
  // 왼쪽 초음파 센서 avg 반환
}

float recursive_moving_average_right(float ad_value) 
{
  static float avg_right = 0.0;  // 오른쪽 초음파 센서의 재귀이동평균 값
  // 데이터 배열을 한 칸씩 왼쪽으로 이동
  for (int i = 0; i <= SIZE - 2; i++) 
 {
    right_sensorData[i] = right_sensorData[i + 1]; // 현재 데이터를 다음 데이터로 이동
  }
    
  right_sensorData[SIZE - 1] = ad_value; //마지막 데이터에 새로운 값을 추가
     
  avg_right = avg_right + (right_sensorData[SIZE - 1] - right_sensorData[0]) / (float)SIZE;  // 재귀이동평균 계산식

  return avg_right;
  // 오른쪽 초음파 센서 avg 반환
}

// 가변 저항의 재귀 이동 평균 계산 함수 선언
float recursive_moving_average_resistance(float ad_value) 
{
  static float avg_resistance = 0.0;  // 가변 저항의 재귀이동평균 값 초기화
  // 데이터 배열을 한 칸씩 왼쪽으로 이동
  for (int i = 0; i <= SIZE - 2; i++) 
  {
    resistance_sensorData[i] = resistance_sensorData[i + 1]; // 현재 데이터를 다음 데이터로 이동
 
  }
    
  resistance_sensorData[SIZE - 1] = ad_value;  //마지막 데이터에 새로운 값을 추가

  avg_resistance = avg_resistance + (resistance_sensorData[SIZE - 1] - resistance_sensorData[0]) / (float)SIZE; // 재귀이동평균 계산식

  return avg_resistance;  // 가변저항센서 avg 반환
}

float resistance = 0.0; // 전역 변수로 가변 저항 값을 선언

// 센서 값을 읽는 함수
void Read_sensor(void) 
{
   float add_value = 0.0;  // 가변 저항 값을 저장할 변수
   add_value = analogRead(A0);  // 아날로그 핀에서 값을 읽음
   resistance = recursive_moving_average_resistance(add_value);  // 이동 평균 계산
   Serial.print("가변저항 : "); Serial.println(resistance); // 가변 저항 값을 시리얼 모니터에 출력
   Serial.println(" ");  // 줄바꿈
}

void setup() 
{
  // 초음파 센서 핀 모드 설정
  pinMode(TRIG0, OUTPUT);  // 초음파 센서 0 트리거핀 출력 모드 설정
  pinMode(ECHO0, INPUT);  // 초음파 센서 0 에코핀 출력 모드 설정

  pinMode(TRIG1, OUTPUT);  // 초음파 센서 1 트리거핀 출력 모드 설정
  pinMode(ECHO1, INPUT);  // 초음파 센서 1 에코핀 출력 모드 설정


  pinMode(TRIG2, OUTPUT); // 초음파 센서 2 트리거핀 출력 모드 설정
  pinMode(ECHO2, INPUT); // 초음파 센서 2 에코핀 출력 모드 설정
  
  Serial.begin(115200);  // 시리얼 통신을 115200bps로 시작

  MsTimer2::set(100, Read_sensor); // 100ms마다 Read_sensor 함수 호출
  MsTimer2::start(); // MsTimer2 타이머 시작

  Wire.begin(); // I2C 통신 시작
}

void loop() 
{
  // 각 초음파 센서에서 거리를 측정
  float front_sonar = sonar[Front].ping_cm(); // 정면 초음파 센서에서 거리 측정
  float left_sonar  = sonar[Left].ping_cm();  // 왼쪽 초음파 센서에서 거리 측정
  float right_sonar = sonar[Right].ping_cm(); // 오른쪽 초음파 센서에서 거리 측정
  
  // 측정 값이 0인 경우 최대 거리로 설정
  if (front_sonar == 0.0 ) front_sonar = MAX_DISTANCE; 
  if (left_sonar == 0.0) left_sonar = MAX_DISTANCE;
  if (right_sonar == 0.0) right_sonar = MAX_DISTANCE;
  
  // 각 초음파 센서의 이동 평균 계산
  float front_avg = recursive_moving_average_front(front_sonar);
  float left_avg = recursive_moving_average_left(left_sonar);
  float right_avg = recursive_moving_average_right(right_sonar);

  // 측정된 값을 시리얼 모니터에 출력
  Serial.print("정면 초음파 센서: "); Serial.print(front_avg); Serial.print("cm ");
  Serial.print("왼쪽 초음파 센서: "); Serial.print(left_avg); Serial.print("cm ");
  Serial.print("오른쪽 초음파 센서: "); Serial.print(right_avg); Serial.println("cm ");
  
  // I2C 통신으로 데이터를 전송하기 위해 유니온을 사용
  union 
  {
    float value;
    byte bytes[4];
  } frontUnion, leftUnion, rightUnion, resistanceUnion;  // 유니온 선언

  // 각 센서 및 가변 저항의 이동 평균 값을 유니온에 저장
  frontUnion.value = front_avg;
  leftUnion.value = left_avg;
  rightUnion.value = right_avg;
  resistanceUnion.value = resistance;

  Wire.beginTransmission(8); // i2c 슬레이브 주소 8번으로 시작
  Wire.write(frontUnion.bytes, 4); // 정면 초음파 센서 값 전송
  Wire.write(leftUnion.bytes, 4); // 왼쪽 초음파 센서 값 전송 
  Wire.write(rightUnion.bytes, 4); // 오른쪽 초음파 센서 값 전송
  Wire.write(resistanceUnion.bytes, 4); // 가변 저항 값 전송
  Wire.endTransmission(); // 데이터 송신 종료

  delay(200);  // 100ms 대기
}


// 아두이노2(수신기)
#include <Wire.h> // I2C 통신 Wire 라이브러리 

// 유니온 선언: 부동 소수점 숫자를 4바이트로 나누어 저장할 수 있는 구조체
union FloatUnion
{
  float value;  // 부동 소수점 숫자 (4바이트)
  byte bytes[4]; // 4바이트 배열
};

void setup() 
{
  Serial.begin(115200); // 시리얼 통신 시작, 통신 속도 115200bps
  Wire.begin(8); // I2C 슬레이브 모드 시작, 슬레이브 주소 8번
  Wire.onReceive(receiveEvent); // 데이터 수신 이벤트 핸들러 등록, 데이터 수신 시 receiveEvent 함수 호출
}

void loop()
{
  delay(100); // 메인 루프 지연 100ms, 주기적으로 실행되도록 함
}

// I2C 데이터 수신 이벤트 핸들러 함수
void receiveEvent(int howMany)
{
  // 수신된 데이터가 16바이트 이상인 경우에만 처리
  if (Wire.available() >= 16)
  { 
    // 데이터를 저장할 FloatUnion 구조체 변수 선언
    FloatUnion frontUnion, leftUnion, rightUnion, resistanceUnion;
    
    // 4바이트의 데이터를 읽어서 frontUnion의 bytes 배열에 저장
    for (int i = 0; i < 4; i++)
    {
      frontUnion.bytes[i] = Wire.read(); // I2C 버퍼에서 바이트를 읽어와 frontUnion의 bytes 배열에 저장
    }
    // 4바이트의 데이터를 읽어서 leftUnion의 bytes 배열에 저장
    for (int i = 0; i < 4; i++)
    {
      leftUnion.bytes[i] = Wire.read(); // I2C 버퍼에서 바이트를 읽어와 leftUnion의 bytes 배열에 저장
    }
    // 4바이트의 데이터를 읽어서 rightUnion의 bytes 배열에 저장
    for (int i = 0; i < 4; i++)
    {
      rightUnion.bytes[i] = Wire.read(); // I2C 버퍼에서 바이트를 읽어와 rightUnion의 bytes 배열에 저장
    }
    // 4바이트의 데이터를 읽어서 resistanceUnion의 bytes 배열에 저장
    for (int i = 0; i < 4; i++)
    {
      resistanceUnion.bytes[i] = Wire.read(); // I2C 버퍼에서 바이트를 읽어와 resistanceUnion의 bytes 배열에 저장
    }

    // 수신된 데이터를 시리얼 모니터로 출력
    Serial.print("정면 초음파 센서: "); // "정면 avg: " 출력
    Serial.print(frontUnion.value); // 정면 평균 값 출력
    Serial.print(" cm, "); // " cm, " 출력
    Serial.print("왼쪽 초음파 센서: "); // "왼쪽 avg: " 출력
    Serial.print(leftUnion.value); // 왼쪽 평균 값 출력
    Serial.print(" cm, "); // " cm, " 출력
    Serial.print("오른쪽 초음파 센서: "); // "오른쪽 avg: " 출력
    Serial.print(rightUnion.value); // 오른쪽 평균 값 출력
    Serial.print(" cm, "); // " cm, " 출력
    Serial.print("가변저항: "); // "가변저항 avg: " 출력
    Serial.print(resistanceUnion.value); // 가변 저항 평균 값 출력
    Serial.println(" "); // 줄바꿈
  }
}
