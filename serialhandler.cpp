#include "serialhandler.h"
#include <QEventLoop>
#include <QTimer>

serialHandler::serialHandler(QObject *parent)
    : QObject{parent}
{
    qDebug() << "serial handler wrong constructor";

}

serialHandler::serialHandler(QObject *parent, config *c)
{

    connected = 0;
    QSerialPortInfo info;
    serial = new QSerialPort(nullptr);
    QList<QSerialPortInfo>portList = info.availablePorts();
    if(portList.size() >= 1){
        QString configPort = c->getValue("comPort");
        qDebug() << "config port: " << configPort;
        for(int i = 0; i<portList.length(); i++){
            qDebug() << "serial port found: " << portList[i].portName();
            if(1){
                qDebug() << "connecting to port: " << portList[i].portName();
                serial->setPortName(portList[i].portName());
            }
        }

        qDebug() << "* init serial settings";
        serial->setBaudRate(2000000);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);

        if(!serial->open(QIODevice::ReadWrite)){
            qDebug() << "serial connection fail";
            //emit serialConnected();
        } else {
            serial->clear();
            serial->flush();
            serial->flush();
            qDebug() << "serial connected";

            QObject::connect(serial, &QSerialPort::readyRead, this, &serialHandler::serialReceived);
            emit serialConnected();
            connected = 1;
        }
    }

    counter = 1;
    lastSerial = 0;





    //serial->write("W: [2016] [01] [170] [0] [0] [0] [0] [0] [0] /");
    //serial->write("WRITE:[2016] [02] [16] [03] [0] [0] [0] [0] [0] /");
    //serial->write("WRITE:[2016] [16] [8] [168] [0] [0] [0] [9] [0] /");
    //qDebug() << "frame: " << uartToFrame("[2016] [16] [8] [168] [0] [0] [0] [9] [0]").toString();
    //serial->write("WRITE:[2016] [33]");
    //serial->write("W: [642] [45] [178] [0] [139] [139] [17] [42] [0] /");




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
    //qDebug() << "serial received" << serialString;


    if(serialString.contains("\r")){
        rxbuffer.clear();
        //readSerial.append(serialString);

    }
    if(serialString.contains("penis")){
        qDebug() << "esp error";
        rxbuffer.clear();
    }
    QStringList bufferSplit = serialString.split("\r");
    //qDebug() << "serial: " << serialString;
    int cleared = 1;
    if(bufferSplit.length() > 1){
        for(int i = 0; i<bufferSplit.length()-1; i++){
            if(bufferSplit.at(i)!= ""){

                if(bufferSplit.at(i).contains("t") && !bufferSplit.at(i).contains("W")){
                    QCanBusFrame tmp = uartToFrame(bufferSplit.at(i));
                  // qInfo() << tmp.toString();
                    //qDebug() << bufferSplit.at(i);
                    if(!tmp.payload().isEmpty()){
                        emit serialFrameReceived(tmp);
                    }

                    //int len = bufferSplit.at(i).length();
                    serialString.remove(bufferSplit.at(i)+"\r");

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
    //qDebug() << msg;
    QCanBusFrame frame;
    int i = 0;
    if(msg.length() < 7){
        return frame;
    }
    //loop for frame id
    while(i < msg.length()){
        if(msg.at(i) == "t"){
            //if(msg.at(i+1)==":"){
                QString tmp;
                int x = 1;
                if(!msg.at(i+x+2).isNull()){
                    tmp.append(msg.mid(i+x,3));
                    frame.setFrameId(tmp.toUInt(nullptr, 16));
                } else {
                    qDebug() << "invalid uart msg: length";
                    return frame;
                }
                i = i+x+3;
                break;
            //} else {
            //    qDebug() << "invalid uart msg, R:";
            //    return frame;
            //}
        }
        i = i + 1;
    }

    QString tmp;
    tmp.append(msg.at(i));
    if(tmp.toUInt(nullptr, 10)*2 != msg.length() - i-1){
        //qDebug() << "message length: " << msg.length() - i;
        return frame;
    }
    i = i+1;

    QByteArray payload;
    //if(idList.contains(frame.frameId())){
    if(1){
        if(msg.length() > i+1 && !msg.at(i).isNull()){
            int payLength = msg.length() - i;

            if(payLength % 2 == 0){
                while(i<msg.length()){
                    QString tmp;
                    if(msg.length() >= i+2){
                        tmp.append(msg.mid(i,2));
                        payload.append(tmp.toUInt(nullptr, 16));
                        i = i + 2;
                    }else{
                        break;
                    }
                }
            } else {
                qDebug() << "frame byte error";
                return frame;
            }

            if(payload.length() > 8){
                qDebug() << "frame payload length error: " << msg;
                return frame;
            }
        }
    }

    frame.setPayload(payload);
    qInfo() << frame.toString();
    //qDebug() << frame.toString();
    return frame;


}

//creates and sends the write message to the esp32
void serialHandler::writeFrame(QCanBusFrame frame)
{
    QString serialMsg = "";
    QString id = QString::number(frame.frameId(),16);
    QString length = QString::number(frame.payload().length(), 16);
    QString framePayload = fr.bytes2String(frame.payload()); //local copy of the payload
    QString payload; //formatted payload that will be sent
    int x = 0;
    for(int i = 0; i<=framePayload.length()-1; i = i+2){
        payload.append(framePayload.at(i));
        payload.append(framePayload.at(i+1));

    }
    //qDebug() << "framepayload: " << framePayload.length();
    if(framePayload.length()<16){
        for(int i = framePayload.length()-1; i < 16; i++){
            payload.insert(i, "0");
        }
    }
    //qDebug() << "appending";
    serialMsg.append(id);
    serialMsg.append(length);
    serialMsg.append(payload);
    serialMsg = serialMsg.toUpper();
    serialMsg.prepend("t");
    serialMsg.append("\r");

   // qDebug() << "serial write: " << serialMsg;
    /*QString ct = QString::number(counter, 10);
    QString msg = "W: [640] [1] [" + ct + "] [16] [0] [0] [0] [0] [0] /";
    //QString msg = "W: [642] [45] [" + ct + "] [0] [139] [139] [17] [42] [0] /";
    serial->write(msg.toStdString().c_str());
    counter = counter + 1;
    if(counter > 16){
        counter = 1;
    }*/



    serial->write(serialMsg.toStdString().c_str());
    serial->flush();

}

void serialHandler::setUsefulIDs(QList<uint> id)
{
    idList = id;
}


