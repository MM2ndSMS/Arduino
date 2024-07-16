#include <NewPing.h>
#include <MsTimer2.h> // MsTimer2 라이브러리

// 초음파 센서 트리거 핀 정의
#define TRIGGER_PIN  12  
// 초음파 센서 에코 핀 정의
#define ECHO_PIN     13 
// 초음파 센서의 최대 거리 정의
#define MAX_DISTANCE 100 

// NewPing 객체 생성
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 

// 초음파 데이터를 저장할 배열
float sonarArray[100] = {0.0}; 
// 타이머 실행 여부를 확인하는 플래그
static bool MsTimer_Run = false; 
// 초음파 데이터를 읽은 횟수를 카운트하는 변수
static int cnt = 0; 

// MsTimer2에 의해 호출될 함수
void Read_Sonar() {
    if (cnt < 100) { 
        // 초음파 값을 읽어서 배열에 저장
        unsigned int distance = sonar.ping_cm(); // cm 단위 거리 읽기
        sonarArray[cnt] = distance; 
        cnt++; // 카운트 증가
    } else { 
        // 100번 읽은 후 타이머를 중지
        MsTimer2::stop(); 
        MsTimer_Run = false; // 타이머 실행 플래그를 false로 설정
    }
}

void setup() {
    MsTimer_Run = true; // 타이머 실행 플래그를 true로 설정
    pinMode(TRIGGER_PIN, OUTPUT); // 트리거 핀을 출력으로 설정
    pinMode(ECHO_PIN, INPUT); // 에코 핀을 입력으로 설정

    Serial.begin(115200); // UART 초기화, 보드레이트를 115200으로 설정
    MsTimer2::set(100, Read_Sonar); // 100ms마다 Read_Sonar 함수를 호출하도록 타이머 설정
    MsTimer2::start(); // 타이머 시작
}

void loop() {
    if (MsTimer_Run == false) { 
        // 타이머가 실행 중이 아니면, 저장된 초음파 데이터를 UART로 전송
        for (int i = 0; i < 100; i++) {
            Serial.print(sonarArray[i]); // 배열의 값을 시리얼로 출력
            Serial.print(" "); // 각 값 사이에 공백 출력

            if ((i + 1) % 10 == 0) { 
                // 10개의 값을 출력한 후 줄 바꿈
                Serial.println(); 
            }
        }
        Serial.println(); // 모든 원소가 출력된 후 줄 바꿈

        cnt = 0; // 카운터를 초기화

        MsTimer_Run = true; // 타이머 실행 플래그를 true로 설정
        MsTimer2::start(); // 타이머 시작
    }
}
