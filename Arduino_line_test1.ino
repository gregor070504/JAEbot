#include <AFMotor.h>// importujeme knižnice pre Adafruit motorshield
const int pwm_frequency = 64000; //ovládacia pwm frekvencia pre motory
//const int motor_speed = 200; // defaultná rýchlosť motorov v rozsahu 0-255
// pre každý motor vytvoríme objekt
AF_DCMotor motor_left1(1,pwm_frequency);// prvý argument je číslo portu, v ktorom je motor zapojený (napr. port M1)
AF_DCMotor motor_left2(4,pwm_frequency);// druhý argument je ovládacia frekvencia, defaultne 1KHz
AF_DCMotor motor_right1(2,pwm_frequency);
AF_DCMotor motor_right2(3,pwm_frequency);

const int line_sensor1_pin = 29;// definujeme čísla pinov pre senzory
const int line_sensor2_pin = 28;
const int line_sensor3_pin = 31;
const int line_sensor4_pin = 30;

// kedže výstup za senzora môže mať len 2 hodnoty, použijeme premennú typu bool
 bool line_val1;// definujeme premenné, do ktorých budeme ukladať výstupy zo senzorov
 bool line_val2;
 bool line_val3;
 bool line_val4;                         

void setup() 
  {     
        pinMode(line_sensor1_pin,INPUT);
        pinMode(line_sensor2_pin,INPUT);
        pinMode(line_sensor3_pin,INPUT);
        pinMode(line_sensor4_pin,INPUT);
        motor_stop(1000);
  }

void loop() {  

 
line_sensor_read();
/// 0 = black  ///  1 = white   ///
// ak všetky senzory dedekujú bielu robot ide rovno

/*
  while (line_val1 == 1 && line_val2 == 1 && line_val3 == 1 && line_val4 == 1)
    {        
      forward(25,75);     
      line_sensor_read();
     }

  
motor_stop(10);
line_sensor_read();
*/

 if (line_val2 == 0 && line_val3 == 1)
     {  
        turn_left(50);
        motor_stop(150);
     }


 else if (line_val2 == 1 && line_val3 == 0)                                                            
      {  
         turn_right(50);
         motor_stop(150);   
      }

 else if (line_val2 == 1 && line_val3 == 1)
 {
  forward(30,100);
  motor_stop(50);
 }

 else
 {
  motor_stop(50);
 }


/*           
 if (line_val1 == 1 && line_val2 == 0 && line_val3 == 1 && line_val4 == 1)
     {  
        turn_left(50);
     }


 else if (line_val1 == 1 && line_val2 == 1 && line_val3 == 0 && line_val4 == 1)                                                            
      {  
         turn_right(50);   
      }


 else if (line_val1 == 0 && line_val2 == 0 && line_val3 == 1 && line_val4 == 1)
     {                          
              forward(50,200);
              motor_stop(250);
                      while(line_val3 == 1)
                              {
                                turn_left(50);
                                line_val3 =digitalRead(line_sensor3_pin);
                              }
              motor_stop(100); 
    }   


 else if (line_val1 == 1 && line_val2 == 1 && line_val3 == 0 && line_val4 == 0)
      {       
              forward(50, 200);
              motor_stop(250);
                       while(line_val2 == 1)
                           {
                              turn_right(50);
                              line_val2 =digitalRead(line_sensor2_pin);
                           }
              motor_stop(100);
      } 

*/
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
      motor_left1.setSpeed(250);
      motor_left2.setSpeed(250);
      motor_right1.setSpeed(200);
      motor_right2.setSpeed(200);
      
      motor_left1.run(FORWARD); 
      motor_left2.run(FORWARD);
      motor_right1.run(BACKWARD);
      motor_right2.run(BACKWARD); 
      delay(run_time);    
   }

   void turn_right(int run_time)
   {
      motor_left1.setSpeed(200);
      motor_left2.setSpeed(200);
      motor_right1.setSpeed(250);
      motor_right2.setSpeed(250);
      
      motor_left1.run(BACKWARD); 
      motor_left2.run(BACKWARD);
      motor_right1.run(FORWARD);
      motor_right2.run(FORWARD); 
      delay(run_time); 
   }
   
   void line_sensor_read()
   {        
       line_val1 =digitalRead(line_sensor1_pin);
       line_val2 =digitalRead(line_sensor2_pin);
       line_val3 =digitalRead(line_sensor3_pin);
       line_val4 =digitalRead(line_sensor4_pin); 

       /*
       display.clearDisplay(); //vyčistenie pamäte na displayi
       display.setTextColor(WHITE); //nastavenie farby textu na bielu
       display.setCursor(0,0); //nastavenie začiatku textu na displayi na ľavý horný roh (0px a 0px)
       display.print("S1: "); display.print(line_val1); 
       display.print("  S2: "); display.println(line_val1);
       display.print("S3: "); display.print(line_val1);
       display.print("  S4: "); display.println(line_val1);
       display.display(); //aktualizovanie zobrazenia na displayi 
       */  
   }
