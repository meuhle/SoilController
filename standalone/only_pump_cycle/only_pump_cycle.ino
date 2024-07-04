#define p1 3
#define p2 4
#define p3 5
#define p4 6
#define p5 7
#define p6 8

#define oneml 1115

unsigned long int Pu1=22;
unsigned long int Pu2=22;
unsigned long int Pu3=22;
unsigned long int Pu4=26;
unsigned long int Pu5=22;
unsigned long int Pu6=15;

boolean pumping = true;


void setup() {
    pinMode(p1,OUTPUT);
    pinMode(p2,OUTPUT);
    pinMode(p3,OUTPUT);
    pinMode(p4,OUTPUT);
    pinMode(p5,OUTPUT);
    pinMode(p6,OUTPUT);
    
    Serial.begin(9600); 

}

void loop() {
 if (pumping == true){

  //filling pipe first 
  digitalWrite(p1, HIGH);
  delay(7000);  //fill pipe
  digitalWrite(p1, LOW); 
  delay(2000);
  
  digitalWrite(p2, HIGH);
  delay(7000);  //fill pipe
  digitalWrite(p2, LOW);
  delay(2000);
  
  digitalWrite(p3, HIGH);
  delay(7000);   //fill pipe
  digitalWrite(p3, LOW);
  delay(2000);
  
  digitalWrite(p4, HIGH); 
  delay(7000);   //fill pipe
  digitalWrite(p4, LOW);
  delay(2000);

  digitalWrite(p5, HIGH);
  delay(7000);  //fill pipe
  digitalWrite(p5, LOW);
  delay(2000);
  /*
  digitalWrite(p6, HIGH); 
  delay(0* oneml);   //fill pipe
  digitalWrite(p6, LOW);
  */

  //fill fert
  digitalWrite(p1, HIGH);
  delay(Pu1* oneml);  //fill pipe
  digitalWrite(p1, LOW); 
  delay(2000);
  
  digitalWrite(p2, HIGH);
  delay(Pu2* oneml);  //fill pipe
  digitalWrite(p2, LOW);
  delay(2000);
  
  digitalWrite(p3, HIGH);
  delay(Pu3* oneml);   //fill pipe
  digitalWrite(p3, LOW);
  delay(2000);
  
  digitalWrite(p4, HIGH); 
  delay(Pu4* oneml);   //fill pipe
  digitalWrite(p4, LOW);
  delay(2000);

  digitalWrite(p5, HIGH);
  delay(Pu5* oneml);  //fill pipe
  digitalWrite(p5, LOW);
  delay(2000);
  /*
  digitalWrite(p6, HIGH); 
  delay(0* oneml);   //fill pipe
  digitalWrite(p6, LOW);
  */
  pumping = false;
  
 }

}
