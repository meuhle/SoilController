#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2 // Pin Arduino a cui colleghiamo il pin DQ del sensore
const int pinLed = LED_BUILTIN; // Utilizzo del LED su scheda 

const int soglia = 25; // Accende il LED su scheda se si superano i 25°C

OneWire oneWire(ONE_WIRE_BUS); // Imposta la connessione OneWire

DallasTemperature sensore(&oneWire); // Dichiarazione dell'oggetto sensore

//Include la libreria
#include <LiquidCrystal_I2C.h>
// Imposta l'indirizzo del display a 0x27 per 16 caratteri 2 linee
LiquidCrystal_I2C lcd(0x27, 16, 2);


void setup(void)
{
  Serial.begin(9600);       // Inizializzazione della serial monitor
  sensore.begin();          // Inizializzazione del sensore
  pinMode(pinLed, OUTPUT);  // pinLed definito come OUTPUT

  // Stampa del messaggio di avvio
  Serial.println("Temperatura rilevata dal sensore DS18B20");
  Serial.println("----------------------------------------");
  lcd.begin(); //Init with pin default ESP8266 or ARDUINO
  lcd.backlight(); //accende la retroilluminazione
  delay(1000);
}

void loop()
{
  sensore.requestTemperatures(); // richiesta lettura temperatura
  
  // Restituzione della temperatura letta
  // in gradi Celsius

  // temperatura in Celsius
  float celsius = sensore.getTempCByIndex(0);
  
  // Visualizzazione delle letture 
  // della temperatura sulla Serial monitor
  
  Serial.print("C:");
  Serial.println(celsius);
  lcd.setCursor(0, 0);
  String st= String(celsius);
  lcd.print("Temp: "+ st );

  delay(1000);
}
