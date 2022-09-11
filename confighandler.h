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

class configHandler : public QObject
{
    Q_OBJECT
public:
    explicit configHandler(QObject *parent = nullptr);
    configHandler(QObject * parent, config *c);



signals:
    void defsFilled();
public slots:
    QString getResourcesPath();
    QString getConfigPath();
    QString getDefPath();
    QStringList getParams();
    void fillDefs(QStringList _selectedParams, Definition* def);
    void fillGear(gear* g);
    void fillTrip(trip* tr);
    void storeTrip(QString trip, QString val);
    void parseConfig();

private:
    QString applicationDir;
    QString _defPath;
    QString ratioPath;
    QString configPath;
    QString resourcesPath;

    QDomDocument configXml;
    QDomDocument defXml;


    void clearConfigXml();
    void setDefPath();
    config * cfg;




};

#endif // CONFIGHANDLER_H
