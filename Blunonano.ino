#include <SoftwareSerial.h>

//LED PIN Setup
int LED_R=12;
int LED_G=11;
int LED_B=10;
int Motor1=A0;
int Motor2=A1;
int i=0; //blink

void setup() {
  
  Serial.begin(115200);  //initial the Serial

  //LED
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  //진동 motor
  pinMode(Motor1, OUTPUT);
  pinMode(Motor2, OUTPUT);

  //digitalRead핀, Speakup 모듈에서 인식한 소리 값을 받아들임
  pinMode(4, INPUT); //노크 IO1
  pinMode(5, INPUT); //경적 IO2
  pinMode(6, INPUT); //사이렌 IO4
  pinMode(7, INPUT); //ON/OFF IO5
  pinMode(8, INPUT); //초인종 IO6
}

void blink(){ //LED Blink로 On과 Off 알리기
    i=0;
    while(i<3){
    digitalWrite(LED_R, HIGH); delay(200);
    digitalWrite(LED_R, LOW); delay(200);
    digitalWrite(LED_G, HIGH); delay(200);
    digitalWrite(LED_G, LOW);delay(200);
    digitalWrite(LED_B, HIGH);delay(200);
    digitalWrite(LED_B, LOW); delay(200);    
    i=i+1;}
}

void loop() {
  //initial value setting
  if(digitalRead(4)==1 && digitalRead(5)==1 && digitalRead(6)==1 
    && digitalRead(8)==1 && digitalRead(7)==1 ){
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, LOW);
  }
  
  //블루노나노 블루투스 처리
  if (Serial.available())  {
    //시리얼 모니터에뜬걸 그대로 보냄(송신)
    Serial.write(Serial.read());  //send what has been received
    Serial.println();   //print line feed character
  } 
  int val=0; //인식할 소리 값
  int state=0; //ON/OFF 상태 구분 변수
  
  //소리 인식 시작
  if((digitalRead(4)==0 && digitalRead(5)==0 && digitalRead(6)==0 
    && digitalRead(8)==0 && digitalRead(7)==1)||Serial.read()=='N'){
       //안드에서 보내온 ON 값인 N를 받는 함수: Serial.read()
    state=1;
    Serial.println("시작");
    blink();
  }
  
  //소리 인식하고있는 상태
  while(state==1){
    //소리 인식 종료 처리
    if((digitalRead(4)==1 && digitalRead(5)==1 && digitalRead(6)==1 
      && digitalRead(8)==1 && digitalRead(7)==0)||Serial.read()=='F'){
        //안드에서 보내온 OFF 값인 F를 받는 함수: Serial.read()
      //ON/OFF 7번 핀만 0일 때, OFF 처리
      state=0;
      Serial.println("종료");
      blink();
      break;   
    }

    //val값 설정 (소리 구별)
    if(digitalRead(4)==1){ //노크 소리
      val=1;
    } else if(digitalRead(5)==1){ //자동차 경적소리
      val=2;
    } else if(digitalRead(6)==1){ //사이렌 소리
      val=3;
    } else if(digitalRead(8)==1){ //초인종 소리
      val=4;
    } else{ //그 외 값들은 defalut 처리
      val=5;
    }

  switch(val){ //소리에 따른 동작 설정
    /*
     * 소리별로 진동으로 소리를 구분할 수 있게 
     * 진동의 delay 값을 다르게 하여
     * 진동 패턴 다르게 설정
    */

    case 1: //노크소리
    Serial.print("K\n");
    digitalWrite(LED_G,HIGH);
    i=0;
      while(i<3){ //진동모터
    digitalWrite(Motor1,HIGH); delay(250);
    digitalWrite(Motor1,LOW); delay(100);
    i=i+1; }
    digitalWrite(LED_G,LOW);
    break;
    
    case 2: //경적소리
    Serial.print("H\n");
    digitalWrite(LED_B,HIGH);
    i=0;
      while(i<3){ //진동모터
    digitalWrite(Motor1,HIGH);  
    digitalWrite(Motor2,HIGH); delay(500);
    digitalWrite(Motor1,LOW);
    digitalWrite(Motor2,LOW); delay(500);
    i=i+1;}
    digitalWrite(LED_B,LOW);
    break;
    
    case 3: //사이렌소리
    Serial.print("S\n");
    digitalWrite(LED_R,HIGH);
    i=0;
      while(i<3){ //진동모터
    digitalWrite(Motor1,HIGH); delay(200);
    digitalWrite(Motor1,LOW); delay(50);
    digitalWrite(Motor2,HIGH); delay(200);
    digitalWrite(Motor2,LOW); delay(50);
    i=i+1;}
    digitalWrite(LED_R,LOW);
    break;
       
    case 4: //초인종소리
    Serial.print("B\n");
    digitalWrite(LED_G,HIGH);
    i=0;
      while(i<3){ //진동모터
    digitalWrite(Motor1,HIGH); delay(500);
    digitalWrite(Motor1,LOW); delay(50);
    digitalWrite(Motor2,HIGH); delay(250);
    digitalWrite(Motor2,LOW); delay(50);
    i=i+1;}
    digitalWrite(LED_G,LOW);
    break;
   
    default: //그 외 값들은 반응X
    digitalWrite(LED_R,LOW);
    digitalWrite(LED_G,LOW);
    digitalWrite(LED_B,LOW);
    break;
    }
  }
}


