#include <Servo.h>


// for led
int num_array[11][8] = {  { 1,1,1,1,1,1,0,0 },  // 0  
                          { 0,1,1,0,0,0,0,0 },  // 1
                          { 1,1,0,1,1,0,1,0 },  // 2
                          { 1,1,1,1,0,0,1,0 },  // 3
                          { 0,1,1,0,0,1,1,0 },  // 4
                          { 1,0,1,1,0,1,1,0 },  // 5
                          { 1,0,1,1,1,1,1,0 },  // 6
                          { 1,1,1,0,0,0,0,0 },  // 7
                          { 1,1,1,1,1,1,1,0 },  // 8
                          { 1,1,1,0,0,1,1,0 },  // 9
                          { 0,0,0,0,0,0,0,1}};  // Punkt

// public vars
int inPin;
int servoPin;
int buzzerPin;
Servo myServo;

int btn_status=0;

int state;
int teeTime;
long startTime;
long waitTime;
long timeout;
long remainingTime;

// setup for program, to run once
void setup() {
  inPin = 12;
  servoPin = 10;
  buzzerPin = 11;

  state = 0;
  teeTime = 0;
  startTime = 0;
  waitTime = 0;
  timeout = 5000;
  remainingTime = 0;

  // for button
  pinMode(inPin, INPUT);

  // for servo
  myServo.attach(servoPin);

  // for buzzer
  pinMode(buzzerPin, OUTPUT);
  
  // for led 
  pinMode(2, OUTPUT);   
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  
  // display '.' as default
  Num_Write(10);

}

void loop() {
  switch(state) {
    case 0: // Warte auf Button Input
      btn_status = digitalRead(inPin);
      if(btn_status == HIGH) {    
        state = 1;
        Num_Write(0);
        delay(300);   
      }
      break;
    
    case 1: // Button x mal gedrückt
      if(digitalRead(inPin) == HIGH) {
        if(teeTime < 10) {
          teeTime++;
        } else {
          teeTime = 1;
        }
        waitTime = millis();
        delay(300);  
        state = 2;
        Num_Write(teeTime);
      }
      break;
    
    case 2: // Zeit x vergangen seit letztem Input
      if(millis() - waitTime > timeout) {
        startTime = millis();
        for (int pos = 90; pos >= 15; pos -= 1) { 
          myServo.write(pos);              
          delay(30);                      
        }
        delay(300);       
        remainingTime = teeTime*1000 - (millis() - startTime)/60;
        long temp = remainingTime/1000 +1;
        state = 3;
        Num_Write(temp);
      } else {
          if(digitalRead(inPin) == HIGH) {
            if(teeTime < 10) {
              teeTime++;
            } else {
              teeTime = 1;
            }
            waitTime = millis();
            delay(300); 
            Num_Write(teeTime);
          }
      }
      break;
    
    case 3: // Zeit x abgelaufen oder erneut Button Input
      if(digitalRead(inPin) == HIGH) {
          if(teeTime < 10) {
            teeTime++;
          } else {
            teeTime = 1;
          }
          remainingTime = teeTime*1000 - (millis() - waitTime)/60;
          delay(300);
          Num_Write(teeTime);
      }
      if((millis() - startTime)/60 > (teeTime*1000)) {
        // end
        state = 0;
        for (int pos = 15; pos <= 90; pos += 1) { 
          myServo.write(pos);              
          delay(30);                      
        }
        delay(300);       
        teeTone();
        teeTime = 0;
        Num_Write(10);
      } else {
          remainingTime = teeTime*1000 - (millis() - startTime)/60;
          long temp = remainingTime/1000 +1;
          Num_Write(temp);
      }
      break;
  } 
}

void teeTone() {
  tone(buzzerPin, 100);
  delay(100); 
  noTone(buzzerPin); 
  delay(150); 
  tone(buzzerPin, 100);
  delay(100); 
  noTone(buzzerPin); 
}

void Num_Write(int number) {
  int pin= 2;
  for (int j=0; j < 8; j++) {
    digitalWrite(pin, num_array[number][j]);
    pin++;
  }
}
