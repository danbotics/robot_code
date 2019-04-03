// *-----------------------------------* \\
// | ENGG1500 Project - 2019 - Team 24 | \\
// *-----------------------------------* \\

/* CHANGE LOG

v 1.0 - Initial
v 1.1 - added additional line sensors
v 1.2 - added IR obstacle sensors to front corners to help with off-axis wall detection
v 1.3 - added Rear facing obstacle sensor to stop reversing if back against wall
      - Reduced startup delay to 1 second

*/


#include <NewPing.h>
#include <Servo.h>

#define ENA 5 // SPEED CONTROL - Left wheel
#define ENB 6 // SPEED CONTROL - Right wheel
#define IN1 8 // LEFT motor
#define IN2 9 // LEFT motor
#define IN3 10 // RIGHT motor
#define IN4 11 // RIGHT motor
#define LS1 A3 // Line Sensor 1 pin (front passenger side)
#define LS2 4 // Line Sensor 2 pin (front middle)
#define LS3 A2 // Line Sensor 3 pin (front driver side)
#define OS1 A5 // IR Obstacle sensor 1 (front passenger side)
#define OS2 A4 // IR Obstacle sensor 2 (front driver side)
#define OS3 A1 // Rear obstacle sensor

#define servo 13 // Servo pin

#define ECHO 12 // Ultrasonic sensor ECHO pin
#define TRIG 7 // Ultrasonic sensor TRIG pin

int leftSpeed = 110; // Left motor "normal" speed
int rightSpeed = 85; // Right motor "normal" speed

int maxDistance = 200;
int distance = 100;

int distanceLeft;
int distanceRight;

NewPing sonar(TRIG, ECHO, maxDistance); // Ultrasonic sensor
Servo eyes; // Declare servo name

void setup() {
  pinMode(ENA,OUTPUT); //set ENA as an output
  pinMode(ENB,OUTPUT); //set ENB as an output
  pinMode(IN1,OUTPUT); //set IN1 as an output
  pinMode(IN2,OUTPUT); //set IN2 as an output
  pinMode(IN3,OUTPUT); //set IN3 as an output
  pinMode(IN4,OUTPUT); //set IN4 as an output
  pinMode(LS1,INPUT); //set Line Sensor 1 pin as an input
  pinMode(LS2,INPUT); //set Line Sensor 2 pin as an input
  pinMode(LS3,INPUT); //set Line Sensor 3 pin as an input
  pinMode(OS1,INPUT); //set Obstacle sensor 1 pin as an input
  pinMode(OS2,INPUT); //set Obstacle sensor 2 pin as an input
  pinMode(OS3,INPUT); //set Obstacle sensor 2 pin as an input
    
  eyes.attach(servo); // Activate servo named "eyes"

  Serial.begin(9600);

  eyes.write(170);
  delay(500);
  eyes.write(10);
  delay(500);
  eyes.write(90); // Set servo to straight ahead position

  analogWrite(ENA, 0); // Set LEFT motor initial speed to 0
  analogWrite(ENB, 0); // Set RIGHT motor initial speed to 0

  delay(1000);// Delay before starting loop
}

void loop() 
{
distance = readPing();
Serial.print("Distance = ");
Serial.println(distance);

if(digitalRead(LS1) == 1 || digitalRead(LS2) == 1 || digitalRead(LS3) == 1) motorStop();
  else {

if (distance <= 8 || digitalRead(OS1) == LOW || digitalRead(OS2) == LOW)
{
  motorStop();
  delay(200);
  if(digitalRead(OS3) == LOW) goto SKIPBACKWARDS;
  leftBackwards();
  rightBackwards();
  motorGo();
  Serial.println("Reversing...");
  delay(250);
  motorStop();
  delay(300);
  SKIPBACKWARDS:
  distanceRight = checkRight();
  Serial.print("Distance (to Right) = ");
  Serial.println(distanceRight);
  delay(300);
  distanceLeft = checkLeft();
  Serial.print("Distance (to Left) = ");
  Serial.println(distanceLeft);
  delay(300);
  
  Serial.print("Front: ");         // --
  Serial.print(distance);          //   |
  Serial.print("    Right: ");     //   |
  Serial.print(distanceRight);     // Decision Debugging
  Serial.print("    Left: ");      //   |
  Serial.println(distanceLeft);    //   |
//delay(4700);                     // --
  
  if(distanceLeft > distanceRight)
    {
      turnLeft();
      motorGo();
      delay(250);
      motorStop();
    }else if(distanceLeft < distanceRight)
    {
      turnRight();
      motorGo();
      delay(325);
      motorStop();
    }
}else
  {
    leftForwards();
    rightForwards();
    Serial.print("Moving forwards! LeftSpeed: ");    
    Serial.print(leftSpeed);
    Serial.print("   RightSpeed: ");    
    Serial.println(rightSpeed);
    motorGo();
  }
}
}

void motorStop(void) //This function sets the LEFT motor direction to FORWARDS 
{
  Serial.println("Stopping...");
  analogWrite(ENA, 0); // Set LEFT motor speed to 0
  analogWrite(ENB, 0); // Set RIGHT motor speed to 0
}

void motorGo(void) //This function sets the LEFT motor direction to FORWARDS 
{
  analogWrite(ENA, leftSpeed); // Set LEFT motor speed to normal
  analogWrite(ENB, rightSpeed); // Set RIGHT motor speed to normal
}

void leftForwards(void) //This function sets the LEFT motor direction to FORWARDS 
{
digitalWrite(IN1,LOW); //IN1
digitalWrite(IN2,HIGH); //IN2
}

void leftBackwards(void) //This function sets the LEFT motor direction to BACKWARDS
{
digitalWrite(IN1,HIGH); //IN1
digitalWrite(IN2,LOW); //IN2
}

void rightForwards(void) //This function sets the RIGHT motor direction to FORWARDS
{
digitalWrite(IN3,LOW); //IN3
digitalWrite(IN4,HIGH); //IN4
}

void rightBackwards(void) //This function sets the RIGHT motor direction to BACKWARDS
{
digitalWrite(IN3,HIGH); //IN3
digitalWrite(IN4,LOW); //IN4
}

void turnRight(void) //This function sets the motor directions opposite so they turn RIGHT
{
Serial.println("Turning right...");
digitalWrite(IN1,LOW); //IN1
digitalWrite(IN2,HIGH); //IN2
digitalWrite(IN3,HIGH); //IN3
digitalWrite(IN4,LOW); //IN4
}

void turnLeft(void) //This function sets the motor directions opposite so they turn LEFT
{
Serial.println("Turning left...");
digitalWrite(IN1,HIGH); //IN1
digitalWrite(IN2,LOW); //IN2
digitalWrite(IN3,LOW); //IN3
digitalWrite(IN4,HIGH); //IN4
}

int checkRight()
{  
  Serial.println("Checking right...");
  eyes.write(10);
  delay(500);
  int distance = readPing();
  delay(100);
  eyes.write(90);
  return distance;
}

int checkLeft()
{
  Serial.println("Checking left...");
  eyes.write(170);
  delay(500);
  int distance = readPing();
  delay(100);
  eyes.write(90);
  return distance;
  delay(100);
}

int readPing()
{
  Serial.println("Pinging...");
  delay(70);
  int cm = sonar.ping_cm();
  if (cm==0){
    cm=250;
  }
  return cm;
}
