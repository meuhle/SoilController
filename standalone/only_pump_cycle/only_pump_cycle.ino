#define p1 3
#define p2 4
#define p3 5
#define p4 6
#define p5 7
#define p6 8

#define oneml 1115
#define NUM_PUMPS 6
#define PLANTS 9
//this must be added for single plant so for single litre
unsigned long int Pu1=22;
unsigned long int Pu2=22;
unsigned long int Pu3=22;
unsigned long int Pu4=26;
unsigned long int Pu5=22;
unsigned long int Pu6=15;

int pumps_pin[NUM_PUMPS] = {p1,p2,p3,p4,p5,p6};
int pumps_ml[NUM_PUMPS] = {Pu1,Pu2,Pu3,Pu4,Pu5,Pu6};
for(int i=0; i<NUM_PUMPS; i++){
    pumps_ml[i] = pumps_ml[i] * PLANTS ;
}

int pump_used = 6
boolean pumping = true;


void setup() {
    for(int i=0;i<NUM_PUMPS; i++){
        pinMode(pumps_pin[i], OUTPUT);
    }    
    //Serial.begin(9600); 
}

void loop() {
 if (pumping == true){
    for( int i = 0 ; i< NUM_PUMPS; i++){
        digitalWrite(pumps_pin[i], HIGH);
        delay(pumps_ml[i] * oneml);  //fill pipe
        digitalWrite(pumps_pin[i], LOW); 
        delay(1000);
    } 
  pumping = false;  
 }
}
