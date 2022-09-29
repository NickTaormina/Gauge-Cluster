#include "gauges.h"
#include <QtQuick>

gauges::gauges(QObject *parent)
    : QObject{parent}
{
}

gauges::gauges(QObject *parent, QObject *main, gear *gear, trip*tr, config * cfg, canData * data)
{
    parent = nullptr;
    //timers
    timer = new QTimer(this);
    testtimer = new QTimer(this);
    speedTime = new QTimer(this);
    sweepTimer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &gauges::updateValue);
    connect(testtimer, &QTimer::timeout, this, &gauges::changeValues);
    connect(speedTime, &QTimer::timeout, this, &gauges::updateSpeedText);

    //initialize config values
    qDebug() << "gauge values set: ";
    minRPM = cfg->getValue("tachMinValue").toInt(nullptr, 10);
    minSpeed = cfg->getValue("speedoMinValue").toInt(nullptr, 10);
    maxRPM = cfg->getValue("tachMaxValue").toInt(nullptr, 10);
    maxSpeed = cfg->getValue("speedoMaxValue").toInt(nullptr, 10);
    minTach = cfg->getValue("tachMinRot").toInt(nullptr, 10);
    maxTach = cfg->getValue("tachMaxRot").toInt(nullptr, 10);
    minSpeedoRot = cfg->getValue("speedoMinRot").toInt(nullptr, 10);
    maxSpeedoRot = cfg->getValue("speedoMaxRot").toInt(nullptr, 10);
    animDuration = cfg->getValue("generalAnimationDuration").toInt(nullptr, 10);
    showAllSpeedNumbers = cfg->getValue("generalShowAllSpeedDigits").toInt(nullptr, 10);
    initialGaugeSweep = cfg->getValue("generalStartupGaugeSweep").toInt(nullptr, 10);
    qDebug() << "gauge sweep: " << initialGaugeSweep;



    currRPMPos = minTach;
    currSpeedPos = minSpeedoRot;
    currSpeed = 0;
    rpmval = 0;
    speedval = 0;
    odoval = 0;
    speed = 0;

    g = gear;
    _trip = tr;
    activeTrip = "tripA";

    //find ui gauge elements to control. should probably move these to QML
    tachNeedle = main->findChild<QObject*>("tachneedle", Qt::FindChildrenRecursively);
    speedoNeedle = main->findChild<QObject*>("speedoneedle", Qt::FindChildrenRecursively);
    fuelNeedle = main->findChild<QObject*>("fuelneedle", Qt::FindChildrenRecursively);
    speedtext = main->findChild<QObject*>("speedText", Qt::FindChildrenRecursively);
    rpmtext = main->findChild<QObject*>("rpmText", Qt::FindChildrenRecursively);
    odotext = main->findChild<QObject*>("odoNum", Qt::FindChildrenRecursively);
    geartext = main->findChild<QObject*>("gearText", Qt::FindChildrenRecursively);
    tripNum = main->findChild<QObject*>("tripNum", Qt::FindChildrenRecursively);
    statustext = main->findChild<QObject*>("statusText", Qt::FindChildrenRecursively);

    //finds ui status elements to control. ex: turn signals
    leftSignal = main->findChild<QObject*>("leftSignal", Qt::FindChildrenRecursively);
    rightSignal = main->findChild<QObject*>("rightSignal", Qt::FindChildrenRecursively);
    lightIndicator = main->findChild<QObject*>("lightIndicator", Qt::FindChildrenRecursively);


    //set the trip indicator value on startup
    tripNum->setProperty("text", _trip->getTrip());


    //TODO: don't start requesting data until gauge sweep done
    sweepFinished = 0;
    if(initialGaugeSweep == 1){
        gaugeSweep();
    }

}



//updates the tach
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

//updates the speed analog gauge
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

//updates the middle speed readout
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

//sets parameter object for the cluster to read from
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

//resets the active trip
void gauges::resetTrip() //TODO: multiple trips
{
    if(activeTrip == "tripA"){
        _trip->resetTrip();
    } else if (activeTrip == "tripB"){
        _trip->resetTrip();
    }

}

//finds the rpm parameter
void gauges::findRPMIndex()
{
    for(int i = 0; i < paramLength; i++){
        if(par[i].getName().toUpper() == "ENGINE SPEED"){
            rpmIndex = i;
            break;
        }
    }
}

//finds the odometer reading parameter
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

//finds index in parameter array for each of the knock params
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

//updates the trip value in cluster
void gauges::updateTrip()
{
    _trip->updateTripDistance(speed, elapsedTimer.elapsed());
    elapsedTimer.restart();
    tripNum->setProperty("text", _trip->getTrip());
    emit tripUpdated("tripA", _trip->getTrip());



}

//displays knock message if there is a knock event (fbk/fkl/dam not normal)
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


//forward step of initial gauge sweep. had to split into 2 functions + timer because no signal
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

//second step of initial gauge sweep. Brings back to start location
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

//updates the light indicator based on light status
void gauges::updateLights(QString status)
{
    if(status == "on"){
        qDebug() << "lights on"; //icon visible
        lightIndicator->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/lightsOn.svg");
        lightIndicator->setProperty("opacity", 1.0);
    } else if (status == "park"){
        qDebug() << "parking lights on"; //icon visible (maybe dimmed)
        lightIndicator->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/lightsOn.svg");
        lightIndicator->setProperty("opacity", 0.7);
    } else{
        //off
        qDebug() << "lights off"; //icon not visible
        lightIndicator->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/lightsOff.svg");
    }
}

//shows which turn signals are on/off
void gauges::updateTurnSignals(QString status)
{
    if(status == "left"){
        qDebug() << "left signal";
        leftSignal->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/onSignal.svg");
        rightSignal->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/offSignal.svg");
    } else if (status == "right"){
        qDebug() << "right signal";
        leftSignal->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/offSignal.svg");
        rightSignal->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/onSignal.svg");
    } else if (status == "hazard"){
        qDebug() << "hazards";
        leftSignal->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/onSignal.svg");
        rightSignal->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/onSignal.svg");
    } else{
        //off
        qDebug() << "turn signals off";
        leftSignal->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/offSignal.svg");
        rightSignal->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/offSignal.svg");
    }
}

//sets gear indicator to N when in neutral
void gauges::updateNeutral(QString status)
{
    if(status == "Neutral"){
        geartext->setProperty("text", "N");
        qDebug() << "neutral";
    } else {
        updateGear();
    }
}

//sets gear indicator to R when in reverse
void gauges::updateReverse(QString status)
{
    if(status == "Reverse"){
        geartext->setProperty("text", "R");
        qDebug() << "Reverse";
    } else {
        updateGear();
    }
}


//startup gauge sweep controller
void gauges::gaugeSweep()
{
    sweepForward();
}

//finds the parameter for vehicle speed
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

//starts/stops the test sweep
void gauges::startTest(){
    if(!timer->isActive()){
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

//sets the odometer text
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

//updates all gauges based on ecu response data
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

//controls the test sweep
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

    if(rpmval >= 2400 && rpmval <= 2800){
        updateReverse("Reverse");
    }

    par[fklIndex].setValue(0);
    par[damIndex].setValue(1.0);
    odoval = odoval + 2;


}
