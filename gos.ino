#include <Servo.h>          //Servo motor library. This is standard library
#include <NewPing.h>
#include <IRremote.h>

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
int opt;
char t;
boolean goesForward = false;
int distance = 100;

NewPing sonar(Trig,Echo, maximum_distance); //sensor function
Servo servo_motor; //our servo name

int RECV_PIN = 3;             //IR receiver
IRrecv irrecv(RECV_PIN);
decode_results results;

int Line0 = A0, Line1 = A1, Line2 = A2, Line3 = A3, Line4 = A4, Line5 = A5;
int RLine0, RLine1, RLine2, RLine3, RLine4, RLine5;

long Previous_Signal,No_Button_Is_Pressed=0;

void Enable_Motor(void)
{
  digitalWrite(enable11, HIGH);
  digitalWrite(enable22, HIGH);
}
void Disable_Motor(void)
{
  digitalWrite(enable11, LOW);
  digitalWrite(enable22, LOW);

}


 void right(){
  digitalWrite(enable11, HIGH);    //enable L298n A channel
  digitalWrite(enable22, HIGH);    //enable L298n B channel
  digitalWrite(motor11, HIGH);    //set IN1 hight level
  digitalWrite(motor12, LOW);     //set IN2 low level
  digitalWrite(motor21, LOW);     //set IN3 low level
  digitalWrite(motor22, HIGH);//set IN4 hight level
  Serial.println("Forward");  //send message to serial monitor
}


void left(){
  digitalWrite(enable11, HIGH);    //enable L298n A channel
  digitalWrite(enable22, HIGH);    //enable L298n B channel
  digitalWrite(motor11, LOW);    //set IN1 hight level
  digitalWrite(motor12, HIGH);     //set IN2 low level
  digitalWrite(motor21, HIGH);     //set IN3 low level
  digitalWrite(motor22, LOW);    //set IN4 hight level
  Serial.println("Back");  //send message to serial monitor
}

void back(){
  digitalWrite(enable11, HIGH);    //enable L298n A channel
  digitalWrite(enable22, HIGH);    //enable L298n B channel
  digitalWrite(motor11, LOW);    //set IN1 hight level
  digitalWrite(motor12, HIGH);     //set IN2 low level
  digitalWrite(motor21, LOW);     //set IN3 low level
  digitalWrite(motor22, HIGH);//set IN4 hight level
  Serial.println("left");  //send message to serial monitor
}

void forward(){
  digitalWrite(enable11, HIGH);    //enable L298n A channel
  digitalWrite(enable22, HIGH);    //enable L298n B channel
  digitalWrite(motor11, HIGH);    //set IN1 hight level
  digitalWrite(motor12, LOW);     //set IN2 low level
  digitalWrite(motor21, HIGH);     //set IN3 low level
  digitalWrite(motor22, LOW);    //set IN4 hight level
  Serial.println("right");  //send message to serial monitor
}

void Stop(){
  digitalWrite(motor11,LOW);
  digitalWrite(motor12,LOW);
  digitalWrite(motor21,LOW);
  digitalWrite(motor22,LOW);  
  Serial.println("Stop!");
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


//-------------------------------------------------------------------------------------------------

void setup(){
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  pinMode(motor11, OUTPUT);   //before useing io pin, pin mode must be set first 
  pinMode(motor21, OUTPUT);
  pinMode(motor12, OUTPUT);
  pinMode(motor12, OUTPUT);
  pinMode(enable11, OUTPUT);
  pinMode(enable22, OUTPUT);

  
  pinMode(Line0, INPUT);
  pinMode(Line1, INPUT);
  pinMode(Line2, INPUT);
  pinMode(Line3, INPUT);
  pinMode(Line4, INPUT);
  pinMode(Line5, INPUT);
  
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

   opt = Serial.read();
   Serial.println(opt);
 
  if(opt==1){
          Scan();
          Path_Finding();
           Serial.println("line");
      
  }
  else if(opt==2){
          Scan_IRSignal();
          No_Button_Is_Pressed++;
          Serial.println("IR");
          
  }
  else if(opt==3){
          bluetooth();
          Serial.println("Bluetooth");
          
  }
  else if(opt==4){
         obstacle();
         Serial.println("Obstacle");
        
  }
  else if(opt==5){
        blueobs();
        Serial.println("Blue + Obs");
      
  }
  else{
       Serial.println("Nothing");                   
                 
    }
   }
  }
//------------------------------------------------------bluetooth_obstacle------------------------------------------------------------------------------------------------------------------------------------------
void blueobs(void){
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
   Stop();
  } 
  else{
  }
  delay(100);
 //obstacle avoidance
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
//------------------------------------------------------Bluetooth-voice------------------------------------------------------------------------------------------------------------------------------------------
void bluetooth(void)
{
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
   Stop();
  } 
  else{
  }
  delay(100);
 } 
//------------------------------------------------------obstacle-------------------------------------------------------------------------------------------------------------------------------------------------
void obstacle(void){
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
 
}

void turnRight(){
  
  digitalWrite(enable11, HIGH);    //enable L298n A channel
  digitalWrite(enable22, HIGH); 
  
  digitalWrite(motor11,HIGH);
  digitalWrite(motor22, HIGH);
  digitalWrite(motor12, LOW);
  digitalWrite(motor21, LOW);
  
  delay(250);

   digitalWrite(motor11,HIGH);
  digitalWrite(motor22, HIGH);
  digitalWrite(motor12, LOW);
  digitalWrite(motor21, LOW);
}

void turnLeft(){
   
  digitalWrite(enable11, HIGH);    //enable L298n A channel
  digitalWrite(enable22, HIGH); 
  
  digitalWrite(motor12,HIGH);
  digitalWrite(motor21, HIGH);
  digitalWrite(motor11, LOW);
  digitalWrite(motor22, LOW);
  delay(250);

  digitalWrite(motor12,HIGH);
  digitalWrite(motor21, HIGH);
  digitalWrite(motor11, LOW);
  digitalWrite(motor22, LOW);
}

//-------------------------------------------------------------Line------------------------------------------------------------------------------------------------------------

void Scan(void)
{
  RLine0 = digitalRead(Line0);
  RLine1 = digitalRead(Line1);
  RLine2 = digitalRead(Line2);
  RLine3 = digitalRead(Line3);
  RLine4 = digitalRead(Line4);
  RLine5 = digitalRead(Line5);

  Serial.print(RLine0);
  Serial.print(RLine1);
  Serial.print(RLine2);
  Serial.print(RLine3);
  Serial.print(RLine4);
  Serial.println(RLine5);
}

void Path_Finding(void)
{
  if ( (RLine0 == 0 && RLine5 == 0  ))
  {
    Serial.println("F");
    forward();
    delay(30);
  }
  else if (( RLine0 == 0) && (RLine5==1))
  {
    Serial.println("L");
    left();
    delay(100);
  }
  else if (( RLine5 == 0) && (RLine0 == 1))
  {
    Serial.println("R");
    right();
    delay(100);
  }
  else if( (RLine5 == 0)  && (RLine4 == 0 ) && (RLine3 == 0)  && ( RLine2 == 0 ) && (RLine1 == 0)  && (RLine0 == 0))
  {
    Serial.println("Stoped");
    Disable_Motor();
   }
}


//-----------------------------------------------------IR_CODE-----------------------------------------------------------------------------------------------------------

void Scan_IRSignal(void)
{  
  if(irrecv.decode(&results)) 
  {
    results.value=results.value/225;
    Serial.println(results.value);
    No_Button_Is_Pressed =0;
    if ((results.value)==74293)
    {
      Enable_Motor();
      left();
      Previous_Signal=results.value;
    }
    else if ((results.value)==74377)
    {
      Enable_Motor();
      right();
      Previous_Signal=results.value;
    }
    else if ((results.value)==74302)
    {
      Enable_Motor();
      forward();
       Serial.println("f");
      Previous_Signal=results.value;
    }
    else if ((results.value)==74359)
    {
      Enable_Motor();
      back();
      Previous_Signal=results.value;
    }
    else if (Previous_Signal == 74359 && (results.value)==19088743)
    {
      Enable_Motor();
      back();
    }
    else if (Previous_Signal == 74302 && (results.value)==19088743)
    {
      Enable_Motor();
      forward();
    }else if (Previous_Signal == 74293 && (results.value)==19088743)
    {
      Enable_Motor();
      left();
    }
    else if (Previous_Signal == 74377 && (results.value)==19088743)
    {
      Enable_Motor();
      right();
    }
    else 
    {
      Disable_Motor();         
    }
    irrecv.resume(); // Receive the next value
    Serial.print("Read Signal   ");
    Serial.println(Previous_Signal); 
    }
    
    if(No_Button_Is_Pressed > 30000)
    {
      Previous_Signal=0;
      Disable_Motor();
      No_Button_Is_Pressed = 31000; 
    }
}
