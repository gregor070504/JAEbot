#include <SoftwareSerial.h>
#include <AFMotor.h>

const int pwm_frequency = 64000; //ovládacia pwm frekvencia pre motory
const int motor_speed = 250; // defaultná rýchlosť motorov v rozsahu 0-255
// pre každý motor vytvoríme objekt
AF_DCMotor motor_left1(1,pwm_frequency);// prvý argument je číslo portu, v ktorom je motor zapojený (napr. port M1)
AF_DCMotor motor_left2(4,pwm_frequency);// druhý argument je ovládacia frekvencia, defaultne 1KHz
AF_DCMotor motor_right1(2,pwm_frequency);
AF_DCMotor motor_right2(3,pwm_frequency);

SoftwareSerial wifiSerial(50, 51);      // RX, TX for ESP8266

bool DEBUG = true;   //show more logs
int responseTime = 10; //communication timeout

void setup()
{
  pinMode(13,OUTPUT);  //set build in led as output
  // Open serial communications and wait for port to open esp8266:
  Serial.begin(115200);
  
  motor_left1.setSpeed(motor_speed);
  motor_left2.setSpeed(motor_speed);
  motor_right1.setSpeed(motor_speed);
  motor_right2.setSpeed(motor_speed);
  motor_stop(2000);
    
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
  motor_stop(10);
  String message = readWifiSerialMessage();
  sendToUno(message,responseTime,DEBUG);
  if(message.indexOf("+I")>-1){
    if(message.indexOf("u")>0){
      forward(200, motor_speed);
      delay(350);
    }
    else if(message.indexOf("d")>0){
      backward(200, motor_speed);
      delay(350);
    }
    else if(message.indexOf("r")>0){
      turn_right(250);
      delay(350);
    }
    else if(message.indexOf("l")>0){
      turn_left(250);
      delay(350);
    }
  }
  delay(responseTime);
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
  sendToWifi(message,responseTime,DEBUG);
  delay(100);
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

void forward(int run_time,int run_speed)// funkcia ma 2 argumenty - čas chodu motora a rýchlosť
   {
      motor_left1.setSpeed(run_speed);// nastavíme určenú rýchlosť
      motor_left2.setSpeed(run_speed);
      motor_right1.setSpeed(run_speed);
      motor_right2.setSpeed(run_speed);
      
      motor_left1.run(FORWARD); 
      motor_left2.run(FORWARD);// roztočíme motory jedným smerom
      motor_right1.run(FORWARD);
      motor_right2.run(FORWARD);   
      delay(run_time); 
   }

void backward(int run_time,int run_speed)
   {
      motor_left1.setSpeed(run_speed);
      motor_left2.setSpeed(run_speed);
      motor_right1.setSpeed(run_speed);
      motor_right2.setSpeed(run_speed);
      
      motor_left1.run(BACKWARD);
      motor_left2.run(BACKWARD);// roztočíme motory druhým smerom
      motor_right1.run(BACKWARD);
      motor_right2.run(BACKWARD);  
      delay(run_time);
   }
    
void motor_stop(int run_time)
   {
      motor_left1.run(RELEASE); 
      motor_left2.run(RELEASE);// zabrzdíme motory
      motor_right1.run(RELEASE);
      motor_right2.run(RELEASE);
      delay(run_time);
   }

void turn_left(int run_time)
   {
      motor_left1.setSpeed(motor_speed);
      motor_left2.setSpeed(motor_speed);
      motor_right1.setSpeed(motor_speed);
      motor_right2.setSpeed(motor_speed);
      
      motor_left1.run(FORWARD); 
      motor_left2.run(FORWARD);
      motor_right1.run(BACKWARD);
      motor_right2.run(BACKWARD); 
      delay(run_time);    
   }

void turn_right(int run_time)
   {
      motor_left1.setSpeed(motor_speed);
      motor_left2.setSpeed(motor_speed);
      motor_right1.setSpeed(motor_speed);
      motor_right2.setSpeed(motor_speed);
      
      motor_left1.run(BACKWARD); 
      motor_left2.run(BACKWARD);
      motor_right1.run(FORWARD);
      motor_right2.run(FORWARD); 
      delay(run_time); 
   }
