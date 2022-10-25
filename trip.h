#ifndef TRIP_H
#define TRIP_H

#include <QObject>
#include <QtXml>


class trip : public QObject
{
    Q_OBJECT
public:
    explicit trip(QObject *parent = nullptr);
    void updateTripDistance(int speed, qint64 time);
    void setMilesTraveled(float m);
    void resetTrip();
    QString getTrip();

signals:

public slots:
private:
    float milesTraveled;
    QDomDocument *xml;


};

#endif // TRIP_H
