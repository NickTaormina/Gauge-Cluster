#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <QSerialPortInfo>
#include <QObject>
#include <QSerialPort>
#include <QDebug>
#include <QCanBus>
#include <frames.h>
#include "config.h"

class serialHandler : public QObject
{
    Q_OBJECT
public:
    explicit serialHandler(QObject *parent = nullptr);
    serialHandler(QObject * parent, config * c);
    bool waitForFramesReceived(int msecs);
    bool waitForBytesWritten(int msecs);
    bool isConnected();
public slots:
    void serialReceived();
    QCanBusFrame uartToFrame(QString msg);
    void writeFrame(QCanBusFrame frame);
    void setUsefulIDs(QList<uint> id);
signals:
    void serialFrameReceived(QCanBusFrame frame);
    void serialConnected();

private:
    QSerialPort *serial;
    QSerialPortInfo info;
    QByteArray rxbuffer;
    QString serialString;
    frames fr;
    int ct;
    QCanBusFrame currentFrame;

    int lastSerial;
    int connected;

    QString readSerial;
    QList<uint> idList;
};

#endif // SERIALHANDLER_H
