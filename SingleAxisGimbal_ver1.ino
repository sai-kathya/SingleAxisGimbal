#include <Wire.h>
#include <Servo.h>
#include <math.h>

Servo myservo;
float offset = 0;
float neutral_pos = 97;

void setup() {

 //establish I2C
 Wire.begin();
 Serial.begin(9600);
 Wire.beginTransmission(0x68);
 Wire.write(0x6B);
 Wire.write(0x00);
 Wire.endTransmission();

 //servo setup
 myservo.attach(9);
 //myservo.write(neutral_pos);
}


void loop() {
 // put your main code here, to run repeatedly:
 //to choose register
 Wire.beginTransmission(0x68);
 Wire.write(0x3B);
 Wire.endTransmission();
 
 //to read from chosen register
 Wire.requestFrom(0x68, 6);
 
 if(Serial.available() > 0){
  offset = Serial.parseFloat();
 }
 
 while(Wire.available() >= 6 ){
   //read and print accelerometer data
   int16_t Xaccel = (Wire.read() << 8) | Wire.read();
   Serial.print("Acceleration along x-axis: ");
   Serial.println(Xaccel);

   int16_t Yaccel = (Wire.read() << 8) | Wire.read();
   Serial.print("Acceleration along y-axis: ");
   Serial.println(Yaccel);

   int16_t Zaccel = (Wire.read() << 8) | Wire.read();
   Serial.print("Acceleration along z-axis: ");
   Serial.println(Zaccel);
   
   Serial.println();

   //calculate tilt angle
   float tilt_angle = atan2(Yaccel, Xaccel) * (180/M_PI);
   Serial.print("Tilt Angle: ");
   Serial.println(tilt_angle);

   Serial.println();

   //fix tilt
   float desired_pos = neutral_pos + offset + tilt_angle;
   if(desired_pos > 170){
    desired_pos = 170;
   }
   if(desired_pos < 5){
    desired_pos = 5;
   }
   Serial.println(desired_pos);
   myservo.write(desired_pos);
 }
 
 Serial.println();
}



