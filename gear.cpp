#include "gear.h"
#include <QDebug>

gear::gear(QObject *parent)
    : QObject{parent}
{
    QFile f("C:/Users/admin/OneDrive - University of Florida/Documents/_Tuning/Gauge Cluster/GaugeCluster/config/ratios.xml");
    xml.setContent(&f);
    f.close();
    parseRatios();
    numGears = 6;

}

gear::gear(QObject *parent, QString path)
{
    parent = nullptr;
    QFile f(path);
    xml.setContent(&f);
    f.close();
    parseRatios();
    numGears = 6;
}

QString gear::calcGear(int rpm, int speed)
{
    int found = 0;
    if(ratios.length()>0){
        float circumference = (float)(tireDiameter/12)*(M_PI);
        float speedInFPM = (speed*5280)/(60);
        float wheelRPM = speedInFPM/circumference;
        float rpmRatio = rpm/finalDrive;
        float gear = rpmRatio/wheelRPM;
        for(int i = 0; i<ratios.length(); i++){

            if(ratios.at(i)/gear >0.99 && ratios.at(i)/gear <1.01){
                i = i+1;
                return QString::number(i);
            }
        }
        if(ratioR/gear > .99 && ratioR/gear < 1.01){ //TODO: fix reverrse indicator
            return "N";
        }

    }
    return "N";
}

QList<float> gear::getRatios()
{
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

void gear::setRatios(QList<float> ratios)
{

}

void gear::setRatioR(float r)
{

}

void gear::setFinalDrive(float fd)
{

}

void gear::setTireDiameter(float td)
{

}

void gear::parseRatios()
{
    QDomElement root = xml.documentElement();
    QDomElement ratio = root.firstChild().toElement();
    while(!ratio.isNull()){
        if(ratio.tagName() == "one"){
            ratios.append(ratio.text().toFloat(nullptr));
        } else if(ratio.tagName() == "two"){
            ratios.append(ratio.text().toFloat(nullptr));
        } else if(ratio.tagName() == "three"){
            ratios.append(ratio.text().toFloat(nullptr));
        } else if(ratio.tagName() == "four"){
           ratios.append(ratio.text().toFloat(nullptr));
        } else if(ratio.tagName() == "five"){
            ratios.append(ratio.text().toFloat(nullptr));
        } else if(ratio.tagName() == "six"){
            ratios.append(ratio.text().toFloat(nullptr));
        } else if(ratio.tagName() == "R"){
            ratioR = ratio.text().toFloat(nullptr);
        } else if(ratio.tagName().toLower() == "final"){
            finalDrive = ratio.text().toFloat(nullptr);
        } else if(ratio.tagName().toLower() == "tire"){
            tireDiameter = ratio.text().toFloat(nullptr);
        }
        ratio = ratio.nextSibling().toElement();
    }
}
