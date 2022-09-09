#ifndef CONFIGHANDLER_H
#define CONFIGHANDLER_H

#include <QObject>
#include <QtXml>
#include <QFile>
#include "string.h"
#include "definition.h"
#include "gear.h"
#include "trip.h"

class configHandler : public QObject
{
    Q_OBJECT
public:
    explicit configHandler(QObject *parent = nullptr);



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




};

#endif // CONFIGHANDLER_H
