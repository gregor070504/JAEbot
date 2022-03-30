#include <SoftwareSerial.h>
#include <Wire.h>
#include <SPI.h>

//OLED display
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
// pre použitie displaya nedefinujeme žiadne piny, pretože na komunikáciu využíva I2C zbernicu (piny SDA a SCL)
Adafruit_SSD1306 display(SCL);    //vytvoríme objekt s názvom display pre OLED display

SoftwareSerial wifiSerial(50, 51);      // RX, TX for ESP8266

bool DEBUG = true;   //show more logs
int responseTime = 10; //communication timeout

void setup()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  pinMode(13,OUTPUT);  //set build in led as output
  // Open serial communications and wait for port to open esp8266:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  wifiSerial.begin(115200);
  while (!wifiSerial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  //sendToWifi("AT+RST",responseTime,DEBUG);
  sendToWifi("AT+CWMODE=2",responseTime,DEBUG); // configure as access point
  sendToWifi("AT+CIFSR",responseTime,DEBUG); //192.168.4.1
  sendToWifi("AT+CIPMUX=1",responseTime,DEBUG); // configure for multiple connections
  delay(100);
  sendToWifi("AT+CIPSERVER=1,65432",responseTime,DEBUG); // turn on server on port 65432
 
  sendToUno("Wifi connection is running!",responseTime,DEBUG);
}

void loop(){
  display.clearDisplay(); //vyčistenie pamäte na displayi
  

  display.setTextColor(WHITE); //nastavenie farby textu na bielu
  display.setCursor(0,0); //nastavenie začiatku textu na displayi na ľavý horný roh (0px a 0px)

  String message = readWifiSerialMessage(); //načítanie správy z wifi modulu
  sendToUno(message,responseTime,DEBUG);  //poslanie správy z wifi modulu na arduino

  if (message){

  message = message.substring(9);
  display.println(message); //vypísanie správy na display
  Serial.println(message);
  
  display.display(); //aktualizovanie zobrazenia na displayi
  delay(5000);
  //sendData(String(message));
  delay(responseTime);
  }

  else{
    delay(responseTime);
  }
  
}

void sendData(String message){
  String len="";
  len+=message.length();
  String command="AT+CIPSEND=0,";
  command+=len;
  command+="\r\n";
  //sendToUno(command,responseTime,DEBUG);
  sendToWifi(command,responseTime,DEBUG);
  delay(100);
  //sendToWifi(message,responseTime,DEBUG);
  //delay(100);
}

/*
* Name: readSerialMessage
* Description: Function used to read data from Arduino Serial.
* Params: 
* Returns: The response from the Arduino (if there is a reponse)
*/
String  readSerialMessage(){
  char value[100]; 
  int index_count =0;
  while(Serial.available()>0){
    value[index_count]=Serial.read();
    index_count++;
    value[index_count] = '\0'; // Null terminate the string
  }
  String str(value);
  str.trim();
  return str;
}

/*
* Name: readWifiSerialMessage
* Description: Function used to read data from ESP8266 Serial.
* Params: 
* Returns: The response from the esp8266 (if there is a reponse)
*/
String  readWifiSerialMessage(){
  char value[100]; 
  int index_count =0;
  while(wifiSerial.available()>0){
    value[index_count]=wifiSerial.read();
    index_count++;
    value[index_count] = '\0'; // Null terminate the string
  }
  String str(value);
  str.trim();
  return str;
}

/*
* Name: sendToWifi
* Description: Function used to send data to ESP8266.
* Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
* Returns: The response from the esp8266 (if there is a reponse)
*/
String sendToWifi(String command, const int timeout, boolean debug){
  String response = "";
  wifiSerial.println(command); // send the read character to the esp8266
  long int time = millis();
  while( (time+timeout) > millis())
  {
    while(wifiSerial.available())
    {
    // The esp has data so display its output to the serial window 
    char c = wifiSerial.read(); // read the next character.
    response+=c;
    }  
  }
  if(debug)
  {
    Serial.println(response);
  }
  return response;
}

/*
* Name: sendToUno
* Description: Function used to send data to Arduino.
* Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
* Returns: The response from the esp8266 (if there is a reponse)
*/
String sendToUno(String command, const int timeout, boolean debug){
  String response = "";
  Serial.println(command); // send the read character to the esp8266
  long int time = millis();
  while( (time+timeout) > millis())
  {
    while(Serial.available())
    {
      // The esp has data so display its output to the serial window 
      char c = Serial.read(); // read the next character.
      response+=c;
    }  
  }
  if(debug)
  {
    Serial.println(response);
  }
  return response;
}
