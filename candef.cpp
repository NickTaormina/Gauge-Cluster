#include "candef.h"

canDef::canDef(QObject *parent)
    : QObject{parent}
{

}

const QString &canDef::getName() const
{
    return name;
}

void canDef::setName(const QString &newName)
{
    name = newName;
}

uint canDef::getFrameID() const
{
    return FrameID;
}

void canDef::setFrameID(uint newFrameID)
{
    FrameID = newFrameID;
}

const QStringList &canDef::getBytes() const
{
    return bytes;
}

void canDef::setBytes(const QStringList &newBytes)
{
    bytes = newBytes;
}

float canDef::getConv() const
{
    return conv;
}

void canDef::setConv(float newConv)
{
    conv = newConv;
}

int canDef::getOffset() const
{
    return offset;
}

void canDef::setOffset(int newOffset)
{
    offset = newOffset;
}

const QMap<uint, QString> &canDef::getTargets() const
{
    return targets;
}

void canDef::setTargets(const QMap<uint, QString> &newTargets)
{
    targets = newTargets;
}
