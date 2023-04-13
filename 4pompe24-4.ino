
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#include <U8g2lib.h>
#include <Wire.h>
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);


#define p1 2
#define p2 3
#define p3 6
#define p4 7

#define oneml 1115
//joystick

int VRx = A1;
int VRy = A0;
int SW = 8;
int xPosition = 0;
int yPosition = 0;
int SW_state = 0;
int mapX = 0;
int mapY = 0;


int screen_page=3; 
int motor=0;
int clean=0;
unsigned long int Pu1=0;
unsigned long int Pu2=0;
unsigned long int Pu3=0;
unsigned long int Pu4=0;
int Page0 = 0;//pagina 0
int PageZ = 5; //ultima pagina aggiornare ogni volta che metto nuova pagina

void u8g2_prepare() {
 u8g2.setFont(u8g2_font_6x10_tf);
 u8g2.setFontRefHeightExtendedText();
 u8g2.setDrawColor(1);
 u8g2.setFontPosTop();
 u8g2.setFontDirection(0);
}


void setup() {
  // put your setup code here, to run once:
  pinMode(p1,OUTPUT);
  pinMode(p2,OUTPUT);
  pinMode(p3,OUTPUT);
  pinMode(p4,OUTPUT);

  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP); 

  u8g2.begin();
  u8g2_prepare();
  //Serial.begin(9600);
}

void loop() {
  xPosition = analogRead(VRx);
  yPosition = analogRead(VRy);
  SW_state = !digitalRead(SW);
  mapX = map(xPosition, 0, 1023, -512, 512);
  mapY = map(yPosition, 0, 1023, 512, -512);
  
  //change page
  if(mapX<=-500 and SW_state==0){
    if(screen_page>Page0){
      screen_page -= 1;
      //Serial.print("screen_page: ");Serial.print(screen_page);Serial.print("\n");
    }
  }
  if(mapX>=500 and SW_state==0){
    if(screen_page<PageZ){
      screen_page += 1;
      //Serial.print("screen_page: ");Serial.print(screen_page);Serial.print("\n");
    }
  }

  //Start pump
   if(SW_state==1 and screen_page==0){
    if(motor==0 && clean==0){
      motor =1;
      //Serial.print("motor: ");Serial.print(motor);Serial.print("\n");
    }
  }
  //Start clean
  if(SW_state==1 and screen_page==1){
    if(clean==0 and motor==0){
      clean = 1;
      //Serial.print("target: ");Serial.print(clean);Serial.print("\n");
    }
  }
  //Pump 1
  if(mapY>=500 and SW_state==0 and screen_page==2){
      Pu1 += 1;
      //Serial.print("R_ON: false");Serial.print("\n");
    }
  if(mapY<=-500 and SW_state==0 and screen_page==2){
   if(Pu1!=0){   
      Pu1 -= 1;
   }
      //Serial.print("R_ON: true");Serial.print("\n");
    }
  //Pump 2
  if(mapY>=500 and SW_state==0 and screen_page==3){
      Pu2 += 1;
      //Serial.print("R_ON: false");Serial.print("\n");
    }
  if(mapY<=-500 and SW_state==0 and screen_page==3){
   if(Pu2!=0){   
      Pu2 -= 1;
   }
      //Serial.print("R_ON: true");Serial.print("\n");
    }
  //Pump 3
  if(mapY>=500 and SW_state==0 and screen_page==4){
      Pu3 += 1;
      //Serial.print("R_ON: false");Serial.print("\n");
    }
  if(mapY<=-500 and SW_state==0 and screen_page==4){
     if(Pu3!=0){
      Pu3 -= 1;
     }
      //Serial.print("R_ON: true");Serial.print("\n");
    }
  //Pump 4
  if(mapY>=500 and SW_state==0 and screen_page==5){
      Pu4 += 1;
      //Serial.print("R_ON: false");Serial.print("\n");
    }
  if(mapY<=-500 and SW_state==0 and screen_page==5){
    if(Pu4!=0){
      Pu4 -= 1;
    }
      //Serial.print("R_ON: true");Serial.print("\n");
    }

    
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);

  
  //is pumping
  if(screen_page ==0){
    u8g2.drawStr(0, 0, "Pumping");

    if(motor==0){
       u8g2.drawStr(20, 20, "false");
    }else{
      u8g2.drawStr(20, 20, "true");
   }
    }
  //is cleaning
  if(screen_page ==1){
    u8g2.drawStr(0, 0, "Cleaning");
    
    if(clean==0){
       u8g2.drawStr(20, 20, "false");
    }else{
      u8g2.drawStr(20, 20, "true");
   }
   }
  //pump 1
  if(screen_page ==2){
    u8g2.drawStr(0, 0, "Pump 1");
    u8g2.setCursor(20, 20);
    u8g2.print(String(Pu1));
    }
  //pump 2
  if(screen_page ==3){
    u8g2.drawStr(0, 0, "Pump 2");
    u8g2.setCursor(20, 20);
    u8g2.print(String(Pu2));
    }
  //pump 3
  if(screen_page ==4){
    u8g2.drawStr(0, 0, "Pump 3");
    u8g2.setCursor(20, 20);
    u8g2.print(String(Pu3));
    }
  //pump 4
  if(screen_page ==5){
    u8g2.drawStr(0, 0, "Pump 4");
    u8g2.setCursor(20, 20);
    u8g2.print(String(Pu4));
    }
  u8g2.sendBuffer();
  if(motor==1){
  digitalWrite(p1, HIGH);
  delay(1000);  //fill pipe
  delay(oneml*Pu1); 
  digitalWrite(p1, LOW); 
  delay(2000);
  
  digitalWrite(p2, HIGH);
  delay(1000);   //fill pipe
  delay(oneml*Pu2);
  digitalWrite(p2, LOW);
  delay(2000);
  
  digitalWrite(p3, HIGH);
  delay(1000);   //fill pipe
  delay(oneml*Pu3);
  digitalWrite(p3, LOW);
  delay(2000);
  
  digitalWrite(p4, HIGH); 
  delay(1000);   //fill pipe
  delay(oneml*Pu4);
  digitalWrite(p4, LOW);
  motor=0;
  }
  else if(clean==1){
    digitalWrite(p1, HIGH);
    delay(5000);  //clean pipe
    digitalWrite(p1, LOW);
    digitalWrite(p2, HIGH);
    delay(5000);  //clean pipe
    digitalWrite(p2, LOW); 
    digitalWrite(p3, HIGH);
    delay(5000);  //clean pipe
    digitalWrite(p3, LOW);
    digitalWrite(p4, HIGH);
    delay(5000);  //clean pipe
    digitalWrite(p4, LOW);
    clean=0;
  } 

  delay(200);
}
