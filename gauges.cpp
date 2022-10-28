#include "gauges.h"
#include <QtQuick>

gauges::gauges(QObject *parent)
    : QObject{parent}
{
}

gauges::gauges(QObject *parent, QObject *main, gear *gear, config * cfg, configHandler * handler, canData * data)
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
    connect(&_weather, &weather::tempRead, this, &gauges::updateTemperatureText); //update temp text when weather report read
    connect(&weatherTimer, &QTimer::timeout, this, &gauges::updateWeatherStatus); //update weather every 5 min
    shiftLightTimer.setSingleShot(false);
    connect(&shiftLightTimer, &QTimer::timeout, this, &gauges::showShiftLight);
    connect(this, &gauges::rpmUpdated, this, &gauges::flashShiftLight);

    //initialize config values
    qDebug() << "*gauge values set: ";
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

    qDebug() << "*gauge sweep: " << initialGaugeSweep;



    currRPMPos = minTach;
    currSpeedPos = minSpeedoRot;
    currSpeed = 0;
    rpmval = 0;
    speedval = 0;
    qDebug() << "odometer: " << cfg->getValue("generalOdometer");
    odoval = cfg->getValue("generalOdometer").toInt(nullptr, 10);

    speed = 0;
    shiftLightMin = cfg->getValue("generalShiftLightThreshold").toUInt(nullptr, 10);
    shiftLightFlashInterval = cfg->getValue("generalShiftLightFlashTimer").toUInt(nullptr, 10);

    g = gear;
    _data = data;



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
    shiftLight = main->findChild<QObject*>("shiftLight", Qt::FindChildrenRecursively);

    //finds ui status elements to control. ex: turn signals
    leftSignal = main->findChild<QObject*>("leftSignal", Qt::FindChildrenRecursively);
    rightSignal = main->findChild<QObject*>("rightSignal", Qt::FindChildrenRecursively);
    lightIndicator = main->findChild<QObject*>("lightIndicator", Qt::FindChildrenRecursively);

    //finds ui parameter objects (from logger)
    topLeftLabel = main->findChild<QObject*>("topLeftLabel", Qt::FindChildrenRecursively);
    topRightLabel = main->findChild<QObject*>("topRightLabel", Qt::FindChildrenRecursively);
    bottomLeftLabel = main->findChild<QObject*>("bottomLeftLabel", Qt::FindChildrenRecursively);
    bottomRightLabel = main->findChild<QObject*>("bottomRightLabel", Qt::FindChildrenRecursively);
    topLeftValue = main->findChild<QObject*>("topLeftValue", Qt::FindChildrenRecursively);
    topRightValue = main->findChild<QObject*>("topRightValue", Qt::FindChildrenRecursively);
    bottomRightValue = main->findChild<QObject*>("bottomRightValue", Qt::FindChildrenRecursively);
    bottomLeftValue = main->findChild<QObject*>("bottomLeftValue", Qt::FindChildrenRecursively);
    clockText = main->findChild<QObject*>("clockText", Qt::FindChildrenRecursively);
    temperatureText = main->findChild<QObject*>("temperatureText", Qt::FindChildrenRecursively);
    fuelBar = main->findChild<QObject*>("fuelBarImage", Qt::FindChildrenRecursively);
    coolantGauge = main->findChild<QObject*>("coolantBarImage", Qt::FindChildrenRecursively);


    //paramater display label init
    if(topLeftLabel){
    topLeftText = "AF Ratio";
    topLeftLabel->setProperty("text", topLeftText);}
    topRightText = "AF Learning";
    topRightLabel->setProperty("text", topRightText);
    bottomRightText = "Oil Temp";
    bottomRightLabel->setProperty("text", bottomRightText);
    bottomLeftText = "Intake Temp";
    bottomLeftLabel->setProperty("text", bottomLeftText);

    //set the gear value on startup
    geartext->setProperty("text", "N");


    //TODO: don't start requesting data until gauge sweep done
    sweepFinished = 0;
    if(initialGaugeSweep == 1){
        gaugeSweep();
    }

    QObject::connect(_data, &canData::turnSignal, this, &gauges::updateTurnSignals);
    QObject::connect(_data, &canData::lights, this, &gauges::updateLights);
    QObject::connect(_data, &canData::reverseSwitch, this, &gauges::updateReverse);
    QObject::connect(_data, &canData::rpmChanged, this, &gauges::setRPMCAN);
    QObject::connect(_data, &canData::speedChanged, this, &gauges::setSpeedCAN);
    QObject::connect(_data, &canData::neutralSwitch, this, &gauges::updateNeutral);
    QObject::connect(_data, &canData::usefulFrameReceived, this, &gauges::updateClock);
    updateClock();

    //fill local trip info and update display
    handler->fillTrip(&trA, "tripA");
    handler->fillTrip(&trB, "tripB");
    qDebug() << "*trip A: " << trA.getTrip();
    qDebug() << "*trip B: " << trB.getTrip();

    activeTrip = cfg->getValue("generalActiveTrip");
    qDebug() << "*activeTrip: " << activeTrip;
    tripNum->setProperty("text", getActiveTripNum());
    updateParamDisplay("Odometer", (double)odoval);
}



//updates the tach
void gauges::setRPM()
{
    if(rpmIndex > -1 && sweepFinished == 1){
        int rpm = (int)par[rpmIndex].getValue();



        int diff = 0;
        double percent = (double)(rpm)/maxRPM;
        double pos;

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
            if(rpmtext){
            rpmtext->setProperty("text", rpm);
            }
        }

        currRPMPos = pos;
    } else {
    }
}

void gauges::setRPMCAN(uint rpm)
{

    if(sweepFinished == 1 && rpm >= 0 && rpm < 10000){
        _rpm = rpm;
        int diff = 0;
        double percent = (double)(rpm)/maxRPM;
        double pos;

        if(maxTach > minTach){ //TODO: implement this correctly
            diff = minTach - maxTach;
            pos = minTach + (percent*diff);

        } else {
            diff = (minTach + maxTach)-180;
            pos = (double)minTach + (double)(percent*diff);
        }
        if(tachNeedle){
            QPropertyAnimation *rpmAnim = new QPropertyAnimation(tachNeedle, "rotation");
            rpmAnim->setDuration(animDuration/4);
            rpmAnim->setStartValue(currRPMPos);
            rpmAnim->setEndValue(pos);
            rpmAnim->start();
        }
        if(rpm < (uint)maxRPM){
            if(rpmtext){
                rpmtext->setProperty("text", rpm);
            }
        }
        emit rpmUpdated(rpm);


        currRPMPos = pos;
    } else {
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
            if(speedoNeedle){
                QPropertyAnimation *speedAnim = new QPropertyAnimation(speedoNeedle, "rotation");
                speedAnim->setDuration(animDuration);
                speedAnim->setStartValue(currSpeedPos);
                speedAnim->setEndValue(pos);
                speedAnim->start();
            }
            currSpeed = speed;
        }
        currSpeedPos = pos;
    } else {
    }
}

//updates the speed analog gauge from passive CAN read
void gauges::setSpeedCAN(double spd)
{
    speed = spd;
    if(sweepFinished == 1){
        if(speedTime->isActive()){
            speedTime->setInterval(5);
        }
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
            int speedDuration = abs((20/(speedDelta))/2);
            speedTime->start(speedDuration);
            if(speedoNeedle){
                QPropertyAnimation *speedAnim = new QPropertyAnimation(speedoNeedle, "rotation");
                speedAnim->setDuration(animDuration);
                speedAnim->setStartValue(currSpeedPos);
                speedAnim->setEndValue(pos);
                speedAnim->start();
            }
            currSpeed = speed;
        }
        currSpeedPos = pos;
        updateTrip();
    }
}

//updates the weather readouts with new data
void gauges::updateWeatherStatus()
{
    _weather.requestWeather();
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
    qDebug() << "*speed index: " << speedIndex;
    findRPMIndex();
    qDebug() << "*rpm index: " << rpmIndex;
    findOdoIndex();
    qDebug() << "*odo index: " << odoIndex;
    getKnockIndexes();
    qDebug() << "*fkl index: " << fklIndex << "fbk index: " << fbkIndex << "dam index: " << damIndex;

    QObject::connect(&par[rpmIndex], &parameter::valueChanged, this, &gauges::updateValue);





}

void gauges::switchActiveTrip(){
    if(activeTrip == "tripA"){
        activeTrip = "tripB";
    }else {
        activeTrip = "tripA";
    }
    qDebug() << "active trip: " << activeTrip;
    emit tripSwapped(activeTrip);
    tripNum->setProperty("text", getActiveTripNum());
}
//resets the given trip
void gauges::resetTrip(QString tr) //TODO: multiple trips
{
    qDebug()<< "reset trip: " << tr;
    if(tr == "tripA"){
        trA.resetTrip();
        tripNum->setProperty("text", getActiveTripNum());
        emit tripUpdated("tripA", "0");
    } else if (tr == "tripB"){
        trB.resetTrip();
        tripNum->setProperty("text", getActiveTripNum());
        emit tripUpdated("tripB", "0");
    }

}

void gauges::setShiftThreshold(QString val)
{
    shiftLightMin = val.toInt(nullptr, 10);
    emit shiftThresholdChanged(val);
}

//finds the rpm parameter
void gauges::findRPMIndex()
{
    rpmIndex = 0;
    for(int i = 0; i < paramLength; i++){
        if(par[i].getName().toUpper() == "ENGINE SPEED"){
            rpmIndex = i;
            break;
        }
    }
}
//finds the parameter for vehicle speed
void gauges::findSpeedIndex()
{
    speedIndex = 0;
    for(int i = 0; i < paramLength; i++){
        if(par[i].getName().toUpper() == "VEHICLE SPEED"){
            speedIndex = i;
            break;
        }
    }
}
//finds the odometer reading parameter
void gauges::findOdoIndex()
{
    odoIndex = 0;
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
        QString currgear = g->calcGear(_rpm, speed);
        //qInfo() << "update gear: " << _rpm << " : " << speed;
        geartext->setProperty("text", currgear);

    }
}

//finds index in parameter array for each of the knock params
void gauges::getKnockIndexes()
{
    fklIndex = 0;
    fbkIndex = 0;
    damIndex = 0;
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

QString gauges::getActiveTripNum(){
    if(activeTrip == "tripA"){
       return trA.getTrip();
    } else {
        return trB.getTrip();
    }

}

void gauges::updateActiveTripDistance(int speed, qint64 time){
    if(activeTrip == "tripA"){
        trA.updateTripDistance(speed, time);
    } else if(activeTrip == "tripB"){
        trB.updateTripDistance(speed, time);
    }
}

//updates the trip value in cluster
void gauges::updateTrip()
{
    qDebug() << "*speed: " << speed << " elapsed time: " << elapsedTimer.elapsed();
    updateActiveTripDistance(speed, elapsedTimer.elapsed());
    elapsedTimer.restart();
    tripNum->setProperty("text", getActiveTripNum());
    //qDebug() << activeTrip << ": " << getActiveTripNum();
    emit tripUpdated(activeTrip, getActiveTripNum());



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
    if(tachNeedle){
        QPropertyAnimation *rpmAnim = new QPropertyAnimation(tachNeedle, "rotation");

        sweepTimer->setInterval(1500);

        if(speedoNeedle){
            QPropertyAnimation *speedAnim = new QPropertyAnimation(speedoNeedle, "rotation");
            //connect(speedAnim, &QPropertyAnimation::finished, this, &gauges::sweepBack);
            speedAnim->setDuration(900);
            speedAnim->setStartValue(minSpeedoRot);
            speedAnim->setEndValue(maxSpeedoRot+360);
            speedAnim->start();
        }

        sweepTimer->start();
        rpmAnim->setDuration(1000);
        rpmAnim->setStartValue(minTach);
        rpmAnim->setEndValue(maxTach + 360);
        rpmAnim->start();

        connect(sweepTimer, &QTimer::timeout, this, &gauges::sweepBack);
    }

}

//second step of initial gauge sweep. Brings back to start location
void gauges::sweepBack()
{
    sweepTimer->stop();
    if(tachNeedle){
        disconnect(sweepTimer, &QTimer::timeout, this, &gauges::sweepBack);

        if(speedoNeedle){
            QPropertyAnimation *speedAnim = new QPropertyAnimation(speedoNeedle, "rotation");
            speedAnim->setDuration(1000);
            speedAnim->setStartValue(maxSpeedoRot+360);
            speedAnim->setEndValue(minSpeedoRot);
            speedAnim->start();
        }
        QPropertyAnimation *rpmAnim = new QPropertyAnimation(tachNeedle, "rotation");
        rpmAnim->setDuration(1000);
        rpmAnim->setStartValue(maxTach+360);
        rpmAnim->setEndValue(minTach);
        rpmAnim->start();
        QTimer* finish = new QTimer(this);
        finish->setSingleShot(true);
        finish->setInterval(1050);
        finish->start();
        QObject::connect(finish, &QTimer::timeout, this, &gauges::sweepDone);

    }
}

void gauges::updateParamDisplay(QString name, double value)
{
    //convert to string to allow formatting
    //qDebug() << "update param display: " << name;

    if(name == "AF Ratio"){
        //qDebug() << "updating ratio: " << value;
        //ensures two decimals
        QString val = QString::number(value, 'f', 2);
        if(val.length()<3){
            val.append(".");
        }
        if(val.length()<5){
            for(int i = val.length(); i<5; i++){
                val.append("0");
            }
        }
        if(topLeftValue){
        topLeftValue->setProperty("text", val);

        }
    }else if (name == "AF Learning 1"){
        if(topRightValue){
            QString val = QString::number(value, 'f', 2);
        topRightValue->setProperty("text", val);}
    } else if (name == "Oil Temp"){
        value = 1.8*value + 32;

            QString val = QString::number(value, 'f', 0);
            if(bottomRightValue){
            bottomRightValue->setProperty("text", val);}


    } else if (name == "Intake Manifold Temperature"){
        if(bottomLeftValue){
            QString val = QString::number(value, 'f', 0);
        bottomLeftValue->setProperty("text", val);}
    } else if(name == "Odometer"){
        QString val = QString::number(value, 'f', 0);
        if(val.length() < 6){
            for(int x = val.length(); x<6; x++){
                val.prepend("0");
            }
        }
        if(val != odotext->property("text").toString()){
            emit odometerUpdated(val);
            odotext->setProperty("text", QVariant(val));
        }
    } else if (name == "Coolant Temp"){
        updateCoolantGauge(value);
    }

    //refresh ui
    //QCoreApplication::processEvents();
}

//changes the outside temperature readout
void gauges::updateTemperatureText(QString t)
{
   QString tr = t + " F";
   temperatureText->setProperty("text", tr);
   weatherTimer.start(300000);
}

void gauges::updateCoolantGauge(double value)
{
    //TODO: change this for coolant bar (supra) vs coolant gauge (oem)
    if(coolantGauge){
    QString filePath = "coolantBar";
    if(value < 100){
        filePath.append("0");
    } else if(value > 100 && value < 125){
        filePath.append("1");
    } else if(value >= 125 && value < 150 ){
        filePath.append("2");
    } else if (value >= 150 && value < 175) {
        filePath.append("3");
    } else if (value >= 175 && value < 220){
        filePath.append("4");
    } else if (value >= 220 && value < 235){
        filePath.append("5");
    } else if (value >= 235 && value < 250){
        filePath.append("6");
    } else {
        filePath.append("7");
    }
    if(filePath != coolantFilePath){
        qDebug() << "opening coolant: " << filePath;
        coolantGauge->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/" + filePath + ".png");
        coolantFilePath = filePath;
    }
    }
}

void gauges::updateFuelBar(double value)
{
    value = value/255;
    //TODO: change this for coolant bar (supra) vs coolant gauge (oem)
    if(fuelBar){
    QString filePath = "fuelBar";
    if(value < .05){
        filePath.append("0");
    } else if(value <.10){
        filePath.append("1");
    } else if(value <.25){
        filePath.append("2");
    } else if (value <.4) {
        filePath.append("3");
    } else if (value <.55){
        filePath.append("4");
    } else if (value <.7){
        filePath.append("5");
    } else if (value <.85){
        filePath.append("6");
    } else {
        filePath.append("7");
    }
    if(filePath != fuelFilePath){
        fuelBar->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/" + filePath + ".png");
        fuelFilePath = filePath;
    }
    }
}

void gauges::flashShiftLight(uint rpm)
{
    if(rpm > shiftLightMin){
        if(!shiftLightTimer.isActive()){
            shiftLight->setProperty("visible", true);
            shiftLightTimer.start(shiftLightFlashInterval);
        }
    } else {
        shiftLightTimer.stop();
        shiftLight->setProperty("visible", false);
    }
}

void gauges::showShiftLight()
{
    if(shiftLight->property("visible") == true){
        shiftLight->setProperty("visible", false);
    } else {
        shiftLight->setProperty("visible", true);
    }
}

QString gauges::getShiftThreshold()
{
    return QString::number(shiftLightMin, 10);
}
QString gauges::getShiftTimer()
{
    return QString::number(shiftLightFlashInterval, 10);
}

void gauges::sweepDone(){
    sweepFinished = 1;

}

//updates the cluster clock
void gauges::updateClock()
{
    QString hour = QString::number(systime.currentDateTime().time().hour());
    if(hour.length() < 2){
        hour.prepend("0");
    }
    QString minute = QString::number(systime.currentDateTime().time().minute());
    if(minute.length() < 2){
        minute.prepend("0");
    }
    QString time = QString(hour + ":" + minute);
    clockText->setProperty("text", time);
}

//updates the light indicator based on light status
void gauges::updateLights(QString status)
{
    if(status == "on"){
        //icon visible
        lightIndicator->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/lightsOn.svg");
        lightIndicator->setProperty("opacity", 1.0);
    } else if (status == "park"){
        //icon visible (dimmed)
        lightIndicator->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/lightsOn.svg");
        lightIndicator->setProperty("opacity", 0.7);
    } else{
        //off: icon not visible
        lightIndicator->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/lightsOff.svg");
    }
}

//shows which turn signals are on/off
void gauges::updateTurnSignals(QString status)
{
    //qDebug() << "updating turn signals";
    if(status == "left"){
        leftSignal->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/onSignal.svg");
        rightSignal->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/offSignal.svg");
    } else if (status == "right"){
        leftSignal->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/offSignal.svg");
        rightSignal->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/onSignal.svg");
    } else if (status == "hazard"){
        leftSignal->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/onSignal.svg");
        rightSignal->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/onSignal.svg");
    } else{
        //off
        leftSignal->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/offSignal.svg");
        rightSignal->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/offSignal.svg");
    }
}

//sets gear indicator to N when in neutral
void gauges::updateNeutral(QString status)
{
    if(status == "Neutral"){
        geartext->setProperty("text", "N");
    } else {
        updateGear();
    }
}

//sets gear indicator to R when in reverse
void gauges::updateReverse(QString status)
{
    if(status == "Reverse"){
        geartext->setProperty("text", "R");
    } else {
        updateGear();
    }
}


//startup gauge sweep controller
void gauges::gaugeSweep()
{
    sweepForward();
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
    if(!testtimer->isActive()){
        qDebug() << "timer started";
        rpmval = 0;
        speedval = 0;
        changeValues();
        testtimer->start(animDuration/2);
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
    setSpeedCAN(speedval);
    setRPMCAN(rpmval);
    //qDebug() << "rpm: " << rpmval;


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
