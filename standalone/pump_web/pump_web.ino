//--------------------Wi-Fi Side-----------------
#include <WiFi.h>
const char* ssid = "Quaglia";
const char* password = "Wirele55";
WiFiClient client = NULL;
WiFiServer server(80);
String header;
bool SD_ready = false;
const int num_line_html = 32;   //define arrray size, must know the html line, you can also use much less rows by just putting everything in same line
String html_line[num_line_html];

void get_page(){
    File file = SD_MMC.open("/html_page.txt");
    if (!file) {
      Serial.println("Failed to open file for reading");
      return;
    }
    int i = 0;
    Serial.println("Reading file line by line:");
    while (file.available()) {
      String line = file.readStringUntil('\n');
      html_line[i] =  line;
      Serial.println(line);
      i++;
    }
    file.close();
}
void print_page(){
  for (int i =0 ; i< sizeof(html_line)/sizeof(html_line)[0];i++){
    client.println(html_line[i]);
  }
}
//---------------------------------------yu

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
unsigned long int Pu1=0;
unsigned long int Pu2=0;
unsigned long int Pu3=0;
unsigned long int Pu4=0;
unsigned long int Pu5=0;
unsigned long int Pu6=0;

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
    get_page();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.print("Camera Stream Ready! Go to: http://");
  Serial.print(WiFi.localIP());  
}

void pump() {
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

void loop(){
    client = server.available();
   if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()  && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        header += c;
        //Serial.print(header); //CHECK THIS TO CHANGE THE PAGES?
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {            
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            if (header.indexOf("GET /pump") != -1) {
              int p =   header.indexOf("GET /pump?val=")+14;
              int pv = header.indexOf(",",p);
              Pu1 = header.substring(p,pv).toInt();
              p = header.indexOf(",",pv);
              Pu2 = header.substring(pv,p).toInt();
              pv = header.indexOf(",",p);
              Pu3 = header.substring(p,pv).toInt();
              p = header.indexOf(",",pv);
              Pu4 = header.substring(pv,p).toInt();
              pv = header.indexOf(",",p);
              Pu5 = header.substring(p,pv).toInt();
              p = header.indexOf(",",pv);
              Pu6 = header.substring(pv,p).toInt();              
              header="";
            }
            print_page();                        
            // The HTTP response ends with another blank line
            if (pumping){
                client.println(<footer style="background-color: red;"> STILL TO PUMP </footer> )
            }else{
                 client.println(<footer  style="background-color: green;"> PUMPED  </footer> )
            }            
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  delay(100);
}

