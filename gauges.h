#ifndef GAUGES_H
#define GAUGES_H

#include <QObject>
#include <QQmlContext>
#include "parameter.h"
#include <QTimer>
#include "gear.h"

class gauges : public QObject
{
    Q_OBJECT
public:
    explicit gauges(QObject *parent = nullptr);

    gauges(QObject *parent, QObject * main);

signals:

public slots:
    void setRPM();
    void setSpeed();
    Q_INVOKABLE void setParamPointer(parameter * parameter, int length);
    void startTimer();
    void updateValue();
    void startTest();
    void setOdometer();

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

    void findSpeedIndex();
    void findRPMIndex();
    void findOdoIndex();

    QObject * tachNeedle;
    QObject * speedoNeedle;
    QObject * fuelNeedle;
    QObject * rpmtext;
    QObject * speedtext;
    QObject * odotext;
    QTimer* timer;
    QTimer* testtimer;
    QTimer* speedTime;

    int rpmval;
    int odoval;
    int speedval;
    void changeValues();
    void updateSpeedText();
    int animDuration;
    gear* g;
};

#endif // GAUGES_H
