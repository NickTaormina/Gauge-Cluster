#include "gauges.h"
#include <QtQuick>

gauges::gauges(QObject *parent)
    : QObject{parent}
{
    testtimer = new QTimer(this);
}

gauges::gauges(QObject *parent, QQmlApplicationEngine *engine)
    : QObject{parent}
{
    testtimer = new QTimer(this);
    connect(testtimer, &QTimer::timeout, this, &gauges::changeValues);
    minRPM = 0;
    minSpeed = 0;
    maxRPM = 7500;
    maxSpeed = 180;
    minTach = 270;
    maxTach = 135;
    minSpeedoRot = 270;
    maxSpeedoRot = 135;
    rpmval = 0;
    speedval = 0;
    currRPMPos = minTach;
    currSpeedPos = minSpeedoRot;

    QList<QObject*> obj = engine->rootObjects();
    for(int i = 0; i < obj.length(); i++){
        qDebug() << obj.at(i);
    }
    QObject* main = obj.at(0);
    qDebug() << main->children();
    tachNeedle = main->findChild<QObject*>("tachneedle", Qt::FindChildrenRecursively);
       qDebug() << tachNeedle;
    speedoNeedle = main->findChild<QObject*>("speedoneedle", Qt::FindChildrenRecursively);
        fuelNeedle = main->findChild<QObject*>("fuelneedle", Qt::FindChildrenRecursively);

    }

void gauges::setRPM()
{
    if(rpmIndex > -1){
        int rpm = (int)par[rpmIndex].getValue();



        int diff = 0;
        double percent = (double)(rpm)/maxRPM;
        double pos = minTach;

        if(maxTach > minTach){ //TODO: implement this correctly
            diff = minTach - maxTach;
            pos = minTach + (percent*diff);

        } else {
            diff = (minTach + maxTach)-180;
            pos = (double)minTach + (double)(percent*diff);
        }

        QPropertyAnimation *rpmAnim = new QPropertyAnimation(tachNeedle, "rotation");
        rpmAnim->setDuration(100);
        rpmAnim->setStartValue(currRPMPos);
        rpmAnim->setEndValue(pos);
        rpmAnim->start();


        currRPMPos = pos;
    } else {
        qDebug() << "rpm not found";
    }
}

void gauges::setSpeed()
{
    if(speedIndex > -1){
        int speed = (int)par[speedIndex].getValue();
        int diff = 0;
        double percent = (double)speed/maxSpeed;
        double pos = minSpeedoRot;
        if(maxSpeedoRot > minSpeedoRot){
            diff = maxSpeedoRot - minSpeedoRot;
            pos = minSpeedoRot + (percent*diff);
        } else {
            diff = (minSpeedoRot + maxSpeedoRot)-180;
            pos = (double)minSpeedoRot + (double)(percent*diff);
        }

        QPropertyAnimation *speedAnim = new QPropertyAnimation(speedoNeedle, "rotation");
        speedAnim->setDuration(100);
        speedAnim->setStartValue(currSpeedPos);
        speedAnim->setEndValue(pos);
        speedAnim->start();
        currSpeedPos = pos;
    } else {
        qDebug() << "speed not found";
    }
}

void gauges::setParamPointer(parameter *parameter, int length)
{
    paramLength = length;
    par = parameter;
    findSpeedIndex();
    findRPMIndex();
}


void gauges::findSpeedIndex()
{
    for(int i = 0; i < paramLength; i++){
        if(par[i].getName().toUpper() == "ENGINE SPEED"){
            rpmIndex = i;
            break;
        }
    }
}

void gauges::findRPMIndex()
{
    for(int i = 0; i < paramLength; i++){
        if(par[i].getName().toUpper() == "VEHICLE SPEED"){
            speedIndex = i;
            break;
        }
    }
}

void gauges::startTimer(){
    if(!testtimer->isActive()){
        qDebug() << "timer started";
        rpmval = 0;
        speedval = 0;
        //changeValues();
        testtimer->start(120);

    } else {
        testtimer->stop();
    }
}


void gauges::changeValues(){
    par[rpmIndex].setValue(rpmval);
        setRPM();
    if(rpmval < maxRPM){
    rpmval = rpmval + 100;
    } else {
        rpmval = 0;
    }
    par[speedIndex].setValue(speedval);
    setSpeed();
    if(speedval < maxSpeed-2){
        speedval = speedval + 2;
    } else {
        speedval = 0;
    }
}
