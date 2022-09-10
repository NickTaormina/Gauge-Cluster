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
    QString getTripNum();
    QString getTrip();

signals:

public slots:
private:
    float milesTraveled;
    QDomDocument *xml;
    QString tripNum;

};

#endif // TRIP_H
