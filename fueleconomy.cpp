#include "fueleconomy.h"

fueleconomy::fueleconomy(QObject *parent)
    : QObject{parent}
{

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
