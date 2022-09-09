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
    gear(QObject *parent, QString path);

signals:
public slots:
    QString calcGear(int rpm, int speed);
    QList<float> getRatios();
    float getFinalDrive();
    float getDiameter();
    void setRatios(QList<float> ratios);
    void setRatioR(float r);
    void setFinalDrive(float fd);
    void setTireDiameter(float td);
private:
    QDomDocument xml;
    QList<float> ratios;
    float ratioR;
    float finalDrive;
    float tireDiameter;


    int numGears;

    void parseRatios();

};

#endif // GEAR_H
