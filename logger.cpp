#include "logger.h"

logger::logger(QObject *parent)
    : QObject{parent}
{

}

logger::logger(QObject *parent, canbus *bus, parameter* par, Definition* def) : QObject{parent}
{
    parent = nullptr;
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &logger::sendLogMessage);
    res = par;
    can = bus;
    definition = def;
    paramLength = 0;
    qDebug() << "logger object initialized";
}



//sends an ecu request based on defs
void logger::sendLogMessage(){
    if(!can->isConnected()){
        qDebug() << "can't log. no device";
        timer->stop();
        return;

    }
    if(definition->getTxLength() > 0){
        qDebug() << "tx bytes" << definition->getTxBytes().toHex();
        qDebug() << "rx array size: " << definition->getRxLength();

            //send the bytes
            can->writeFrames(fr.string2Uint("000007E0"), definition->getTxBytes());

        } else {
            qDebug() << "error: no device or message";
        }

}

//starts the timer responsible for requesting ecu data
void logger::startLogging()
{
    if(logging == 0){
        logging = 1;
        sendLogMessage();
        QObject::connect(this, &logger::ecuResponseParsed, this, &logger::sendLogMessage);

    } else {
        stopLogging();
    }
}

//stop request ecu data
void logger::stopLogging()
{
    qDebug() << "logging stopped";
    logging = 0;
    QObject::disconnect(this, &logger::ecuResponseParsed, this, &logger::sendLogMessage);
    timer->stop();

}

//create the parameter objects needed to store the received information
void logger::createParamArray()
{
    qDebug() << "create param array: " << definition->getNumParams();
    res = new parameter[definition->getNumParams()];
    for(int i = 0; i < definition->getNumParams(); i++){
        qDebug() << "setting par: " << i << " : " << definition->getParamNames(i);
        res[i].setName(definition->getParamNames(i));
        res[i].setFormat(definition->getFormat(i));
        res[i].setUnit(definition->getUnit(i));
        res[i].setPID(definition->getPID(i));
    }
    paramLength = definition->getNumParams();

    emit setParams(res, paramLength);
}

//reads the response from the ecu and combines frames if necessary
void logger::combineECUResponse(QCanBusFrame frame)
{
    if(logging == 1){
        qDebug() << "combine ecu";
        if(frame.payload().at(0) == 16){
            msgLength = static_cast<quint8>(frame.payload().at(1));
        } else {
            parseECUResponse(frame.payload());
            return;
        }
        multiPayload.append(frame.payload());
        if(msgLength >= multiPayload.length()){

            parseECUResponse(multiPayload);
            multiPayload.clear();
        }
    }
}

//converts the ecu response to a usable value
void logger::parseECUResponse(QByteArray rxmsg)
{
    if(logging == 1){
    //qDebug() << "rxmsg: " << rxmsg;
    //look for the starting point of the actual useful data. after the first "E8" byte
    //qDebug() << "parse: ";
    if(!rxmsg.isEmpty()){
        int pos = -1;
        for(int i = 0; i < rxmsg.size(); i++){
            if(fr.base10Value(rxmsg.at(i)) == 232){
                qDebug() << "found E8 byte at: " << i;
                pos = i+1;
                break;
            }
        }
        //iterates through each of the received bytes and uses definitoin to convert them to a usable value. //TODO: make this work with more than 2 byte params
        if(pos>-1){
            int calcpos = 0;
            for(int i = pos; i<rxmsg.length();){
                qDebug() << "calcpos: " <<calcpos << "rx: " << definition->getRxBytes(calcpos);
                if(definition->getRxBytes(calcpos) == 1){
                    qDebug() << "conv" << definition->getConv(calcpos);
                    qDebug() << "offset" << definition->getOffset(calcpos);
                    qDebug() << "raw value" << fr.base10Value(rxmsg.at(i));
                    if(definition->getInvert(calcpos) == 1){
                        res[calcpos].setValue(fr.base10Value(rxmsg.at(i)), definition->getConv(calcpos), definition->getOffset(calcpos), definition->getInvert(calcpos));
                    } else {
                        res[calcpos].setValue(fr.base10Value(rxmsg.at(i)), definition->getConv(calcpos), definition->getOffset(calcpos));
                    }
                    i = i + 1;
                } else if (definition->getRxBytes(calcpos) == 2){
                    QByteArray tmp = rxmsg.mid(i,2);
                    int val = fr.base10Value(tmp);
                    qDebug() << "raw 2 byte val: " << val;
                    if(definition->getInvert(calcpos) == 1){
                        res[calcpos].setValue(val, definition->getConv(calcpos), definition->getOffset(calcpos), definition->getInvert(calcpos));
                    }else{
                        res[calcpos].setValue(val, definition->getConv(calcpos), definition->getOffset(calcpos));
                    }
                    i = i + 2;
                }
                res[calcpos].setName(definition->getParamNames(calcpos));
                res[calcpos].setUnit(definition->getUnit(calcpos));
                qDebug() << res[calcpos].getName() + ": " << res[calcpos].getValue();
                emit paramUpdated(res[calcpos].getName(), res[calcpos].getValue());
                calcpos++;
                //break when all required params are converted, even if there are extra bytes at the end
                if(calcpos >= definition->getNumParams()){
                    break;
                }

            }
        }

        emit ecuResponseParsed();
    }
    }
}
