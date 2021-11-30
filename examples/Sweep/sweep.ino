#include <ServoSpeedPWMDriver.h>

CServoSpeedPWMDriver driver = CServoSpeedPWMDriver(); 

int frequency = 50;
int minPulse = 130; //adjust accordingly for min angle 0- check current when this value is too low current can be up to 5 amps for MGR996
int maxPulse = 505; //adjust accordingly for max angle 180- check current when this value is too high current can be up to 5 amps for MGR996
int speed = 20;  //lower is faster
const int numServos = 1;
int initialAngle = 90; //initial servo angle- this is not speed controlled
const int servoIndex = 0;
bool servoSweeping[numServos]; //sweep flag

void setup() {
    
  //setup the driver
  driver.setup(frequency,numServos,minPulse,maxPulse,speed);
     
  //set the initial angle
  for(int i=0;i<numServos;i++)
  {
       driver.getDriver()->setPWM(i,0,driver.getAnglePulse(initialAngle));
       driver.setSpeed(i,speed);
       servoSweeping[i] = true;
  }
  delay(1000);
}

void loop() {
  
  
  //sweep servos
  for(int i=0;i<numServos;i++)
  {
    if(driver.moving(i) == false)
    {
      if(servoSweeping[i] == true) //first position
      {
        driver.setAngle(i,0);
        servoSweeping[i] = false;
      }
      else
      {
        driver.setAngle(i,180); //second position
        servoSweeping[i]= true;
      }
    }
  }
  driver.update();
}