#ifndef TRIP_H
#define TRIP_H

#include <QObject>
#include <QtXml>

class trip : public QObject
{
    Q_OBJECT
public:
    explicit trip(QObject *parent = nullptr);
    trip(QObject *parent, QDomDocument* xml, QString tr);
    void updateTripDistance(int speed, float time);
    void setMilesTraveled(float m);
    void resetTrip();
    QString getTripNum();

signals:

public slots:
private:
    float milesTraveled;
    QDomDocument *xml;
    void readXML();
    QString tripNum;
};

#endif // TRIP_H
