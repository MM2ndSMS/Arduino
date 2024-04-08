#define SONAR_NUM 4            // 초음파 센서 개수
#define MAX_DISTANCE 150       // 최대 거리 (cm)

// 초음파 센서 핀 정의
#define FRONT_TRIG_PIN 8
#define FRONT_ECHO_PIN 9
#define REAR_TRIG_PIN 10
#define REAR_ECHO_PIN 11
#define RIGHT_TRIG_PIN 12
#define RIGHT_ECHO_PIN 13
#define LEFT_TRIG_PIN 6
#define LEFT_ECHO_PIN 7

float UltrasonicSensorData[SONAR_NUM];
int mode = -1; // 로봇의 모드를 저장하는 변수

void setup() {
  Serial.begin(9600); // 시리얼 통신 시작
  // 초음파 센서 핀을 출력 또는 입력으로 설정
  pinMode(FRONT_TRIG_PIN, OUTPUT);
  pinMode(REAR_TRIG_PIN, OUTPUT);
  pinMode(RIGHT_TRIG_PIN, OUTPUT);
  pinMode(LEFT_TRIG_PIN, OUTPUT);
  pinMode(FRONT_ECHO_PIN, INPUT);
  pinMode(REAR_ECHO_PIN, INPUT);
  pinMode(RIGHT_ECHO_PIN, INPUT);
  pinMode(LEFT_ECHO_PIN, INPUT);
}

void loop() {
  Robot_Mode_Define(); // 로봇의 모드를 정의하는 함수 호출
  Serial.println("Mode: " + String(mode)); // 모드를 시리얼 모니터에 출력
  delay(1000); // 필요에 따라 지연시간 조정
}


// 초음파 센서 값을 읽는 함수
void read_ultrasonic_sensor() {
  // 각 초음파 센서에서 거리를 읽어와 저장
  UltrasonicSensorData[0] = readUltrasonic(FRONT_TRIG_PIN, FRONT_ECHO_PIN);
  UltrasonicSensorData[1] = readUltrasonic(REAR_TRIG_PIN, REAR_ECHO_PIN);
  UltrasonicSensorData[2] = readUltrasonic(RIGHT_TRIG_PIN, RIGHT_ECHO_PIN);
  UltrasonicSensorData[3] = readUltrasonic(LEFT_TRIG_PIN, LEFT_ECHO_PIN);
}

// 초음파 센서 거리를 읽는 함수
float readUltrasonic(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // 에코 시간을 거리(cm)로 변환하여 반환
  return pulseIn(echoPin, HIGH) / 58.0;  
}

// 초음파 센서 값 표시 함수
void Sonar_Data_Display(int flag) {
  char Sonar_data_display[40];
  if (flag == 0) return;
  else {
    sprintf(Sonar_data_display, "F:%s B:%s R:%s L:%s",
            UltrasonicSensorData[0] == 0 ? "?" : String(UltrasonicSensorData[0]).c_str(),
            UltrasonicSensorData[1] == 0 ? "?" : String(UltrasonicSensorData[1]).c_str(),
            UltrasonicSensorData[2] == 0 ? "?" : String(UltrasonicSensorData[2]).c_str(),
            UltrasonicSensorData[3] == 0 ? "?" : String(UltrasonicSensorData[3]).c_str());
    Serial.println(Sonar_data_display);
  }
}


// 로봇 모드를 정의하는 함수
void Robot_Mode_Define() {
  read_ultrasonic_sensor(); // 초음파 센서 값을 읽음
  // 센서 값이 0인 경우 최대 거리로 설정
  for (int i = 0; i < SONAR_NUM; i++) {
    if (UltrasonicSensorData[i] == 0) UltrasonicSensorData[i] = MAX_DISTANCE;
  }
  Sonar_Data_Display(1); // 초음파 센서 값 표시
  // 로봇 모드 정의
  // 모드 0: 전방과 후방 모두 안전한 경우
  if ((UltrasonicSensorData[2] >= 15) && (UltrasonicSensorData[3] >= 15)) {
    mode = 0;
  }
  // 모드 1: 전방과 후방 모두 위험한 경우
  if ((UltrasonicSensorData[2] <= 15) && (UltrasonicSensorData[3] <= 15)) {
    mode = 1;
  }
  // 모드 2: 오른쪽이 위험한 경우
  if ((UltrasonicSensorData[3] <= 35) && (UltrasonicSensorData[2] >= 40)) {
    mode = 2;
  }
  // 모드 3: 왼쪽이 위험한 경우
  if ((UltrasonicSensorData[2] <= 35) && (UltrasonicSensorData[3] >= 40)) {
    mode = 3;
  }
}
