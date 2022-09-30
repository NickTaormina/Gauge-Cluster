#include "candata.h"

canData::canData(QObject *parent)
    : QObject{parent}
{

}
canData::canData(canDef *def, int count)
{
    qDebug() << "candata const";
    //populate local variables
    defCount = count;


    //signals
    connect(this, &canData::usefulFrameReceived, this, &canData::processUsefulFrame);
}

//checks if can frame is defined
void canData::receiveCanData(QCanBusFrame frame)
{
    uint id = frame.frameId();
    //qDebug() << "receive id:" << id;
    if(idList.contains(id)){
        emit usefulFrameReceived(frame);
    } else {
        return;
    }
}

void canData::fillData(canDef *def, int count)
{
    defCount = count;
    _def = def;
    qDebug() << "candata set";
    for(int i = 0; i < defCount; i++){
        qDebug() << "can name: " << _def[i].getName();
        qDebug() << "canID: " << _def[i].getFrameID();
       nameList.append(_def[i].getName());
       idList.append(_def[i].getFrameID());
    }
    qDebug() << "id list: " << idList;
}


//emits signal with can status (from def) for cluster to handle
void canData::emitter(QString name, QString status)
{
    if(name == "Turn Signals"){
        emit turnSignal(status);
    } else if(name == "Handbrake"){
        emit handbrake(status);
    } else if(name == "Lights"){
        emit lights(status);
    } else if(name == "Defrost"){
        emit acDefrost(status);
    } else if(name == "AC Fan"){
        emit acFan(status);
    } else if(name == "AC Temp"){
        emit acTemp(status);
    } else if(name == "Neutral Switch"){
        emit neutralSwitch(status);
    } else if(name == "Reverse Switch"){
        emit reverseSwitch(status);
    }
}

//processes can info for def with specific values (ex: lights)
//TODO: processing for bit specific values like defrost
void canData::targetProcess(QMap<uint, QString> t, QByteArray p, int index)
{
    QStringList bytes;
    bytes.append(_def[index].getBytes());
    //qDebug() << "payload: " << p;
    uint x;
    if(bytes.length() == 1){

        x = static_cast<quint8>(p.at(bytes.at(0).toUInt(nullptr, 10))); //can data assumed to be in base 10
        //qDebug() << "bytes:" << bytes.at(0).toUInt(nullptr, 10);
        //qDebug() << "to uint:" << QString::number(static_cast<quint8>(p.at(bytes.at(0).toUInt(nullptr, 10))));
    } else {
        //TODO: implement multiple byte params
        QByteArray tmp;
        for(int i = 0; i<bytes.length(); i++){
            //qDebug() << "tmp append:" << QString::number(p.at(bytes.at(i).toUInt(nullptr, 10)));
            tmp.append(p.at(bytes.at(i).toUInt(nullptr, 10)));
        }
        //qDebug() << "tmp: " << tmp;
        //qDebug() << "to uint:" << QString::number(p.at(bytes.at(0).toUInt(nullptr, 10)));
    }

    //qDebug() << t;
    if(t.contains(x)){
        QString status = t.value(x);
        //qDebug() << "status: " << status;
        emitter(_def[index].getName(), status);
    }


}

//processes can info for def that acts like normal parameter (ex: oil temp)
void canData::valueProcess(QByteArray p, int index)
{
    QStringList bytes;
    bytes.append(_def[index].getBytes());
    QByteArray tmp;
    for(int i = 0; i<bytes.length(); i++){
        tmp.append(p.at(bytes.at(i).toUInt(nullptr, 10)));
    }
    uint x = tmp.toUInt(nullptr, 10);
    double value;
    value = x*_def[index].getConv();
    value = value + _def[index].getOffset();
    emit valueChanged(_def[index].getName(), value);
}

//sends useful frame to be given a value and emitted
void canData::processUsefulFrame(QCanBusFrame frame)
{
    //qDebug() << "useful frame";
    QList<int> indexes;
    for(int i = 0; i<defCount; i++){
        if(idList.at(i) == frame.frameId()){
            //qDebug() << "appending index: " << idList.at(i) << " : " << i;
            indexes.append(i);
        }
    }
    //qDebug() << "indexes length: " << indexes.length();
    QByteArray payload = frame.payload();
    for(int i = 0; i<indexes.length(); i++){
        QMap<uint, QString> targets = _def[indexes.at(i)].getTargets();
        //qDebug() << "targets count: " << targets.count();
        //qDebug() << "targets" << targets;
        if(!targets.isEmpty()){
           // qDebug() << "processing target value: " << frame.toString();
            targetProcess(targets, payload, indexes.at(i));
        } else {
            valueProcess(payload, indexes.at(i));
        }
    }
}
