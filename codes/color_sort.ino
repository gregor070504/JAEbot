// ďalšie info k RGB senzoru nájdete na stránke výrobcu https://www.adafruit.com/product/1334
#include <Wire.h>
#include <Adafruit_TCS34725.h>// importujeme knižnice potrebné pre používanie RGB senzora

// pre použitie tohoto senzora nedefinujeme žiadne piny, pretože na komunikáciu využíva I2C zbernicu (piny SDA a SCL)

Adafruit_TCS34725 rgb_senzor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);// vytvoríme objekt s názvom tcs pre náš senzor
float red, green, blue;// vytvoríme premenné pre načítanie hodnôt zo senzora
int R, G, B;

//OLED display
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
// pre použitie displaya nedefinujeme žiadne piny, pretože na komunikáciu využíva I2C zbernicu (piny SDA a SCL)
Adafruit_SSD1306 display(SCL);    //vytvoríme objekt s názvom display pre OLED display


void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

// overíme ži je pripojený funkčný sezor
  if (rgb_senzor.begin()) {
    
  } else {
    Serial.println("TCS34725 senzor sa nenašiel, skontrolujte zapojenie");
    while (1); // ak nenajde senzor, tak spustíme spustíme nekonečný cyklus, pretože zbytok kódu nemá zmysel vykonať
  }

}

void loop() {

  display.clearDisplay(); //vyčistenie pamäte na displayi
  display.setTextColor(WHITE); //nastavenie farby textu na bielu
  display.setCursor(32,0); //nastavenie začiatku textu na displayi na ľavý horný roh (0px a 0px)


  rgb_senzor.setInterrupt(false);// zapneme LED diódu senzora
  delay(60);  // počkáme 60ms
  rgb_senzor.getRGB(&red, &green, &blue);// načítame výslup zo senzora do premenných
  rgb_senzor.setInterrupt(true);// vypneme LED senzora

  R = int(red);
  G = int(green);
  B = int(blue);
  
// vypíšeme namerané hodnoty na serial monitor
  Serial.print("R:\t"); Serial.print(R);// \t sa na výstupe nezobrazí ale vytvorí medzeru medzi textom
  Serial.print("\tG:\t"); Serial.print(G);// načítané hodnoty premeníme na celé čísla pomocou int()
  Serial.print("\tB:\t"); Serial.println(B);// pre posledný výpis použijeme pritln aby sa daľšie výpisi zobrazovali na novom riadku


if ((R <= 70 && R > 55) && (G <= 100 && G > 85) && (B <= 125 && B > 110)){
  Serial.println(" biela "); 
  display.println(" biela "); 
 
}

else if ((R <= 105 && R >= 80) && (G <= 100 && G >= 85) && (B <= 110 && B >= 95)){
  Serial.println(" cierna  "); 
  display.println(" cierna "); 
}

else if ((R <= 150 && R >= 110) && (G <= 85 && G >=60) && (B <= 105 && B >= 75)){
  Serial.println(" cervena  ");
  display.println(" cervena "); 
  
}

else if ((R <= 110 && R >= 80) && (G <= 150 && G >= 100) && (B <= 110 && B >= 95)){
  Serial.println(" zelena  "); 
  display.println(" zelena "); 
  
}

else if ((R <= 75 && R >= 30) && (G <= 100 && G >= 80) && (B <= 160 && B >= 130)){
  Serial.println(" modra  "); 
  display.println(" modra "); 
  
}

display.display();
delay(250);
 
 
}
