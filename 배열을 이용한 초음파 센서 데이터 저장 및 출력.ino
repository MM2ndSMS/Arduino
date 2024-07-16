#include <NewPing.h>
#include <MsTimer2.h> // MsTimer2 라이브러리

#define TRIGGER_PIN  12  // 초음파 센서 트리거 핀
#define ECHO_PIN     13 // 초음파 센서 에코 핀
#define MAX_DISTANCE 100 // 초음파 센서의 최대 거리

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing 객체 생성

// 전역 변수 및 배열 선언
float sonarArray[100] = {0.0}; // 초음파 데이터를 저장할 배열
static bool MsTimer_Run = false; // 타이머 실행 여부를 확인하는 플래그
static int cnt = 0; // 초음파 데이터를 읽은 횟수를 카운트하는 변수

// MsTimer2에 의해 호출될 함수
void Read_Sonar() {
    if (cnt < 100) {
        // 초음파 값을 읽어서 배열에 저장
        unsigned int distance = sonar.ping_cm(); // cm 단위 거리 읽기
        sonarArray[cnt] = distance;
        cnt++;
    } else {
        // 100번 읽은 후 타이머를 중지
        MsTimer2::stop();
        MsTimer_Run = false;
    }
}

void setup() {
    MsTimer_Run = true;
    pinMode(TRIGGER_PIN, OUTPUT); 
    pinMode(ECHO_PIN, INPUT);  
    
    Serial.begin(115200); // UART 초기화
    MsTimer2::set(100, Read_Sonar); // 100ms마다 Read_Sonar 함수를 호출하도록 타이머 설정
    MsTimer2::start();
}

void loop() {
    if (MsTimer_Run == false) {
        // 타이머가 실행 중이 아니면, 저장된 초음파 데이터를 UART로 전송
        for (int i = 0; i < 100; i++) 
        {
            Serial.print(sonarArray[i]);
            Serial.print(" ");
      
            if ((i + 1) % 10 == 0)     
            {
               Serial.println(); 
            }
        }
        Serial.println();                 // 모든 원소가 출력된 후 줄 바꿈
        
        // 카운터를 초기화
        cnt = 0;
        
        // 타이머 시작
        MsTimer_Run = true;
        MsTimer2::start();
    }
}
