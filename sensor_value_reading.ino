// tento kód bude načítávať hodnoty zo vštkých senzorov a vypisovať ich na OLED displej
#include <Wire.h> // knižnica pre komunikáciu pomocou I2C zbernice
// ultrazvukový senzor
const int trig_pin = 22;// definujeme spúšťací pin
const int echo_pin = 23;// a pin na prijatie odozvy
// definujeme premenné pre čas a vzdialenosť
long echo_time;
int distance;

// infračervené senzory
const int ir_sensor1_pin = 24;  // definujeme vstupné piny pre IR senzory
const int ir_sensor2_pin = 25;
const int ir_sensor3_pin = 27;
// kedže výstup za senzora môže mať len 2 hodnoty, použijeme premennú typu bool
bool ir_sensor1_detect = HIGH;  // a premenné pre detekciu prekážok
bool ir_sensor2_detect = HIGH;
bool ir_sensor3_detect = HIGH;

// line senzor
const int line_sensor1_pin = 29;// definujeme čísla pinov pre senzory
const int line_sensor2_pin = 28;
const int line_sensor3_pin = 31;
const int line_sensor4_pin = 30;

bool line_sensor1_value;// definujeme premenné, do ktorých budeme ukladať výstupy zo senzorov
bool line_sensor2_value;
bool line_sensor3_value;
bool line_sensor4_value;

// RGB senzor
#include <Adafruit_TCS34725.h>// importujeme knižnice potrebné pre používanie RGB senzora
// pre použitie tohoto senzora nedefinujeme žiadne piny, pretože na komunikáciu využíva I2C zbernicu (piny SDA a SCL)
Adafruit_TCS34725 rgb_senzor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);// vytvoríme objekt s názvom tcs pre náš senzor
float red, green, blue;// vytvoríme premenné pre načítanie hodnôt zo senzora

// fotorezistor
const int light_sensor_pin = A10;// definujeme pin senzora, pre fotorezistor použijeme vstupný analógový pin
int light_value;// vytvoríme premennú pre čítanie hodnôt zo senzora

// Gyroskop/Akcelerometer
#include <SPI.h>
#include <MPU6050.h>
// pre použitie tohoto senzora nedefinujeme žiadne piny, pretože na komunikáciu využíva I2C zbernicu (piny SDA a SCL)
MPU6050 mpu;    //vytvoríme objekt s názvom mpu pre náš senzor

//DHT senzor
#include "DHT.h"
const int DHTPIN=32;     //digitálny pin pripojený k dht senzoru
#define DHTTYPE DHT11   //definovanie typu senzoru - v tomto prípade DHT 11
DHT dht(DHTPIN, DHTTYPE);   //vytvoríme objekt s názvom dht pre náš senzor DHT

//OLED display
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
// pre použitie displaya nedefinujeme žiadne piny, pretože na komunikáciu využíva I2C zbernicu (piny SDA a SCL)
Adafruit_SSD1306 display(SCL);    //vytvoríme objekt s názvom display pre OLED display

//Rotačný enkóder - prepínanie medzi jednotlivými senzormi
const int encoderPinA = 34;
const int encoderPinB = 35;
const int btnPin=36;
int encoderPos = 0;
int encoderPinALast = LOW;
int n = LOW;
int btnStateLast=LOW;
int btnState=LOW;

void setup() {
Serial.begin(9600); // inicializujeme komunikáciu so sériovým monitorom na 9600 baud  
// ultrazvukový senzor
  pinMode(trig_pin, OUTPUT); // nastavíme spúšťací pin na výstup
  pinMode(echo_pin, INPUT); // a pin pre odozvu na vstup
  
// infračervené senzory
  pinMode(ir_sensor1_pin, INPUT);// nastavýme vstupné piny
  pinMode(ir_sensor2_pin, INPUT);
  pinMode(ir_sensor3_pin, INPUT);

//line senzor
  pinMode(line_sensor1_pin,INPUT);// nastavíme piny senzorov na vstup 
  pinMode(line_sensor2_pin,INPUT);
  pinMode(line_sensor3_pin,INPUT);
  pinMode(line_sensor4_pin,INPUT);

// RGB senzor
  if (rgb_senzor.begin()) {
  
    } else {
      Serial.println("TCS34725 senzor sa nenašiel, skontrolujte zapojenie");
      while (1); // ak nenajde senzor, tak spustíme spustíme nekonečný cyklus, pretože zbytok kódu nemá zmysel vykonať
    }

// fotorezistor
  pinMode(light_sensor_pin, INPUT);// nastavíme pin senzora na vstup
  
// Gyroskop/Akcelerometer - MPU6050
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)){    
    Serial.println("Nenašiel sa MPU6050 senzor. Skontrolujte zapojenie senzora."); // ak nenájde senzor vypíše chybovú hlášku a pokračuje v hľadaní senzora
    delay(500);
  }
  
// senzor DHT11
  dht.begin();

// OLED display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

// Rotačný enkóder
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
}

void loop() {
  n = digitalRead(encoderPinA); //do premennej n si načítame hodnotu na pine A rotačného enkóderiu (HIGH alebo LOW)
  if ((encoderPinALast == LOW) && (n == HIGH)) {  //overíme či sa hodnota na pine A zmenila z LOW na HIGH
    if (digitalRead(encoderPinB) == LOW) {  //overíme či hodnota na pine B rotačného enkóderu je LOW
      encoderPos--; //ak je hodnota na pine B LOW pozícia enkóderu sa zvyšuje (otáča sa doprava)
    } else {
      encoderPos++; //ak je hodnota na pine B HIGH (nie je LOW) pozícia enkóderu sa znižuje (otáča sa doľava)
    }
  }
  if ((encoderPinALast == HIGH) && (n == LOW)) {  //overíme či sa hodnota na pine A zmenila z HIGH na LOW
    if (digitalRead(encoderPinB) == HIGH) { //overíme či hodnota na pine B rotačného enkóderu je HIGH
      encoderPos--; //ak je hodnota na pine B HIGH pozícia enkóderu sa zvyšuje (otáča sa doprava)
    } else {
      encoderPos++; //ak je hodnota na pine B LOW (nie je HIGH) pozícia enkíderu sa znižuje (otáča sa doľava)
    }
  }
  Serial.println(encoderPos); //vypíšeme pozíciu na Serial monitor pre overenie
  encoderPinALast = n;  //nastavíme poslednú hodnotu na pine A
  if(encoderPos>6){  //overíme či je pozícia enkóderu väčšia ako 6 alebo menšia ako -6
    encoderPos=0; //nastavíme pozíciu opäť na 0
  }
  else if(encoderPos<0){
    encoderPos=0; //nastavíme pozíciu opäť na 0
  }
  switch(encoderPos){
    case 0:
      read_ultrasonic_sensor();
      break;
    case 1:
      read_ir_value();
      break;
    case 2:
      read_line_sensor();
      break;
    case 3:
      read_rgb_sensor();
      break;
    case 4:
      read_photoresistor();
      break;
    case 5:
      read_mpu_sensor();
      break;
    case 6:
      read_dht_sensor();
      break;
  }
}

void read_ultrasonic_sensor()
{
  display.clearDisplay(); //vyčistenie pamäte na displayi

  display.setTextColor(WHITE); //nastavenie farby textu na bielu
  display.setCursor(0,0); //nastavenie začiatku textu na displayi na ľavý horný roh (0px a 0px)
  
  // vypneme spúšťací pin
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  // zapneme spúšťací pin na 10 mikrosekúnd
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  
  // z echo_pin načítame čas odzvy v mikrosekundách
  echo_time = pulseIn(echo_pin, HIGH);
  // prepočítame čas odozvy na vzdialenosť pomocou rýchlosti zvuku
  distance = echo_time / 29 / 2;
  // vypíšeme vzdialenosť na Serial Monitor
  display.print("Vzdialenosť: "); display.print(distance); display.println(" cm");

  //aktualizovanie zobrazenia na displayi
  display.display();
  delay(250);   
}

void read_ir_value()
{
  display.clearDisplay(); //vyčistenie pamäte na displayi

  display.setTextColor(WHITE); //nastavenie farby textu na bielu
  display.setCursor(0,0); //nastavenie začiatku textu na displayi na ľavý horný roh (0px a 0px)
  
  ir_sensor1_detect = digitalRead(ir_sensor1_pin);// načítame výstupy zo senzorov do premmenných
  ir_sensor2_detect = digitalRead(ir_sensor2_pin);
  ir_sensor3_detect = digitalRead(ir_sensor3_pin);

// senzor 1
  if (ir_sensor1_detect == LOW)// ak sa detekuje prekážka vypíšeme to na display
  {display.println("senzor1: Prekážka!");}
  
  else// inak vypíšeneme "Žiadna prekážka"
  {display.println("senzor1: Žiadna prekážka");}
  
// senzor 2
  if (ir_sensor2_detect == LOW)
  {display.println("senzor2: Prekážka!");}
  
  else
  {display.println("senzor2: Žiadna prekážka");}

//senzor 3
  if (ir_sensor3_detect == LOW)
  {display.println("senzor3: Prekážka!");}
  
  else
  {display.println("senzor3: Žiadna prekážka");}

  //aktualizovanie zobrazenia na displayi
  display.display();
  delay(200);// počkáme 200ms   
}

void read_line_sensor()
{
  display.clearDisplay(); //vyčistenie pamäte na displayi

  display.setTextColor(WHITE); //nastavenie farby textu na bielu
  display.setCursor(0,0); //nastavenie začiatku textu na displayi na ľavý horný roh (0px a 0px)
  
  line_sensor1_value = digitalRead(line_sensor1_pin);// načítame výstup zo senzorov do premenných
  line_sensor2_value = digitalRead(line_sensor2_pin);
  line_sensor3_value = digitalRead(line_sensor3_pin);
  line_sensor4_value = digitalRead(line_sensor4_pin);
  
  // pre každý senzor do riadku vypíšeme snímanú farbu podľa hodnoty výstupu  
  /////senzor 1
  if (line_sensor1_value == HIGH)
  {display.println("senzor 1: biela");}
  
  else
  {display.println("senzor 1: čierna");}
  
  /////senzor 2
  if (line_sensor2_value == HIGH)
  {display.println("senzor 2: biela");}
  
  else
  {display.println("senzor 2: čierna");}
  
  /////senzor 3
  if (line_sensor3_value == HIGH)
  {display.println("senzor 3: biela");}
  
  else
  {display.println("senzor 3: čierna");}
  
  /////senzor 4
  if (line_sensor4_value == HIGH)
  {display.println("senzor 4: biela");}// za posledným výpisom použijeme println aby sa nová séria výpisov zobrazila v novom riadku
  
  else
  {display.println("senzor 4: čierna");}

  //aktualizovanie zobrazenia na displayi
  display.display();
  delay(500);// počkáme 500ms (0,5s)  
}

void read_rgb_sensor()
{
  display.clearDisplay(); //vyčistenie pamäte na displayi
  
  display.setTextColor(WHITE); //nastavenie farby textu na bielu
  display.setCursor(0,0); //nastavenie začiatku textu na displayi na ľavý horný roh (0px a 0px)
  
  rgb_senzor.setInterrupt(false);// zapneme LED diódu senzora
  delay(60);  // počkáme 60ms
  rgb_senzor.getRGB(&red, &green, &blue);// načítame výslup zo senzora do premenných
  rgb_senzor.setInterrupt(true);// vypneme LED senzora
  
  // vypíšeme namerané hodnoty na serial monitor
  display.print("R:  "); display.println(int(red));
  display.print("G:  "); display.println(int(green));// načítané hodnoty premeníme na celé čísla pomocou int()
  display.print("B:  "); display.println(int(blue));// pre posledný výpis použijeme pritln aby sa daľšie výpisi zobrazovali na novom riadku

  //aktualizovanie zobrazenia na displayi
  display.display();
}

void read_photoresistor()
{
  display.clearDisplay(); //vyčistenie pamäte na displayi

  display.setTextColor(WHITE); //nastavenie farby textu na bielu
  display.setCursor(0,0); //nastavenie začiatku textu na displayi na ľavý horný roh (0px a 0px)
  
  light_value = analogRead(light_sensor_pin);// načítame výstup zo senzora do premennej
  display.print("intenzita svetla: "); display.println(light_value);// vypíšeme hodnotu na serial monitor

  //aktualizovanie zobrazenia na displayi
  display.display();
  delay(500);// počkáme 500ms
}

void read_mpu_sensor()
{
  Vector rawGyro = mpu.readRawGyro();
  Vector normGyro = mpu.readNormalizeGyro();
  display.clearDisplay(); //vyčistenie pamäte na displayi

  display.setTextColor(WHITE); //nastavenie farby textu na bielu
  display.setCursor(0,0); //nastavenie začiatku textu na displayi na ľavý horný roh (0px a 0px)

  //vypísanie hodnôt z Gyroskopa/Akcelerometra na display
  display.print(" Xnorm = ");
  display.println(normGyro.XAxis);
  display.print(" Ynorm = ");
  display.println(normGyro.YAxis);
  display.print(" Znorm = ");
  display.println(normGyro.ZAxis);

  //aktualizovanie zobrazenia na displayi
  display.display();
}

void read_dht_sensor()
{
  // Čakanie na meranie - 2 sekundy
  delay(2000);

  display.clearDisplay(); //vyčistenie pamäte na displayi

  display.setTextColor(WHITE); //nastavenie farby textu na bielu
  display.setCursor(0,0); //nastavenie začiatku textu na displayi na ľavý horný roh (0px a 0px)

  // Čítanie vlhkosti vzduchu
  float h = dht.readHumidity();
  // Čítanie teploty v stupňoch Celzia
  float t = dht.readTemperature();
  // Čítanie teploty v stupňoch Fahrenheita
  float f = dht.readTemperature(true);

  // Kontorla meraní
  if (isnan(h) || isnan(t) || isnan(f)) {
    display.println("Nedokázaal som prečítať hodnoty z DHT senzora"); // ak arduino nedokáže prečítať hodnoty (hodnoty su NaN) vypíše chabovú hlášku
    return;
  }

  // výpočet teplotného indexu v stupňoch Fahrenheita
  float hif = dht.computeHeatIndex(f, h);
  
  // Výpočet teplotného indexu v stupňoch Celzia
  float hic = dht.computeHeatIndex(t, h, false);

  // vypísanie hodnôt
  display.print(" Vlhkosť: ");
  display.print(h);
  display.println(" %");
  display.print("Teplota: ");
  display.print(t);
  display.println(" C");

  //aktualizovanie zobrazenia na displayi
  display.display();
}
