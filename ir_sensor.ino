const int ir_sensor1_pin = 24;  // definujeme vstupné piny pre IR senzory
const int ir_sensor2_pin = 25;
const int ir_sensor3_pin = 27;

// kedže výstup za senzora môže mať len 2 hodnoty, použijeme premennú typu bool
bool ir_sensor1_detect = HIGH;  // a premenné pre detekciu prekážok
bool ir_sensor2_detect = HIGH;
bool ir_sensor3_detect = HIGH;

void setup() {
  pinMode(ir_sensor1_pin, INPUT);// nastavýme vstupné piny
  pinMode(ir_sensor2_pin, INPUT);
  pinMode(ir_sensor3_pin, INPUT);
  Serial.begin(9600);// začneme komunikáciu so serial monitor na 9600 baud 
}

void loop() {
  ir_sensor1_detect = digitalRead(ir_sensor1_pin);// načítame výstupy zo senzorov do premmenných
  ir_sensor2_detect = digitalRead(ir_sensor2_pin);
  ir_sensor3_detect = digitalRead(ir_sensor3_pin);

// senzor 1
  if (ir_sensor1_detect == LOW)// ak sa detekuje prekážka vypíšeme to na serial monitor
  {
    Serial.println("senzor1: Prekážka!");
  }
  else// inak vypíšeneme "Žiadna prekážka"
  {
    Serial.println("senzor1: Žiadna prekážka");
  }
  
// senzor 2
  if (ir_sensor2_detect == LOW)
  {
    Serial.println("senzor2: Prekážka!");
  }
  else
  {
    Serial.println("senzor2: Žiadna prekážka");
  }

//senzor 3
  if (ir_sensor3_detect == LOW)
  {
    Serial.println("senzor3: Prekážka!");
  }
  else
  {
    Serial.println("senzor3: Žiadna prekážka");
  }
  delay(1000);// počkáme 1000ms
}
