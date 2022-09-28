#include "serialhandler.h"


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
        serial->setBaudRate(QSerialPort::Baud115200);
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

    serial->write("WRITE:[2016] [01] [170] [0] [0] [0] [0] [0] [0] /");

    serialString = "";
}


void serialHandler::serialReceived()
{
    rxbuffer = serial->readAll();
    serialString += QString::fromStdString(rxbuffer.toStdString());
    QStringList bufferSplit = serialString.split("\\");
    if(bufferSplit.length() > 1){
        for(int i = 0; i<bufferSplit.length(); i++){
            if(bufferSplit.at(i)!= ""){
                qDebug() << "split: " << i << " : " << bufferSplit.at(i);
                if(bufferSplit.at(i).contains("READ:")){
                    //do stuff with read message
                    emit messageRead();
                    qDebug() << "READ: ";
                }
                rxbuffer.clear();
                serialString.clear();
            }
        }
    }
    if(bufferSplit.length() > 20){
        bufferSplit.clear();
    }

}

//fills in frame when messageRead is called
QCanBusFrame serialHandler::readFrame()
{

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
    qDebug() << "sent serial frame: " << serialMsg.toStdString().c_str();



}

