#include "gauges.h"
#include <QtQuick>

gauges::gauges(QObject *parent)
    : QObject{parent}
{
}

gauges::gauges(QObject *parent, QObject * main, gear* gear, config* cfg, configHandler *handler, canData *data, defWindow * defwin, fueleconomy * fe)
{
    //store objects
    parent = nullptr;
    g = gear;
    _data = data;
    handbrake = false;
    _fe = fe;
    _defWin = defwin;
    _main = main;


    //timers
    timer = new QTimer(this);
    testtimer = new QTimer(this);
    speedTime = new QTimer(this);
    sweepTimer = new QTimer(this);
    shiftTimer = new QTimer(this);
    mpgTimer = new QTimer(this);
    mpgTimer->setInterval(30000);
    shiftLightTimer.setSingleShot(false);
    shiftTimer->setSingleShot(true);
    clockTimer.setInterval(1000);


    //timer connects
    connect(timer, &QTimer::timeout, this, &gauges::updateValue);
    connect(testtimer, &QTimer::timeout, this, &gauges::changeValues);
    connect(speedTime, &QTimer::timeout, this, &gauges::updateSpeedText);
    //connect(&_weather, &weather::tempRead, this, &gauges::updateTemperatureText); //update temp text when weather report read
    //connect(&weatherTimer, &QTimer::timeout, this, &gauges::updateWeatherStatus); //update weather every 5 min
    connect(&shiftLightTimer, &QTimer::timeout, this, &gauges::showShiftLight);
    connect(this, &gauges::rpmUpdated, this, &gauges::flashShiftLight);
    connect(shiftTimer, &QTimer::timeout, this, &gauges::onShiftTimerTimeout);
    connect(&clockTimer, &QTimer::timeout, this, &gauges::updateClock);
    connect(sweepTimer, &QTimer::timeout, this, &gauges::fadeInGauges);



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
    qDebug() << "*gauge sweep setting: " << initialGaugeSweep;
    shiftLightMin = cfg->getValue("generalShiftLightThreshold").toUInt(nullptr, 10);
    shiftLightFlashInterval = cfg->getValue("generalShiftLightFlashTimer").toUInt(nullptr, 10);
    odoval = cfg->getValue("generalOdometer").toDouble(nullptr);
    activeTrip = cfg->getValue("generalActiveTrip");
    shiftTimerDuration = cfg->getValue("generalShiftTargetTimer").toInt(nullptr, 10);
    qDebug() << "shift timer duration: " << shiftTimerDuration;

    fuelResMin = 383;
    fuelResMax = 19;
    fuelSamples = 0;
    fuelBarMin = 7;
    storedSample = false;

    //initialize temp vars to avoid error
    prevRPMPos = minTach;
    prevSpeedPos = minSpeedoRot;
    prevSpeed = 0;
    rpmval = 0;
    speedval = 0;
    _speed = 0;
    _rpm = 0;








    //find ui gauge elements to control. should probably move these to QML

    shiftTargetRect = main->findChild<QObject*>("speedoHandler", Qt::FindChildrenRecursively);
    tachNeedle = main->findChild<QObject*>("tachneedle", Qt::FindChildrenRecursively);
    speedoNeedle = main->findChild<QObject*>("speedoneedle", Qt::FindChildrenRecursively);
    fuelNeedle = main->findChild<QObject*>("fuelneedle", Qt::FindChildrenRecursively);
    speedtext = main->findChild<QObject*>("speedText", Qt::FindChildrenRecursively);
    rpmtext = main->findChild<QObject*>("rpmText", Qt::FindChildrenRecursively);
    odotext = main->findChild<QObject*>("odoNum", Qt::FindChildrenRecursively);
    geartext = main->findChild<QObject*>("gearText", Qt::FindChildrenRecursively);
    tripNum = main->findChild<QObject*>("tripNum", Qt::FindChildrenRecursively);
    statustext = main->findChild<QObject*>("statusText", Qt::FindChildrenRecursively);
    statustext->setProperty("text", "");
    shiftLight = main->findChild<QObject*>("shiftLight", Qt::FindChildrenRecursively);
    shiftTargetRect = main->findChild<QObject*>("shiftTargetRect", Qt::FindChildrenRecursively);
    neutralText = main->findChild<QObject*>("neutralText", Qt::FindChildrenRecursively);
    refText = main->findChild<QObject*>("refText", Qt::FindChildrenRecursively);
    clutchText = main->findChild<QObject*>("clutchText", Qt::FindChildrenRecursively);
    fuelText = main->findChild<QObject*>("fuelText", Qt::FindChildrenRecursively);
    cruiseText = main->findChild<QObject*>("cruiseText", Qt::FindChildrenRecursively);
    cruiseImage = main->findChild<QObject*>("cruiseImage", Qt::FindChildrenRecursively);
    setImage = main->findChild<QObject*>("setImage", Qt::FindChildrenRecursively);
    brakeImage = main->findChild<QObject*>("brakeImage", Qt::FindChildrenRecursively);
    celImage = main->findChild<QObject*>("celImage", Qt::FindChildrenRecursively);
    oilImage = main->findChild<QObject*>("oilImage", Qt::FindChildrenRecursively);
    seatbeltImage = main->findChild<QObject*>("seatbeltImage", Qt::FindChildrenRecursively);
    tpmsImage = main->findChild<QObject*>("tpmsImage", Qt::FindChildrenRecursively);
    doorImage = main->findChild<QObject*>("doorImage", Qt::FindChildrenRecursively);
    fuelImage = main->findChild<QObject*>("fuelImage", Qt::FindChildrenRecursively);

    //finds ui status elements to control. ex: turn signals
    leftSignal = main->findChild<QObject*>("leftSignal", Qt::FindChildrenRecursively);
    rightSignal = main->findChild<QObject*>("rightSignal", Qt::FindChildrenRecursively);
    lightIndicator = main->findChild<QObject*>("lightIndicator", Qt::FindChildrenRecursively);

    //finds ui can parameter objects
    clockText = main->findChild<QObject*>("clockText", Qt::FindChildrenRecursively);
    temperatureText = main->findChild<QObject*>("temperatureText", Qt::FindChildrenRecursively);
    fuelBar = main->findChild<QObject*>("fuelBarImage", Qt::FindChildrenRecursively);
    coolantGauge = main->findChild<QObject*>("coolantBarImage", Qt::FindChildrenRecursively);
    throttleBar = main->findChild<QObject*>("throttleBar", Qt::FindChildrenRecursively);
    boostGauge = main->findChild<QObject*>("boostGauge", Qt::FindChildrenRecursively);
    mpgText = main->findChild<QObject*>("mpgText", Qt::FindChildrenRecursively);
    sessionText = main->findChild<QObject*>("sessionText", Qt::FindChildrenRecursively);
    mpgBar = main->findChild<QObject*>("mpgBar", Qt::FindChildrenRecursively);
    sessionMPGGauge = main->findChild<QObject*>("sessionMPGGauge", Qt::FindChildrenRecursively);

    //finds ui parameter objects (from logger)
    statusRect = main->findChild<QObject*>("statusRect", Qt::FindChildrenRecursively);



    initUIElements();


    //set the gear value on startup
    geartext->setProperty("text", "N");


    //TODO: don't start requesting data until gauge sweep done
    sweepFinished = 0;
    if(initialGaugeSweep == 1){
        gaugeSweep();
    }


    //can info connects
    QObject::connect(_data, &canData::turnSignal, this, &gauges::updateTurnSignals);
    QObject::connect(_data, &canData::lights, this, &gauges::updateLights);
    QObject::connect(_data, &canData::reverseSwitch, this, &gauges::updateReverse);
    QObject::connect(_data, &canData::rpmChanged, this, &gauges::setRPMCAN);
    QObject::connect(_data, &canData::fuelChanged, this, &gauges::setFuelCAN);
    QObject::connect(_data, &canData::speedChanged, this, &gauges::setSpeedCAN);
    QObject::connect(_data, &canData::neutralSwitch, this, &gauges::updateNeutral);
    QObject::connect(_data, &canData::clutchSwitch, this, &gauges::updateClutch);
    QObject::connect(_data, &canData::handbrake, this, &gauges::updateHandbrake);
    QObject::connect(_data, &canData::cruiseStatusChanged, this, &gauges::updateCruiseStatus);
    QObject::connect(_data, &canData::gearChanged, this, &gauges::updateGearFromCAN);
    QObject::connect(_data, &canData::checkEngineLight, this, &gauges::showCEL);
    QObject::connect(_data, &canData::oilLight, this, &gauges::showOil);
    QObject::connect(_data, &canData::tpmsLight, this, &gauges::showTPMS);
    QObject::connect(_data, &canData::seatbeltLight, this, &gauges::showSeatbelt);
    QObject::connect(_data, &canData::doorLight, this, &gauges::showDoor);





    //fill trip & odo info and update display
    sessionStart = odoval;
    lastOdoval = odoval;
    updateCANParam("Odometer", odoval);
    handler->fillTrip(&trA, "tripA");
    handler->fillTrip(&trB, "tripB");
    qDebug() << "*trip A start: " << trA.getTripStart();
    qDebug() << "*trip B start: " << trB.getTripStart();
    qDebug() << "*odometer: " << odoval;
    qDebug() << "*activeTrip: " << activeTrip;
    tripNum->setProperty("text", getActiveTripNum());
    _fe->setOdometerStart(odoval);



    //start the clock
    updateClock();
    clockTimer.start();



    //test variables
    /*_speed=40;
    _rpm = 3000;
    currentGear = "3";
    QTimer::singleShot(3000, this, [this](){
        qDebug() << "clutch in";
        updateClutch("In");
    });*/



    QStringList paramList = _defWin->selectedParams();

    paramList.append("Oil Temp");



    QStringList filter;
    filter.append("Odometer");
    filter.append("Vehicle Speed");
    filter.append("Dynamic Advance Multiplier");
    filter.append("Maf Corr Final");

    QMap<QString, QString> rename;
    rename.insert("AF Learning 1", "AF Learning");
    rename.insert("Intake Manifold Temperature", "Intake Temp");
    rename.insert("Dynamic Advance Multiplier", "DAM");
    rename.insert("Feedback Knock Correction", "FBK");
    rename.insert("Fine Knock Learn", "FKL");
    _paramDisplay = new paramDisplay(statusRect, qmlEngine(_main), paramList);
    _paramDisplay->setFilterList(filter, 1);
    _paramDisplay->setParamRename(rename);



}



//updates the tach with can data
void gauges::setRPMCAN(uint rpm)
{

    if(sweepFinished == 1 && rpm >= 0 && rpm < 10000){
        //qDebug() << "_rpm: " << _rpm << " rpm: " << rpm;
        /*
        if(_rpm > 0 && ((rpm/_rpm) > 10 || (rpm/_rpm) < .1) && (_rpm) < .95*static_cast<quint8>(maxRPM) && !clutch){
            qDebug() << "returning: " << rpm/_rpm;
            return;
        }*/
        _rpm = rpm;
        _fe->setRpm(rpm);
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
            tachNeedle->setProperty("rotation", pos);
            /*
            QPropertyAnimation *rpmAnim = new QPropertyAnimation(tachNeedle, "rotation");
            rpmAnim->setDuration(animDuration/4);
            rpmAnim->setStartValue(currRPMPos);
            rpmAnim->setEndValue(pos);
            rpmAnim->start();*/
        }
        if(rpm < (uint)maxRPM){
            if(rpmtext){
                rpmtext->setProperty("text", rpm);
            }
        }
        emit rpmUpdated(rpm);
        refText->setProperty("visible", false);
        QTimer::singleShot(5, this, [this](){
            refText->setProperty("visible", true);
        });


        prevRPMPos = pos;
    } else {
    }
}


//updates the speed analog gauge from passive CAN read
void gauges::setSpeedCAN(double spd)
{
    _speed = spd;
    _fe->setSpeed(_speed);
    if(sweepFinished == 1){
        if(speedTime->isActive()){
            speedTime->setInterval(5);
        }
        int diff = 0;
        double percent = (double)_speed/maxSpeed;
        double pos = minSpeedoRot;
        if(maxSpeedoRot > minSpeedoRot){
            diff = maxSpeedoRot - minSpeedoRot;
            pos = minSpeedoRot + (percent*diff);
        } else {
            diff = (minSpeedoRot + maxSpeedoRot)-180;
            pos = (double)minSpeedoRot + (double)(percent*diff);
        }
        int speedDelta = _speed - prevSpeed;
        if(speedDelta != 0){
            int speedDuration = abs((20/(speedDelta))/2);
            speedTime->start(speedDuration);
            if(speedoNeedle){
                QPropertyAnimation *speedAnim = new QPropertyAnimation(speedoNeedle, "rotation");
                speedAnim->setDuration(animDuration);
                speedAnim->setStartValue(prevSpeedPos);
                speedAnim->setEndValue(pos);
                speedAnim->start();
            }
            prevSpeed = _speed;
        }
        prevSpeedPos = pos;
    }
}



void gauges::onShiftTimerTimeout()
{
   // qDebug() << "shift timer timeout";
    shiftTimer->stop();
   /* statustext->setProperty("text", "next gear");
    QTimer::singleShot(500, this, [this](){
        statustext->setProperty("text", "");
    });*/
    updateTargetShiftRPM(g->calcNextGearRPM(_speed));
}

//updates the weather readouts with new data
void gauges::updateWeatherStatus()
{
    _weather.requestWeather();
}

//updates the middle speed readout
void gauges::updateSpeedText(){
    if(_speed < maxSpeed){
        int s = speedtext->property("text").toInt(nullptr);
        if(s >= _speed){
            speedTime->stop();
            speedtext->setProperty("text", _speed);
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

    qDebug() << "*fkl index: " << fklIndex << "fbk index: " << fbkIndex << "dam index: " << damIndex;

    //QObject::connect(&par[rpmIndex], &parameter::valueChanged, this, &gauges::updateValue);





}

void gauges::switchActiveTrip(){
    if(activeTrip == "tripA"){
        activeTrip = "tripB";
    }else {
        activeTrip = "tripA";
    }
    qDebug() << "*active trip: " << activeTrip;
    emit tripSwapped(activeTrip);
    tripNum->setProperty("text", getActiveTripNum());
}
//resets the given trip
void gauges::resetTrip(QString tr) //TODO: multiple trips
{
    qDebug()<< "*reset trip: " << tr;
    if(tr == "tripA"){
        trA.resetTrip(odoval);
        tripNum->setProperty("text", getActiveTripNum());
        emit tripUpdated("tripA", QString::number(odoval));
    } else if (tr == "tripB"){
        trB.resetTrip(odoval);
        tripNum->setProperty("text", getActiveTripNum());
        emit tripUpdated("tripB", QString::number(odoval));
    }

}

//sets the shift threshold varable from settings window
void gauges::setShiftThreshold(QString val)
{
    shiftLightMin = val.toInt(nullptr, 10);
    emit shiftThresholdChanged(val);
}

//toggles if throttle bar visible from settings window
void gauges::toggleThrottleBar()
{
    qDebug() << "*toggle throttle bar: ";
    if(throttleBar->property("visible") == QVariant(true)){
        throttleBar->setProperty("visible", false);
    } else {
        throttleBar->setProperty("visible", true);
    }
}

void gauges::setFuelCAN(float fuel)
{
    //qDebug() << "resistance: " << fuel;
    double value;
    int diff = fuelResMin-fuelResMax;
    value = (double)(fuel-fuelResMax)/(double)diff;
    //qDebug() << "value: " << value;
    value = 1-value;
    if(value < 0){
        value = 0;
    }
    if(value > 1){
        value = 1;
    }
    //qDebug() << "fuel percent: " << value;
    updateFuelBar(value);
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

//gets/updates mileage of active trip
QString gauges::getActiveTripNum(){
    if(activeTrip == "tripA"){
       return trA.getTrip(odoval);
    } else {
        return trB.getTrip(odoval);
    }

}




//updates the trip value in cluster
void gauges::updateTrip()
{
    tripNum->setProperty("text", getActiveTripNum());
}

//displays knock message if there is a knock event (fbk/fkl/dam not normal)
void gauges::showKnock()
{
    /*
    if(par[fklIndex].getValue() == 0 && par[fbkIndex].getValue() == 0 && par[damIndex].getValue() == 1){
        //statustext->setProperty("text", "");
        //statustext->setProperty("visible", false);
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

    } */
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

//used to update display for AP params
void gauges::updateParamDisplay(QString name, double value)
{

    if(name == "Maf Corr Final"){
        _fe->setMafCorr(value);
    } else if(name == "AF Ratio") {
        _fe->setAFR(value);
    }

    //mpgText->setProperty("text", QString::number(_fe->getMpg(), 'f', 1));
    _paramDisplay->updateValue(name, value);



}

//updates gauges with can values
void gauges::updateCANParam(QString name, double value)
{
    if (name == "Oil Temp"){
        _paramDisplay->updateValue(name, value);
    } else if (name == "Coolant Temp"){
        updateCoolantGauge(value);
    } else if (name == "Cruise Speed"){
        updateCruiseSpeed(value);
    } else if (name == "Ambient Temp"){
        QString tr = QString::number(value) + " F";
        temperatureText->setProperty("text", tr);
    } else if (name == "Boost"){
        double boost = value + 15;
        //qDebug() << "boost val: " << boost;
        boost = boost/(double)40;
        //qDebug() << "boost val: " << boost;
        boostGauge->setProperty("text", QString::number(value, 'f', 1));
        boostGauge->setProperty("value", QVariant(boost*100));
    }else if (name == "Accelerator Position"){
        accelPos = qRound(value);
        throttleBar->setProperty("value", QVariant(value/100));
        if(shiftTimer->isActive() && value > 25){
            shiftTimer->stop();
            //qDebug() << "downshift";

            updateTargetShiftRPM(g->calcDownshiftRPM(_speed));
        }
    } else if(name == "Odometer"){
        odoval = value;

        if(odoval > lastOdoval){
            lastOdoval = odoval;
            if(activeTrip == "tripA"){
                _fe->updateTripMPG(&trA);
                qDebug() << "A mpg: " << trA.getTripMPG();
            } else {
                _fe->updateTripMPG(&trB);
                qDebug() << "B mpg: " << trB.getTripMPG();
            }
        }
        updateTrip();

        QString val = QString::number(qFloor(value), 'f', 0);
        if(val.length() < 6){
            for(int x = val.length(); x<6; x++){
                val.prepend("0");
            }
        }
        if(val != odotext->property("text").toString()){
            emit odometerUpdated(QString::number(value,'f',1));
            int ind = val.indexOf("-");
            if(ind > -1){
                val.remove(ind,1);
            }
            odotext->setProperty("text", QVariant(val));
        }
    } else if (name == "Fuel Consumption" && _rpm > 0){
        if(_speed > 0 && value > 0){
            _fe->setInstantMPGFromCAN(_speed/value);

        } else if (_speed > 0){
            _fe->setInstantMPGFromCAN(45);
        } else {
            _fe->setInstantMPGFromCAN(0);
            _fe->updateMPGNotMoving();

        }
        //qDebug() << _fe->getInstantMPGFromCAN();
        if(_fe->getMpg() > 0){
            mpgText->setProperty("text", QString::number(_fe->getMpg(), 'f', 1));
            mpgBar->setProperty("value", QVariant((_fe->getMpg()/76)));
        } else {
            mpgText->setProperty("text", "0.0");
            mpgBar->setProperty("value", QVariant((0)));
        }
        sessionMPGGauge->setProperty("value", QVariant(_fe->getSessionAvg()));
        sessionMPGGauge->setProperty("text", QString::number(_fe->getSessionAvg(), 'f', 1));
    }
}

//changes the outside temperature readout (deprecated)
void gauges::updateTemperatureText(QString t)
{

   QString tr = t + " F";
   //temperatureText->setProperty("text", tr);
   //weatherTimer.start(600000);

}

//updates the coolant gauge given temperature value
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

//averages fuel readings to prevent gauge jumpiness
double gauges::getFuelAvg(double value)
{
    if(fuelSamples < 500){
        fuelSamples++;
        if(fuelSamples == 500){
            fuelLevelStart = fuelValueAvg;
            if(fuelLevelStart > 0){
                qDebug() << "*setting start fuel level: " << fuelValueAvg;
                _fe->setTankLevelStart(fuelValueAvg);
                storedSample = true;
            } else {
                storedSample = false;
                fuelSamples = 1;
            }
        }
    }

    //qDebug() << "fuel avg: " << (fuelValueAvg + ((double)1/(double)fuelSamples)*(double)((double)value-(double)fuelValueAvg));
    fuelValueAvg = (fuelValueAvg + ((double)1/(double)fuelSamples)*(double)((double)value-(double)fuelValueAvg));
    return fuelValueAvg;
}

//updates the fuel gauge given percent fuel
void gauges::updateFuelBar(double value)
{
    value = getFuelAvg(value);
    int bar = 0;
    //QString text = QString::number(value);
    //fuelText->setProperty("text", text);
    if(fuelBar){
        if(value < 0.15){
            fuelImage->setProperty("visible", true);
        } else {
            fuelImage->setProperty("visible", false);
        }

        if(value > 0.99){
            _fe->setOdometerTankStart(odoval);
        }
    QString filePath = "fuelBar";
    if(value < .05){
        bar = 0;
    } else if(value <.10){
        bar = 1;
    } else if(value <.25){
        bar = 2;
    } else if (value <.4) {
        bar = 3;
    } else if (value <.55){
        bar = 4;
    } else if (value <.7){
        bar = 5;
    } else if (value <.85){
        bar = 6;
    } else {
        bar = 7;
    }

    if(bar < fuelBarMin){
        fuelBarMin = bar;
    }

    filePath.append(QString::number(bar));
    if(filePath != fuelFilePath){
        fuelBar->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/" + filePath + ".png");
        fuelFilePath = filePath;
    }
    }
}

//paints the shift target rpm marker
void gauges::updateTargetShiftRPM(uint rpm)
{
   // qDebug() << "shift target call";
    if(rpm > 1200 && currentGear != "N" && currentGear != "R" && _speed > 10){
        /*
        clutchText->setProperty("text", "updateTarget");
        clutchText->setProperty("visible", true);
        QTimer::singleShot(500, this, [this](){
            clutchText->setProperty("text", "clutch");
            clutchText->setProperty("visible", false);
        });*/
        double percent = (double)(rpm)/maxRPM;
        double pos;
        int diff = 0;

        if(maxTach > minTach){ //TODO: implement this correctly
            diff = minTach - maxTach;
            pos = minTach + (percent*diff);

        } else {
            diff = (minTach + maxTach)-180;
            pos = (double)minTach + (double)(percent*diff);
        }

        if(shiftTargetRect){
            if(shiftTargetRect->property("visible")==false){
                //qDebug() << "not visible rn";
                shiftTargetRect->setProperty("rotation", pos);
                shiftTargetRect->setProperty("opacity", 0);
                shiftTargetRect->setProperty("visible", true);
                QPropertyAnimation *anim = new QPropertyAnimation(shiftTargetRect, "opacity");
                anim->setDuration(100);
                anim->setStartValue(0);
                anim->setEndValue(1.0);
                anim->start();
                QTimer::singleShot(1350, this, [this](){ //fadeout animation
                    QPropertyAnimation *anim = new QPropertyAnimation(shiftTargetRect, "opacity");
                    anim->setDuration(100);
                    anim->setStartValue(1.0);
                    anim->setEndValue(0);
                    anim->start();
                });
                QTimer::singleShot(1500, this, [this](){
                   shiftTargetRect->setProperty("visible", false);
                });
            }
        }
    }
}

void gauges::updateCruiseStatus(QString status)
{
    //qDebug() << "cruise: " << status;
    if(status == "off"){
        cruiseEnabled = 0;
        cruiseActive = 0;
        cruiseImage->setProperty("visible", false);
        setImage->setProperty("visible", false);
    } else if(status == "on"){
        cruiseEnabled = 1;
        cruiseImage->setProperty("visible", true);
    } else if(status == "active"){
        cruiseEnabled = 1;
        cruiseActive = 1;
        cruiseImage->setProperty("visible", true);
        setImage->setProperty("visible", true);
    } else if(status == "inactive"){
        cruiseActive = 0;
        setImage->setProperty("visible", false);
    }
}

void gauges::updateCruiseSpeed(double speed)
{
    cruiseSpeed = speed;
    if(cruiseText){
        cruiseText->setProperty("text", QString::number(cruiseSpeed));
        if(cruiseEnabled == 1){
            cruiseText->setProperty("visible", true);
        } else {
            cruiseText->setProperty("visible", false);
        }
    }
}

void gauges::updateGearFromCAN(QString status)
{
    //qDebug() << "status: " << status;
    g->setCurrentGear(status);
    if(status == "N" || status == "0"){
        status = "N";
        neutral = true;
        //qDebug() << "neutral";
    } else {
        neutral = false;
    }
    if(!reverse && !clutch){
        geartext->setProperty("text", status);
    }
}

void gauges::showCEL(QString status)
{
    if(status == "cel"){
        celImage->setProperty("visible", true);
    } else {
        celImage->setProperty("visible", false);
    }
}

void gauges::showTPMS(QString status)
{
    if(status == "Warning"){
        tpmsImage->setProperty("visible", true);
    } else {
        tpmsImage->setProperty("visible", false);
    }
}

void gauges::showSeatbelt(QString status)
{
    if(status == "Unbuckled"){
        seatbeltImage->setProperty("visible", true);
    } else {
        seatbeltImage->setProperty("visible", false);
    }
}

void gauges::showOil(QString status)
{
    if(status == "Warning"){
        oilImage->setProperty("visible", true);
    } else {
        oilImage->setProperty("visible", false);
    }
}

void gauges::showDoor(QString status)
{
    if(status == "driver"){
        driverDoor = true;
    } else if (status == "driverClosed"){
        driverDoor = false;
    } else if(status == "passenger"){
        passengerDoor = true;
    } else if (status == "passengerClosed"){
        passengerDoor = false;
    } else if(status == "rearPassenger"){
        rearPassengerDoor = true;
    } else if (status == "rearPassengerClosed"){
        rearPassengerDoor = false;
    } else if(status == "rearDriver"){
        rearDriverDoor = true;
    } else if (status == "rearDriverClosed"){
        rearDriverDoor = false;
    } else if(status == "trunk"){
        trunkDoor = true;
    } else if (status == "trunkClosed"){
        trunkDoor = false;
    }
    if(driverDoor || passengerDoor || rearPassengerDoor || rearDriverDoor || trunkDoor){
        doorImage->setProperty("visible", true);
    } else {
        doorImage->setProperty("visible", false);
    }
}

void gauges::redrawParamDisplay()
{

    QStringList params = _defWin->selectedParams();
    params.append("Oil Temp");
    QStringList filter;
    filter.append("Odometer");
    filter.append("Vehicle Speed");
    filter.append("Dynamic Advance Multiplier");
    filter.append("Maf Corr Final");

    QMap<QString, QString> rename;
    rename.insert("AF Learning 1", "AF Learning");
    rename.insert("Intake Manifold Temperature", "Intake Temp");
    rename.insert("Dynamic Advance Multiplier", "DAM");
    rename.insert("Feedback Knock Correction", "FBK");
    rename.insert("Fine Knock Learn", "FKL");
    _paramDisplay->setFilterList(filter, 0);
    _paramDisplay->setParamList(params, 1);
    _paramDisplay->setParamRename(rename);
}


//controls the shift light flashing
void gauges::flashShiftLight(uint rpm)
{
    if(rpm >= shiftLightMin){
        if(!shiftLightTimer.isActive()){
            shiftLight->setProperty("visible", true);
            shiftLightTimer.start(shiftLightFlashInterval);
        }
    } else {
        shiftLightTimer.stop();
        shiftLight->setProperty("visible", false);
    }
}

//toggles shift light visibility
void gauges::showShiftLight()
{
    if(shiftLight->property("visible") == true){
        shiftLight->setProperty("visible", false);
    } else {
        shiftLight->setProperty("visible", true);
    }
}

//get the shift light rpm setting
QString gauges::getShiftThreshold()
{
    return QString::number(shiftLightMin, 10);
}
//get the shift light flash interval
QString gauges::getShiftTimer()
{
    return QString::number(shiftLightFlashInterval, 10);
}

//fades in the param display after gauge sweep
void gauges::fadeInGauges()
{
    qDebug() << "*fading in ui elements";



    if(throttleBar){
        QPropertyAnimation *speedAnim = new QPropertyAnimation(throttleBar, "opacity");
        speedAnim->setDuration(500);
        speedAnim->setStartValue(0);
        speedAnim->setEndValue(1.0);
        speedAnim->start();
    }
    if(mpgBar){
        QPropertyAnimation *speedAnim = new QPropertyAnimation(mpgBar, "opacity");
        speedAnim->setDuration(500);
        speedAnim->setStartValue(0);
        speedAnim->setEndValue(1.0);
        speedAnim->start();
    }

    if(temperatureText){
        QPropertyAnimation *speedAnim = new QPropertyAnimation(temperatureText, "opacity");
        speedAnim->setDuration(500);
        speedAnim->setStartValue(0);
        speedAnim->setEndValue(1.0);
        speedAnim->start();
    }

    if(speedtext){
        QPropertyAnimation *speedAnim = new QPropertyAnimation(speedtext, "opacity");
        speedAnim->setDuration(500);
        speedAnim->setStartValue(0);
        speedAnim->setEndValue(1.0);
        speedAnim->start();
    }
    if(boostGauge){
        QPropertyAnimation *speedAnim = new QPropertyAnimation(boostGauge, "opacity");
        speedAnim->setDuration(500);
        speedAnim->setStartValue(0);
        speedAnim->setEndValue(1.0);
        speedAnim->start();
    }
    if(sessionMPGGauge){
        QPropertyAnimation *speedAnim = new QPropertyAnimation(sessionMPGGauge, "opacity");
        speedAnim->setDuration(500);
        speedAnim->setStartValue(0);
        speedAnim->setEndValue(1.0);
        speedAnim->start();
    }

}

//calls functions after gauge sweep (broken)
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

//controls the animations for ui start (but not after gauge sweep)
void gauges::initUIElements()
{

    if(geartext){
        QPropertyAnimation *speedAnim = new QPropertyAnimation(geartext, "opacity");
        speedAnim->setDuration(750);
        speedAnim->setStartValue(0);
        speedAnim->setEndValue(1.0);
        speedAnim->start();
    }

    if(odotext){
        QPropertyAnimation *speedAnim = new QPropertyAnimation(odotext, "opacity");
        speedAnim->setDuration(500);
        speedAnim->setStartValue(0);
        speedAnim->setEndValue(1.0);
        speedAnim->start();
    }

    if(coolantGauge){
        QPropertyAnimation *speedAnim = new QPropertyAnimation(coolantGauge, "opacity");
        speedAnim->setDuration(500);
        speedAnim->setStartValue(0);
        speedAnim->setEndValue(1.0);
        speedAnim->start();
    }


}



//updates the light indicator based on light status
void gauges::updateLights(QString status)
{
    if(status == "parkOff"){
        park = false;
    } else if (status == "park"){
        park = true;
    } else if (status == "off"){
        lights = false;
    } else if (status == "on"){
        lights = true;
        park = true;
    } else if (status == "flashOff"){
        flash = false;
    } else if (status == "flash"){
        flash = true;
    }


    if(!flash){
        lightIndicator->setProperty("visible", park);
        lightIndicator->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/lightsOn.svg");
        lightIndicator->setProperty("showBlue", false);

    } else {
         lightIndicator->setProperty("visible", true);
         lightIndicator->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/highbeam.png");
         lightIndicator->setProperty("showBlue", true);
    }

    if(park && !lights){
        lightIndicator->setProperty("opacity", 0.7);
    } else {
        lightIndicator->setProperty("opacity", 1.0);
    }

}

//shows which turn signals are on/off
void gauges::updateTurnSignals(QString status)
{
    if(status == "left"){
        leftSignalBool = true;
    } else if (status == "leftOff"){
        leftSignalBool = false;
    } else if (status == "right"){
        rightSignalBool= true;
    } else if (status == "rightOff"){
        rightSignalBool = false;
    }

    leftSignal->setProperty("visible", leftSignalBool);
    rightSignal->setProperty("visible", rightSignalBool);

}

//sets gear indicator to N when in neutral
void gauges::updateNeutral(QString status)
{

}

//sets gear indicator to R when in reverse
void gauges::updateReverse(QString status)
{
    if(status == "Reverse"){
        reverse = true;
        //qDebug() << "reverse";
        geartext->setProperty("text", "R");
    } else {
        reverse = false;
    }
}

void gauges::updateClutch(QString status)
{
    if(status == "In"){
        if(!changeGear){
            changeGear = true;
            if(!shiftTimer->isActive()){
                shiftTimer->start(shiftTimerDuration);
            }
        }
        //qDebug() << "clutch in";
        clutch = true;
        //clutchText->setProperty("visible", true);

    } else {
        clutch = false;
        changeGear = false;
        //clutchText->setProperty("visible", false);
        shiftTimer->stop();
    }
}

void gauges::updateHandbrake(QString status)
{
    if(status == "engaged"){
        handbrake = true;
    } else {
        handbrake = false;
    }

    if(brakeImage){
        brakeImage->setProperty("visible", handbrake);
    }
}


//startup gauge sweep controller
void gauges::gaugeSweep()
{
    sweepForward();
}


//possibly deprecated
void gauges::startTimer(){
    qDebug() << "gauges::startTimer called";
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
        par[fbkIndex].setValue(-1.41);
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
    /*
    int digits = 6;
    QString str = QString::number(val);
    int length = str.length();
    if(length < digits){
        int mult = digits-length;
        for(int i = 0; i<mult; i++){
            str.prepend("0");
        }
    }
    odotext->setProperty("text", (QVariant)str);*/
}

//DEPRECATED: updates all gauges based on ecu response data
void gauges::updateValue()
{
    if(1){
        setOdometer();
        if(!neutral && !reverse){
        }
        updateTrip();
        showKnock();
    }
}

//controls the test sweep
void gauges::changeValues(){

    if(rpmval < maxRPM){
    rpmval = rpmval + 100;
    } else {
        rpmval = 0;
    }

    if(speedval < maxSpeed-2){
        speedval = speedval + 2;
    } else {
        speedval = 0;
    }

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

    if(fbkIndex > 0){
        updateParamDisplay("Feedback Knock Correction", par[fbkIndex].getValue());
    }
    par[fklIndex].setValue(0);
    par[damIndex].setValue(1.0);
    odoval = odoval + 2;


}
