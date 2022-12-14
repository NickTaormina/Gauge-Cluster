#include "candata.h"

canData::canData(QObject *parent)
    : QObject{parent}
{

}
canData::canData(canDef *def, int count)
{
    qDebug() << "*candata construct";
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
    qDebug() << "*candata set";
    for(int i = 0; i < defCount; i++){
        qDebug() << "*can name: " << _def[i].getName();
        qDebug() << "*canID: " << _def[i].getFrameID();
       nameList.append(_def[i].getName());
       idList.append(_def[i].getFrameID());
    }
    idList.append(2024);
    idList.append(321);
    qDebug() << "*can id list: " << idList;

}

void canData::getUsefulIDs()
{
    emit usefulIDsFound(idList);
}


//emits signal with can STATUS (from def) for cluster to handle
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
    } else if(name == "Clutch Switch"){
        emit clutchSwitch(status);
    } else if (name == "CEL"){
        emit checkEngineLight(status);
    } else if (name == "Cruise" || name == "Cruise Active"){
        emit cruiseStatusChanged(status);
    } else if (name == "TPMS"){
        emit tpmsLight(status);
    } else if (name == "Oil Light"){
        emit oilLight(status);
    } else if (name == "Seatbelt"){
        emit seatbeltLight(status);
    } else if (name == "Door"){
        emit doorLight(status);
    }
}

//emits signal with can VALUE (from def) for cluster to handle
void canData::valueEmitter(QString name, double value)
{
    if(name == "Vehicle Speed"){
        emit speedChanged(value);
    } else {
        emit valueChanged(name, value);
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
        if(p.length() < bytes.at(0).toInt(nullptr, 10)){
            return;
        } else {
            x = static_cast<quint8>(p.at(bytes.at(0).toUInt(nullptr, 10))); //can data assumed to be in base 10
            //qDebug() << "bytes:" << bytes.at(0).toUInt(nullptr, 10);
            //qDebug() << "to uint:" << QString::number(static_cast<quint8>(p.at(bytes.at(0).toUInt(nullptr, 10))));
        }
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
    //qDebug() << "bytes: " << bytes;
    double value;
    uint x;
    if(bytes.length() == 1){
        //error checking for broken payloads
        if(p.length() < bytes.at(0).toInt(nullptr, 10)){
            return;
        }else {
            x = static_cast<quint8>(p.at(bytes.at(0).toUInt(nullptr, 10)));
            //qDebug() << "x:" << QString::number(static_cast<quint8>(p.at(bytes.at(0).toUInt(nullptr, 10))));
        }
    } else {
        QString tmp;
        for(int i = 0; i<bytes.length(); i++){
            //error check
            if(p.length() < bytes.at(i).toInt(nullptr, 10)){
                return;
            }else {
                //convert payload value to binary. it didnt like gaing straight to a string
                QString val = QString::number(static_cast<quint8>(p.at(bytes.at(i).toUInt(nullptr, 10))), 2);
                //qDebug() << "val: " << val;

                //add padding zeroes
                if(val.length() < 8){
                    for(int u = val.length(); u<8; u++){
                        val.prepend("0");
                    }
                }

                //add in correct endian order. 1 = big ( i think)
                if(_def[index].getEndian() == 1){
                    tmp.append(val);
                }else{
                    tmp.prepend(val);
                }
            }
        }
        //convert binary back into usable value
        x = tmp.toUInt(nullptr, 2);
    }


    //run value through def equation
    value = x*_def[index].getConv();
    value = value + _def[index].getOffset();

    //emit converted value
    valueEmitter(_def[index].getName(), value);
}

void canData::rpmProcess(QByteArray p)
{
    if(p.length() < 6){
        return;
    } else {
        quint8 byte4 = static_cast<quint8>(p.at(4));
        quint8 byte5 = static_cast<quint8>(p.at(5)) & 0x1F;
        uint rpm = (byte5 << 8) | byte4;
        emit rpmChanged(rpm);
    }
}

//bit defs start from the RIGHT
void canData::bitProcess(QMap<uint, QString> t, QByteArray p, QStringList b, int index)
{
    QStringList bytes;
    bytes.append(_def[index].getBytes());
    //qDebug() << "bit: " << _def[index].getName();
    int length = b.length();
    //error checking for small payload
    if(p.length() < bytes.at(0).toInt(nullptr, 10)){
        return;
    } else {
        if(length == 1){
            int bitIndex = b.at(0).toInt(nullptr, 10);
            QString bit = QString::number(static_cast<quint8>(p.at(bytes.at(0).toUInt(nullptr, 10))), 2);
            //qDebug() << bit;
            if(bit.length() < 8){
                for(int i = bit.length(); i<8; i++){
                    bit.prepend("0");
                }
            }
            //qDebug() << "processed: " << bit;
            bitIndex = 8-bitIndex-1;
            //qDebug() << "bit index: " << bitIndex;
            QString valString = (bit.at(bitIndex));
            //qDebug() << "val string: " << valString;
            uint val = valString.toUInt(nullptr, 10);
            if(t.contains(val)){
                QString status = t.value(val);
                emitter(_def[index].getName(), status);
            }
        } else {
         //TODO: work with defs with multiple bit addr
        }
    }


}

//processes fuel frame and returns resistance
void canData::fuelProcess(QByteArray p)
{
    if(p.length() < 6){
        return;
    } else {
        quint8 byte1 = static_cast<quint8>(p.at(0));
        quint8 byte2 = static_cast<quint8>(p.at(1)) & 0x0F;
        uint tmp = (byte2 << 8) | byte1;

        float fuel = (float)tmp/2;
        emit fuelChanged(fuel);
    }
}

void canData::gearProcess(QByteArray p)
{
    if(p.length() < 7){
        return;
    } else {
        int gear = static_cast<quint8>(p.at(6)) & 0x07;


        QString status;
        if(gear == 7){
            status = "N";
        } else {
            status = QString::number(gear);
        }
        emit gearChanged(status);
    }
}

//sends useful frame to be given a value and emitted
void canData::processUsefulFrame(QCanBusFrame frame)
{
    //qDebug() << frame.toString();
    QList<int> indexes;
    for(int i = 0; i<defCount; i++){
        if(idList.at(i) == frame.frameId()){
            indexes.append(i);
        }
    }
    //rpm processing is hard coded in, because its too complicated for config
    if(frame.frameId() == 321){
        rpmProcess(frame.payload());
        gearProcess(frame.payload());
    }

    //fuel processing
    if(frame.frameId() == 642){
        fuelProcess(frame.payload());
    }
    QByteArray payload = frame.payload();
    for(int i = 0; i<indexes.length(); i++){
        QMap<uint, QString> targets = _def[indexes.at(i)].getTargets();
        //qDebug() << "name: " << _def[indexes.at(i)].getName();
        if(!targets.isEmpty()){
            if(!_def[indexes.at(i)].getBits().isEmpty()){
                bitProcess(targets, frame.payload(), _def[indexes.at(i)].getBits(), indexes.at(i));
            } else {
                targetProcess(targets, payload, indexes.at(i));
            }

        } else {
            valueProcess(payload, indexes.at(i));
        }
    }
}
