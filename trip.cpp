#include "trip.h"
#include <QString>

/*
Handles trip computer calculations
*/
trip::trip(QObject *parent)
    : QObject{parent}
{

}


//uses avg speed and elapsed time since last call to determine trip distance
void trip::updateTripDistance(int speed, qint64 time)
{
    float delta;
    float convSpeed = speed*0.000277778; //mph to miles/sec
    if(time < 1000){
    delta = convSpeed*((float)time/1000);
    } else{
        delta = 0;
    }
    milesTraveled = milesTraveled + delta;
}

void trip::setMilesTraveled(float m)
{
    milesTraveled = m;
}

void trip::resetTrip()
{
    milesTraveled = 0;
}

QString trip::getTripNum()
{
    return tripNum;
}

QString trip::getTrip()
{
    if(milesTraveled > 0 && milesTraveled < 10000){
        return QString::number(milesTraveled, 'f', 1);
    } else {
        return "0.0";
    }
}

