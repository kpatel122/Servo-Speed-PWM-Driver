#include <ServoSpeedPWMDriver.h>

//configure pulses, servo ust be attached to position 0

CServoSpeedPWMDriver driver = CServoSpeedPWMDriver(); 

int frequency = 50;
int minPulse = 130; //adjust accordingly for min angle 0- check current when this value is too low current can be up to 5 amps for MGR996
int maxPulse = 505; //adjust accordingly for max angle 180- check current when this value is too high current can be up to 5 amps for MGR996
int speed = 20;  //lower is faster
int numServos = 1;
int initialAngle = 90; //initial servo angle- this is not speed controlled
const int servoIndex = 0;
 

void ReadCommand()
{
  
  //comand sequence goes angle, moves servo 0- used to work out min and max pulse length
  
  //read the servo index, 0 -15
  int angle = Serial.parseInt();
  
  Serial.print("angle: ");
  Serial.println(angle);

  
  //set the servo angle
  driver.setAngle(servoIndex,angle);
   
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
 
