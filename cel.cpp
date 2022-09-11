#include "cel.h"

/*
check engine light management. hadnles reading and clearing CELs
*/
cel::cel(QObject *parent)
    : QObject{parent}
{

}
cel::cel(QObject *parent, canbus *bus, config *c)
{
    parent = nullptr;
    hasCEL = 0;
    cfg = c;
    celbytes = fr.string2Bytes(cfg->getValue("cel"));
}

//sends & reads the message to check codes

void cel::readCELS()
{
    QByteArray rxmsg;
    /*
    accessport splits into multiple types of cel requests. going to need a read-write for each type
    */
    //init request expects 02E8FF
    can->writeFrames(fr.string2Uint("000007E0"), fr.string2Bytes("05A80000006200"));
    rxmsg = can->readFrames(fr.string2Uint("000007E8"), fr.string2Uint("02"), 0); //02E8FF
    qDebug() << "cel init receive: " << rxmsg;

    //second init
    can->writeFrames(fr.string2Uint("000007E0"), fr.string2Bytes("02010000000000"));
    rxmsg = can->readFrames(fr.string2Uint("000007E8"), fr.string2Uint("06"), 0); //expects 064100BE3DA813
    qDebug() << "cel init receive: " << rxmsg;

    //third init
    can->writeFrames(fr.string2Uint("000007E0"), fr.string2Bytes("01030000000000"));
    rxmsg = can->readFrames(fr.string2Uint("000007E8"), fr.string2Uint("02"), 0); //expects 02430000000000
    qDebug() << "cel init receive: " << rxmsg;

    //fourth init
    can->writeFrames(fr.string2Uint("000007E0"), fr.string2Bytes("01070000000000"));
    rxmsg = can->readFrames(fr.string2Uint("000007E8"), fr.string2Uint("02"), 0); //expects 02470000000000
    qDebug() << "cel init receive: " << rxmsg;

    //trans init
    can->writeFrames(fr.string2Uint("000007E1"), fr.string2Bytes("01030000000000"));
    //rxmsg = can->readFrames(fr.string2Uint("000007E8"), fr.string2Uint("02"), 0); //nothing came back
    //qDebug() << "cel init receive: " << rxmsg;

    //ecu ID request
    can->writeFrames(fr.string2Uint("000007E0"), fr.string2Bytes("01AA0000000000"));
    rxmsg = can->readFrames(fr.string2Uint("000007E8")); //expects long message. ecu ID starts on byte 7 (including page#). its 7 bytes long including page #
    //there is an 04 at pos 1 on page 27 and a 20 pos 1 on page 28
    qDebug() << "cel init receive: " << rxmsg;


    //first set. asks for a bunch of params in long message then expects 00s back
    can->writeFrames(fr.string2Uint("000007E0"), celbytes);
    rxmsg = can->readFrames(fr.string2Uint("000007E8"), fr.string2Uint("48"), 1);
    qDebug() << "received cel message: " << rxmsg;

    //everything here and after expects 00 values in return only
    //check if any values are not 00

    //TODO: fill in the rest of the messages
    //TODO: figure out what each section of CELs is for


}

void cel::setCelBytes(QByteArray txmsg)
{
    celbytes.append(txmsg);
}


