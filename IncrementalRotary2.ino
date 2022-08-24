//=======================================LIBRARY===========================================
#include "EncoderStepCounter.h"
#include "LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd(0x27, 16, 2);

//=======================================DONE==============================================


//===================================INITIALIZATION===========================================
//DEFINE PIN ENCODER 

#define ENCODER_PINA 2
#define ENCODER_PINB 3

//DEFINE PIN BUTTON 
#define rst_button 7
#define A_button 4  // 50 M
#define B_button 5  // 70 M
#define C_button 6  // 90 M

//DEFINE PIN LED INDIKATOR
#define led_kuning 9
#define led_hijau  10

#define buzzer 8

#define PI 3.1415926535897932384626433832795
float d = 96.2; //IN MM
double distanceC= (PI*d) / 360; // distanceC = keliling lingkaran / 360 pulse 
signed long position = 0;
signed char pos = 0;
double distance_M = 0;
double result = 0;
int rst_state = 0;
int A_state = 0;
int B_state = 0;
int C_state = 0;
int state = 0;

// DEFINE MILLIS()
unsigned long current = 0;
unsigned long pre_time = 0;



// Create instance for one full step encoder
EncoderStepCounter encoder(ENCODER_PINA, ENCODER_PINB);

//=======================================FUNCTION===========================================

// Call tick on every change interrupt
void interrupt() {
  encoder.tick();
   pos = encoder.getPosition();
  if (pos != 0) {
    position += pos;
    encoder.reset();
  }
}


//=======================================DONE==============================================

//=======================================MAIN PROGRAM===========================================


void setup() {
  Serial.begin(9600);
  pinMode(rst_button, INPUT_PULLUP);
  pinMode(A_button, INPUT_PULLUP);
  pinMode(B_button, INPUT_PULLUP);
  pinMode(C_button, INPUT_PULLUP);
 
  pinMode(buzzer, OUTPUT);
 
  pinMode(led_kuning,OUTPUT);
  pinMode(led_hijau,OUTPUT);
  
  // Initialize encoder
  encoder.begin();
  // Initialize interrupts
  attachInterrupt(digitalPinToInterrupt(ENCODER_PINA), interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PINB), interrupt, CHANGE);

  //LCD INIT
  lcd.init();
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("CABLE LENGTH");
  lcd.setCursor(5,1);
  lcd.print("MEASUREMENT");
  delay(2000);
  lcd.clear();
  
}



void loop() {

    
   current = millis();
   if (current-pre_time>33){
        rst_state = digitalRead(rst_button);
         A_state = digitalRead(A_button);
         B_state = digitalRead(B_button);
         C_state = digitalRead(C_button);
   
          if (rst_state == LOW){
              lcd.clear();
              position=0; 
               state = 0;
              }
        pre_time = current;
   }

   
    distance_M = (position/10)*distanceC;
    result = distance_M / 100;
    lcd.setCursor(0,1);
    lcd.print("Meter:");
    lcd.print(result);
    Serial.println(position);
   
    if (A_state==LOW){
       lcd.clear();
       position=0;
      state = 1; 
    }

    else if (B_state==LOW){
      lcd.clear();
      position=0;
      state = 2; 
    }

    else if (C_state==LOW){
      lcd.clear();
      position=0;
      state = 3; 
    }


  // jika ingin mengubah batas pengukuran ubah parameter result > x (x parameter nilai (meter))
    
    switch(state){
      case 0:
      digitalWrite(buzzer,LOW);
      digitalWrite(led_kuning,LOW);
      digitalWrite(led_hijau,HIGH);
      
      break;

      
      case 1: 
      
      lcd.setCursor(5,0);
      lcd.print("50 Meter");
      digitalWrite(led_kuning,HIGH);
      digitalWrite(led_hijau,LOW);
      if(result>50){
        digitalWrite(buzzer,HIGH);
      }
      else{
        digitalWrite(buzzer,LOW);
      }
      break;


      case 2: 
      lcd.setCursor(5,0);
      lcd.print("70 Meter");
      digitalWrite(led_kuning,HIGH);
      digitalWrite(led_hijau,LOW);
      if(result>70){
        digitalWrite(buzzer,HIGH);
      }
      else{
        digitalWrite(buzzer,LOW);
      }
      break;


      case 3: 
      lcd.setCursor(5,0);
      lcd.print("90 Meter");
      digitalWrite(led_kuning,HIGH);
      digitalWrite(led_hijau,LOW);
      if(result>90){
        digitalWrite(buzzer,HIGH);
      }
      else{
        digitalWrite(buzzer,LOW);
      }
      break;
    }
    
   
}

//=======================================DONE==============================================
