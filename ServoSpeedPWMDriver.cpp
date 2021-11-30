 #include "ServoSpeedPWMDriver.h"

/*!
 *  @brief  must be called in arduino setup, sets initial values
 *  @param  frequency MGR frequency is 50
 *  @param  numberServos number of servos to use, max is 16
 *  @param  minimumPulseLength minum pulse length, adjust to find angle 0
 *  @param  maximumPulseLength maximum  pulse length, adjust to find angle 180
 *  @param  servoSpeed speed of the servos, it's the delay between pulse updates so lower value is faster movement
 *  @return 1 on success 0 on failure (too many servos) 
 */

int CServoSpeedPWMDriver::setup(int frequency, int numberServos, int minimumPulseLength, int maximumPulseLength,uint8_t servoSpeed)
{

    //check number of servos
    if(numberServos>=MAX_SERVOS)
        return 0;

    //default angle is set to 90
    int startAngle = 90;

    //set the number of seervos
    this->numServos = numberServos;

    //create PWM driver
    pwm = new Adafruit_PWMServoDriver();
    pwm->begin();
    pwm->setPWMFreq(frequency);

    //store pulse values
    this->minPulseLength = minimumPulseLength;
    this->maxPulseLength = maximumPulseLength;

    //set initial angle
    int pulseLength;
    pulseLength = map(startAngle, 0, 180, minPulseLength, maxPulseLength);

    //go through all servos and set initial values
    for(int i =0; i < numServos; i++)
    {
        //set the curent angle
        currentAngle[i] = startAngle;
        pwm->setPWM(i, 0, pulseLength);
        
        //initial flag values
        isMoving[i] = false;
        positionSet[i] = false;
        waitingForDelay[i] = false;

        //servo speed
        speed[i] = servoSpeed;
    }
    return 1;
}

/*!
 *  @brief  sets the absulute angle of the servo
 *  @param  servoIndex which servo to set
 *  @param  angle angle to set
 */

void CServoSpeedPWMDriver::setAngle(uint8_t servoIndex, angle_type angle)
{
    //set the position flag
    if(positionSet[servoIndex] == false)
    {
        positionSet[servoIndex] = true;  
    }

    //set the destination angle
    destinationAngle[servoIndex] = angle;
    
    //set the moving flag
    isMoving[servoIndex] = true;
    
    //initial start time for non blocking moves
    initialMillis[servoIndex] = millis();
}

/*!
 *  @brief  must be called in arduino loop to update servo movemeoents
 */
void CServoSpeedPWMDriver::update()
{
    //angle update PWM pulse length
    static long pulselength;

    //loop through all servos
    for(int i =0; i < numServos; i++)
    {
        //check if are already at the destination angle or if we are moving at all
        if(isMoving[i] == false || (currentAngle[i] == destinationAngle[i]))
            continue;

        //get the delta angle to move to
        int targetDelta = destinationAngle[i] - currentAngle[i];    
        targetDelta = abs(targetDelta);

        //the destination angle is higher than the current angle, positive PWM movement- PWM gets larger
        if(destinationAngle[i] > currentAngle[i])
        {
            //check if its time to perform PWM update
            if(waitingForDelay[i] == false)
            {
                //angle goes up by 1
                currentAngle[i] = currentAngle[i] + 1;
                
                //calculate new pulse vale
                pulselength = map(currentAngle[i], 0, 180, minPulseLength, maxPulseLength);
                pwm->setPWM(i, 0, pulselength);    

                //setup non blocking delay as we have just updated the movemt
                waitingForDelay[i] = true;
                initialMillis[i] = millis();

                 //check if the movement means we have arrived at the destination
                if(currentAngle[i] >= destinationAngle[i])
                {
                    isMoving[i] = false;
                    return;
                }
            }
            else
            {
                //we are in a wait state, check if the delta wait is higher than the delay set (speed)
                if( (millis() - initialMillis[i]) >= speed[i] )
                {
                    //we can perform an update on the next tick
                    waitingForDelay[i] = false;
                    return;
                } 
            }
        }
         //the destination angle is lower than the current angle, negative PWM movement- PWM gets smaller
        else if(destinationAngle[i] < currentAngle[i])
        {
            //check if its time to perform PWM update
            if(waitingForDelay[i] == false)
            {
                //angle goes down by 1
                currentAngle[i] = currentAngle[i] - 1;
                
                //calculate PWM pulse length for the new angle
                pulselength = map(currentAngle[i], 0, 180, minPulseLength, maxPulseLength);
                
                //set the new angle
                pwm->setPWM(i, 0, pulselength);    
                
                //setup non blocking wait
                waitingForDelay[i] = true;
                initialMillis[i] = millis();


                 //check if the movement means we have arrived at the destination
                if(currentAngle[i] <= destinationAngle[i])
                {
                    isMoving[i] = false;
                    return;
                }
            }
            else
            {
                //we are in a wait state, check if the delta wait is higher than the delay set (speed)
                if( (millis() - initialMillis[i]) >= speed[i] )
                {
                    //we can perform an update on the next tick
                    waitingForDelay[i] = false;
                    return;
                }    
            }
        }
    }

}
