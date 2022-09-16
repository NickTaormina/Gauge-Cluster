#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <QSerialPortInfo>
#include <QObject>
#include <QSerialPort>
#include <QDebug>

class serialHandler : public QObject
{
    Q_OBJECT
public:
    explicit serialHandler(QObject *parent = nullptr);
public slots:
    void serialReceived();
signals:
private:
    QSerialPort *serial;
    QSerialPortInfo info;
    QByteArray rxbuffer;
    QString serialString;
};

#endif // SERIALHANDLER_H
