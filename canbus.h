#ifndef CANBUS_H
#define CANBUS_H

#include <QObject>
#include <QtSerialBus>
#include "config.h"
#include "serialhandler.h"

class canbus : public QObject
{
    Q_OBJECT
public:
    explicit canbus(QObject *parent = nullptr);
    canbus(QObject *parent, config * c);
    QCanBusDevice* dev();
    void connectToCanDevice();
    QByteArray readFrames();
    QByteArray readFrames(uint frameID);
    QByteArray readFrames(uint frameID, char filter, int ignore);
    void writeFrames(uint frameID, QByteArray bytes);
    void writeFrames(uint frameID, QByteArray bytes, uint override);
    bool isConnected();

signals:

private:
    serialHandler* serial;
    QCanBusDevice *_dev;
    QStringList _devList;
    void setConfigVars(config*cfg);
    int useJ2534;
    int baudRate;
};

#endif // CANBUS_H
