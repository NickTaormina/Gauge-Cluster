#include "definition.h"

/*
ECU definition object that stores all parameter def information
*/
Definition::Definition()
{
}

//get total number of defined parameters
int Definition::getNumParams()
{
return paramNames.length();
}

//remove all defined parameters
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

//returns parameter's PID
int Definition::getPID(int i){
    return pID.at(i);
}

//set PIDs for all parameters
void Definition::setPID(QList<int> p){
    pID = p;
}

//add PID
void Definition::appendPID(int i){
    pID.append(i);
}

//returns the transmit payload to log defined parameters
QByteArray Definition::getTxBytes(){
    if(static_cast<quint8>(txBytes.at(0)) != 16){
        txBytes.insert(0, txBytes.length()).toHex();
        txBytes.insert(0, 16).toHex();
    }
    return txBytes;
}

//set transmission payload
void Definition::setTxBytes(QByteArray bytes){
    txBytes = bytes;
}
//add to the end of transmission payload
void Definition::appendTx(QByteArray bytes){
    txBytes.append(bytes);
}
//add to beginning of transmission payload (for message number)
void Definition::insertTxBytes(int byte){
    txBytes.insert(0, byte).toHex();
}

//returns length of transmission payload
int Definition::getTxLength(){
    return txBytes.length();
}

//returns the response length for given parameter
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

int Definition::getRxMessageLength()
{
    int count = 0;
    for(int i = 0; i<rxBytes.length(); i++){
        count = count + rxBytes.at(i);
    }
    return count;
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

int Definition::getSignedVal(int i) const
{
    return signedVal.at(i);
}

void Definition::appendSignedVal(int i)
{
    signedVal.append(i);
}

void Definition::appendType(QString t)
{
    type.append(t);
}

QString Definition::getType(int i)
{
    return type.at(i);
}







