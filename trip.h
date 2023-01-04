#ifndef TRIP_H
#define TRIP_H

#include <QObject>
#include <QtXml>


class trip : public QObject
{
    Q_OBJECT
public:
    explicit trip(QObject *parent = nullptr);
    void resetTrip(double m);
    QString getTrip(double odo);
    QString getTripStart();



signals:

public slots:
private:

    double tripStart;




};

#endif // TRIP_H
