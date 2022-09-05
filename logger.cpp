#include "logger.h"

logger::logger(QObject *parent)
    : QObject{parent}
{

}

logger::logger(QObject *parent, canbus *bus, parameter* par, Definition* def) : QObject{parent}
{
    parent = nullptr;
    qDebug() << "logger const";
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &logger::log);
    res = par;
    can = bus;
    definition = def;
}



//sends an ecu request based on defs, then processes the received bytes
void logger::log(){
    if(!can->isConnected()){
        qDebug() << "can't log. no device";
        return;

    } else {
        qDebug() << "balls";
    }
    qDebug() << "rx array: " << definition->getRxLength();
    //res = new parameter[definition->getRxLength()];
    if(definition->getTxLength() > 0){
        qDebug() << "receive tx format";
        qDebug() << "bytes" << definition->getTxBytes();
        qDebug() << "rx array: " << definition->getRxLength();

                //send the bytes
            can->writeFrames(QString("000007E0").toUInt(nullptr, 16), definition->getTxBytes());
                //read the received bytes. filter out acknowledge message
            QByteArray rxmsg = can->readFrames(QString("000007E0").toUInt(nullptr, 16), QString("48").toUInt(nullptr, 16));

                qDebug() << "rxbytes " <<rxmsg;
                if(!rxmsg.isEmpty()){
                    qDebug() << "rx not empty";
                    int pos = -1;
                    for(int i = 0; i < rxmsg.size(); i++){
                        if(fr.base10Value(rxmsg.at(i)) == 232){
                            qDebug() << "found 232: " << i;
                            pos = i+1;
                            break;
                        }
                    }

                    if(pos>-1){
                        int calcpos = 0;
                        for(int i = pos; i<rxmsg.length(); i++){
                            qDebug() << "calcpos: " <<calcpos << "rx: " << definition->getRxBytes(calcpos);
                            if(definition->getRxBytes(calcpos) == 1){
                                qDebug() << "conv" << definition->getConv(calcpos);
                                qDebug() << "offset" << definition->getOffset(calcpos);
                                qDebug() << "raw value" << fr.base10Value(rxmsg.at(i));
                                if(definition->getInvert(calcpos) == 1){
                                    res[calcpos].setValue(fr.base10Value(rxmsg.at(i)), definition->getConv(calcpos), definition->getOffset(calcpos), definition->getInvert(calcpos));
                                }else {
                                res[calcpos].setValue(fr.base10Value(rxmsg.at(i)), definition->getConv(calcpos), definition->getOffset(calcpos));
                                }
                            } else if (definition->getRxBytes(calcpos) == 2){
                                QByteArray tmp = rxmsg.mid(i,2);
                                int val = fr.base10Value(tmp);
                                qDebug() << "tmp: " << val;
                                if(definition->getInvert(calcpos) == 1){
                                    res[calcpos].setValue(val, definition->getConv(calcpos), definition->getOffset(calcpos), definition->getInvert(calcpos));
                                }else{
                                    res[calcpos].setValue(val, definition->getConv(calcpos), definition->getOffset(calcpos));
                                }
                            }
                            res[calcpos].setName(definition->getParamNames(calcpos));
                            res[calcpos].setUnit(definition->getUnit(calcpos));
                            qDebug() << "converted: " << res[calcpos].getName() + ": " << res[calcpos].getValue();
                            calcpos++;
                            if(calcpos >= definition->getRxLength()){
                                break;
                            }
                        }
                    }
                }

            rxmsg.clear();

        }else {
            qDebug() << "error: no device or message";
        }

}

void logger::startLogging()
{
    if(!timer->isActive()){
        timer->start(1000);
    } else {
        stopLogging();
    }
}

void logger::stopLogging()
{
    timer->stop();
    qDebug() << "logging stopped";
}

void logger::createParamArray()
{
    res = new parameter[definition->getRxLength()];
    for(int i = 0; i < definition->getRxLength(); i++){
        res[i].setName(definition->getParamNames(i));
        res[i].setFormat(definition->getFormat(i));
        res[i].setUnit(definition->getUnit(i));
        res[i].setPID(definition->getPID(i));
    }
    qDebug() << "create param array";
    emit setParams(res, definition->getRxLength());
}
