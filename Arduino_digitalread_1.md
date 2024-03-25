```언어
#define ledPin 13
#define inPin 7 
int val = 0; 

void setup() {
  pinMode(ledPin, OUTPUT);    
  pinMode(inPin, INPUT);      
  Serial.begin(9600);
}

void loop() {
  val = digitalRead(inPin);   

  Serial.print("Input data = ");
  Serial.println(val);
  digitalWrite(ledPin, val);  
}
```
