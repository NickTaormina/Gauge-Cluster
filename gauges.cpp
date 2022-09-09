#include "gauges.h"
#include <QtQuick>

gauges::gauges(QObject *parent)
    : QObject{parent}
{
    timer = new QTimer(this);
}

gauges::gauges(QObject *parent, QObject *main, gear *gear, trip*tr)
{
    timer = new QTimer(this);
    testtimer = new QTimer(this);
    speedTime = new QTimer(this);
    sweepTimer = new QTimer(this);
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
    g = gear;
    _trip = tr;

    tachNeedle = main->findChild<QObject*>("tachneedle", Qt::FindChildrenRecursively);
    speedoNeedle = main->findChild<QObject*>("speedoneedle", Qt::FindChildrenRecursively);
    fuelNeedle = main->findChild<QObject*>("fuelneedle", Qt::FindChildrenRecursively);
    speedtext = main->findChild<QObject*>("speedText", Qt::FindChildrenRecursively);
    rpmtext = main->findChild<QObject*>("rpmText", Qt::FindChildrenRecursively);
    odotext = main->findChild<QObject*>("odoNum", Qt::FindChildrenRecursively);
    geartext = main->findChild<QObject*>("gearText", Qt::FindChildrenRecursively);
    tripNum = main->findChild<QObject*>("tripNum", Qt::FindChildrenRecursively);
    statustext = main->findChild<QObject*>("statusText", Qt::FindChildrenRecursively);

    sweepFinished = 0;
    gaugeSweep();

}

gauges::gauges(QObject *parent, QObject *main)
    : QObject{parent}
{
    timer = new QTimer(this);
    testtimer = new QTimer(this);
    speedTime = new QTimer(this);
    sweepTimer = new QTimer(this);
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
    geartext = main->findChild<QObject*>("gearText", Qt::FindChildrenRecursively);
    tripNum = main->findChild<QObject*>("tripNum", Qt::FindChildrenRecursively);
    statustext = main->findChild<QObject*>("statusText", Qt::FindChildrenRecursively);

    sweepFinished = 0;
    gaugeSweep();
}

void gauges::setRPM()
{
    if(rpmIndex > -1 && sweepFinished == 1){
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
        if(rpm < maxRPM){
            rpmtext->setProperty("text", rpm);
        }

        currRPMPos = pos;
    } else {
        qDebug() << "rpm not found";
    }
}

void gauges::setSpeed()
{
    if(speedIndex > -1 && sweepFinished == 1){
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
    if(speed < maxSpeed){
        int s = speedtext->property("text").toInt(nullptr);
        if(s >= speed){
            speedTime->stop();
            speedtext->setProperty("text", speed);
        } else{
            speedtext->setProperty("text", (s + 1));
        }
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
    getKnockIndexes();
    qDebug() << "fkl index: " << fklIndex << "fbk index: " << fbkIndex << "dam index: " << damIndex;

    QObject::connect(&par[rpmIndex], &parameter::valueChanged, this, &gauges::updateValue);
    par[fbkIndex].setValue(-1.41);



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

//TODO: trigger this on clutch in. neutral detection
void gauges::updateGear()
{
    if(g){
        QString currgear = g->calcGear(par[rpmIndex].getValue(), speed);
        geartext->setProperty("text", currgear);

    }
}

void gauges::getKnockIndexes()
{
    for(int i = 0; i < paramLength; i++){
        if(par[i].getName().toUpper() == "FINE KNOCK LEARN"){
            fklIndex = i;
        } else if(par[i].getName().toUpper() == "FEEDBACK KNOCK CORRECTION"){
            fbkIndex = i;
        } else if(par[i].getName().toUpper() == "DYNAMIC ADVANCE MULTIPLIER"){
            damIndex = i;
        }
    }
}

void gauges::updateTrip()
{
    _trip->updateTripDistance(speed, elapsedTimer.elapsed());
    elapsedTimer.restart();
    tripNum->setProperty("text", _trip->getTrip());


}

void gauges::showKnock()
{
    if(par[fklIndex].getValue() == 0 && par[fbkIndex].getValue() == 0 && par[damIndex].getValue() == 1){
        statustext->setProperty("text", "");
        statustext->setProperty("visible", false);
        return;
    } else {
        QString str;
        if(par[fbkIndex].getValue() != 0){
            str.append("FBK: ");
            str.append(QString::number(par[fbkIndex].getValue()));
            str.append("\n");
        }
        if(par[fklIndex].getValue() != 0){
            str.append("FKL: " + QString::number(par[fklIndex].getValue()) + "\n");
        }
        if(par[damIndex].getValue() != 1){
            str.append("DAM: " + QString::number(par[damIndex].getValue()) + "\n");
        }
        statustext->setProperty("text", (QVariant)str);
        statustext->setProperty("visible", true);

    }
}

void gauges::sweepForward()
{
    QPropertyAnimation *speedAnim = new QPropertyAnimation(speedoNeedle, "rotation");
    QPropertyAnimation *rpmAnim = new QPropertyAnimation(tachNeedle, "rotation");

    sweepTimer->setInterval(1100);
    speedAnim->setDuration(1000);
    speedAnim->setStartValue(minSpeedoRot);
    speedAnim->setEndValue(maxSpeedoRot+360);
    speedAnim->start();
    sweepTimer->start();
    rpmAnim->setDuration(1000);
    rpmAnim->setStartValue(minTach);
    rpmAnim->setEndValue(maxTach + 360);
    rpmAnim->start();

    connect(sweepTimer, &QTimer::timeout, this, &gauges::sweepBack);


}

void gauges::sweepBack()
{
    sweepTimer->stop();
    disconnect(sweepTimer, &QTimer::timeout, this, &gauges::sweepBack);
    QPropertyAnimation *speedAnim = new QPropertyAnimation(speedoNeedle, "rotation");
    speedAnim->setDuration(1000);
    speedAnim->setStartValue(maxSpeedoRot+360);
    speedAnim->setEndValue(minSpeedoRot);
    speedAnim->start();
    QPropertyAnimation *rpmAnim = new QPropertyAnimation(tachNeedle, "rotation");
    rpmAnim->setDuration(1000);
    rpmAnim->setStartValue(maxTach+360);
    rpmAnim->setEndValue(minTach);
    rpmAnim->start();
    sweepFinished = 1;
}

void gauges::gaugeSweep()
{
    sweepForward();






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
        elapsedTimer.start();

    } else {
        testtimer->stop();
    }
}

void gauges::setOdometer()
{
    int digits = 6;
    int val = par[odoIndex].getValue();
    QString str = QString::number(val);
    int length = str.length();
    if(length < digits){
        int mult = digits-length;
        for(int i = 0; i<mult; i++){
            str.prepend("0");
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
        updateGear();
        updateTrip();
        showKnock();
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


    if(rpmval > 2000){
        double fbk = par[fbkIndex].getValue();
        if(fbk < 0){
            par[fbkIndex].setValue(fbk + 0.35);
            qDebug() << fbk;
        }
        if(fbk < 0.1 && fbk >-0.1){
            fbk = 0;
            par[fbkIndex].setValue(fbk);
        }
    }
    par[fklIndex].setValue(0);
    par[damIndex].setValue(1.0);
    odoval = odoval + 2;


}
