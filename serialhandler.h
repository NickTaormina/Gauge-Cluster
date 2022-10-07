#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <QSerialPortInfo>
#include <QObject>
#include <QSerialPort>
#include <QDebug>
#include <QCanBus>
#include <frames.h>

class serialHandler : public QObject
{
    Q_OBJECT
public:
    explicit serialHandler(QObject *parent = nullptr);
    bool waitForFramesReceived(int msecs);
    QByteArray waitForEcuResponse(int msecs);
    bool waitForBytesWritten(int msecs);
public slots:
    void serialReceived();
    QCanBusFrame uartToFrame(QString msg);
    void writeFrame(QCanBusFrame frame);
signals:
    void messageRead(QCanBusFrame frame);
    void ecuResponse(QCanBusFrame frame);
    void testRes();
private:
    QSerialPort *serial;
    QSerialPortInfo info;
    QByteArray rxbuffer;
    QString serialString;
    frames fr;
    int ct;
    QCanBusFrame currentFrame;

    int lastSerial;
};

#endif // SERIALHANDLER_H
