//RTC libraries
#include <DS3231.h>
#include <Wire.h>

//Arduino pin variables
#define Bt1 2    //Button 1
#define Bt2 3    //Button 2
#define Bt3 4    //Button 3
#define Q1 5     //BCD output 1
#define Q2 6     //BCD output 2
#define Q4 7     //BCD output 4
#define Q8 8     //BCD output 8
#define L1 9     //Latch Enable 1
#define L2 10    //Latch Enable 2
#define L3 11    //Latch Enable 3
#define L4 12    //Latch Enable 4
#define C 13     //Colon LEDs
#define DP 17    //Decimal Point

unsigned long startMillis;  
unsigned long currentMillis;
const unsigned long period1 = 30000;  
const unsigned long period2 = 36000;  

int Bt1State = 0;

int Bt2State = 0;
int Bt2Counter = 0;

int Bt3State = 0;
int Bt3Counter = 0;

int H;
int M;
int T1;
int T2;
int T3;
int T4;

DS3231 clock;
bool h12Flag;
bool pmFlag;

void setup() {
  pinMode(Bt1,INPUT);
  pinMode(Bt2,INPUT);
  pinMode(Bt3,INPUT);
  pinMode(Q1,OUTPUT);
  pinMode(Q2,OUTPUT);
  pinMode(Q4,OUTPUT);
  pinMode(Q8,OUTPUT);
  pinMode(L1,OUTPUT);
  pinMode(L2,OUTPUT);
  pinMode(L3,OUTPUT);
  pinMode(L4,OUTPUT);
  pinMode(C,OUTPUT);
  pinMode(DP,OUTPUT);
  //Serial.begin(9600);
  Wire.begin();
  startMillis = millis();
}

void loop() {
  
  currentMillis = millis();
  Bt1State = digitalRead(Bt1);
  
  if(Bt1State == HIGH){     //If Bt1 was pressed, set time
    
    Bt2Counter = H;
    Bt3Counter = M;
    
    while(1){
      digitalWrite(C,LOW);
      digitalWrite(DP,HIGH);
      delay(200);
      Bt1State = digitalRead(Bt1);
      Bt2State = digitalRead(Bt2);
      Bt3State = digitalRead(Bt3);
      
      if(Bt2State == HIGH){           //If Bt2 was pressed, increment hours
        if(Bt2Counter > 22){
          Bt2Counter = -1;
        }
        Bt2Counter++;
        T3 = Bt2Counter % 10;   //hundreds
        T4 = Bt2Counter / 10;   //thousands
        
        //Display minutes as they change
        E3();
        B();
        delay(70);
        Display(T3);
        E4();
        B();
        delay(70);
        Display(T4);
        
      }
      
      if(Bt3State == HIGH){           //If Bt3 was pressed, increment minutes
        if(Bt3Counter > 58){
          Bt3Counter = -1;
        }
        
        Bt3Counter++;
        T1 = Bt3Counter % 10;   //ones
        T2 = Bt3Counter / 10;   //tens

        //Display minutes as they change
        E1();
        B();
        delay(70);
        Display(T1);
        E2();
        B();
        delay(70);
        Display(T2);

        
      }
      
      if(Bt1State == HIGH){           //If Bt1 was pressed, set the time
        clock.setClockMode(false);    //Set 24h mode
        clock.setHour(Bt2Counter);    //Set hours
        clock.setMinute(Bt3Counter);  //Set minutes
        clock.setSecond(0);           //Set seconds to 0
        break;
      }
    }
    delay(200);
  }

  else{ //If Bt1 wasn't pressed, display time/temperature

    if((currentMillis - startMillis) <= period1){     //If less than 30s have passed, show time
      digitalWrite(DP,HIGH);
      
      //Flash colon LED's
      if(abs((currentMillis - startMillis) % 750) <= 20){
            digitalWrite(C,LOW);
            if(abs((currentMillis - startMillis) % 1500) <= 20){
              digitalWrite(C,HIGH);
            }   
        }
      
      H = clock.getHour(h12Flag,pmFlag);
      M = clock.getMinute();
      int Time = ((H * 100) + M);
      
      //Serial.print("Time: ");
      //Serial.print(H);
      //Serial.print(":");
      //Serial.println(M);

      //Display time
      T1 = (Time % 10);          //Ones
      T2 = ((Time / 10)%10);     //Tens
      T3 = ((Time / 100)%10);    //Hundreds
      T4 = (Time / 1000);        //Thousands
      E1();
      Display(T1);
      delay(2);
      E2();
      Display(T2);
      delay(2);
      E3();
      Display(T3);
      delay(2);
      E4();
      Display(T4);
      delay(2);
    }
    
    if((currentMillis - startMillis) >= period1){   //If more than 30s and less than 36s passed, show temperature
      float T = clock.getTemperature();
      
      //Serial.print("Temperature:  ");
      //Serial.println(T);
      
      int Temp = ( T * 100);
      int Te1 = (Temp % 10);          //Ones
      int Te2 = ((Temp / 10)%10);     //Tens
      int Te3 = ((Temp / 100)%10);    //Hundreds
      int Te4 = (Temp / 1000);        //Thousands
      
      //Display temperature
      digitalWrite(C,LOW);
      digitalWrite(DP,LOW);
      E1();
      Display(Te1);
      delay(2);
      E2();
      Display(Te2);
      delay(2);
      E3();
      Display(Te3);
      delay(2);
      E4();
      Display(Te4);
      delay(2);
    }
    if((currentMillis - startMillis) >= period2){
      startMillis = currentMillis;
    }
  }

}

//Functions for displaying specific digits
void X0(){
  digitalWrite(Q1,LOW);
  digitalWrite(Q2,LOW);
  digitalWrite(Q4,LOW);
  digitalWrite(Q8,LOW);}
void X1(){
  digitalWrite(Q1,HIGH);
  digitalWrite(Q2,LOW);
  digitalWrite(Q4,LOW);
  digitalWrite(Q8,LOW);}
void X2(){
  digitalWrite(Q1,LOW);
  digitalWrite(Q2,HIGH);
  digitalWrite(Q4,LOW);
  digitalWrite(Q8,LOW);}
void X3(){
  digitalWrite(Q1,HIGH);
  digitalWrite(Q2,HIGH);
  digitalWrite(Q4,LOW);
  digitalWrite(Q8,LOW);}
void X4(){
  digitalWrite(Q1,LOW);
  digitalWrite(Q2,LOW);
  digitalWrite(Q4,HIGH);
  digitalWrite(Q8,LOW);}
void X5(){
  digitalWrite(Q1,HIGH);
  digitalWrite(Q2,LOW);
  digitalWrite(Q4,HIGH);
  digitalWrite(Q8,LOW);}
void X6(){
  digitalWrite(Q1,LOW);
  digitalWrite(Q2,HIGH);
  digitalWrite(Q4,HIGH);
  digitalWrite(Q8,LOW);}
void X7(){
  digitalWrite(Q1,HIGH);
  digitalWrite(Q2,HIGH);
  digitalWrite(Q4,HIGH);
  digitalWrite(Q8,LOW);}
void X8(){
  digitalWrite(Q1,LOW);
  digitalWrite(Q2,LOW);
  digitalWrite(Q4,LOW);
  digitalWrite(Q8,HIGH);}
void X9(){
  digitalWrite(Q1,HIGH);
  digitalWrite(Q2,LOW);
  digitalWrite(Q4,LOW);
  digitalWrite(Q8,HIGH);}
  
//Functions for latching the displays
void E1(){
  digitalWrite(L1,LOW);
  digitalWrite(L2,HIGH);
  digitalWrite(L3,HIGH);
  digitalWrite(L4,HIGH);}
void E2(){
  digitalWrite(L1,HIGH);
  digitalWrite(L2,LOW);
  digitalWrite(L3,HIGH);
  digitalWrite(L4,HIGH);}
void E3(){
  digitalWrite(L1,HIGH);
  digitalWrite(L2,HIGH);
  digitalWrite(L3,LOW);
  digitalWrite(L4,HIGH);}
void E4(){
  digitalWrite(L1,HIGH);
  digitalWrite(L2,HIGH);
  digitalWrite(L3,HIGH);
  digitalWrite(L4,LOW);}
void B(){
  digitalWrite(Q1,HIGH);
  digitalWrite(Q2,HIGH);
  digitalWrite(Q4,HIGH);
  digitalWrite(Q8,HIGH);}
  
//Function for actual digit display
void Display(int N){
  if(N == 0){X0();}
  if(N == 1){X1();}
  if(N == 2){X2();}
  if(N == 3){X3();}
  if(N == 4){X4();}
  if(N == 5){X5();}
  if(N == 6){X6();}
  if(N == 7){X7();}
  if(N == 8){X8();}
  if(N == 9){X9();}
}
