#include "definition.h"


Definition::Definition()
{
}

void Definition::clearAll(){
    txBytes.clear();
    rxBytes.clear();
    pID.clear();
    conv.clear();
    offset.clear();
    paramNames.clear();
    invert.clear();
    units.clear();
}
int Definition::getPID(int i){
    return pID.at(i);
}
void Definition::setPID(QList<int> p){
    pID = p;
}
void Definition::appendPID(int i){
    pID.append(i);
}

QByteArray Definition::getTxBytes(){
    if(static_cast<quint8>(txBytes.at(0)) != 16){
        txBytes.insert(0, txBytes.length()).toHex();
        txBytes.insert(0, 16).toHex();
    }
    return txBytes;
}
void Definition::setTxBytes(QByteArray bytes){
    txBytes = bytes;
}
void Definition::appendTx(QByteArray bytes){
    txBytes.append(bytes);
}
void Definition::insertTxBytes(int byte){
    txBytes.insert(0, byte).toHex();
}

int Definition::getTxLength(){
    return txBytes.length();
}

int Definition::getRxBytes(int pos){
    return rxBytes.at(pos);
}
void Definition::setRxBytes(QList<int> rx){
    rxBytes = rx;
}
void Definition::appendRx(int rx){
    rxBytes.append(rx);
}
int Definition::getRxLength(){
    return rxBytes.length();
}

QString Definition::getParamNames(int pos){
    return paramNames.at(pos);
}
void Definition::setParamNames(QList<QString> names){
    paramNames = names;
}
void Definition::appendParamNames(QString names){
    paramNames.append(names);
}
QString Definition::getNameFromPID(int id){
    for(int i = 0; i<pID.length(); i++){
        if(pID.at(i) == id){
            return paramNames.at(i);
        }
    }
    return "NULL";
}

double Definition::getConv(int pos){
    return conv.at(pos);
}
void Definition::setConv(QList<double> c){
    conv = c;
}
void Definition::appendConv(double c){
    conv.append(c);
}
int Definition::getConvLength(){
    return conv.length();
}

float Definition::getOffset(int pos){
    return offset.at(pos);
}
void Definition::setOffset(QList<float> o){
    offset = o;
}
void Definition::appendOffset(float o){
    offset.append(o);
}
int Definition::getOffsetLength(){
    return offset.length();
}

int Definition::getInvert(int i){
    return invert.at(i);
}
void Definition::setInvert(QList<int> p){
    invert = p;
}
void Definition::appendInvert(int i){
    invert.append(i);
}

QString Definition::getUnit(int i){
    return units.at(i);
}
void Definition::setUnits(QList<QString> s){
    units = s;
}
void Definition::appendUnits(QString str){
    units.append(str);
}

int Definition::getFormat(int i){
    return format.at(i);
}
void Definition::setFormat(QList<int> f){
    format = f;
}
void Definition::appendFormat(int i){
    format.append(i);
}




