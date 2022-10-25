#ifndef CANBUS_H
#define CANBUS_H

#include <QObject>
#include <QtSerialBus>
#include "config.h"
#include "serialhandler.h"
#include "frames.h"


class canbus : public QObject
{
    Q_OBJECT
public:
    explicit canbus(QObject *parent = nullptr);
    canbus(QObject *parent, config * c, serialHandler * s);
    QCanBusDevice* dev();
    void connectToCanDevice();
    QByteArray readFrames();
    QByteArray readFrames(uint frameID);
    QByteArray readFrames(uint frameID, char filter, int ignore);
    void writeFrames(uint frameID, QByteArray bytes);
    bool isConnected();
public slots:
    void sendQueuedMessage();
    void receiveSerialFrame(QCanBusFrame frame);
signals:
    void ecuAck();
    void messageRead(QCanBusFrame frame);
    void ecuResponse(QCanBusFrame frame);
private:
    serialHandler* serial;
    QCanBusDevice *_dev;
    QStringList _devList;
    void setConfigVars(config*cfg);
    int useJ2534;
    int baudRate;

    QByteArray queuedMessage;

    QCanBusFrame ack;

    frames fr;
};

#endif // CANBUS_H
