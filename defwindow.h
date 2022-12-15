#ifndef DEFWINDOW_H
#define DEFWINDOW_H

#include <QObject>
#include <QFile>
#include <QtXml>
#include <QList>
#include "definition.h"
#include "confighandler.h"


class defWindow : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString defPath READ defPath WRITE setDefPath NOTIFY defPathChanged)
    Q_PROPERTY(QStringList params READ params WRITE setParams NOTIFY paramsChanged)
    Q_PROPERTY(QStringList selectedParams READ selectedParams NOTIFY selectedParamsChanged)
public:
    explicit defWindow(QObject *parent = nullptr);
    defWindow(QObject *parent, Definition * d);

    QString defPath(); // returns defpath
    QStringList params();
    QStringList selectedParams();
    int getParamLength();
    Q_INVOKABLE void parseDefs();
    Q_INVOKABLE bool isSelected(QString string);
    Q_INVOKABLE void removeSelectedParam(QString str);
    Q_INVOKABLE void printSelectedParams();
    Q_INVOKABLE void clearSelected();
    Q_INVOKABLE void fillDefs();
    Q_INVOKABLE void startTestSweep();
    void setDefPointer(Definition* definition);
signals:
    void defPathChanged(); //called when def path is set
    void paramsChanged();
    void selectedParamsChanged();
    void defsFilled();
    void testSweep();

public slots:
    void setDefPath(QString); // sets definition poath
    void setParams(QList<QString>);
    void appendParams(QString);
    QString getParam(int);
    void setSelectedParams(QString param);
private:
    QString _defPath;
    //QStringListModel *_stringList;
    QStringList _params;
    //QDomDocument xml;
    QStringList _selectedParams;
    Definition* def;
    configHandler handle;

};

#endif // DEFWINDOW_H
