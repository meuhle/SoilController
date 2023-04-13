/*
 * Serial Commands:
 *   enterph -> enter the calibration mode
 *   calph   -> calibrate with the standard buffer solution, two buffer solutions(4.0 and 7.0) will be automaticlly recognized
 *   exitph  -> save the calibrated parameters and exit from calibration mode
*/
//btn
const int buttonPin = 4;     // the number of the pushbutton pin
int buttonState = 0;  
int lastButtonState = 0;
int cont =0;
 

//DHT11
#include "DHT.h"
#define DHTPIN 6     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

//PH
#include "DFRobot_PH.h"
#include <EEPROM.h>
#define PH_PIN A2
float voltage,phValue,temperature;
DFRobot_PH ph;

//TempH2O
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2 // Pin Arduino a cui colleghiamo il pin DQ del sensore
OneWire oneWire(ONE_WIRE_BUS); // Imposta la connessione OneWire
DallasTemperature sensore(&oneWire); 

//TDS
#define TdsSensorPin A1
#define VREF 5.0      // analog reference voltage(Volt) of the ADC
#define SCOUNT  30           // sum of sample point
int analogBuffer[SCOUNT];    // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0, copyIndex = 0;
float averageVoltage = 0, tdsValue = 0, temp=25 ;


//Liquid Crystal
#include <LiquidCrystal_I2C.h>
// Imposta l'indirizzo del display a 0x27 per 16 caratteri 2 linee
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
  
    

    //btn
    pinMode(buttonPin, INPUT);
    
    //dht
    Serial.println(F("DHTxx test!"));
    dht.begin();
    
    //ph
    ph.begin();

    //TempH2O
    sensore.begin();
    
    //TDS
    pinMode(TdsSensorPin, INPUT);
    
    //lcd
    lcd.begin(); //Init with pin default ESP8266 or ARDUINO
    lcd.backlight(); //accende la retroilluminazione
    Serial.begin(9600); 
}

void loop()
{

   buttonState = digitalRead(buttonPin);
  if (buttonState != lastButtonState) {
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    cont++;
    Serial.println(cont);
    switch(cont){
    case 1:
      funct_dht();
      break;
    case 2:
      funct_ds18();
      break;
    case 3:
      funct_tds();
      break;
    case 4:
      funct_ph();
      break;
  }
  
  }/*
  if (cont ==1){
    funct_dht();
  }else if (cont ==2){
    funct_ds18();
  }else if (cont ==3){
    funct_tds();
  }else if (cont ==4){
    funct_ph();
  }*/
  delay(50);
  }
  lastButtonState = buttonState;
  if (cont>=4){
    cont=0;
  }
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

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DHT Humid: "+hs +"%");
  lcd.setCursor(0, 1);
  lcd.print("Temperature " + ts );


  /*Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
    */delay (50);
}

void funct_ds18(){
  sensore.requestTemperatures(); // richiesta lettura temperatura
  
  // Restituzione della temperatura letta
  // in gradi Celsius

  // temperatura in Celsius
  float celsius = sensore.getTempCByIndex(0);
  
  // Visualizzazione delle letture 
  // della temperatura sulla Serial monitor
  
  /*Serial.print("C:");
  Serial.println(celsius);*/
  String st= String(celsius);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DS18B20" );
  lcd.setCursor(0, 1);
  lcd.print("Temp: "+ st );


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
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TDS");
  lcd.setCursor(0, 1);
  lcd.print("ppm : "+ ppm );
  
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

        temperature = readTemperature();         // read your temperature sensor to execute temperature compensation
        voltage = analogRead(PH_PIN)/1024.0*5000;  // read the voltage
        phValue = ph.readPH(voltage,temperature);  // convert voltage to pH with temperature compensation
        /*Serial.print("temperature:");
        Serial.print(temperature,1);
        Serial.print("^C  pH:");
        Serial.println(phValue,2);*/
        String st = String(temperature);
        String sp = String(phValue);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("PH Temp: " + st);
        lcd.setCursor(0, 1);
        lcd.print("valore Ph: " + sp);
    ph.calibration(voltage,temperature);           // calibration process by Serail CMD
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
