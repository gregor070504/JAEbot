// ďalšie info k RGB senzoru nájdete na stránke výrobcu https://www.adafruit.com/product/1334
#include <Wire.h>
#include <Adafruit_TCS34725.h>// importujeme knižnice potrebné pre používanie RGB senzora

// pre použitie tohoto senzora nedefinujeme žiadne piny, pretože na komunikáciu využíva I2C zbernicu (piny SDA a SCL)

Adafruit_TCS34725 rgb_senzor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);// vytvoríme objekt s názvom tcs pre náš senzor
float red, green, blue;// vytvoríme premenné pre načítanie hodnôt zo senzora

void setup() {
  Serial.begin(9600);

// overíme či je pripojený funkčný sezor
  if (rgb_senzor.begin()) {

  } else {
    Serial.println("TCS34725 senzor sa nenašiel, skontrolujte zapojenie");
    while (1); // ak nenajde senzor, tak spustíme spustíme nekonečný cyklus, pretože zbytok kódu nemá zmysel vykonať
  }

}

void loop() {

  rgb_senzor.setInterrupt(false);// zapneme LED diódu senzora
  delay(60);  // počkáme 60ms
  rgb_senzor.getRGB(&red, &green, &blue);// načítame výslup zo senzora do premenných
  rgb_senzor.setInterrupt(true);// vypneme LED senzora
  
// vypíšeme namerané hodnoty na serial monitor
  Serial.print("R:\t"); Serial.print(int(red));// \t sa na výstupe nezobrazí ale vytvorí medzeru medzi textom
  Serial.print("\tG:\t"); Serial.print(int(green));// načítané hodnoty premeníme na celé čísla pomocou int()
  Serial.print("\tB:\t"); Serial.println(int(blue));// pre posledný výpis použijeme pritln aby sa daľšie výpisi zobrazovali na novom riadku
 
}
