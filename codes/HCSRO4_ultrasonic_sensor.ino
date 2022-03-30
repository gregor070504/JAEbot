const int trig_pin = 22;// definujeme spúšťací pin
const int echo_pin = 23;// a pin na prijatie odozvy
// definujeme premenné pre čas a vzdialenosť
long echo_time;
int distance;
void setup() {
pinMode(trig_pin, OUTPUT); // nastavíme spúšťací pin na výstup
pinMode(echo_pin, INPUT); // a pin pre odozvu na vstup
Serial.begin(9600); // inicializujeme komunikáciu so sériovým monitorom na 9600 baud
}
void loop() {
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
Serial.print("Vzdialenosť: "); Serial.print(distance); Serial.println(" cm");
delay(250);
}
