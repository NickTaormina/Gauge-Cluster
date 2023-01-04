#include "trip.h"
#include <QString>

/*
Handles trip computer calculations
*/
trip::trip(QObject *parent)
    : QObject{parent}
{

}





void trip::resetTrip(double m)
{
    tripStart = m;
}


QString trip::getTrip(double odo)
{
    return QString::number(odo-tripStart);
}

QString trip::getTripStart()
{
    return QString::number(tripStart);
}

