#ifndef ECUCOMM_H
#define ECUCOMM_H

#include <QtSerialBus>
#include <QObject>
#include <QTimer>
#include "canbus.h"
#include "frames.h"



class ecuComm : public QObject
{
    Q_OBJECT
public:
    explicit ecuComm(QObject *parent, canbus * can);
    QByteArray sendConvo(QCanBusFrame frame, int timeout);
    QByteArray waitConvo(uint frameid, int timeout);

    double getFuelLevel();
public slots:
    void receiveCommFrame(QCanBusFrame frame);
signals:
    void responseReceived(QCanBusFrame frame);
    void dataReceived(QByteArray data);
private slots:
    void timeout();
private:
    bool m_received;

    canbus * _can;
    frames fr;

    uint targetID;

    int tries;
    int maxTries;
    bool retrying;
    void resetTries();

    QByteArray responsePayload;

};

#endif // ECUCOMM_H
