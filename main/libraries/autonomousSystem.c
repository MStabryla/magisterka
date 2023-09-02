#include "movement.h"
#include "distance.h"
#include "../pin_config.h"

#define MIN_DISTANCE 28
#define SPEED 255
#define MILI_TO_TURN 100

float getFrontDistance()
{
    return getDistance(ECHO_1,TRIG_1);
}
float getRightDistance()
{
    return getDistance(ECHO_2,TRIG_2);
}
float getLeftDistance()
{
    return getDistance(ECHO_3,TRIG_3);
}


bool detectBlockade()
{
    return getFrontDistance() <= MIN_DISTANCE;
}
// jeżeli true, to znaczy, że robot skręca
bool autonomousMove()
{
    if(autoMode)
    {
        //Serial.println("forward");
        Forward(SPEED);
    } 
    else
    {
        //Serial.println("stop");
        Stop();
        return true;
    }
    float right = 0;
    float left = 0;
    //Tutaj wykrywamy, czy robot nie jest za daleko od innych robotów
    if(detectBlockade())
    {
        right = getRightDistance();
        left = getLeftDistance();
        if(right >= left)
        {
            //Albo wykrywamy za pomocą żyroskopu albo krokowo
            //Serial.println("right");
            Right(SPEED);
            return true;
        }
        else
        {
            //Serial.println("left");
            Left(SPEED);
            return true;
        }
    }
    return false;
}