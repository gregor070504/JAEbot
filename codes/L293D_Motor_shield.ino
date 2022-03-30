// tento kód slúži na overenie funkčnosti motor shieldu a motorov
// v prípade že sa niektorý z motorov točí nesprávnym smerom treba zmeniť jeho polaritu prehodením káblov

#include <Servo.h>// importujeme knižnice pre ovládanie sevo motorov
#include <AFMotor.h>// importujeme knižnice pre Adafruit motorshield

// pre každý motor vytvoríme objekt
AF_DCMotor motor_left1(1, MOTOR12_64KHZ);// prvý argument je číslo portu, v ktorom je motor zapojený (napr. port M1)
AF_DCMotor motor_left2(2, MOTOR12_64KHZ);// druhý argument je ovládacia frekvencia, defaultne 1KHz
AF_DCMotor motor_right1(3, MOTOR12_64KHZ);
AF_DCMotor motor_right2(4, MOTOR12_64KHZ);

Servo servo1; //vytvoríme sevo objektu
Servo servo2;
void setup() {
// nastavíme rýchlosť motorov v rozsahu 0-255
  motor_left1.setSpeed(128);
  motor_left2.setSpeed(128);
  motor_right1.setSpeed(128);
  motor_right2.setSpeed(128);
  servo1.attach(9);// nastavíme pin pre 10 pre ovládanie serva, motor shield pre servo motory využíva piny 9 a 10
  servo2.attach(10);
}
 
void loop() {
  
  servo1.write(0);// nastavíme servá na nulovú pozíciu
  servo2.write(0);
  motor_left1.run(FORWARD); 
  motor_left2.run(FORWARD);// roztočíme motory jedným smerom
  motor_right1.run(FORWARD);
  motor_right2.run(FORWARD);
  delay(1000);// počkáme 1000ms (1s)
 
  servo1.write(90);// nastavíme seva na 90 stupňov
  servo2.write(90);
  motor_left1.run(BACKWARD);
  motor_left2.run(BACKWARD);// roztočíme motory druhým smerom
  motor_right1.run(BACKWARD);
  motor_right2.run(BACKWARD);
  delay(1000);
  
  servo1.write(180);// nastavíme seva na 180 stupňov - koniec rozsahu
  servo2.write(90);
  motor_left1.run(RELEASE); 
  motor_left2.run(RELEASE);// zabrzdíme motory
  motor_right1.run(RELEASE);
  motor_right2.run(RELEASE);  
  delay(1000);
  
}
