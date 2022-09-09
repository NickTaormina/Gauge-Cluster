#include "trip.h"

trip::trip(QObject *parent)
    : QObject{parent}
{

}

trip::trip(QObject *parent, QDomDocument* x, QString tr)
{
    xml = x;
    tripNum = tr;
    readXML();
}

void trip::updateTripDistance(int speed, float time)
{
    float delta = speed*(time/(1000*60*60));
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
