#ifndef PARAMDISPLAY_H
#define PARAMDISPLAY_H

#include <QObject>
#include <QtQuick>
#include <QQmlContext>


class paramDisplay : public QObject
{
    Q_OBJECT
public:
    paramDisplay(QObject *parent, QQmlEngine * engine, QStringList l);
    QStringList getParamList();
public slots:
    void updateValue(QString name, double value);
    void setParamList(QStringList l, int reinit);
    void setFilterList(QStringList filter, int reinit);
    void setCanDisplayList(QStringList l, int reinit);
    void setParamRename(QMap<QString, QString> list);
    void initDisplay();

signals:
private:
    void filterParamList();

    void reInitialize();
    int getNewNumObjects();
    void calcNumObjects();

    QString formatText(QString name, double value);


    QStringList paramList;
    int totalWidth;

    int numParams;
    int numObjects;
    int maxWidth;

    void updateMinMax(int index, double value);

    void clearLists();
    QList<QObject*> paramBaseObjects;
    QList<QObject*> paramLabels;
    QList<QObject*> paramValues;
    QList<QObject*> minLabels;
    QList<QObject*> maxLabels;

    QStringList minValues;
    QStringList maxValues;

    QObject * _parent;
    QQmlEngine * _engine;

    QStringList filterList;

    QMap<QString, QString> renameList;

    void swapTest();
    int swap;
};

#endif // PARAMDISPLAY_H
