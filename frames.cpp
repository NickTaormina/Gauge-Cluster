#include "frames.h"

frames::frames()
{

}

int frames::base10Value(QByteArray bytes){
     return bytes.toHex().toUInt(nullptr, 16);
}

int frames::base10Value(char c){
    return static_cast<quint8>(c);
}

QString frames::bytes2String(QByteArray bytes){
    return QString(bytes.toHex()).toUpper();
}
QByteArray frames::string2Bytes(QString str){
    str.remove(' ');
    str.remove('\n');
    return QByteArray::fromHex(str.toLatin1());
}
