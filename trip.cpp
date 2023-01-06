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

double trip::getTrip()
{
    return tripDistance;
}


QString trip::getTrip(double odo)
{
    tripDistance = odo - tripStart;
    QString val = QString::number(odo-tripStart, 'f', 1);
    int ind = val.indexOf("-");
    if(ind > -1){
        val.remove(ind,1);
    }
    return val;
}

QString trip::getTripStart()
{
    QString val = QString::number(tripStart, 'f', 1);
    int ind = val.indexOf("-");
    if(ind > -1){
        val.remove(ind,1);
    }
    return val;
}

double trip::getTripSamples() const
{
    qDebug() << "trip samples: " << (tripDistance*10);
    return (tripDistance*10);
}

void trip::updateTripSamples()
{
    tripSamples++;
}

void trip::setTripSamples(double newTripSamples)
{
    tripSamples = newTripSamples;
}

double trip::getTripMPG() const
{
    return tripMPG;
}

void trip::setTripMPG(double newTripMPG)
{
    tripMPG = newTripMPG;
}


