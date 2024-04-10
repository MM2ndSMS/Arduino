#define TRIG_PIN_FRONT 6
#define ECHO_PIN_FRONT 7
#define TRIG_PIN_BACK 8
#define ECHO_PIN_BACK 9
#define TRIG_PIN_RIGHT 10
#define ECHO_PIN_RIGHT 11
#define TRIG_PIN_LEFT 12
#define ECHO_PIN_LEFT 13

#define MAX_DISTANCE 150

int UltrasonicSensorData[4]; 

int readUltrasonicSensor(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH);
  float distance = duration / 58; 

  if (distance == 0 || distance > MAX_DISTANCE) {
    distance = MAX_DISTANCE; 
  }

  return distance;
}

void readUltrasonicSensors() {
  UltrasonicSensorData[0] = readUltrasonicSensor(TRIG_PIN_FRONT, ECHO_PIN_FRONT); // 전면
  UltrasonicSensorData[1] = readUltrasonicSensor(TRIG_PIN_BACK, ECHO_PIN_BACK);   // 후면
  UltrasonicSensorData[2] = readUltrasonicSensor(TRIG_PIN_RIGHT, ECHO_PIN_RIGHT); // 오른쪽
  UltrasonicSensorData[3] = readUltrasonicSensor(TRIG_PIN_LEFT, ECHO_PIN_LEFT);   // 왼쪽
}

void displayUltrasonicSensorData() {
  char Sonar_data_display[40];
  sprintf(Sonar_data_display,"F:");
  Serial.print(Sonar_data_display);
  Serial.print(UltrasonicSensorData[0]);
  Serial.print(" B:");
  Serial.print(UltrasonicSensorData[1]);
  Serial.print(" R:");
  Serial.print(UltrasonicSensorData[2]);
  Serial.print(" L:");
  Serial.println(UltrasonicSensorData[3]);
}

void setup() {
  Serial.begin(9600); // 시리얼 통신 시작

  pinMode(TRIG_PIN_FRONT, OUTPUT);
  pinMode(ECHO_PIN_FRONT, INPUT);
  pinMode(TRIG_PIN_BACK, OUTPUT);
  pinMode(ECHO_PIN_BACK, INPUT);
  pinMode(TRIG_PIN_RIGHT, OUTPUT);
  pinMode(ECHO_PIN_RIGHT, INPUT);
  pinMode(TRIG_PIN_LEFT, OUTPUT);
  pinMode(ECHO_PIN_LEFT, INPUT);
}

int Robot_Mode_Define(void)
{
  readUltrasonicSensors();
  int i;
  int mode = -1;
  displayUltrasonicSensorData();
  for(i=0;i<4;i++)
  {
    if(UltrasonicSensorData[i] ==0) UltrasonicSensorData[i] = MAX_DISTANCE;
  }
  if( (UltrasonicSensorData[2] >= 15) && (UltrasonicSensorData[3] >= 15) )
  {
    mode =0;
  }
  if( (UltrasonicSensorData[2] <= 15) && (UltrasonicSensorData[3] <= 15) )
  {
    mode =1;
  }
  if( (UltrasonicSensorData[3] <= 35) && (UltrasonicSensorData[2] >= 40) )
  {
    mode =2;
  }
  if( (UltrasonicSensorData[2] <= 35) && (UltrasonicSensorData[3] >= 40) )
  {
    mode =3;
  }
  return mode;
}


void loop() {
  int mode = Robot_Mode_Define(); // 로봇 모드 결정
  Serial.println(mode); // 모드 출력
  delay(50); // 모드 체크 간격
}
