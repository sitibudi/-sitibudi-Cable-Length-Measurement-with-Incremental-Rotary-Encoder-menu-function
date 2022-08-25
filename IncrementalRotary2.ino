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
#define down_button 4  // 50 M
#define up_button 5  // 70 M
#define select_button 6  // 90 M

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
int down_state = 0;
int up_state = 0;
int selectl_state = 0;
int state = 0;

int menu = 1;
int page = 1;
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




void reset(){
    if (rst_state == LOW){
          position=0; 
         }
}


void updateMenu(){

  digitalWrite(led_hijau,HIGH);
  digitalWrite(led_kuning,LOW);
  
  switch(menu){
      case 1:

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("> Pengukuran");
      lcd.setCursor(0,1);
      lcd.print(" 45 Meter");
      
      break;

      
      case 2: 

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("> 45 Meter");
      lcd.setCursor(0,1);
      lcd.print(" 50 Meter");
      break;


      case 3: 
      
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(" 45 Meter");
      lcd.setCursor(0,1);
      lcd.print("> 50 Meter");
      break;


      case 4: 
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("> 90 Meter");
      lcd.setCursor(0,1);
      lcd.print(" 100 Meter");
      break;

      case 5: 
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(" 90 Meter");
      lcd.setCursor(0,1);
      lcd.print("> 100 Meter");
      break;
    }
}

void hasil(int page){
           distance_M = (position/10)*distanceC;
    result = distance_M / 100;
    lcd.print(result);

    return result;
}
void Action(){
  // jika ingin mengubah batas pengukuran ubah parameter result > x (x parameter nilai (meter))
  
    
  switch(menu){
      case 1:
      
      lcd.setCursor(0,0);
      digitalWrite(led_hijau,LOW);
      digitalWrite(led_kuning,HIGH);
      lcd.clear();
      reset();
      
    
      lcd.print("PENGUKURAN");
      lcd.setCursor(0,1);
      lcd.print("METER:");
      lcd.print(result);
      
      
      break;

      
      case 2: 
      digitalWrite(led_hijau,LOW);
      digitalWrite(led_kuning,HIGH);
      lcd.clear();
      reset();
      lcd.setCursor(0,0);
      lcd.print("45 Meter");
      lcd.setCursor(0,1);
      lcd.print("METER:");
      lcd.print(result);
     
      if(result>45){
        digitalWrite(buzzer,HIGH);
      }
      else{
        digitalWrite(buzzer,LOW);
      }
      
      break;


      case 3: 
      digitalWrite(led_hijau,LOW);
      digitalWrite(led_kuning,HIGH);
      lcd.clear();
      reset();
      lcd.setCursor(0,0);
      lcd.print("50 Meter");
      lcd.setCursor(0,1);
      lcd.print("METER:");
      lcd.print(result);
     
      if(result>50){
        digitalWrite(buzzer,HIGH);
      }
      else{
        digitalWrite(buzzer,LOW);
      }
      break;


      case 4: 
      digitalWrite(led_hijau,LOW);
      digitalWrite(led_kuning,HIGH);
      lcd.clear();
      reset();
      
      lcd.setCursor(0,0);
      lcd.print("90 Meter");
      lcd.setCursor(0,1);
      lcd.print("METER:");
      lcd.print(result);
      if(result>90){
        digitalWrite(buzzer,HIGH);
      }
      else{
        digitalWrite(buzzer,LOW);
      }
      
      break;

      case 5: 
      digitalWrite(led_hijau,LOW);
      digitalWrite(led_kuning,HIGH);
      lcd.print(result);
      
      reset();
      lcd.setCursor(0,0);
      lcd.print("100 Meter");
      lcd.setCursor(0,1);
      lcd.print("METER:");
      
      if(result>100){
        digitalWrite(buzzer,HIGH);
      }
      else{
        digitalWrite(buzzer,LOW);
      }
      break;
    }

  
}


//=======================================DONE==============================================

//=======================================MAIN PROGRAM===========================================


void setup() {
  Serial.begin(9600);
  pinMode(rst_button, INPUT_PULLUP);
  pinMode(down_button, INPUT_PULLUP);
  pinMode(up_button, INPUT_PULLUP);
  pinMode(select_button, INPUT_PULLUP);
 
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
  lcd.setCursor(3,1);
  lcd.print("MEASUREMENT");
  delay(2000);
  lcd.clear();
  updateMenu();
  
}



void loop() {

    // get the value length in meter
      distance_M = (position/10)*distanceC;
      result = distance_M / 100;
    //==========================================
   
   current = millis();
   if (current-pre_time>33){
        rst_state = digitalRead(rst_button);
         down_state = digitalRead(down_button);
         up_state = digitalRead(up_button);
         selectl_state = digitalRead(select_button);
        pre_time = current;
   }
   
    if (down_state==LOW and page==1){
               menu++;
               if(menu>5){
                menu=1;
                  }
               updateMenu();
               delay(100);
               }

          if (up_state==LOW and page==1 ){
               menu--;
               if(menu<1){
                menu=5;
              }
              updateMenu();
              delay(100);
              
          }
          
    if (selectl_state==LOW){
              page=2;
              state++;
              lcd.clear();
              if (state>1){
              state=0;
              }

              if(state == 0){
                Action();
                page=2;
                }
                delay(100);


              if(state == 1){
                updateMenu();
                page=1;
                }
                delay(100);

    }
   
              
}

//=======================================DONE==============================================
