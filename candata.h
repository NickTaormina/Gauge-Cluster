#ifndef CANDATA_H
#define CANDATA_H

#include <QObject>
#include <QMap>
#include <QCanBus>
#include <canbus.h>
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
public slots:
    void receiveCanData(QCanBusFrame frame);
private:
    QStringList nameList;
    QList<uint> idList;
    canDef *_def;
    int defCount;

    void emitter(QString name, QString status);
    void targetProcess(QMap<uint, QString> t, QByteArray p, int index);
    void valueProcess(QByteArray p, int index);

signals:
    void usefulFrameReceived(QCanBusFrame frame);
private slots:
    void processUsefulFrame(QCanBusFrame frame);
};

#endif // CANDATA_H
