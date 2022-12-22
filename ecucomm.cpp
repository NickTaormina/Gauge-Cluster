#include "ecucomm.h"

ecuComm::ecuComm(QObject *parent, canbus * can)
    : QObject{parent}
{
    _can = can;
    maxTries = 3;
}

QByteArray ecuComm::sendConvo(QCanBusFrame frame, int timeout)
{
    qDebug() << "sendConvo";
    // Send the message
    _can->writeFrames(frame.frameId(), frame.payload());

    targetID = frame.frameId();
    QByteArray response;

    // Start a timer to wait for the response
    QTimer timer;
    timer.setSingleShot(true);
    timer.start(timeout);


    // Wait for the response signal or timeout
    m_received = false;
    connect(&timer, &QTimer::timeout, this, &ecuComm::timeout);
    connect(this, &ecuComm::responseReceived, &timer, &QTimer::stop);
    connect(this, &ecuComm::responseReceived, this, [&](QCanBusFrame frame) { m_received = true;
             response = frame.payload();
             qDebug() << "response received";

    });
    while (!m_received && timer.isActive())
        QCoreApplication::processEvents();

    if(!m_received && tries < maxTries){
        retrying = true;
        tries = tries + 1;
        sendConvo(frame, timeout);
    }
    qDebug() << "sendConvo done";
    // Return true if the response was received, false otherwise
    return response;
}

QByteArray ecuComm::waitConvo(uint frameid, int timeout)
{
    qDebug() << "wait convo";
    targetID = frameid;
    QByteArray response;

    // Start a timer to wait for the response
    QTimer timer;
    timer.setSingleShot(true);
    timer.start(timeout);


    // Wait for the response signal or timeout
    m_received = false;
    connect(&timer, &QTimer::timeout, this, &ecuComm::timeout);
    connect(this, &ecuComm::dataReceived, &timer, &QTimer::stop);
    connect(this, &ecuComm::dataReceived, this, [&](QByteArray data) {
        response = data;
        qDebug() << "data received";
    });
    while (!m_received && timer.isActive())
        QCoreApplication::processEvents();


    // Return true if the response was received, false otherwise
    return response;
}

double ecuComm::getFuelLevel()
{
    QByteArray temp;
    QCanBusFrame frame;
    frame.setFrameId(2016);
    frame.setPayload(fr.string2Bytes("023e0000000000"));
    qDebug() << "first msg";
    temp = sendConvo(frame, 100);
    frame.setPayload(fr.string2Bytes("02100300000000"));
    qDebug() << "sec msg";
    temp = sendConvo(frame, 100);

    //init done, send actual request
    qDebug() << "actual msg";
    _can->writeFrames(2016, fr.string2Bytes("10082c01f300f42f010100000000"));
    QByteArray resp = waitConvo(2016, 100);

    qDebug() << "spam msg";
    _can->writeFrames(2016, fr.string2Bytes("0322f3000000"));
    resp = waitConvo(2016, 100);

    if(resp.length() >5 && resp.at(0) == 4 && resp.at(1) == 98 && resp.at(2) == 243){
        qDebug() << resp.at(4)/255;
        return resp.at(4)/255;
    }


}

void ecuComm::receiveCommFrame(QCanBusFrame frame)
{
    if(frame.frameId() == targetID){
        emit responseReceived(frame);
        responsePayload = fr.combineResponse(frame);
        if(!responsePayload.isNull()){
            emit dataReceived(responsePayload);
        }
    }
}

void ecuComm::timeout()
{

}

void ecuComm::resetTries()
{
    if(!retrying){
        tries = 0;
    } else {
        if(tries >= maxTries){
            retrying = 0;
        }
    }

}
