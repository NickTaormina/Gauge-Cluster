#include "serialhandler.h"


serialHandler::serialHandler(QObject *parent)
    : QObject{parent}
{
    QSerialPortInfo info;
    serial = new QSerialPort(nullptr);
    QList<QSerialPortInfo>portList = info.availablePorts();
    if(portList.size() == 1){
        qDebug() << "serial port: " << portList[0].portName();
        serial->setPortName(portList[0].portName());
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

    serial->write("FRAME:[2016] [33] [227] [212] [248] [227] [213] [0] [0]/");

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
            }
        }
    }

}
