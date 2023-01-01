#ifndef PARAMETER_H
#define PARAMETER_H

#include <QObject>

class parameter : public QObject
{
    Q_OBJECT
public:
    explicit parameter(QObject *parent = nullptr);
    parameter(QObject *parent, int pid, QString name, double val);
    void setPID(int i);
    int getPID();

    void setName(QString n);
    QString getName();

    void setUnit(QString u);
    QString getUnit();
    void setValue(double val);
    double getValue();
    double getValue(int val, double conv, float offset, int invert);
    double getValue(int val, double conv, float offset);

    void setValue(int val, double conv, float offset);
    void setValue(int val, double conv, float offset, int invert);

    void setFormat(int i);
    int getFormat();

    void setSigned(int i);
    int getSigned();
signals:
    void valueChanged();
private:
    int pID;
    QString name;
    double value;
    QString unit;
    int format;
    int signedVal;


};
#endif // PARAMETER_H
