#ifndef GAUGES_H
#define GAUGES_H

#include <QObject>
#include <QQmlContext>
#include "parameter.h"
#include <QTimer>

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
    parameter * par;
    int paramLength;
    int currRPM;
    int currSpeed;
    int speed;
    double currRPMPos;
    double currSpeedPos;

    void findSpeedIndex();
    void findRPMIndex();

    QObject * tachNeedle;
    QObject * speedoNeedle;
    QObject * fuelNeedle;
    QObject * rpmtext;
    QObject * speedtext;
    QTimer* timer;
    QTimer* testtimer;
    QTimer* speedTime;

    int rpmval;
    int speedval;
    void changeValues();
    void updateSpeedText();
    int animDuration;
};

#endif // GAUGES_H
