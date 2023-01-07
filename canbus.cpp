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

    //sets up acknowledgement message for ecu
    ack.setFrameId(2016);
    ack.setPayload(fr.string2Bytes("3000000000000000"));

    if (QCanBus::instance()->plugins().contains(QStringLiteral("socketcan"))) {
        // plugin available
        QString errorString;
        _dev = QCanBus::instance()->createDevice(
            QStringLiteral("socketcan"), QStringLiteral("can0"), &errorString);
        if (!_dev) {
            // Error handling goes here
            qDebug() << errorString;
        } else {
            _dev->setConfigurationParameter(QCanBusDevice::BitRateKey, QVariant());
            _dev->connectDevice();
            if(_dev->state() == QCanBusDevice::ConnectedState){
                qDebug() << "connected socketcan";
                QObject::connect(_dev,&QCanBusDevice::framesReceived,this,[this](){
                    QVector<QCanBusFrame> frames = _dev->readAllFrames();
                    for(int i = 0; i<frames.count(); i++){
                        receiveSerialFrame(frames.at(i));
                    }
                });
            }
        }

    }
}

QCanBusDevice *canbus::dev()
{
    if(_dev){
        return _dev;
    } else {
        return NULL;
    }
}




//formats tx bytes and sends to ecu
void canbus::writeFrames(uint frameID, QByteArray bytes)
{
    int numFrames = qCeil((double)bytes.length()/7); //TODO: separate single frame message better. look if first byte is page#

    if(useJ2534 == 1){
        if(_dev){
            if(numFrames <= 1){
                QCanBusFrame frame = QCanBusFrame(frameID, bytes);
                //qDebug() << frame.toString();
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
            //qDebug() << "frames written";
        } else {
            qInfo() << "failed to send";
        }
    } else {
        if(serial){
            //qDebug() << "canbus write frame";
            queuedMessage.clear();
            if(numFrames <= 1){
                QCanBusFrame frame = QCanBusFrame(frameID, bytes);
               // qDebug() << frame.toString();
                _dev->writeFrame(frame);
            } else {
                int count = 0;
                 QCanBusFrame frame;
                while(count <= numFrames){
                    QByteArray payload = bytes.mid(1+(7*count), 7);
                    if(count == 0){
                        payload.insert(0,16).toHex();
                        frame = QCanBusFrame(frameID, payload);
                       // qInfo() << "writing start msg: " << frame.toString();
                        //qDebug() << "write: " << frame.toString();
                        _dev->writeFrame(frame);
                    } else {
                        payload.insert(0, 33+(count-1)).toHex();
                        queuedMessage.append(payload);
                    }

                    count++;
                }

               // qInfo() << "queued msg: " << queuedMessage;


            }
        } else {
            qDebug() << "failed to send";
        }
    }
}

//sends the second "half" of the param request message after ecu acknowledgement
void canbus::sendQueuedMessage()
{
    //qInfo() << "sending queued msg";
    if(queuedMessage.length() <1){
        queuedMessage.clear();
        return;
    }

    int numFrames = queuedMessage.length()/8;
    int count = 0;
    //QEventLoop loop;
    while(count<=numFrames){
        //if(!loop.isRunning()){
        QByteArray payload = queuedMessage.mid(0+(8*count), 8); 
        QCanBusFrame frame = QCanBusFrame(2016, payload);
        //qInfo() << "sending frame: " << frame.toString();
        //qDebug() << "write: " << frame.toString();
        QThread::msleep(5);
        _dev->writeFrame(frame);




        //QTimer::singleShot(5, &loop, SLOT(quit()));
        //loop.exec();
        count++;
    }
    queuedMessage.clear();

}

//sends messages based on frame content
void canbus::receiveSerialFrame(QCanBusFrame frame)
{
    //qInfo() << frame.toString();
    if(frame.frameId() == 321){
        //qDebug() << "frame: " << frame.toString();
    }
    if(frame.frameId() == 2016){
        //qInfo() << "written: " << frame.toString();
        //qDebug() << "written: " << frame.toString();
    }
    if(frame.frameId() == 2024){
        //qDebug() << "ecu response: " << frame.toString();
        if(frame.payload().at(0) == 16){
            //respond with ack message if ecu sends multipart
            _dev->writeFrame(ack);
        }
        if(frame.payload().at(0) == 48){
            //sends queued messages when ecu ack received
            emit ecuAck();
        } else {
            //sends ecu response to get processed into real data
            //qInfo() << "sending new frame for process";
            emit ecuResponse(frame);
        }

    } else {
        emit messageRead(frame);
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
        if(serial->isConnected() || _dev->state() == QCanBusDevice::ConnectedState){
            return true;
        }
    }


    return false;
}

void canbus::sendSlot(QCanBusFrame frame)
{
    writeFrames(frame.frameId(), frame.payload());
}



void canbus::setConfigVars(config *cfg)
{
    useJ2534 = cfg->getValue("useJ2534").toInt(nullptr, 10);

    qDebug() << "*j2534: " << useJ2534;
    baudRate = cfg->getValue("baudRate").toInt(nullptr, 10);
    qDebug() << "*baudrate: " << baudRate;
}

