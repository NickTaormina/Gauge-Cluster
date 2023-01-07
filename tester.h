#ifndef TESTER_H
#define TESTER_H

#include <QtSerialBus>
#include <QObject>
#include <QDataStream>
#include <QFile>


class tester : public QObject
{
    Q_OBJECT
public:
    explicit tester(QObject *parent = nullptr);
signals:
    void testFrameProcessed(QCanBusFrame frame);
    void processFinished();

public slots:
    void testFileSelected(QString url);
    void processFile(QString url);
signals:
private:
};

#endif // TESTER_H
