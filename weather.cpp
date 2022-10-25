#include "weather.h"
#include "qnetworkreply.h"

weather::weather(QObject *parent)
    : QObject{parent}
{
manager = new QNetworkAccessManager();

QObject::connect(manager, &QNetworkAccessManager::finished, this, &weather::processResponse);
requestWeather();

}

const QString &weather::getTemperature() const
{

    return temperature;
}

void weather::setTemperature(const QString &newTemperature)
{
    temperature = newTemperature;
}

void weather::requestWeather()
{
    request.setUrl(QUrl("https://api.openweathermap.org/data/2.5/weather?lat=29.62&lon=-82.37&appid=4cbf3cf0b1fa59eb3a76a4b153deb4f7"));
    manager->get(request);
}

void weather::processResponse(QNetworkReply* r)
{
    if(r->error()){
        qDebug() << "reply error";
        return;
    }

    QString answer = r->readAll();
    //qDebug() << answer;
    QString temp = answer.mid(answer.indexOf("temp") + 6, 20);
    //qDebug() << "temp:" << temp;
    QString tmp2;
    int len = 0;
    for(int i = 0; i<temp.length(); i++){
        if(temp.at(i)!=","){
            len = len + 1;
        } else {
            break;
        }
    }
    tmp2 = temp.left(len);
    double temper = 1.8*(tmp2.toDouble(nullptr) - 273.15) + 32;
    int rounded = qRound(temper);
    temperature = QString::number(rounded);
    emit tempRead(temperature);
}
