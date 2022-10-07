#include "serialhandler.h"
#include <QEventLoop>
#include <QTimer>

serialHandler::serialHandler(QObject *parent)
    : QObject{parent}
{
    QSerialPortInfo info;
    serial = new QSerialPort(nullptr);
    QList<QSerialPortInfo>portList = info.availablePorts();
    if(portList.size() >= 1){

        for(int i = 0; i<portList.length(); i++){
            qDebug() << "serial port: " << portList[i].portName();
            if(portList[i].portName() == "COM6"){
                qDebug() << "connecting to port: " << portList[i].portName();
                serial->setPortName(portList[i].portName());
            }
        }

        qDebug() << "settings";
        serial->setBaudRate(600000);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);

        if(!serial->open(QIODevice::ReadWrite)){
            qDebug() << "serial connection fail";
        } else {
            qDebug() << "serial connected";

            QObject::connect(serial, &QSerialPort::readyRead, this, &serialHandler::serialReceived);

        }
    }
    lastSerial = 0;
    //QTimer * timer = new QTimer(this);
    //timer->setInterval(1000);
    //QObject::connect(timer, &QTimer::timeout, this, &serialHandler::serialReceived);


    //timer->start();

    serial->write("WRITE:[2016] [01] [170] [0] [0] [0] [0] [0] [0] /");


    serialString = "";

    qDebug() << "processed frame test: " << uartToFrame("READ:[2016] [01] [170] [255] [4] [22] [39] [57] [02]").toString();
}

//returns true if message is received
bool serialHandler::waitForFramesReceived(int msecs)
{
    enum {Received = 0, Error, Timeout};
    QEventLoop loop;
    QObject::connect(serial, &QSerialPort::readyRead, &loop, [&]() {loop.exit(Received);});
    if(msecs >= 0){
        QTimer::singleShot(msecs, &loop, [&]() {loop.exit(Timeout);});
    }
    int result = loop.exec(QEventLoop::ExcludeUserInputEvents);
    if(result == Timeout){
        qDebug() << "frame timed out";
    }
    return result;
}

//waits for ecu response and returns all responses if received. if multi part message, it'll keep adding payload
//to array until it times out.
//TODO: exit based on msg length from ecu
QByteArray serialHandler::waitForEcuResponse(int msecs)
{
    int multiPartMsg = 0;
    QByteArray rx = "";
    QString test;
    enum {Received = 0, Error, Timeout};
    QEventLoop loop;
    //QObject::connect(this, &serialHandler::testRes, &loop, [&]() {qDebug() << "exit"; loop.exit(Received);});

    if(msecs >= 0){
        QTimer::singleShot(msecs, &loop, [&]() {loop.exit(Timeout);});
    }
    int result = loop.exec(QEventLoop::ExcludeUserInputEvents);
    /*
    QObject::connect(this, &serialHandler::ecuResponse, [&loop, this] (QCanBusFrame lamFrame) {
        if(lamFrame.isValid()){
            qDebug() << "valid frame";
        }
        if(lamFrame.payload().at(0) == 10){
            qDebug() << "multi part msg";

        }
       // qDebug() << "lam test: " << lamFrame.toString();
        //QByteArray tmp = lamFrame.payload();
        //qDebug() << "tmp: " << tmp;
        //rx.insert(0, tmp);
        //rx.append(lamFrame.payload());
        //qDebug() << "frame set test: " << QString(rx.toHex().toUpper());
        if(1){
           qDebug() << "extining";
           loop.exit(Received);
        }

    } );
    if(result == Timeout){
        qDebug() << "frame timed out";
    } else if(result == Received){
        qDebug() << "loop received";
        return rx;
        //QObject::disconnect(this, &serialHandler::ecuResponse, this, nullptr);
        //qDebug() << "rx lam: " << rx;
    }*/

    return rx;


}

bool serialHandler::waitForBytesWritten(int msecs)
{
    serial->waitForBytesWritten(msecs);
}


//processing for all received serial messages
//if given READ: command, turn it into a frame
void serialHandler::serialReceived()
{
    rxbuffer = serial->readAll();
    serialString += QString::fromStdString(rxbuffer.toStdString());
    //Debug() << "serial received";
    if(serialString.contains("\\")){
        rxbuffer.clear();

    QStringList bufferSplit = serialString.split("\\");
    int cleared = 1;
    if(bufferSplit.length() > 1){
        for(int i = 0; i<bufferSplit.length()-1; i++){
            if(bufferSplit.at(i)!= ""){

                if(bufferSplit.at(i).contains("READ:") && !bufferSplit.at(i).contains("WRITE")){
                    //do stuff with read message
                    currentFrame = uartToFrame(bufferSplit.at(i));
                    //qDebug() << "frame: " << currentFrame.toString();
                    //qDebug() << bufferSplit.at(i);
                    if(currentFrame.frameId() == 885){
                        qDebug() << "door frame: " << currentFrame.toString();
                    }
                    if(currentFrame.frameId() == 2024){
                       // qDebug() << "ecu response: " << currentFrame.toString();
                        emit ecuResponse(currentFrame);
                        emit testRes();
                    } else {
                        emit messageRead(currentFrame);
                    }
                    int len = bufferSplit.at(i).length();
                    serialString.remove(bufferSplit.at(i)+"\\");
                    if(i == bufferSplit.length()-2){
                        //qDebug() << "end";
                        if(lastSerial == serialString.length()){
                            //qDebug() << "clearing";
                            serialString.clear();
                        } else {

                            lastSerial = serialString.length();
                        }

                    }
                    //qDebug() << "length: " << len << " serial: " << serialString.length() << " : " << bufferSplit.length();

                    bufferSplit.replace(i, "");
                    //qDebug() << "buffer length: " << bufferSplit.length();
                }
                else{
                    cleared = 0;
                }

            }
        }
        /*
        if(serialString.at(serialString.length()-1) == "\\"){
            qDebug() << "clearing buffer: ";
            rxbuffer.clear();
            serialString.clear();
        }*/


        if(cleared == 1){
            //qDebug() << "cleared";
            //rxbuffer.clear();
            bufferSplit.clear();
            //serialString.clear();
        }
    }


    if(bufferSplit.length() > 100){
        //qDebug() << "overloaded";
        //qDebug() << serialString;
        bufferSplit.clear();
        rxbuffer.clear();
        serialString.clear();
    }
    }

}

//returns a filled frame from given esp32 message
QCanBusFrame serialHandler::uartToFrame(QString msg)
{
    QCanBusFrame frame;
    int i = 0;

    //loop for frame id
    while(i < msg.length()){

        if(msg.at(i) == "["){
            QString tmp;
            int x = 1;
            while(x<6){
                //qDebug() << "msg length: " << msg.length() << " i+x: " << i+x;
                if(msg.length()-1 < i+x){
                    //qDebug() << "ERROR";
                    return frame;
                    break;
                }
                if(msg.at(i+x) != "]"){
                    tmp.append(msg.at(i+x));
                    x = x + 1;
                } else {
                    frame.setFrameId(tmp.toUInt(nullptr, 10));
                    break;
                }
            }
            i = i + x;
            break;
        }
        i = i + 1;
    }
    QByteArray payload;
    while(i < msg.length()){

            if(msg.at(i) == "["){
                QString tmp;
                int x = 1;
                while(x<5){
                    //qDebug() << "msg length: " << msg.length() << " i+x: " << i+x;
                    if(msg.length()-1 < i+x){
                        break;
                    }
                    if(msg.at(i+x) != "]" && !msg.at(i+x).isNull()){
                        tmp.append(msg.at(i+x));
                        x = x + 1;
                    } else {
                        payload.append(tmp.toUInt(nullptr, 10));
                        tmp.clear();
                        break;
                    }
                }
                i = i + x;
            }
            i = i + 1;
        }
    frame.setPayload(payload);
    return frame;
}

//creates and sends the write message to the esp32
void serialHandler::writeFrame(QCanBusFrame frame)
{
    QString serialMsg = "WRITE: ";
    QString id = "[" + QString::number(frame.frameId()) + "] ";
    QString framePayload = fr.bytes2String(frame.payload()); //local copy of the payload
    QString payload; //formatted payload that will be sent
    int x = 0;
    for(int i = 0; i<=framePayload.length()-1; i = i+2){
        payload.insert(x, "[");
        QString tmp; //tmp used to turn 2 hex digits to dec
        tmp.append(framePayload.at(i));
        tmp.append(framePayload.at(i+1));
        QString be = QString::number(fr.string2Uint(tmp));
        uint b = fr.string2Uint(tmp);
        int l = fr.numDigits(b);
        payload.insert(x+1, be);
        payload.insert(x+l+1, "] ");
        x = x+l+3;
        tmp.clear();

    }
    serialMsg.append(id);
    serialMsg.append(payload);
    serialMsg.append("/");
    serial->write(serialMsg.toStdString().c_str());
    //qDebug() << "sent serial frame: " << serialMsg.toStdString().c_str();



}

