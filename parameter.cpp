#include "parameter.h"
#include "qdebug.h"
#include <qmath.h>
parameter::parameter(QObject *parent)
    : QObject{parent}
{

}

parameter::parameter(QObject *parent, int pid, QString name, double val) : QObject{parent}{
    parent = nullptr;
    setPID(pid);
    setName(name);
    setValue(val);
}

void parameter::setPID(int i){
    pID = i;
}
int parameter::getPID(){
    return pID;
}

void parameter::setName(QString n){
    name = n;
}
QString parameter::getName(){
    return name;
}

void parameter::setUnit(QString u){
    unit = u;
}
QString parameter::getUnit(){
    return unit;
}
void parameter::setValue(double val){
    value = val;
    //emit valueChanged();
}
double parameter::getValue(){
    return value;
}
double parameter::getValue(int val, double conv, float offset, int invert){
    if(invert == 1){
        if(val != 0){
        return (conv/(double)val) + offset;
        }
    } else {
        return ((double)val*conv) + offset;
    }
    return 0;
}
double parameter::getValue(int val, double conv, float offset){
    value = ((double)val*conv) + offset;
    return value;
}


void parameter::setValue(int val, double conv, float offset){
    value = ((double)val*conv) + offset;
}

float parameter::bytesToFloat(QByteArray bytes){
    union {
        unsigned char b[4];
        float f;
      } u;
      u.b[0] = bytes.at(3);
      u.b[1] = bytes.at(2);
      u.b[2] = bytes.at(1);
      u.b[3] = bytes.at(0);
      return u.f;
}
void parameter::setValue(QByteArray val, double conv, float offset)
{
    float _f = parameter::bytesToFloat(val);
   // qDebug() << "float: " << _f;
    value = (double)_f*conv + offset;

}
void parameter::setValue(int val, double conv, float offset, int invert){
    if(invert == 1){
        if(val != 0){
        value = (conv/(double)val) + offset;
        }
    } else {
        value = ((double)val*conv) + offset;
    }
}
void parameter::setFormat(int i){
    format = i;
}
int parameter::getFormat(){
    return format;
}

void parameter::setSigned(int i)
{
    signedVal = i;
}

int parameter::getSigned()
{
    return signedVal;
}
