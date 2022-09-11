#include "config.h"




config::config(QObject *parent)
{

}

void config::setValue(QString key, QString val)
{
    storedConfig.insert(key, val);
}

QString config::getValue(QString key)
{
    return storedConfig.value(key);
}


