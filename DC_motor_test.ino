// tento kód slúži na overenie funkčnosti motor shieldu a motorov
// v prípade že sa niektorý z motorov točí nesprávnym smerom treba zmeniť jeho polaritu prehodením káblov

#include <Servo.h>// importujeme knižnice pre ovládanie sevo motorov
#include <AFMotor.h>// importujeme knižnice pre Adafruit motorshield
const int pwm_frequency = 64000; //ovládacia pwm frekvencia pre motory
const int motor_speed = 250; // defaultná rýchlosť motorov v rozsahu 0-255
// pre každý motor vytvoríme objekt
AF_DCMotor motor_left1(1,pwm_frequency);// prvý argument je číslo portu, v ktorom je motor zapojený (napr. port M1)
AF_DCMotor motor_left2(4,pwm_frequency);// druhý argument je ovládacia frekvencia, defaultne 1KHz
AF_DCMotor motor_right1(2,pwm_frequency);
AF_DCMotor motor_right2(3,pwm_frequency);

Servo servo1; //vytvoríme sevo objektu
Servo servo2;
void setup() {
// nastavíme rýchlosť motorov

  motor_left1.setSpeed(motor_speed);
  motor_left2.setSpeed(motor_speed);
  motor_right1.setSpeed(motor_speed);
  motor_right2.setSpeed(motor_speed);
  motor_stop(2000);

}
 
void loop() {
  

turn_left(500);
motor_stop(500);
turn_right(500);
motor_stop(500);

forward(1000,50);// prvý agument je čas, druhý rýchlosť
backward(1000,50);

  
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


 
