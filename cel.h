#ifndef CEL_H
#define CEL_H

#include <QObject>
#include <QByteArray>
#include "canbus.h"
#include "frames.h"
#include "config.h"

class cel : public QObject
{
    Q_OBJECT
public:
    explicit cel(QObject *parent = nullptr);
    cel(QObject * parent, canbus* bus, config*c);
    void readCELS();
    void setCelBytes(QByteArray txmsg);

signals:
    void engineError();
private:
    int hasCEL;
    canbus* can;
    frames fr;
    QByteArray celbytes;
    config * cfg;
};

#endif // CEL_H
