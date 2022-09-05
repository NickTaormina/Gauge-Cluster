#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include "definition.h"
#include "parameter.h"
#include "frames.h"
#include "canbus.h"
#include <QString>
#include <QTimer>

class logger : public QObject
{
    Q_OBJECT
public:
    explicit logger(QObject *parent = nullptr);
    logger(QObject *parent, canbus *bus, parameter* par, Definition * def);
public slots:
    Q_INVOKABLE void log();
    Q_INVOKABLE void startLogging();
    Q_INVOKABLE void stopLogging();
    void createParamArray();
signals:
void setParams(parameter * parameter, int length);
private:
    Definition* definition;
    parameter *res;
    frames fr;
    canbus* can;
    QTimer * timer;
    int paramLength;
};

#endif // LOGGER_H
