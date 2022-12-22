#ifndef FRAMES_H
#define FRAMES_H
#include <QByteArray>
#include <QString>
#include <QtSerialBus>

//General frame data handling/conversion
class frames
{
public:
    frames();
    int base10Value(QByteArray bytes);
    int base10Value(char c);
    QString bytes2String(QByteArray bytes);
    QByteArray string2Bytes(QString str);
    uint string2Uint(QString str);

    int numDigits(int in);

    QList<QCanBusFrame> bytes2Frames(uint id, QByteArray bytes);

    QByteArray combineResponse(QCanBusFrame frame);
private:
    QByteArray multiPayload;
    int multiPartMsg;
    int msgLength;
};

#endif // FRAMES_H
