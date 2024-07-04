
#define DHTPIN 1     // Digital pin connected to the DHT sensor
#define PH_PIN A2
#define ONE_WIRE_BUS 2 // Pin Arduino a cui colleghiamo il pin DQ del sensore
#define TdsSensorPin A1
#define p1 3
#define p2 4
#define p3 5
#define p4 6
#define ev 7

#define oneml 1115

//sensors lib
//DHT11
#include "DHT.h"
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

//PH
#include "DFRobot_PH.h"
//#include <EEPROM.h>
float voltage,phValue,temperature;
DFRobot_PH ph;

//TempH2O
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(ONE_WIRE_BUS); // Imposta la connessione OneWire
DallasTemperature sensore(&oneWire); 

//TDS
#define VREF 5.0      // analog reference voltage(Volt) of the ADC
#define SCOUNT  30           // sum of sample point
int analogBuffer[SCOUNT];    // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0, copyIndex = 0;
float averageVoltage = 0, tdsValue = 0, temp=25 ;

//pump
unsigned long int Pu1=0;
unsigned long int Pu2=0;
unsigned long int Pu3=0;
unsigned long int Pu4=0;

unsigned long liter =1000;

boolean pumping = false;

int state=6;


void setup() {
  // put your setup code here, to run once:
  //DHT11
    Serial.println(F("DHTxx test!"));
    dht.begin();
    
    //PH
    ph.begin();

    //TempH2O
    sensore.begin();
    
    //TDS
    pinMode(TdsSensorPin, INPUT);

    //pump
    pinMode(p1,OUTPUT);
    pinMode(p2,OUTPUT);
    pinMode(p3,OUTPUT);
    pinMode(p4,OUTPUT);
    pinMode(ev,OUTPUT);
    
    Serial.begin(9600); 
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0)
  {
  char in = Serial.read();
  state = in - '0';
  Serial.println(state);
  }
  switch(state){
  case 1:
    prova_pump();
    break;
  case 2: 
    funct_dht();   //dht   
    break;
  case 3: 
    funct_ds18();  //dallas
    break;
  case 4: 
    funct_ph();  //ps
    break;
  case 5: 
    funct_tds();   //tds
    break;
  case 0:
    break;
  case 6:
    Serial.println("Ciao");
    break;
    
  }
  
}


void prova_pump(){
  digitalWrite(p1, HIGH);
  delay(100);  //fill pipe
  digitalWrite(p1, LOW); 
  delay(1000);
  
  digitalWrite(p2, HIGH);
  delay(100);   //fill pipe
  digitalWrite(p2, LOW);
  delay(1000);
  
  digitalWrite(p3, HIGH);
  delay(100);   //fill pipe
  digitalWrite(p3, LOW);
  delay(1000);
  
  digitalWrite(p4, HIGH); 
  delay(100);   //fill pipe
  digitalWrite(p4, LOW);
  delay(1000);

  digitalWrite(ev, HIGH); 
  delay(100);   //fill pipe
  digitalWrite(ev, LOW);
  delay(1000);
}

void funct_dht(){
  float h = dht.readHumidity(); 
  float t = dht.readTemperature();  // Read temperature as Celsius (the default)
  float f = dht.readTemperature(true);// Read temperature as Fahrenheit (isFahrenheit = true)
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  float hif = dht.computeHeatIndex(f, h);// Compute heat index in Fahrenheit (the default)
  float hic = dht.computeHeatIndex(t, h, false);// Compute heat index in Celsius (isFahreheit = false)
  String hs = String(h);
  String ts = String(t);
  Serial.println("DHT Humid: "+hs +"%");
  Serial.println("Temperature " + ts );
}



void funct_ds18(){
  sensore.requestTemperatures(); // richiesta lettura temperatura  
  float celsius = sensore.getTempCByIndex(0);  // temperatura in Celsius
  String st= String(celsius);
  Serial.println("DS18B20" );
  Serial.println("Temp: "+ st );
  }

 void funct_tds(){
    analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);    //read the analog value and store into the buffer
    analogBufferIndex++;
    if (analogBufferIndex == SCOUNT)
      analogBufferIndex = 0;
  

    for (copyIndex = 0; copyIndex < SCOUNT; copyIndex++)
      analogBufferTemp[copyIndex] = analogBuffer[copyIndex];
    averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (float)VREF / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
    float compensationCoefficient = 1.0 + 0.02 * (temp - 25.0); //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
    float compensationVolatge = averageVoltage / compensationCoefficient; //temperature compensation
    tdsValue = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge - 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge) * 0.5; //convert voltage value to tds value
    
  String ppm= String(tdsValue);
  Serial.println("TDS");
  Serial.println("ppm : "+ ppm );
  
}

int getMedianNum(int bArray[], int iFilterLen)
{
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++)
  {
    for (i = 0; i < iFilterLen - j - 1; i++)
    {
      if (bTab[i] > bTab[i + 1])
      {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
  else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  return bTemp;
}

void funct_ph(){

        //temperature = readTemperature();         // read your temperature sensor to execute temperature compensation
        temperature = tempdall();  
        voltage = analogRead(PH_PIN)/1024.0*5000;  // read the voltage
        phValue = ph.readPH(voltage,temperature);  // convert voltage to pH with temperature compensation
        String st = String(temperature);
        String sp = String(phValue);
        Serial.println("PH Temp: " + st);
        Serial.println("valore Ph: " + sp);
        //ph.calibration(voltage,temperature);           // calibration process by Serail CMD
        return;
}

float readTemperature()
{
  delay(2000);
  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  }
  return t;
}
float tempdall(){
  sensore.requestTemperatures(); // richiesta lettura temperatura  
  float celsius = sensore.getTempCByIndex(0);  // temperatura in Celsius
  return celsius;
}

void funct_pump(){
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
  
  pumping = false;
}
