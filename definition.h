#ifndef DEFINITION_H
#define DEFINITION_H

#include <QList>
#include <QtXml>



class Definition
{
public:
    Definition();


    int getPID(int i);
    void setPID(QList<int> p);
    void appendPID(int i);

    QByteArray getTxBytes();
    void setTxBytes(QByteArray bytes);
    void appendTx(QByteArray bytes);
    int getTxLength();
    void insertTxBytes(int byte);
    void clearAll();

    int getRxBytes(int pos);
    void setRxBytes(QList<int> rx);
    void appendRx(int rx);
    int getRxLength();

    QString getParamNames(int pos);
    QString getNameFromPID(int id);
    void setParamNames(QList<QString> names);
    void appendParamNames(QString names);
    int getParamLength();

    double getConv(int pos);
    void setConv(QList<double> c);
    void appendConv(double c);
    int getConvLength();

    float getOffset(int pos);
    void setOffset(QList<float> o);
    void appendOffset(float o);
    int getOffsetLength();

    int getInvert(int i);
    void setInvert(QList<int> p);
    void appendInvert(int i);

    QString getUnit(int i);
    void setUnits(QList<QString> s);
    void appendUnits(QString str);

    int getFormat(int i);
    void setFormat(QList<int> f);
    void appendFormat(int i);
private:
    QByteArray txBytes;
    QList<int> pID;
    QList<int> rxBytes;
    QList<QString> paramNames;
    QList<double> conv;
    QList<float> offset;
    QList<int> invert;
    QList<QString> units;
    QList<int> format;




};

#endif // DEFINITION_H
