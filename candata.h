#ifndef CANDATA_H
#define CANDATA_H

#include <QObject>
#include <QMap>
#include <QCanBus>
#include "canbus.h"
#include "candef.h"

class canData : public QObject
{
    Q_OBJECT
public:
    explicit canData(QObject *parent = nullptr);
    canData(canDef *def, int count);

signals:
    void turnSignal(QString dir);
    void lights(QString status);
    void acTemp(QString temp);
    void acFan(QString speed);
    void acDefrost(QString status);
    void handbrake(QString status);
    void neutralSwitch(QString status);
    void reverseSwitch(QString status);
    void valueChanged(QString name, double value);

    void rpmChanged(uint rpm);
    void speedChanged(double spd);
public slots:
    void receiveCanData(QCanBusFrame frame);
    void fillData(canDef* def, int count);
private:
    QStringList nameList;
    QList<uint> idList;
    canDef *_def;
    int defCount;

    void emitter(QString name, QString status);
    void valueEmitter(QString name, double value);
    void targetProcess(QMap<uint, QString> t, QByteArray p, int index);
    void valueProcess(QByteArray p, int index);
    void rpmProcess(QByteArray p);
    void bitProcess(QMap<uint, QString> t, QByteArray p, QStringList b, int index);

signals:
    void usefulFrameReceived(QCanBusFrame frame);
private slots:
    void processUsefulFrame(QCanBusFrame frame);
};

#endif // CANDATA_H
