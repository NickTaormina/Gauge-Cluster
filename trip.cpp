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


