#include "gauges.h"
#include <QtQuick>

gauges::gauges(QObject *parent)
    : QObject{parent}
{
}

gauges::gauges(QObject *parent, QObject * main, gear* gear, config* cfg, configHandler *handler, canData *data, defWindow * defwin)
{
    //store objects
    parent = nullptr;
    g = gear;
    _data = data;
    handbrake = false;


    //timers
    timer = new QTimer(this);
    testtimer = new QTimer(this);
    speedTime = new QTimer(this);
    sweepTimer = new QTimer(this);
    shiftTimer = new QTimer(this);
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
    odoval = cfg->getValue("generalOdometer").toInt(nullptr, 10);
    activeTrip = cfg->getValue("generalActiveTrip");
    shiftTimerDuration = cfg->getValue("generalShiftTargetTimer").toInt(nullptr, 10);
    qDebug() << "shift timer duration: " << shiftTimerDuration;

    fuelResMin = 383;
    fuelResMax = 19;

    fuelBarMin = 7;

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





    //fill trip & odo info and update display
    handler->fillTrip(&trA, "tripA");
    handler->fillTrip(&trB, "tripB");
    qDebug() << "*trip A: " << trA.getTrip();
    qDebug() << "*trip B: " << trB.getTrip();
    qDebug() << "*odometer: " << odoval;
    qDebug() << "*activeTrip: " << activeTrip;
    tripNum->setProperty("text", getActiveTripNum());
    updateParamDisplay("Odometer", (double)odoval);


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



    QStringList paramList = defwin->selectedParams();

        paramList.append("Oil Temp");


        QStringList filter;
        filter.append("Odometer");
        filter.append("Vehicle Speed");
        filter.append("Dynamic Advance Multiplier");

        QMap<QString, QString> rename;
        rename.insert("AF Learning 1", "AF Learning");
        rename.insert("Intake Manifold Temperature", "Intake Temp");
        rename.insert("Dynamic Advance Multiplier", "DAM");
        rename.insert("Feedback Knock Correction", "FBK");
        rename.insert("Fine Knock Learn", "FKL");
    _paramDisplay = new paramDisplay(statusRect, qmlEngine(main), paramList);
        _paramDisplay->setFilterList(filter, 0);
        _paramDisplay->initDisplay();
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
        updateTrip();
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
    qDebug() << "*active trip: " << activeTrip;
    emit tripSwapped(activeTrip);
    tripNum->setProperty("text", getActiveTripNum());
}
//resets the given trip
void gauges::resetTrip(QString tr) //TODO: multiple trips
{
    qDebug()<< "*reset trip: " << tr;
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
        if(!clutch && !neutral && !reverse){
            currentGear = g->calcGear(_rpm, _speed);
            //qInfo() << "update gear: " << _rpm << " : " << speed;
            geartext->setProperty("text", currentGear);
        } else if (neutral && !reverse){
            geartext->setProperty("text", "N");
        }


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
    //qDebug() << "*speed: " << _speed << " elapsed time: " << elapsedTimer.elapsed();
    updateActiveTripDistance(_speed, elapsedTimer.elapsed());
    elapsedTimer.restart();
    tripNum->setProperty("text", getActiveTripNum());
    //qDebug() << activeTrip << ": " << getActiveTripNum();
    emit tripUpdated(activeTrip, getActiveTripNum());



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

    if(name == "Odometer"){
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
       } else {
           _paramDisplay->updateValue(name, value);
       }


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
    } else if (name == "Accelerator Position"){
        accelPos = qRound(value);
        throttleBar->setProperty("value", QVariant(value/100));
        if(shiftTimer->isActive() && value > 25){
            shiftTimer->stop();
            //qDebug() << "downshift";

            updateTargetShiftRPM(g->calcDownshiftRPM(_speed));
        }
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
    if(fuelSamples > 100){
        fuelSamples = 1;
        return fuelValueAvg;
    }
    fuelSamples = fuelSamples + 1;
    return (fuelValueAvg + (1/fuelSamples)*(value-fuelValueAvg));
}

//updates the fuel gauge given percent fuel
void gauges::updateFuelBar(double value)
{
    value = getFuelAvg(value);
    int bar = 0;
    //QString text = QString::number(value);
    //fuelText->setProperty("text", text);
    if(fuelBar){
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

    filePath.append(QString::number(fuelBarMin));
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
    if(status == "off"){
        cruiseEnabled = 0;
        cruiseActive = 0;
        cruiseImage->setProperty("visible", false);
        setImage->setProperty("visible", false);
    } else if(status == "cancel"){
        cruiseEnabled = 1;
        cruiseActive = 0;
        cruiseImage->setProperty("visible", true);
        setImage->setProperty("visible", false);
    } else if(status == "active"){
        cruiseEnabled = 1;
        cruiseActive = 1;
        cruiseImage->setProperty("visible", true);
        setImage->setProperty("visible", true);
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
    if(status == "on"){
        //icon visible
        lightIndicator->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/lightsOn.svg");
        lightIndicator->setProperty("opacity", 1.0);
    } else if (status == "park"){
        //icon visible (dimmed)
        lightIndicator->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/lightsOn.svg");
        lightIndicator->setProperty("opacity", 0.7);
    } else if (status == "flash" || status == "toggle") {
        lightIndicator->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/highbeam.png");
        lightIndicator->setProperty("opacity", 1.0);
    } else{
        //off: icon not visible
        lightIndicator->setProperty("source", "file:///" + QCoreApplication::applicationDirPath() + "/resources/images/lightsOff.svg");
    }
}

//shows which turn signals are on/off
void gauges::updateTurnSignals(QString status)
{
    if(status == "left"){
        leftSignal->setProperty("visible", true);
        rightSignal->setProperty("visible", false);
    } else if (status == "right"){
        leftSignal->setProperty("visible", false);
        rightSignal->setProperty("visible", true);
    } else if (status == "hazard"){
        leftSignal->setProperty("visible", true);
        rightSignal->setProperty("visible", true);
    } else{
        //off
        leftSignal->setProperty("visible", false);
        rightSignal->setProperty("visible", false);
    }
}

//sets gear indicator to N when in neutral
void gauges::updateNeutral(QString status)
{
    if(status == "Neutral"){
        neutral = true;
        //qDebug() << QDateTime::currentDateTime().toString("mm.ss") + ": neutral";
        if(neutralText){
            /*
            neutralText->setProperty("visible", true);
            neutralText->setProperty("text", "Neutral");
            neutralText->setProperty("opacity", 1.0);*/
        }
        if(!reverse){
            geartext->setProperty("text", "N");
            return;
        }

    } else {
        //qDebug() << QDateTime::currentDateTime().toString("mm.ss") + ": not";
        neutral = false;
        if(neutralText){
            /*
        neutralText->setProperty("visible", false);
        neutralText->setProperty("text", "");
        neutralText->setProperty("opacity", 0);*/
        }
        updateGear();
    }
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
        updateGear();
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

//DEPRECATED: updates all gauges based on ecu response data
void gauges::updateValue()
{
    if(speedIndex > -1){
        setOdometer();
        if(!neutral && !reverse){
            updateGear();
        }
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

    if(fbkIndex > 0){
        updateParamDisplay("Feedback Knock Correction", par[fbkIndex].getValue());
    }
    par[fklIndex].setValue(0);
    par[damIndex].setValue(1.0);
    odoval = odoval + 2;


}
