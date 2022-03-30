// V tomto kóde využívame 4 infračervené senzory z IR panelu, označené číslamy 1,2,5,6 z ľava do prava

const int line_sensor1_pin = 29;// definujeme čísla pinov pre senzory
const int line_sensor2_pin = 28;
const int line_sensor3_pin = 31;
const int line_sensor4_pin = 30;

// kedže výstup za senzora môže mať len 2 hodnoty, použijeme premennú typu bool
 bool line_sensor1_value;// definujeme premenné, do ktorých budeme ukladať výstupy zo senzorov
 bool line_sensor2_value;
 bool line_sensor3_value;
 bool line_sensor4_value;

void setup() {
  pinMode(line_sensor1_pin,INPUT);// nastavíme piny senzorov na vstup 
  pinMode(line_sensor2_pin,INPUT);
  pinMode(line_sensor3_pin,INPUT);
  pinMode(line_sensor4_pin,INPUT);
  Serial.begin(9600);// začneme komunikáciu so serial monitor na 9600 baud

}

void loop() {

line_sensor1_value = digitalRead(line_sensor1_pin);// načítame výstup zo senzorov do premenných
line_sensor2_value = digitalRead(line_sensor2_pin);
line_sensor3_value = digitalRead(line_sensor3_pin);
line_sensor4_value = digitalRead(line_sensor4_pin);

// pre každý senzor do riadku vypíšeme snímanú farbu podľa hodnoty výstupu  
/////senzor 1
if (line_sensor1_value == HIGH)
{
  Serial.print("senzor 1: biela   ");   
}
else
{
  Serial.print("senzor 1: čierna   ");   
}

/////senzor 2
if (line_sensor2_value == HIGH)
{
  Serial.print("senzor 2: biela   ");   
}
else
{
  Serial.print("senzor 2: čierna   ");   
}

/////senzor 3
if (line_sensor3_value == HIGH)
{
  Serial.print("senzor 3: biela   ");   
}
else
{
  Serial.print("senzor 3: čierna   ");   
}

/////senzor 4
if (line_sensor4_value == HIGH)
{
  Serial.println("senzor 4: biela   ");// za posledným výpisom použijeme println aby sa nová séria výpisov zobrazila v novom riadku    
}
else
{
  Serial.println("senzor 4: čierna   ");   
}
delay(500);// počkáme 500ms (0,5s)
}
