#include "canbus.h"

canbus::canbus(QObject *parent)
    : QObject{parent}
{

}

QCanBusDevice *canbus::dev()
{
    if(_dev){
        return _dev;
    } else {
        return NULL;
    }
}

void canbus::connectToCanDevice()
{
    if(QCanBus::instance()->plugins().contains(QStringLiteral("passthrucan"))){
        //j2534 available
        printf("passthru plugin found");
        const auto adapters = QCanBus::instance()->
                availableDevices(QStringLiteral("passthrucan"));
        for (const QCanBusDeviceInfo &info : adapters){
            printf("device");
                _devList.append(info.name());
            //list available adapters in ui. info.name replaced with driver on linux
        }
    } else {
        qDebug() << "no passthru plugin";
    }
    if(_devList.length() > 0){
        if(_devList.length() == 1){
            qDebug() << "one device found: " << _devList.at(0);
            _dev = QCanBus::instance()->createDevice(QStringLiteral("passthrucan"), _devList.at(0));
            if(_dev){
                _dev->connectDevice();
            } else {
                qDebug() << "failed to add selected can device";
            }
        } else {
            qDebug() << "multiple devices found: " << _devList;
            //TODO: add a way to choose the device
        }
    } else {
        qDebug() << "no devices found";
    }
}

//returns bytes from all received frames
QByteArray canbus::readFrames()
{
    QByteArray rxmsg;
    _dev->waitForFramesReceived(100);
    if(_dev){
        while(_dev->framesAvailable()){
            const QCanBusFrame frame = _dev->readFrame();
            rxmsg.append(frame.payload());
            qDebug() << "payload: " << frame.payload();
        }
    } else {
        printf("no can device");
    }
    qDebug() << "rxmsg" << rxmsg;
    return rxmsg;
}

//only returns frames from certain id
QByteArray canbus::readFrames(uint frameID)
{
    QByteArray rxmsg;
    _dev->waitForFramesReceived(100);
    if(_dev){
        while(_dev->framesAvailable()){
            const QCanBusFrame frame = _dev->readFrame();
            if(frame.frameId() == frameID){
                rxmsg.append(frame.payload());
            }
            qDebug() << "payload: " << frame.payload();
        }
    } else {
        printf("no can device");
    }
    qDebug() << "rxmsg" << rxmsg;
    return rxmsg;
}

QByteArray canbus::readFrames(uint frameID, char filter)
{
    QByteArray rxmsg;
    _dev->waitForFramesReceived(100);
    if(_dev){
        while(_dev->framesAvailable()){
            const QCanBusFrame frame = _dev->readFrame();
            if(frame.frameId() == frameID && frame.payload().at(0) != filter){
                rxmsg.append(frame.payload());
                qDebug() << "filtered payload: " << frame.payload().toHex();
            }

        }
    } else {
        printf("no can device");
    }
    qDebug() << "rxmsg" << rxmsg;
    return rxmsg;
}

//formats tx bytes and sends to ecu
void canbus::writeFrames(uint frameID, QByteArray bytes)
{
    int numFrames = bytes.length()/7;
    if(_dev){
        if(numFrames < 1){
            QCanBusFrame frame = QCanBusFrame(frameID, bytes);
            qDebug() << frame.toString();
            _dev->writeFrame(frame);
        } else {
            int count = 0;
            while(count <= numFrames){
                QByteArray payload = bytes.mid(1+(7*count), 7);
                if(count == 0){
                    payload.insert(0,16).toHex();
                } else {
                    payload.insert(0, 33+(count-1)).toHex();
                }

                QCanBusFrame frame = QCanBusFrame(frameID, payload);
                _dev->writeFrame(frame);
                _dev->waitForFramesWritten(50);
                count ++;
            }
        }
        _dev->waitForFramesWritten(1000);
        qDebug() << "frames written";
    } else {
        printf("failed to send");
    }
}

bool canbus::isConnected()
{
    if(_dev){
        if(_dev->state() == QCanBusDevice::ConnectedState){
            return true;
        }
    }

    return false;
}

