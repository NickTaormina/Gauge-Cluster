#include "gear.h"
#include <QDebug>

gear::gear(QObject *parent)
    : QObject{parent}
{

    numGears = 6;
    circumference = 1;
    currentGear = 3;

}



QString gear::calcGear(int rpm, int speed)
{

    if(ratios.length()>0){
        float speedInFPM = (speed*5280)/(60);
        float wheelRPM = speedInFPM/circumference;
        float rpmRatio = rpm/finalDrive;
        float gear = rpmRatio/wheelRPM;

        for(int i = 0; i<ratios.length(); i++){
            //qDebug() << "speed: " << speed << " | rpm: " << rpm << " | ratio: " << ratios.at(i)/gear;
            if(ratios.at(i)/gear >0.87 && ratios.at(i)/gear <1.13){
                i = i+1; // turn index into real gear number. index 3 is gear 4
                currentGear = i;
                return QString::number(i);
            }
        }

    }
    return "N";
}

QList<float> gear::getRatios()
{
    for(int i = 0; i<ratios.length(); i++){
        qDebug() << "ratio: " << i << " : " << ratios.at(i);
    }
    return ratios;
}

float gear::getFinalDrive()
{
    return finalDrive;
}

float gear::getDiameter()
{
    return tireDiameter;
}

void gear::setRatios(QList<float> r)
{
    ratios = r;
}

void gear::setRatioR(float r)
{
    ratioR = r;
}

void gear::setFinalDrive(float fd)
{
    finalDrive = fd;
}

void gear::setTireDiameter(float td)
{
    tireDiameter = td;
    circumference = (float)(tireDiameter/12)*(M_PI);
}

uint gear::calcNextGearRPM(int speed)
{
   // qDebug() << "next gear rpm: " << currentGear;
    if(ratios.length()>0 && currentGear >0 && circumference !=1){
        float speedInFPM = (speed*5280)/(60);
        float wheelRPM = speedInFPM/circumference;
        if(currentGear < 6){
            return ratios.at(currentGear)*wheelRPM*finalDrive;
        }
    }

    return 0;
}
uint gear::calcDownshiftRPM(int speed)
{
    //qDebug() << "downshift rpm: " << currentGear;
    if(ratios.length()>0 && currentGear >1 && circumference !=1){
        float speedInFPM = (speed*5280)/(60);
        float wheelRPM = speedInFPM/circumference;
        if(currentGear < 6){
            return ratios.at(currentGear-2)*wheelRPM*finalDrive;
        }
    }

    return 0;
}

void gear::setCurrentGear(QString g)
{
    if(g != "N" && g!= "0"){
        currentGear = g.toInt(nullptr, 10);
    } else {
        currentGear = 0;
    }
}


