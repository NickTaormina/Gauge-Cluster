#ifndef FUELECONOMY_H
#define FUELECONOMY_H

#include <QObject>
#include "QDebug"

class fueleconomy : public QObject
{
    Q_OBJECT
#define E10_DENSITY .743 //g/mL
#define GALLON_TO_ML 3785.41

public:
    explicit fueleconomy(QObject *parent = nullptr);
    double getFuelFlowRate() const;
    void setFuelFlowRate(double newFuelFlowRate);

    double getMpg() const;
    void setMpg(double newMpg);



    int getRangeEstimate(double fuel) const;
    void setRangeEstimate(int newRangeEstimate);

    double getTankMPG(double odo, double fuel);
    void setTankMPG(double newTankMPG);

    double getOdometerStart() const;
    void setOdometerStart(double newOdometerStart);

    double getOdometerTankStart() const;
    void setOdometerTankStart(double newOdometerTankStart);

    double getMafCorr() const;
    void setMafCorr(double newMafCorr);

    double getAFR() const;
    void setAFR(double newAFR);

    int getSpeed() const;
    void setSpeed(int newSpeed);

    uint getRpm() const;
    void setRpm(uint newRpm);

public slots:

signals:
private:


    int rangeEstimate;
    double tankMPG;
    double odometerStart;
    double odometerTankStart;



    //instant mpg vars
    double mpg;
    double mafCorr;
    double AFR;
    uint rpm;
    int speed;
    double fuelFlowRate;
    void updateInstantMPG();


};

#endif // FUELECONOMY_H
