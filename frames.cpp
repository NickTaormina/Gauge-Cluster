#include "frames.h"
/*
    Various functions for making CAN data easier to work with.
*/
frames::frames()
{

}

//converts frame to readable numbers
int frames::base10Value(QByteArray bytes){
     return bytes.toHex().toUInt(nullptr, 16);
}

//converts a single byte to decimal
int frames::base10Value(char c){
    return static_cast<quint8>(c);
}

//converts payload to string to ui/logging
QString frames::bytes2String(QByteArray bytes){
    return QString(bytes.toHex()).toUpper();
}

//converts string to payload for user frame transmission
QByteArray frames::string2Bytes(QString str){
    str.remove(' ');
    str.remove('\n');
    return QByteArray::fromHex(str.toLatin1());
}

uint frames::string2Uint(QString str)
{
    return str.toUInt(nullptr, 16);
}

