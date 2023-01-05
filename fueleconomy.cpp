#include "fueleconomy.h"

fueleconomy::fueleconomy(QObject *parent)
    : QObject{parent}
{
    mafCorr = -1;
    AFR = -1;
    speed = -1;
    rpm = -1;
    mpg = -1;
    tankLevelStart = -1;
}

double fueleconomy::getFuelFlowRate() const
{
    return fuelFlowRate;
}

void fueleconomy::setFuelFlowRate(double newFuelFlowRate)
{
    fuelFlowRate = newFuelFlowRate;
}

double fueleconomy::getMpg() const
{
    return mpg;
}

void fueleconomy::setMpg(double newMpg)
{
    mpg = newMpg;
}

int fueleconomy::getRangeEstimate(double fuel) const
{
    return tankMPG*fuel;
}

void fueleconomy::setRangeEstimate(int newRangeEstimate)
{
    rangeEstimate = newRangeEstimate;
}

double fueleconomy::getTankMPG(double odo, double fuel)
{
    tankMPG = (odo-odometerTankStart)/((1-fuel)*15.9);
    return tankMPG;
}

void fueleconomy::setTankMPG(double newTankMPG)
{
    tankMPG = newTankMPG;
}

double fueleconomy::getOdometerStart() const
{
    return odometerStart;
}

void fueleconomy::setOdometerStart(double newOdometerStart)
{
    odometerStart = newOdometerStart;
}

double fueleconomy::getOdometerTankStart() const
{
    return odometerTankStart;
}

void fueleconomy::setOdometerTankStart(double newOdometerTankStart)
{
    odometerTankStart = newOdometerTankStart;
}

double fueleconomy::getMafCorr() const
{
    return mafCorr;
}

void fueleconomy::setMafCorr(double newMafCorr)
{
    mafCorr = newMafCorr;
    updateInstantMPG();
}

double fueleconomy::getAFR() const
{
    return AFR;
}

void fueleconomy::setAFR(double newAFR)
{
    AFR = newAFR;
    updateInstantMPG();
}

int fueleconomy::getSpeed() const
{
    return speed;
}

void fueleconomy::setSpeed(int newSpeed)
{
    speed = newSpeed;
}

uint fueleconomy::getRpm() const
{
    return rpm;
}

void fueleconomy::setRpm(uint newRpm)
{
    rpm = newRpm;
}

double fueleconomy::getTankLevelStart() const
{
    return tankLevelStart;
}

void fueleconomy::setTankLevelStart(double newTankLevelStart)
{
    tankLevelStart = newTankLevelStart;
}
double fueleconomy::getMPGAvg(double value)
{
    if(mpgSamples < 5){
        mpgSamples++;
    }
    //qDebug() << "value: " << value << " : " << instantMPGFromCAN;
    if(value <= 0){
        value = instantMPGFromCAN;
    }
    value = (value + instantMPGFromCAN)/2;
    mpgAvg = (mpgAvg + ((double)1/(double)mpgSamples)*(double)((double)value-(double)mpgAvg));
    return mpgAvg;
}

double fueleconomy::getInstantMPGFromCAN() const
{
    return instantMPGFromCAN;
}

void fueleconomy::setInstantMPGFromCAN(double newInstantMPGFromCAN)
{
    instantMPGFromCAN = newInstantMPGFromCAN;
    updateInstantMPG();
}
void fueleconomy::updateInstantMPG()
{
    //grams fuel/second
    //ml fuel/s
    //miles traveled per second
    //ml per mile
    //convert to gallons/mile
    //gallons per mile to miles per gallon
    if(speed == 0){
        mpg = 0;
    } else {
        mpg = getMPGAvg(1/(((mafCorr/AFR)/(E10_DENSITY))/(speed/3600.0)/GALLON_TO_ML));
    }

}
