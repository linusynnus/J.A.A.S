#include <Servo.h>          //Servo motor library. This is standard library
#include <NewPing.h>        //Ultrasonic sensor function library. You must install this library

//our L298N control pins
/*const int LeftMotorForward = 7;
const int LeftMotorBackward = 6;
const int RightMotorForward = 5;
const int RightMotorBackward = 4;
*/
#define motor11 9             //Right Motor pins
#define motor12 8

#define motor21 12            //Left Motor pins
#define motor22 13

#define enable11 10          //Right Motor enable pin 
#define enable22 11          //Left Motor enable pin 

//sensor pins
#define Echo  6              //Ultrasonic's echo pin
#define Trig 5              //Ultrasonic's Trigger pin



#define maximum_distance 200
char t;
boolean goesForward = false;
int distance = 100;

NewPing sonar(Trig,Echo, maximum_distance); //sensor function
Servo servo_motor; //our servo name
void forward(){
  digitalWrite(enable11, HIGH);    //enable L298n A channel
  digitalWrite(enable22, HIGH);    //enable L298n B channel
  digitalWrite(motor11, HIGH);    //set IN1 hight level
  digitalWrite(motor12, LOW);     //set IN2 low level
  digitalWrite(motor21, LOW);     //set IN3 low level
  digitalWrite(motor22, HIGH);//set IN4 hight level
  Serial.println("Forward");  //send message to serial monitor
}


void back(){
  digitalWrite(enable11, HIGH);    //enable L298n A channel
  digitalWrite(enable22, HIGH);    //enable L298n B channel
  digitalWrite(motor11, LOW);    //set IN1 hight level
  digitalWrite(motor12, HIGH);     //set IN2 low level
  digitalWrite(motor21, HIGH);     //set IN3 low level
  digitalWrite(motor22, LOW);    //set IN4 hight level
  Serial.println("Back");  //send message to serial monitor
}

void left(){
  digitalWrite(enable11, HIGH);    //enable L298n A channel
  digitalWrite(enable22, HIGH);    //enable L298n B channel
  digitalWrite(motor11, LOW);    //set IN1 hight level
  digitalWrite(motor12, HIGH);     //set IN2 low level
  digitalWrite(motor21, LOW);     //set IN3 low level
  digitalWrite(motor22, HIGH);//set IN4 hight level
  Serial.println("left");  //send message to serial monitor
}

void right(){
  digitalWrite(enable11, HIGH);    //enable L298n A channel
  digitalWrite(enable22, HIGH);    //enable L298n B channel
  digitalWrite(motor11, HIGH);    //set IN1 hight level
  digitalWrite(motor12, LOW);     //set IN2 low level
  digitalWrite(motor21, HIGH);     //set IN3 low level
  digitalWrite(motor22, LOW);    //set IN4 hight level
  Serial.println("right");  //send message to serial monitor
}

void stop(){
  digitalWrite(motor11,LOW);
  digitalWrite(motor12,LOW);
  digitalWrite(motor21,LOW);
  digitalWrite(motor22,LOW);  
  Serial.println("Stop!");
}


void setup(){
  Serial.begin(9600);
  pinMode(motor11, OUTPUT);   //before useing io pin, pin mode must be set first 
  pinMode(motor21, OUTPUT);
  pinMode(motor12, OUTPUT);
  pinMode(motor12, OUTPUT);
  pinMode(enable11, OUTPUT);
  pinMode(enable22, OUTPUT);
  
  servo_motor.attach(2); //our servo pin

  servo_motor.write(115);
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
}

void loop(){
  
   if(Serial.available()){
   t = Serial.read();
   Serial.println(t);
  }
  if(t == '1'){
   forward();
   t=5;
  }
 
  else if(t == '2'){      //move reverse (all motors rotate in reverse direction)
    back();
    t=5;
 

  }
 
  else if(t == '3'){      //turn right (left side motors rotate in forward direction, right side motors doesn't rotate)
   left();
   t=5;
  }
 
  else if(t == '4'){      //turn left (right side motors rotate in forward direction, left side motors doesn't rotate)
    right();
    t=5;
  }
  else if(t== '5'){
   moveStop();
  } 
  else{
  }
  delay(100);
 
  int distanceRight = 0;
  int distanceLeft = 0;
  delay(50);


  if (distance <= 35){
    moveStop();
    delay(300);
    moveBackward();
    delay(400);
    moveStop();
    delay(300);
    distanceRight = lookRight();
    delay(300);
    distanceLeft = lookLeft();
    delay(300);

    if (distanceRight >= distanceLeft){
      turnRight();
      moveStop();
    }
    else{
      turnLeft();
      moveStop();
    }
  }
  else{
    moveForward(); 
  }
    distance = readPing();
}

int lookRight(){  
  servo_motor.write(50);
  delay(500);
  int distance = readPing();
  delay(100);
  servo_motor.write(115);
  return distance;
}

int lookLeft(){
  servo_motor.write(170);
  delay(500);
  int distance = readPing();
  delay(100);
  servo_motor.write(115);
  return distance;
  delay(100);
}

int readPing(){
  delay(70);
  int cm = sonar.ping_cm();
  if (cm==0){
    cm=250;
  }
  return cm;
}

void moveStop(){
  
  digitalWrite(motor11, LOW);
  digitalWrite(motor21, LOW);
  digitalWrite(motor12, LOW);
  digitalWrite(motor22, LOW);
}

void moveForward(){

  if(!goesForward){

    goesForward=true;

  digitalWrite(enable11, HIGH);    //enable L298n A channel
  digitalWrite(enable22, HIGH); 
    
  digitalWrite(motor11,HIGH);
  digitalWrite(motor21, HIGH);
  digitalWrite(motor12, LOW);
  digitalWrite(motor22, LOW);
   
  }
}

void moveBackward(){

  goesForward=false;

  digitalWrite(enable11, HIGH);    //enable L298n A channel
  digitalWrite(enable22, HIGH); 
  
  digitalWrite(motor12,HIGH);
  digitalWrite(motor22, HIGH);
  digitalWrite(motor11, LOW);
  digitalWrite(motor21, LOW);
  
 /* digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);
  
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorForward, LOW);
  */
}

void turnRight(){
  
  digitalWrite(enable11, HIGH);    //enable L298n A channel
  digitalWrite(enable22, HIGH); 
  
  digitalWrite(motor11,HIGH);
  digitalWrite(motor22, HIGH);
  digitalWrite(motor12, LOW);
  digitalWrite(motor21, LOW);
  
  /*
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);
  
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorForward, LOW);
  */
  delay(250);

   digitalWrite(motor11,HIGH);
  digitalWrite(motor22, HIGH);
  digitalWrite(motor12, LOW);
  digitalWrite(motor21, LOW);

  /*digitalWrite(motor11,HIGH);
  digitalWrite(motor21, HIGH);
  digitalWrite(motor12, LOW);
  digitalWrite(motor22, LOW);
  /*
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);
 */
  
  
}

void turnLeft(){
   
  digitalWrite(enable11, HIGH);    //enable L298n A channel
  digitalWrite(enable22, HIGH); 
  
  digitalWrite(motor12,HIGH);
  digitalWrite(motor21, HIGH);
  digitalWrite(motor11, LOW);
  digitalWrite(motor22, LOW);
  /*
  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
*/
  delay(250);

  digitalWrite(motor12,HIGH);
  digitalWrite(motor21, HIGH);
  digitalWrite(motor11, LOW);
  digitalWrite(motor22, LOW);
 /* 
  digitalWrite(motor11,HIGH);
  digitalWrite(motor21, HIGH);
  digitalWrite(motor12, LOW);
  digitalWrite(motor22, LOW);*/
 /* 
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);*/
}
