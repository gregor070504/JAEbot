const int light_sensor_pin = A10;// definujeme pin senzora, pre fotorezistor použijeme vstupný analógový pin
int light_value;// vytvoríme premennú pre čítanie hodnôt zo senzora

void setup() {
  pinMode(light_sensor_pin, INPUT);// nastavíme pin senzora na vstup
  Serial.begin(9600);// začneme komunikáciu so serailmonitor na 9600 baud
}

void loop() {
  light_value = analogRead(light_sensor_pin);// načítame výstup zo senzora do premennej
  Serial.print("intenzita svetla: "); Serial.println(light_value);// vypíšeme hodnotu na serial monitor
  delay(500);// počkáme 500ms

}
