const int c = 261;// zadefinujeme konštanty pre tóny s frekvenciamy, ktoré im zodpovedajú (v Hertzoch) 
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 392;
const int a = 440;
const int h = 493;

const int speaker_pin = 53; //definujeme pin na ktorom je pripojený reproduktor alebo bzučiak

int note_array[] = {c,d,e,f,g,a,h};// vytvoríme pole,ktoré obsahuje konštanty tónov
void setup()
{
  pinMode(speaker_pin, OUTPUT);// nastavýme výstupný pin pre reproduktor
}

void loop()
{
 
// pomocou for cyklu prehráme všetky tóny v poli note_array
for (int i = 0; i < 7; i++) {
  // vygenerujeme tóny pomocov funkcie tone
  // prvý argument je výstupný pin a druhý je frekvencia tónu
  tone(speaker_pin, note_array[i]);// ku konštantám z pola note_array pristupujeme pomocov indexu i, ktorý sa pri každom vykonní cyklu zvýši o 1
  delay(250);// počkáme 250ms
  noTone(speaker_pin);// ukončíme tón
  delay(250);// počkáme 250ms
}  
}
