#ifndef FRAMES_H
#define FRAMES_H
#include <QByteArray>
#include <QString>

class frames
{
public:
    frames();
    int base10Value(QByteArray bytes);
    int base10Value(char c);
    QString bytes2String(QByteArray bytes);
    QByteArray string2Bytes(QString str);
};

#endif // FRAMES_H
