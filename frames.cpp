#include "frames.h"
/*
    Various functions for making CAN data easier to work with.
*/
frames::frames()
{

}

//converts frame to readable numbers
int frames::base10Value(QByteArray bytes){
    int val = 0;
        for (int i = 0; i < bytes.size(); ++i) {
            val = (val << 8) | (bytes.at(i) & 0xff);
        }
        return val;
    // return bytes.toHex().toUInt(nullptr, 16);
}

//converts a single byte to decimal
int frames::base10Value(char c){
    return static_cast<quint8>(c);
}

//converts payload to string to ui/logging
QString frames::bytes2String(QByteArray bytes){
    return QString(bytes.toHex()).toUpper();
}

//returns the number of digits in input
int frames::numDigits(int in)
{
    int l = 1;
    //gets number of digits
    while(in/=10){
        l++;
    }
    return l;
}


QList<QCanBusFrame> frames::bytes2Frames(uint id, QByteArray bytes)
{

}

QByteArray frames::combineResponse(QCanBusFrame frame)
{
    if(frame.payload().at(0) == 16){
        //qInfo() << "multi part";
        multiPartMsg = 1;
        msgLength = static_cast<quint8>(frame.payload().at(1));
        //qDebug() << "msg length: " << msgLength;
        if(msgLength < 16 && msgLength > 6){
            msgLength = 16;
        }
        multiPayload.clear();
        multiPayload.append(frame.payload());
    } else if (frame.payload().at(0) <16){
        multiPartMsg = 0;
        //qInfo() << "not mfult";
        return frame.payload();

    }
    if(frame.payload().at(0) != 48 && frame.payload().at(0) != 16 && msgLength > 6){
        //qDebug() << "combining frame: " << frame.toString();
        if(multiPayload != frame.payload()){
            //qDebug() << "appending";
            multiPayload.append(frame.payload());
        }
        if(msgLength <= multiPayload.length()){
            //qDebug() << "sending frame to process: " << multiPayload.length();
            multiPartMsg = 0;
            msgLength = 0;
            return multiPayload;
            multiPayload.clear();
        }
    } else if (frame.payload().at(0) == 48){
        //qInfo() << "ack msg";
    }

    return NULL;
}

//converts string to payload for user frame transmission
QByteArray frames::string2Bytes(QString str){
    str.remove(' ');
    str.remove('\n');
    return QByteArray::fromHex(str.toLatin1());
}

//turns string to uint for frame id
uint frames::string2Uint(QString str)
{
    return str.toUInt(nullptr, 16);
}

