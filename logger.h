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
    Q_INVOKABLE void sendLogMessage();
    Q_INVOKABLE void startLogging();
    Q_INVOKABLE void stopLogging();
    void createParamArray();
    void combineECUResponse(QCanBusFrame frame);
    void parseECUResponse(QByteArray rxmsg);
signals:
    void setParams(parameter * parameter, int length);
    void ecuResponseParsed();
    void paramUpdated(QString name, double value);
private:
    Definition* definition;
    parameter *res;
    frames fr;
    canbus* can;
    QTimer * timer;
    int paramLength;
    int msgLength;
    int multiPartMsg;
    QByteArray multiPayload;

    int logging;
};

#endif // LOGGER_H
