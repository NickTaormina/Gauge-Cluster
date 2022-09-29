#ifndef CANDEF_H
#define CANDEF_H

#include <QObject>
#include <QMap>

class canDef : public QObject
{
    Q_OBJECT
public:
    explicit canDef(QObject *parent = nullptr);

    const QString &getName() const;
    void setName(const QString &newName);

    uint getFrameID() const;
    void setFrameID(uint newFrameID);

    const QStringList &getBytes() const;
    void setBytes(const QStringList &newBytes);

    float getConv() const;
    void setConv(float newConv);

    int getOffset() const;
    void setOffset(int newOffset);

    const QMap<uint, QString> &getTargets() const;
    void setTargets(const QMap<uint, QString> &newTargets);

signals:

private:
    QString name;
    uint FrameID;
    QStringList bytes;
    float conv;
    int offset;
    QMap<uint, QString> targets;
};

#endif // CANDEF_H
