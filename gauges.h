#ifndef GAUGES_H
#define GAUGES_H

#include <QObject>
#include <QQmlContext>
#include "parameter.h"
#include <QTimer>
#include "gear.h"
#include "trip.h"
#include "config.h"
#include "candata.h"
#include <QtMath>
#include <QDateTime>
#include "weather.h"
#include "confighandler.h"

class gauges : public QObject
{
    Q_OBJECT
public:
    explicit gauges(QObject *parent = nullptr);
    gauges(QObject *parent, QObject * main, gear* gear, trip* tr, config* cfg, configHandler *handler, canData *data);

signals:
void tripUpdated(QString trip, QString val);
void tripSwapped(QString trip);
public slots:
    void setRPM();
    void setRPMCAN(uint rpm);
    void setSpeed();
    Q_INVOKABLE void setParamPointer(parameter * parameter, int length);
    Q_INVOKABLE void resetTrip(QString tr);
    void startTimer();
    void updateValue();
    void startTest();
    void setOdometer();
    void gaugeSweep();
    void sweepForward();
    void sweepBack();
    void updateParamDisplay(QString name, double value);
    void updateTemperatureText(QString t);

    //can indicators
    void updateLights(QString status);
    void updateTurnSignals(QString status);
    void updateNeutral(QString status);
    void updateReverse(QString status);
    void setSpeedCAN(double speed);

    void updateWeatherStatus();
    QString getActiveTripNum();
    void updateActiveTripDistance(int speed, qint64 time);
    Q_INVOKABLE void switchActiveTrip();
private:

    int minRPM;
    int maxRPM;
    int maxTach;
    int minTach;
    int minSpeed;
    int maxSpeed;
    int maxSpeedoRot;
    int minSpeedoRot;
    int rpmIndex;
    int speedIndex;
    int odoIndex;
    parameter * par;
    int paramLength;
    int currRPM;
    int currSpeed;
    int speed;
    int _rpm;
    double currRPMPos;
    double currSpeedPos;

    int fbkIndex;
    int fklIndex;
    int damIndex;

    void findSpeedIndex();
    void findRPMIndex();
    void findOdoIndex();
    void updateGear();
    void getKnockIndexes();

    void updateTrip();
    void showKnock();

    void sweepDone();
    void updateClock();


    //parameter display
    QString topLeftText;
    QString topLeftVal;
    QString topRightText;
    QString topRightVal;
    QString bottomRightText;
    QString bottomRightVal;
    QString bottomLeftText;
    QString bottomLeftVal;

    //ui gauge objects
    QObject * tachNeedle;
    QObject * speedoNeedle;
    QObject * fuelNeedle;
    QObject * rpmtext;
    QObject * speedtext;
    QObject * odotext;
    QObject * geartext;
    QObject * tripNum;
    QObject * statustext;

    //ui status objects
    QObject * leftSignal;
    QObject * rightSignal;
    QObject * lightIndicator;

    //ui parameter objects
    QObject * topLeftLabel;
    QObject * topLeftValue;
    QObject * topRightLabel;
    QObject * topRightValue;
    QObject * bottomRightLabel;
    QObject * bottomRightValue;
    QObject * bottomLeftLabel;
    QObject * bottomLeftValue;
    QObject * clockText;
    QObject * temperatureText;

    //timers
    QTimer* timer;
    QTimer* testtimer;
    QTimer* speedTime;
    QTimer* sweepTimer;
    QTimer weatherTimer;
    QElapsedTimer elapsedTimer;
    qint64 elapsed;

    int rpmval;
    int odoval;
    int speedval;
    void changeValues();
    void updateSpeedText();
    int animDuration;
    int sweepFinished;

    QString activeTrip;

    //booleans
    int showAllSpeedNumbers;
    int initialGaugeSweep;


    QDateTime systime;
    //class objects
    gear* g;
    trip* _trip;

    trip trA;
    trip trB;
    //configHandler * _cfgHand;
    canData * _data;
    weather _weather;
signals:
};

#endif // GAUGES_H
