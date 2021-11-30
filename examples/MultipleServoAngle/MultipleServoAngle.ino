#include <ServoSpeedPWMDriver.h>

//non blocking speed moement

CServoSpeedPWMDriver driver = CServoSpeedPWMDriver(); 

int frequency = 50;
int minPulse = 130; //adjust accordingly for min angle 0- check current when this value is too low current can be up to 5 amps for MGR996
int maxPulse = 505; //adjust accordingly for max angle 180- check current when this value is too high current can be up to 5 amps for MGR996
int speed = 20;  //lower is faster
int numServos = 1; //adjust to the number of connected servos
int initialAngle = 90; //initial servo angle- this is not speed controlled
 

void ReadCommand()
{
  
  //comand sequence goes servo number than angle
  
  //read the servo index, 0 -15
  byte servo = Serial.parseInt();
  
  Serial.print("servo: ");
  Serial.println(servo);

  //wait until another number is sent, this is the angle of the servo
  while(!Serial.available()) 
  {
  }

  //read the angle 
  int angle = Serial.parseInt();

  Serial.print("angle: ");
  Serial.println(angle);
  
  //set the servo angle
  driver.setAngle(servo,angle);
   
}

void setup()
{
    Serial.begin(9600);
    
    //setup the driver
    driver.setup(frequency,numServos,minPulse,maxPulse,speed);
     
    //set the initial angle
    for(int i=0;i<numServos;i++)
    {
       driver.getDriver()->setPWM(i,0,driver.getAnglePulse(initialAngle));
      driver.setSpeed(i,speed);
    }
} 

void loop()
{
  //command sequence goes servo index then angle
   if (Serial.available() > 0) 
   {
     ReadCommand();        
   }             
        
  //update the servo movment
  driver.update();
    
}
 
