#include "movement.h"
#include "distance.h"
#include "../pin_config.h"

#define MIN_DISTANCE 28
#define SPEED 255
#define MILI_TO_TURN 1000
#define TURN_DIST_DIFF_TOL 5

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
float right = 0;
float left = 0;
float old_right = 0;
float old_left = 0;
bool turnAround = false;

bool autonomousMove()
{
    if(!autoMode)
    {
        Stop();
        return false;
    }     
    if(detectBlockade())
    {
        
        right = getRightDistance();
        left = getLeftDistance();
        if(turnAround)
        {
            if(abs(right - old_left) <= TURN_DIST_DIFF_TOL && abs(left - old_right) <= TURN_DIST_DIFF_TOL )
            {
                turnAround = false;
            }
        }
        else if(right <= MIN_DISTANCE && left <= MIN_DISTANCE)
        {
            old_right = right;
            old_left = left;
            Right(SPEED);
            turnAround = true;
            return true;
        }
        else if(right >= left)
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
    else
    {
        Forward(SPEED);
        turnAround = false;
        //Tutaj wykrywamy, czy robot nie jest za daleko od innych robotów
        //TooFarFromOtherRobots();
    }
    return false;
}