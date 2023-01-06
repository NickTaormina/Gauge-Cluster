#ifndef CONFIGHANDLER_H
#define CONFIGHANDLER_H

#include <QObject>
#include <QtXml>
#include <QFile>
#include "string.h"
#include "definition.h"
#include "gear.h"
#include "trip.h"
#include "config.h"
#include "candef.h"
#include "candata.h"
#include "fueleconomy.h"

class configHandler : public QObject
{
    Q_OBJECT
public:
    explicit configHandler(QObject *parent = nullptr);
    configHandler(QObject * parent, config *c);




signals:
    void defsFilled();
    void canFilled();
public slots:
    QString getResourcesPath();
    QString getConfigPath();
    QString getDefPath();
    QStringList getParams();
    void fillDefs(QStringList _selectedParams, Definition* def);
    void fillGear(gear* g);
    void fillTrip(trip* tr, QString tripName);
    void fillEconomy(fueleconomy * fe);
    void storeTrip(QString trip, QString val);
    void storeTripMPG(QString trip, double mpg);
    void parseConfig();
    void fillCan(canDef * d);
    void swapTrip(QString trip);
    void storeOdometer(QString val);
    void storeShiftLightThreshold(QString val);
    int getCanCount();
    void storeShiftLightTimer(QString val);
private:
    QString applicationDir;
    QString _defPath;
    QString ratioPath;
    QString configPath;
    QString resourcesPath;
    QString canPath;

    QDomDocument configXml;
    QDomDocument defXml;


    void clearConfigXml();
    void setDefPath();
    config * cfg;

    int canCount;




};

#endif // CONFIGHANDLER_H
