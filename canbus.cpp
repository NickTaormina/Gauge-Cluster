#include "canbus.h"

/*
handles canbus operations for tactrix & cobb specific stuff
*/

canbus::canbus(QObject *parent)
    : QObject{parent}
{

}

canbus::canbus(QObject *parent, config *c, serialHandler* s)
{
    parent = nullptr;
    setConfigVars(c);
    serial = s;
}

QCanBusDevice *canbus::dev()
{
    if(_dev){
        return _dev;
    } else {
        return NULL;
    }
}

//connects to j2534 device. only supports a single device rn
void canbus::connectToCanDevice()
{
    if(useJ2534 == 1){
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
    } else {
        qDebug() << "using esp32-can";
    }
}

//returns bytes from all received frames
QByteArray canbus::readFrames()
{

    QByteArray rxmsg;
    if(useJ2534 == 1){
        _dev->waitForFramesReceived(200);
        if(_dev){
            while(_dev->framesAvailable()){
                const QCanBusFrame frame = _dev->readFrame();
                rxmsg.append(frame.payload());
                qDebug() << "payload: " << frame.payload();
            }
        } else {
            printf("no can device");
        }
    } else {
        rxmsg = serial->waitForEcuResponse(200);
        qDebug() << "tried to read from esp32:";
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


//message with given filter. ignore flag determines if if keeps or ignores msg starting with filter
QByteArray canbus::readFrames(uint frameID, char filter, int ignore)
{
    QByteArray rxmsg;
    if(useJ2534 == 1){
        _dev->waitForFramesReceived(100);
        if(_dev){
            while(_dev->framesAvailable()){
                const QCanBusFrame frame = _dev->readFrame();
                if(ignore == 1 && frame.frameId() == frameID && frame.payload().at(0) != filter){
                    rxmsg.append(frame.payload());
                    qDebug() << "filtered payload: " << frame.payload().toHex();
                } else if (ignore == 0 && frame.frameId() == frameID && frame.payload().at(0) == filter){
                    rxmsg.append(frame.payload());
                    qDebug() << "filtered payload: " << frame.payload().toHex();
                }

            }
        } else {
            printf("no can device");
        }
        qDebug() << "rxmsg" << rxmsg;

    } else {
        rxmsg = serial->waitForEcuResponse(100);
        qDebug() << "tried to read from esp32:";
    }
    qDebug() << "rxmsg" << rxmsg;
    return rxmsg;
}

//formats tx bytes and sends to ecu
void canbus::writeFrames(uint frameID, QByteArray bytes)
{
    int numFrames = bytes.length()/7; //TODO: separate single frame message better. look if first byte is page#

    if(useJ2534 == 1){
        if(_dev){
            if(numFrames <= 1){
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
                    count++;
                }
            }
            _dev->waitForFramesWritten(1000);
            qDebug() << "frames written";
        } else {
            printf("failed to send");
        }
    } else {
        if(serial){
            qDebug() << "canbus write frame";
            if(numFrames <= 1){
                QCanBusFrame frame = QCanBusFrame(frameID, bytes);
                qDebug() << frame.toString();
                serial->writeFrame(frame);
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
                    QEventLoop loop;
                    QTimer::singleShot(100, &loop, SLOT(quit()));
                    loop.exec();
                    serial->writeFrame(frame);
                    serial->waitForBytesWritten(5);


                    count++;
                }
            }
        } else {
            printf("failed to send");
        }
    }
}

void canbus::writeFrames(uint frameID, QByteArray bytes, uint override)
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

//returns true if connected to a can device
bool canbus::isConnected()
{
    if(useJ2534 == 1){
        if(_dev){
            if(_dev->state() == QCanBusDevice::ConnectedState){
                return true;
            }
        }
    }
    else{
        if(serial){
            return true;
        }
    }


    return false;
}

void canbus::setConfigVars(config *cfg)
{
    useJ2534 = cfg->getValue("useJ2534").toInt(nullptr, 10);

    qDebug() << "j2534: " << useJ2534;
    baudRate = cfg->getValue("baudRate").toInt(nullptr, 10);
    qDebug() << "baudrate: " << baudRate;
}

