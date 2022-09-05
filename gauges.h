#ifndef GAUGES_H
#define GAUGES_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "parameter.h"
#include <QTimer>

class gauges : public QObject
{
    Q_OBJECT
public:
    explicit gauges(QObject *parent = nullptr);

    gauges(QObject *parent, QQmlApplicationEngine *engine);

signals:

public slots:
    void setRPM();
    void setSpeed();
    Q_INVOKABLE void setParamPointer(parameter * parameter, int length);
    void startTimer();

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
    double currRPMPos;
    double currSpeedPos;

    void findSpeedIndex();
    void findRPMIndex();

    QObject * tachNeedle;
    QObject * speedoNeedle;
    QObject * fuelNeedle;
    QTimer* testtimer;

    int rpmval;
    int speedval;
    void changeValues();
};

#endif // GAUGES_H
