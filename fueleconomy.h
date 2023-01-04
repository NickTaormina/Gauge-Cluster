#ifndef FUELECONOMY_H
#define FUELECONOMY_H

#include <QObject>

class fueleconomy : public QObject
{
    Q_OBJECT
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

public slots:

signals:
private:
    double fuelFlowRate;
    double mpg;
    int rangeEstimate;
    double tankMPG;
    double odometerStart;
    double odometerTankStart;

};

#endif // FUELECONOMY_H
