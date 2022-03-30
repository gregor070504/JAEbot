#include <AFMotor.h>
#include <Servo.h>
#include <Wire.h> // knižnica pre komunikáciu pomocou I2C zbernice
#include <Servo.h>// importujeme knižnice pre ovládanie sevo motorov
#include <AFMotor.h>// importujeme knižnice pre Adafruit motorshield
const int pwm_frequency = 64000; //ovládacia pwm frekvencia pre motory
const int motor_speed = 250; // defaultná rýchlosť motorov v rozsahu 0-255
// pre každý motor vytvoríme objekt
AF_DCMotor motor_left1(1,pwm_frequency);// prvý argument je číslo portu, v ktorom je motor zapojený (napr. port M1)
AF_DCMotor motor_left2(4,pwm_frequency);// druhý argument je ovládacia frekvencia, defaultne 1KHz
AF_DCMotor motor_right1(2,pwm_frequency);
AF_DCMotor motor_right2(3,pwm_frequency);
Servo klepeto;
Servo radar_servo;               
int radar_position = 0; //definujeme začiatočnú pozíciu serva
int object_pos;
const int trig_pin = 22;// definujeme spúšťací pin
const int echo_pin = 23;// a pin na prijatie odozvy
// definujeme premenné pre čas a vzdialenosť
long echo_time;
int distance;

const int ir_sensor_pin = 25;
bool ir_sensor_detect = HIGH;

//OLED display
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
// pre použitie displaya nedefinujeme žiadne piny, pretože na komunikáciu využíva I2C zbernicu (piny SDA a SCL)
Adafruit_SSD1306 display(SCL);    //vytvoríme objekt s názvom display pre OLED display

void setup() {
  
  // definovanie pinov serva
  klepeto.attach(9);
  klepeto.write(0);
  radar_servo.attach (10);
  radar_servo.write(0);
  
  pinMode(trig_pin, OUTPUT); // nastavíme spúšťací pin na výstup
  pinMode(echo_pin, INPUT); // a pin pre odozvu na vstup

  // OLED display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  motor_left1.setSpeed(motor_speed);
  motor_left2.setSpeed(motor_speed);
  motor_right1.setSpeed(motor_speed);
  motor_right2.setSpeed(motor_speed);
  motor_stop(2000);

  pinMode(ir_sensor_pin, INPUT);
}

void loop() {
  for(radar_position; radar_position<180; radar_position=radar_position+5){
    radar_servo.write(radar_position);
    read_ultrasonic_sensor();
    if (distance <= 30)
    {
      //display.setCursor(0,2);
      display.print("prekazka");
      display.display();
      delay(3000);
      object_pos = (radar_position - 90);
      if (object_pos < 0)
      { 
        object_pos = abs(object_pos);
        for(object_pos; object_pos > 0; object_pos--)
        {
          turn_left(15);
          motor_stop(10);
        }
        grab();
        
      }
      else
      {
       for(object_pos; object_pos > 0; object_pos--)
        {
          turn_right(15);
          motor_stop(10);
        } 
        grab();
      }
    }
    
  }
  for(radar_position; radar_position>0; radar_position=radar_position-5){
    radar_servo.write(radar_position);
    read_ultrasonic_sensor();
        if (distance <= 30)
    {
      //display.setCursor(0,2);
      display.print("prekazka");
      display.display();
      delay(3000);
      object_pos = radar_position - 90;
      if (object_pos < 0)
      { 
        object_pos = abs(object_pos);
        for(object_pos; object_pos > 0; object_pos--)
        {
          turn_left(15);
          motor_stop(10);
        }
        grab();
        
      }
      else
      {
       for(object_pos; object_pos > 0; object_pos--)
        {
          turn_right(15);
          motor_stop(10);
        }
        grab(); 
      }
    }
  }
}

void read_ultrasonic_sensor()
{
  display.clearDisplay(); //vyčistenie pamäte na displayi

  display.setTextColor(WHITE); //nastavenie farby textu na bielu
  display.setCursor(0,0); //nastavenie začiatku textu na displayi na ľavý horný roh (0px a 0px)
  
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
  display.print("Vzdialenosť: "); display.print(distance); display.println(" cm");

  //aktualizovanie zobrazenia na displayi
  display.display();  
}

  void grab(){
    ir_sensor_detect = digitalRead(ir_sensor_pin);
    while (ir_sensor_detect == HIGH)
    {
      forward(50,50); 
      ir_sensor_detect = digitalRead(ir_sensor_pin);  
    }
    motor_stop(250);
    klepeto.write(180);
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
