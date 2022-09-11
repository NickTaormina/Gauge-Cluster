#ifndef CANBUS_H
#define CANBUS_H

#include <QObject>
#include <QtSerialBus>

class canbus : public QObject
{
    Q_OBJECT
public:
    explicit canbus(QObject *parent = nullptr);
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
    QCanBusDevice *_dev;
    QStringList _devList;
};

#endif // CANBUS_H
