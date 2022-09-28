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
public slots:
    void serialReceived();
    QCanBusFrame readFrame();
    void writeFrame(QCanBusFrame frame);
signals:
    void messageRead();
private:
    QSerialPort *serial;
    QSerialPortInfo info;
    QByteArray rxbuffer;
    QString serialString;
    frames fr;
    int ct;
};

#endif // SERIALHANDLER_H
