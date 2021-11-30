#ifndef SERVOSPEEDPWMDRIVER_H
#define SERVOSPEEDPWMDRIVER_H

#include "Arduino.h"
#include <Adafruit_PWMServoDriver.h>

#define MAX_SERVOS 16 //adafruit PWM driver supports 16 servos

#define angle_type int

/*!
 *  @brief  Class that stores state and functions for interacting with Adafruit PWM servo driver board
 * 
 */
class CServoSpeedPWMDriver
{
    public:
    
    void setSpeed(uint8_t servoIndex, uint8_t speed) {this->speed[servoIndex] = speed;}
    void update(); //must be called once per loop
    bool moving(uint8_t servoIndex) {return isMoving[servoIndex];}
    void setAngle(uint8_t servoIndex, angle_type angle);
    int setup(int frequency, int numberServos, int minimumPulseLength, int maximumPulseLength, uint8_t servoSpeed );//must be called in setup
    Adafruit_PWMServoDriver* getDriver(){return pwm;}
    angle_type getAnglePulse(angle_type angle) {return map(angle, 0, 180, minPulseLength, maxPulseLength);}

    private:
  
    bool isMoving[MAX_SERVOS];
    bool positionSet[MAX_SERVOS];  
    bool waitingForDelay[MAX_SERVOS];
    unsigned long initialMillis[MAX_SERVOS];
    angle_type currentAngle[MAX_SERVOS];
    angle_type destinationAngle[MAX_SERVOS];
    uint8_t speed[MAX_SERVOS];
    Adafruit_PWMServoDriver *pwm;  
    int numServos;
    int minPulseLength;
    int maxPulseLength;

};


#endif
