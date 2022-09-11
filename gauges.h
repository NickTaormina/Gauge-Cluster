#ifndef GAUGES_H
#define GAUGES_H

#include <QObject>
#include <QQmlContext>
#include "parameter.h"
#include <QTimer>
#include "gear.h"
#include "trip.h"
#include "config.h"

class gauges : public QObject
{
    Q_OBJECT
public:
    explicit gauges(QObject *parent = nullptr);
    gauges(QObject *parent, QObject * main, gear* gear, trip* tr, config* cfg);

signals:
void tripUpdated(QString trip, QString val);
public slots:
    void setRPM();
    void setSpeed();
    Q_INVOKABLE void setParamPointer(parameter * parameter, int length);
    Q_INVOKABLE void resetTrip();
    void startTimer();
    void updateValue();
    void startTest();
    void setOdometer();
    void gaugeSweep();
    void sweepForward();
    void sweepBack();

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



    //ui objects
    QObject * tachNeedle;
    QObject * speedoNeedle;
    QObject * fuelNeedle;
    QObject * rpmtext;
    QObject * speedtext;
    QObject * odotext;
    QObject * geartext;
    QObject * tripNum;
    QObject * statustext;

    //timers
    QTimer* timer;
    QTimer* testtimer;
    QTimer* speedTime;
    QTimer* sweepTimer;
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



    //class objects
    gear* g;
    trip* _trip;
signals:
    void sweepDone();
};

#endif // GAUGES_H
