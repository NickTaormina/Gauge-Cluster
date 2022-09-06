#include "gauges.h"
#include <QtQuick>

gauges::gauges(QObject *parent)
    : QObject{parent}
{
    timer = new QTimer(this);
}

gauges::gauges(QObject *parent, QObject *main)
    : QObject{parent}
{
    timer = new QTimer(this);
    testtimer = new QTimer(this);
    speedTime = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &gauges::updateValue);
    connect(testtimer, &QTimer::timeout, this, &gauges::changeValues);
    connect(speedTime, &QTimer::timeout, this, &gauges::updateSpeedText);
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
    animDuration = 100;
    currSpeed = 0;
    odoval = 0;
    speed = 0;
    g = new gear;

    tachNeedle = main->findChild<QObject*>("tachneedle", Qt::FindChildrenRecursively);  
    speedoNeedle = main->findChild<QObject*>("speedoneedle", Qt::FindChildrenRecursively);
    fuelNeedle = main->findChild<QObject*>("fuelneedle", Qt::FindChildrenRecursively);
    speedtext = main->findChild<QObject*>("speedText", Qt::FindChildrenRecursively);
    rpmtext = main->findChild<QObject*>("rpmText", Qt::FindChildrenRecursively);
    odotext = main->findChild<QObject*>("odoNum", Qt::FindChildrenRecursively);
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
        rpmtext->setProperty("text", rpm);

        currRPMPos = pos;
    } else {
        qDebug() << "rpm not found";
    }
}

void gauges::setSpeed()
{
    if(speedIndex > -1){
        if(speedTime->isActive()){
            speedTime->setInterval(5);
        }
        speed = (int)par[speedIndex].getValue();
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
        int speedDelta = speed - currSpeed;
        if(speedDelta != 0){
            int speedDuration = (animDuration/(speedDelta))/2;
            speedTime->start(speedDuration);
            QPropertyAnimation *speedAnim = new QPropertyAnimation(speedoNeedle, "rotation");
            speedAnim->setDuration(animDuration);
            speedAnim->setStartValue(currSpeedPos);
            speedAnim->setEndValue(pos);
            speedAnim->start();
            currSpeed = speed;
        }
        currSpeedPos = pos;
    } else {
        qDebug() << "speed not found";
    }
}

void gauges::updateSpeedText(){
    int s = speedtext->property("text").toInt(nullptr);
    if(s >= speed){
        speedTime->stop();
        speedtext->setProperty("text", speed);
    } else{
        speedtext->setProperty("text", (s + 1));
    }
}

void gauges::setParamPointer(parameter *parameter, int length)
{
    paramLength = length;
    par = parameter;
    findSpeedIndex();
    qDebug() << "speed index: " << speedIndex;
    findRPMIndex();
    qDebug() << "rpm index: " << rpmIndex;
    findOdoIndex();
    qDebug() << "odo index: " << odoIndex;
    QObject::connect(&par[rpmIndex], &parameter::valueChanged, this, &gauges::updateValue);



}


void gauges::findRPMIndex()
{
    for(int i = 0; i < paramLength; i++){
        if(par[i].getName().toUpper() == "ENGINE SPEED"){
            rpmIndex = i;
            break;
        }
    }
}

void gauges::findOdoIndex()
{
    for(int i = 0; i < paramLength; i++){
        if(par[i].getName().toUpper() == "ODOMETER"){
            odoIndex = i;
            break;
        }
    }
}

void gauges::findSpeedIndex()
{
    for(int i = 0; i < paramLength; i++){
        if(par[i].getName().toUpper() == "VEHICLE SPEED"){
            speedIndex = i;
            break;
        }
    }
}

void gauges::startTimer(){
    if(!timer->isActive()){
        qDebug() << "timer started";
        rpmval = 0;
        speedval = 0;
        timer->start(animDuration);

    } else {
        timer->stop();
    }
}
void gauges::startTest(){
    if(!testtimer->isActive()){
        qDebug() << "timer started";
        rpmval = 0;
        speedval = 0;
        changeValues();
        testtimer->start(animDuration);

    } else {
        testtimer->stop();
    }
}

void gauges::setOdometer()
{
    qDebug() << "set odo";
    int digits = 6;
    int val = par[odoIndex].getValue();
    qDebug() << "val: " << val;
    QString str = QString::number(val);
    qDebug() << "str: " << str;
    int length = str.length();
    qDebug() << "str length: " << length;
    if(length < digits){
        int mult = digits-length;
        qDebug() << "mult: " << mult;
        for(int i = 0; i<mult; i++){
            str.prepend("0");
            qDebug() << "str: " <<str;
        }
    }
    odotext->setProperty("text", (QVariant)str);
}

void gauges::updateValue()
{
    if(speedIndex > -1){
        setRPM();
        setSpeed();
        setOdometer();
    }
}


void gauges::changeValues(){
    par[rpmIndex].setValue(rpmval);
    if(rpmval < maxRPM){
    rpmval = rpmval + 100;
    } else {
        rpmval = 0;
    }
    par[speedIndex].setValue(speedval);
    if(speedval < maxSpeed-2){
        speedval = speedval + 2;
    } else {
        speedval = 0;
    }
    par[odoIndex].setValue(odoval);
    odoval = odoval + 2;


}
