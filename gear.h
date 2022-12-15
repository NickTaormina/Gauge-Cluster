#ifndef GEAR_H
#define GEAR_H

#include <QObject>
#include "parameter.h"
#include <QtXml>
#include <QString>
#include <math.h>

class gear : public QObject
{
    Q_OBJECT
public:
    explicit gear(QObject *parent = nullptr);


signals:
public slots:
    QString calcGear(int rpm, int speed);
    QList<float> getRatios();
    float getFinalDrive();
    float getDiameter();
    void setRatios(QList<float> r);
    void setRatioR(float r);
    void setFinalDrive(float fd);
    void setTireDiameter(float td);
    uint calcNextGearRPM(int speed);
    uint calcDownshiftRPM(int speed);
private:

    QList<float> ratios;
    float ratioR;
    float finalDrive;
    float tireDiameter;
    float circumference;
    int numGears;
    int currentGear;

};

#endif // GEAR_H
