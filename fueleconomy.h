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

    double getTankLevelStart() const;
    void setTankLevelStart(double newTankLevelStart);

    double getInstantMPGFromCAN() const;
    void setInstantMPGFromCAN(double newInstantMPGFromCAN);

public slots:

signals:
private:


    int rangeEstimate;
    double tankMPG;
    double odometerStart;
    double odometerTankStart;
    double tankLevelStart;



    //instant mpg vars
    double mpg;
    double mafCorr;
    double AFR;
    uint rpm;
    int speed;
    double fuelFlowRate;
    void updateInstantMPG();
    int mpgSamples;
    double mpgAvg;
    double getMPGAvg(double value);
    double instantMPGFromCAN;


};

#endif // FUELECONOMY_H
