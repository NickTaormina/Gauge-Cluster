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
    double getTrip();
    QString getTrip(double odo);
    QString getTripStart();


    double getTripMPG() const;
    void setTripMPG(double newTripMPG);

    double getTripSamples();



signals:

public slots:
private:
    double tripDistance;

    double tripStart;
    double tripMPG;




};

#endif // TRIP_H
