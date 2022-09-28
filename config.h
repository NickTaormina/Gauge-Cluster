#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QMap>
#include <QString>

class config : public QObject
{
    Q_OBJECT
public:
    explicit config(QObject *parent = nullptr);
    void setValue(QString key, QString val);
    QString getValue(QString key);
    enum {
        DEFS, CAN, LOGGER, TRIP, GAUGES
    };



signals:
private:
    QMap<QString, QString> storedConfig;
};

#endif // CONFIG_H
