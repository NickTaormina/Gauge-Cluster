#ifndef WEATHER_H
#define WEATHER_H

#include <QObject>
#include <QNetworkAccessManager>

class weather : public QObject
{
    Q_OBJECT
public:
    explicit weather(QObject *parent = nullptr);
    const QString &getTemperature() const;
    void setTemperature(const QString &newTemperature);
    void requestWeather();

public slots:

signals:
    void tempRead(QString t);
private:
    QNetworkAccessManager  * manager;
    QString temperature;
    QNetworkRequest request;
private slots:
    void processResponse(QNetworkReply * r);

};

#endif // WEATHER_H
