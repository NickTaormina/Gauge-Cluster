#ifndef TRIP_H
#define TRIP_H

#include <QObject>

class trip : public QObject
{
    Q_OBJECT
public:
    explicit trip(QObject *parent = nullptr);

signals:

};

#endif // TRIP_H
