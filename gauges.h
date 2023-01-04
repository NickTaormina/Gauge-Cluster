#ifndef GAUGES_H
#define GAUGES_H

#include <QObject>
#include <QQmlContext>
#include "parameter.h"
#include <QTimer>
#include "gear.h"
#include "trip.h"
#include "config.h"
#include "candata.h"
#include <QtMath>
#include <QDateTime>
#include "weather.h"
#include "confighandler.h"
#include "defwindow.h"
#include "paramdisplay.h"
#include "fueleconomy.h"


class gauges : public QObject
{
    Q_OBJECT
public:
    explicit gauges(QObject *parent = nullptr);
    gauges(QObject *parent, QObject * main, gear* gear, config* cfg, configHandler *handler, canData *data, defWindow * defwin, fueleconomy * fe);

signals:
void tripUpdated(QString trip, QString val);
void odometerUpdated(QString val);
void tripSwapped(QString trip);
void rpmUpdated(uint rpm);
void shiftThresholdChanged(QString val);
void shiftTimerChanged(QString val);

public slots:
    void setRPMCAN(uint rpm);
    Q_INVOKABLE void setParamPointer(parameter * parameter, int length);
    Q_INVOKABLE void resetTrip(QString tr);
    Q_INVOKABLE void setShiftThreshold(QString val);
    Q_INVOKABLE void toggleThrottleBar();

    void setFuelCAN(float fuel);
    void startTimer();
    void updateValue();
    void startTest();
    void setOdometer();
    void gaugeSweep();
    void sweepForward();
    void sweepBack();
    void updateParamDisplay(QString name, double value);
    void updateCANParam(QString name, double value);
    void updateTemperatureText(QString t);
    void updateCoolantGauge(double value);
    void updateFuelBar(double value);
    void updateTargetShiftRPM(uint rpm);
    void updateCruiseSpeed(double speed);
    void updateGearFromCAN(QString status);

    void showCEL(QString status);
    void showTPMS(QString status);
    void showSeatbelt(QString status);
    void showOil(QString status);
    void showDoor(QString status);



    void flashShiftLight(uint rpm);
    void showShiftLight();

    Q_INVOKABLE QString getShiftThreshold();

    //can indicators
    void updateLights(QString status);
    void updateTurnSignals(QString status);
    void updateNeutral(QString status);
    void updateReverse(QString status);
    void updateClutch(QString status);
    void updateHandbrake(QString status);
    void setSpeedCAN(double speed);
    void updateCruiseStatus(QString status);

    void onShiftTimerTimeout();

    void updateWeatherStatus();
    QString getActiveTripNum();
    Q_INVOKABLE void switchActiveTrip();
    Q_INVOKABLE QString getShiftTimer();

    void fadeInGauges();
private:

    //gauges constants
    int minRPM;
    int maxRPM;
    int maxTach;
    int minTach;
    int minSpeed;
    int maxSpeed;
    int maxSpeedoRot;
    int minSpeedoRot;


    //param values
    parameter * par;
    int paramLength;
    int fbkIndex;
    int fklIndex;
    int damIndex;


    //current values
    int prevSpeed;
    int _speed;
    int _rpm;
    double prevRPMPos;
    double prevSpeedPos;
    int accelPos;
    QString currentGear;

    //switches
    bool neutral;
    bool clutch;
    bool reverse;
    bool handbrake;

    bool changeGear;




    void getKnockIndexes();

    void updateTrip();
    void showKnock();

    void sweepDone();
    void updateClock();

    void initUIElements();

    double getFuelAvg(double value);

    double sessionStart;
    double sessionFuelLevel;


    //parameter display
    QString topLeftText;
    QString topLeftVal;
    QString topRightText;
    QString topRightVal;
    QString bottomRightText;
    QString bottomRightVal;
    QString bottomLeftText;
    QString bottomLeftVal;

    //ui gauge objects
    QObject * tachNeedle;
    QObject * speedoHandler;
    QObject * speedoNeedle;
    QObject * fuelNeedle;
    QObject * rpmtext;
    QObject * speedtext;
    QObject * odotext;
    QObject * geartext;
    QObject * tripNum;
    QObject * statustext;
    QObject * shiftLight;
    QObject * shiftTargetRect;
    QObject * neutralText;
    QObject * refText;
    QObject * clutchText;
    QObject * fuelText;
    QObject * cruiseText;
    QObject * cruiseImage;
    QObject * setImage;
    QObject * brakeImage;
    QObject * celImage;
    QObject * oilImage;
    QObject * seatbeltImage;
    QObject * tpmsImage;
    QObject * doorImage;

    //ui status objects
    QObject * leftSignal;
    QObject * rightSignal;
    QObject * lightIndicator;

    //ui parameter objects
    QObject * statusRect;
    QObject * statusVerticalBar;
    QObject * topLeftLabel;
    QObject * topLeftValue;
    QObject * topRightLabel;
    QObject * topRightValue;
    QObject * bottomRightLabel;
    QObject * bottomRightValue;
    QObject * bottomLeftLabel;
    QObject * bottomLeftValue;
    QObject * clockText;
    QObject * temperatureText;
    QObject * coolantGauge;
    QObject * fuelBar;
    QObject * throttleBar;
    QObject * boostGauge;

    //timers
    QTimer* timer;
    QTimer* testtimer;
    QTimer* speedTime;
    QTimer* sweepTimer;
    QTimer weatherTimer;
    QTimer shiftLightTimer;
    QTimer clockTimer;
    QTimer * shiftTimer;
    QElapsedTimer elapsedTimer;
    qint64 elapsed;

    int rpmval;
    double odoval;
    int speedval;
    void changeValues();
    void updateSpeedText();
    int animDuration;
    int sweepFinished;
    int shiftTimerDuration;

    uint shiftLightMin;
    int shiftLightFlashInterval;

    QString activeTrip;
    QString coolantFilePath;
    QString fuelFilePath;

    //booleans
    int showAllSpeedNumbers;
    int initialGaugeSweep;


    QDateTime systime;
    //class objects
    gear* g;

    trip trA;
    trip trB;
    //configHandler * _cfgHand;
    canData * _data;
    weather _weather;
    paramDisplay* _paramDisplay;
    fueleconomy * _fe;

    //fuel variables
    int fuelResMin;
    int fuelResMax;
    int fuelSamples;
    int fuelBarMin;
    double fuelValueAvg;


    //cruise control
    int cruiseSpeed;
    int cruiseEnabled;
    int cruiseActive;

    //light variables
    bool park;
    bool lights;
    bool flash;
    bool flashToggle;
    bool leftSignalBool;
    bool rightSignalBool;

    //door variables
    bool driverDoor;
    bool passengerDoor;
    bool rearPassengerDoor;
    bool rearDriverDoor;
    bool trunkDoor;



signals:
};

#endif // GAUGES_H
