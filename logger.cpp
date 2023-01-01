#include "logger.h"

logger::logger(QObject *parent)
    : QObject{parent}
{

}

logger::logger(QObject *parent, canbus *bus, parameter* par, Definition* def) : QObject{parent}
{
    parent = nullptr;
    timer = new QTimer(this);
    loggerTimer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &logger::sendLogMessage);
    res = par;
    can = bus;
    definition = def;
    paramLength = 0;
    qDebug() << "*logger object initialized";
    logging = 0;
    processing = 0;
    loggerTimer->setInterval(75);
    loggerTimer->setSingleShot(false);
    connect(loggerTimer, &QTimer::timeout, this, &logger::sendLogMessage);
    etimer.start();
}



//sends an ecu request based on defs
void logger::sendLogMessage(){
    //qInfo() << "sending log message";
    //qDebug() << "log message";
    if(!can->isConnected()){
        qDebug() << "can't log. no device";
        stopLogging();
        return;

    }
    if(definition->getTxLength() > 0){
       // //qInfo() << "tx bytes" << definition->getTxBytes().toHex();
        if(loggerTimer->isActive()){
            loggerTimer->start();
        }
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
        //timer->start(100);
        loggerTimer->start();
        qDebug() << "started logging";
        //QObject::connect(this, &logger::ecuResponseParsed, this, &logger::sendLogMessage);

    } else {
        stopLogging();
        timer->stop();
        loggerTimer->stop();
    }
}

//stop request ecu data
void logger::stopLogging()
{
    qDebug() << "logging stopped";
    logging = 0;
    QObject::disconnect(this, &logger::ecuResponseParsed, this, &logger::sendLogMessage);
    //timer->stop();
    loggerTimer->stop();

}

//create the parameter objects needed to store the received information
void logger::createParamArray()
{
    qDebug() << "*create param array: " << definition->getNumParams();
    res = new parameter[definition->getNumParams()];
    for(int i = 0; i < definition->getNumParams(); i++){
        qDebug() << "*setting par: " << i << " : " << definition->getParamNames(i);
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
    //qInfo() << "combine ecu respo: " << frame.toString();
    //qDebug() << "ecu frame: " << frame.toString();
    if(1){
        //qInfo () << "first if: " << frame.payload().at(0);

        if(frame.payload().at(0) == 16){
            //qInfo() << "multi part";
            multiPartMsg = 1;
            msgLength = static_cast<quint8>(frame.payload().at(1));
            //qDebug() << "msg length: " << msgLength;
            if(msgLength < 16 && msgLength > 6){
                msgLength = 16;
            }
            multiPayload.clear();
            multiPayload.append(frame.payload());
        } else if (frame.payload().at(0) <16){
            multiPartMsg = 0;
            //qInfo() << "not mfult";
            parseECUResponse(frame.payload());

        }
        if(frame.payload().at(0) != 48 && frame.payload().at(0) != 16 && msgLength > 6){
            //qDebug() << "combining frame: " << frame.toString();
            if(multiPayload != frame.payload()){
                //qDebug() << "appending";
                multiPayload.append(frame.payload());
            }
            if(msgLength <= multiPayload.length()){
                //qDebug() << "sending frame to process: " << multiPayload.length();
                multiPartMsg = 0;
                msgLength = 0;
                parseECUResponse(multiPayload);
                multiPayload.clear();
            }
        } else if (frame.payload().at(0) == 48){
            //qInfo() << "ack msg";
        }
    }
    //qInfo() << "exiting combine";
}

//converts the ecu response to a usable value
void logger::parseECUResponse(QByteArray rxmsg)
{
    //qInfo() << "multi payload: " << rxmsg;
    //qInfo() << "el time: " << etimer.elapsed();

    if(processing != 2){
        etimer.restart();
        processing = 1;
    if(logging == 1){
    //qDebug() << "rxmsg: " << rxmsg;
    //look for the starting point of the actual useful data. after the first "E8" byte
    //qDebug() << "parse: ";
    if(!rxmsg.isEmpty()){
        int pos = -1;
        for(int i = 0; i < rxmsg.size(); i++){
            if(fr.base10Value(rxmsg.at(i)) == 232){
                //qInfo() << "found E8 byte at: " << i;
                pos = i+1;
                break;
            }
        }
        //iterates through each of the received bytes and uses definitoin to convert them to a usable value. //TODO: make this work with more than 2 byte params
        if(pos>-1){
            int calcpos = 0;
            for(int i = pos; i<rxmsg.length();){
                //qDebug() << "calcpos: " <<calcpos << "rx: " << definition->getRxBytes(calcpos) << " : " << definition->getParamNames(calcpos);
                if(definition->getRxBytes(calcpos) == 1){
                    //qInfo() << "conv" << definition->getConv(calcpos);
                    //qInfo() << "offset" << definition->getOffset(calcpos);
                    //qInfo() << "raw value" << fr.base10Value(rxmsg.at(i));
                    if(definition->getSignedVal(calcpos) == 1){
                        //qDebug() << "signed: " << definition->getParamNames(calcpos);
                        if(fr.base10Value(rxmsg.at(i)) >= 128){
                            if(definition->getInvert(calcpos) == 1){
                                res[calcpos].setValue(fr.base10Value(rxmsg.at(i)) - 255, definition->getConv(calcpos), definition->getOffset(calcpos), definition->getInvert(calcpos));
                            } else {
                                res[calcpos].setValue(fr.base10Value(rxmsg.at(i)) - 255, definition->getConv(calcpos), definition->getOffset(calcpos));
                            }
                        }
                    } else {
                        if(definition->getInvert(calcpos) == 1){
                            res[calcpos].setValue(fr.base10Value(rxmsg.at(i)), definition->getConv(calcpos), definition->getOffset(calcpos), definition->getInvert(calcpos));
                        } else {
                            res[calcpos].setValue(fr.base10Value(rxmsg.at(i)), definition->getConv(calcpos), definition->getOffset(calcpos));
                        }
                    }

                    i = i + 1;
                } else if (definition->getRxBytes(calcpos) == 2){
                    QByteArray tmp = rxmsg.mid(i,2);
                    int val = fr.base10Value(tmp);
                    //qInfo() << "raw 2 byte val: " << val;
                    if(definition->getInvert(calcpos) == 1){
                        res[calcpos].setValue(val, definition->getConv(calcpos), definition->getOffset(calcpos), definition->getInvert(calcpos));
                    }else{
                        res[calcpos].setValue(val, definition->getConv(calcpos), definition->getOffset(calcpos));
                    }
                    i = i + 2;
                }
                res[calcpos].setName(definition->getParamNames(calcpos));
                res[calcpos].setUnit(definition->getUnit(calcpos));
                //qDebug() << res[calcpos].getName() + ": " << res[calcpos].getValue();
                emit paramUpdated(res[calcpos].getName(), res[calcpos].getValue());

                calcpos++;
                //break when all required params are converted, even if there are extra bytes at the end
                if(calcpos >= definition->getNumParams()){
                    //qInfo() << "all params done";
                    emit ecuResponseParsed();
                    break;
                }
                //qInfo() << "for loop it";
            }
        }
    //qInfo() << "response fully parsed";

    processing = 0;
    }
    }
}

}
