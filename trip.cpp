#include "trip.h"
#include <QString>

trip::trip(QObject *parent)
    : QObject{parent}
{

}

trip::trip(QObject *parent, QDomDocument* x, QString tr)
{
    xml = x;
    tripNum = tr;
    readXML();
    milesTraveled = 0;

}

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

void trip::readXML()
{
    QDomElement xt = xml->firstChild().firstChild().toElement();
    while(!xt.isNull()){
        if(xt.tagName() == "trip"){
            milesTraveled = xt.text().toFloat(nullptr);
            qDebug() << milesTraveled;
            xt = xt.firstChild().toElement();
            qDebug() << xt.tagName();
            //QDomElement newValue = xml->createElement(xt.tagName());
            //QDomText text = xml->createTextNode("balls");
            //newValue.appendChild(text);
            //xt = xml->firstChild().firstChild().toElement();
            //xt.replaceChild()
            xt.setNodeValue("balls");
            qDebug() << xt.text();
            break;
        }
        xt = xt.nextSibling().toElement();
    }
}
