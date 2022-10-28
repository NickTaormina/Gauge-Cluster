#include "serialhandler.h"
#include <QEventLoop>
#include <QTimer>

serialHandler::serialHandler(QObject *parent)
    : QObject{parent}
{
    connected = 0;
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

        qDebug() << "*settings";
        serial->setBaudRate(921600);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);

        if(!serial->open(QIODevice::ReadWrite)){
            qDebug() << "serial connection fail";
            //emit serialConnected();
        } else {
            qDebug() << "serial connected";

            QObject::connect(serial, &QSerialPort::readyRead, this, &serialHandler::serialReceived);
            emit serialConnected();
            connected = 1;
        }
    }

    lastSerial = 0;





    serial->write("W: [2016] [01] [170] [0] [0] [0] [0] [0] [0] /");
    //serial->write("WRITE:[2016] [02] [16] [03] [0] [0] [0] [0] [0] /");
    //serial->write("WRITE:[2016] [16] [8] [168] [0] [0] [0] [9] [0] /");
    //qDebug() << "frame: " << uartToFrame("[2016] [16] [8] [168] [0] [0] [0] [9] [0]").toString();
    //serial->write("WRITE:[2016] [33]");


    serialString = "";

}

serialHandler::serialHandler(QObject *parent, config *c)
{
    connected = 0;
    QSerialPortInfo info;
    serial = new QSerialPort(nullptr);
    QList<QSerialPortInfo>portList = info.availablePorts();
    if(portList.size() >= 1){
        QString configPort = c->getValue("comPort");
        qDebug() << "*config port: " << configPort;
        for(int i = 0; i<portList.length(); i++){
            qDebug() << "serial port: " << portList[i].portName();
            if(portList[i].portName() == configPort){
                qDebug() << "connecting to port: " << portList[i].portName();
                serial->setPortName(portList[i].portName());
            }
        }

        qDebug() << "*settings";
        serial->setBaudRate(921600);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);

        if(!serial->open(QIODevice::ReadWrite)){
            qDebug() << "serial connection fail";
            //emit serialConnected();
        } else {
            qDebug() << "serial connected";

            QObject::connect(serial, &QSerialPort::readyRead, this, &serialHandler::serialReceived);
            emit serialConnected();
            connected = 1;
        }
    }

    lastSerial = 0;





    serial->write("W: [2016] [01] [170] [0] [0] [0] [0] [0] [0] /");
    //serial->write("WRITE:[2016] [02] [16] [03] [0] [0] [0] [0] [0] /");
    //serial->write("WRITE:[2016] [16] [8] [168] [0] [0] [0] [9] [0] /");
    //qDebug() << "frame: " << uartToFrame("[2016] [16] [8] [168] [0] [0] [0] [9] [0]").toString();
    //serial->write("WRITE:[2016] [33]");


    serialString = "";

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



//allows external classes to access serial's waitforwritten
bool serialHandler::waitForBytesWritten(int msecs)
{
    return serial->waitForBytesWritten(msecs);
}

//checks if theres a serial connection
bool serialHandler::isConnected()
{
    if(connected == 1){
        return true;
    } else {
        return false;
    }
}


//processing for all received serial messages
//if given READ: command, turn it into a frame
void serialHandler::serialReceived()
{

    rxbuffer = serial->readAll();

    //serial->clear();
    //serial->flush();
    if(!rxbuffer.isEmpty()){
        serialString += QString::fromStdString(rxbuffer.toStdString());
    }
    //serialString.replace(readSerial, "");
    //Debug() << "serial received";
    if(serialString.contains("\\")){
        rxbuffer.clear();
        //readSerial.append(serialString);

    }

    QStringList bufferSplit = serialString.split("\\");
    //qDebug() << "serial: " << serialString;
    int cleared = 1;
    if(bufferSplit.length() > 1){
        for(int i = 0; i<bufferSplit.length()-1; i++){
            if(bufferSplit.at(i)!= ""){

                if(bufferSplit.at(i).contains("R:") && !bufferSplit.at(i).contains("W")){
                    QCanBusFrame tmp = uartToFrame(bufferSplit.at(i));
                  // qInfo() << tmp.toString();
                    //qDebug() << bufferSplit.at(i);
                    if(!tmp.payload().isEmpty()){
                        emit serialFrameReceived(tmp);
                    }

                    //int len = bufferSplit.at(i).length();
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

        if(idList.contains(frame.frameId())){


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
        }
    frame.setPayload(payload);
    //qInfo() << frame.toString();
    return frame;

}

//creates and sends the write message to the esp32
void serialHandler::writeFrame(QCanBusFrame frame)
{
    QString serialMsg = "W: ";
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
    //qDebug() << "framepayload: " << framePayload.length();
    if(framePayload.length()<16){
        for(int i = framePayload.length(); i < 16; i=i+2){
            payload.insert(payload.length()-1, " [0]");
        }
    }
    serialMsg.append(id);
    serialMsg.append(payload);
    serialMsg.append("/");
    //qDebug() << "serial write: " << serialMsg;
    serial->write(serialMsg.toStdString().c_str());
    serial->flush();

}

void serialHandler::setUsefulIDs(QList<uint> id)
{
    idList = id;
}

