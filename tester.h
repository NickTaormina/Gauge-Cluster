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
    QCanBusFrame extractFrame(const QByteArray &data);
    void removeCharacters(const QString& fileName, const QString& characters);
signals:
private:
    QThread * thread;
};

#endif // TESTER_H
