#include "paramdisplay.h"

paramDisplay::paramDisplay(QObject *parent, QQmlEngine *engine, QStringList l)
{

    _parent = parent;
    _engine = engine;
    paramList = l;
    qDebug() << "num displayed params: " << l.count();




    totalWidth = 460;
    maxWidth = 250;


    //swapTest();
}

void paramDisplay::updateValue(QString name, double value)
{
    if(paramList.contains(name)){
        int index = paramList.indexOf(name);
        if(index > -1 && paramValues.count()-1 >= index){
            paramValues.at(index)->setProperty("text", formatText(name, value));
        }
        updateMinMax(index, value);
    }
}

//formats certain logging params to match AP. TODO: make this reference a config
QString paramDisplay::formatText(QString name, double value)
{
    if(name == "AF Ratio"){
        QString val = QString::number(value, 'f', 2);
        if(val.length()<3){
            val.append(".");
        }
        if(val.length()<5){
            for(int i = val.length(); i<5; i++){
                val.append("0");
            }
        }
        return val;
    } else if (name == "AF Learning 1"){
        return QString::number(value, 'f', 2);
    } else if (name == "Oil Temp"){
        //value = 1.8*value + 32;
        return QString::number(value, 'f', 0);
    } else if (name == "Intake Manifold Temperature"){
        return QString::number(value, 'f', 0);
    } else if(name == "Odometer"){
        QString val = QString::number(value, 'f', 0);
        if(val.length() < 6){
            for(int x = val.length(); x<6; x++){
                val.prepend("0");
            }
        }
        return val;
    } else if (name == "Feedback Knock Correction"){
        return QString::number(value, 'f', 2);
    } else if (name == "Dynamic Advance Multiplier"){
        return QString::number(value, 'f', 2);
    } else if (name == "Fine Knock Learn") {
        return QString::number(value, 'f', 2);
    } else {
        return QString::number(value, 'f', 1);
    }
}

void paramDisplay::updateMinMax(int index, double value)
{
    if(minValues.count() < paramList.count() || maxValues.count() < paramList.count()){
        qDebug() << "error: min/max array not set";
        return;

    } else {
        //qDebug() << paramList.at(index) << ": min: "<< minValues.at(index).toDouble()<<" : max: " << maxValues.at(index).toDouble();
        if(maxValues.at(index)==""){
            maxValues.replace(index, formatText(paramList.at(index), value));
            maxLabels.at(index)->setProperty("text", formatText(paramList.at(index), value));
        } else if (value > maxValues.at(index).toDouble(nullptr)){
            maxValues.replace(index, formatText(paramList.at(index), value));
            maxLabels.at(index)->setProperty("text", formatText(paramList.at(index), value));
        }
        if(minValues.at(index)==""){
            minValues.replace(index, formatText(paramList.at(index), value));
            minLabels.at(index)->setProperty("text", formatText(paramList.at(index), value));
        } else if (value < minValues.at(index).toDouble(nullptr)){
            minValues.replace(index, formatText(paramList.at(index), value));
            minLabels.at(index)->setProperty("text", formatText(paramList.at(index), value));
        }
    }
}

//returns list of monitored params
QStringList paramDisplay::getParamList()
{
    return paramList;
}


//sets the list of monitored params
void paramDisplay::setParamList(QStringList l, int reinit)
{
    if(l != paramList){
        paramList = l;
        if(reinit == 1){
            reInitialize();
        }
    }
}

void paramDisplay::setFilterList(QStringList filter, int reinit)
{
    filterList = filter;
    if(reinit == 1){
        reInitialize();
    }
}

void paramDisplay::setCanDisplayList(QStringList l, int reinit)
{
    for(int i = 0; i<l.count(); i++){
        paramList.append(l.at(i));
    }
    if(reinit == 1){
        reInitialize();
    }
}

//must be called after displays been initialized
void paramDisplay::setParamRename(QMap<QString, QString> list)
{
    renameList = list;
    for(int i = 0; i<paramList.count(); i++){
        if(list.contains(paramList.at(i))){
            paramLabels.at(i)->setProperty("text", list.value(paramList.at(i)));
        }
    }
}

void paramDisplay::initDisplay()
{
    clearLists();
    filterParamList();
    calcNumObjects();
    int width = totalWidth/numObjects;
    if(width > maxWidth){
        width = maxWidth;
    }


    for(int i = 0; i<numObjects; i++){
        QQmlComponent component(_engine, QUrl(QStringLiteral("qrc:/resources/ui/LogObject.qml")), _parent);
        QObject* base = component.create();
        //test1->setParent(qobject_cast<QQuickItem*>(topLeftLabel));
        base->setProperty("x", width*i - 10);
        qDebug() << "setting width: " << i << " : " << width;
        base->setProperty("width", width);
        base->setProperty("y", 0);

        paramLabels.append(base->findChild<QObject*>("paramLabel1", Qt::FindChildrenRecursively));
        paramValues.append(base->findChild<QObject*>("paramValue1", Qt::FindChildrenRecursively));

        paramLabels.append(base->findChild<QObject*>("paramLabel2", Qt::FindChildrenRecursively));
        paramValues.append(base->findChild<QObject*>("paramValue2", Qt::FindChildrenRecursively));

         minLabels.append(base->findChild<QObject*>("topMin", Qt::FindChildrenRecursively));
         minLabels.append(base->findChild<QObject*>("bottomMin", Qt::FindChildrenRecursively));

         maxLabels.append(base->findChild<QObject*>("topMax", Qt::FindChildrenRecursively));
         maxLabels.append(base->findChild<QObject*>("bottomMax", Qt::FindChildrenRecursively));


        if(i == numObjects -1){
            QObject*bar = base->findChild<QObject*>("verticalRect", Qt::FindChildrenRecursively);
            bar->setProperty("visible", false);
        }


        QQuickItem *item = qobject_cast<QQuickItem*>(base);
        item->setParentItem(qobject_cast<QQuickItem*>(_parent));
        paramBaseObjects.append(base);
    }

    int fontSize;
    fontSize = 22 - 4*(numObjects);
    if(fontSize < 8) {
        fontSize = 8;
    }
    qDebug() << "fontSize: " << fontSize;
    qDebug() << "fontSize2: " << fontSize*1.7;
    QFont labelFont = paramLabels.at(0)->property("font").value<QFont>();
    labelFont.setPointSize(fontSize);
    QFont valueFont = paramValues.at(0)->property("font").value<QFont>();
    valueFont.setPointSize(fontSize+10);


    QFont minMaxFont = minLabels.at(0)->property("font").value<QFont>();
    minMaxFont.setPointSize(fontSize-2);

    for(int i = 0; i<paramList.count(); i++){
        if(paramLabels.at(i) && paramValues.at(i)){
           paramLabels.at(i)->setProperty("text", paramList.at(i));
           paramLabels.at(i)->setProperty("font", labelFont);


           paramValues.at(i)->setProperty("font", valueFont);

           minLabels.at(i)->setProperty("font", minMaxFont);
           maxLabels.at(i)->setProperty("font", minMaxFont);
        } else {
            qDebug() << "ERROR: param display index mismatch";
            return;
        }
    }

    int count = paramList.count();
    if(count % 2 != 0){
        QObject * horizBar = paramBaseObjects.at(numObjects-1)->findChild<QObject*>("horizRect", Qt::FindChildrenRecursively);
        horizBar->setProperty("visible", false);
        QObject * rect = paramBaseObjects.at(numObjects-1)->findChild<QObject*>("textRectTop", Qt::FindChildrenRecursively);

        qvariant_cast<QObject*>(
            rect->property("anchors")
        )->setProperty("verticalCenterOffset", QVariant::fromValue(0));


        paramLabels.at(count)->setProperty("text", "");
        paramValues.at(count)->setProperty("text", "");
    }
    for(int i = 0; i<paramList.count(); i++){
        minValues.append("");
        maxValues.append("");
    }
}

void paramDisplay::filterParamList()
{
    for(int i = 0; i<filterList.count(); i++){
        if(paramList.contains(filterList.at(i))){
            paramList.removeOne(filterList.at(i));
        }
    }
}

void paramDisplay::reInitialize()
{
    filterParamList();
    if(paramList.count() == paramLabels.count()){
        qDebug() << "similar list";
        int count = paramList.count();
        for(int i = 0; i<count; i++){
            paramLabels.at(i)->setProperty("text", paramList.at(i));
        }
        if(count % 2 != 0){
            paramLabels.at(count)->setProperty("text", "");
            paramValues.at(count)->setProperty("text", "");
        }
    } else {
        initDisplay();
    }

}

int paramDisplay::getNewNumObjects()
{
    return qRound((double)paramList.count()/2);
}

void paramDisplay::calcNumObjects()
{
    numObjects = qRound((double)paramList.count()/2);
}



void paramDisplay::clearLists()
{
    for(int i = 0; i<paramBaseObjects.count(); i++){
        delete paramBaseObjects.at(i);
    }
    paramBaseObjects.clear();
    paramLabels.clear();
    paramValues.clear();
}

void paramDisplay::swapTest()
{
    QTimer * timer = new QTimer(this);
    timer->setInterval(4000);
    timer->setSingleShot(false);
    swap = 0;
    QObject::connect(timer, &QTimer::timeout, this, [this](){
        qDebug() << "swapping display";
        QStringList test;
        test.append("AF Learning");
        test.append("AF Ratio");
        test.append("Intake Temp");
        test.append("Oil Temp");
        if(swap == 1){
            test.append("Feedback Knock");
            test.append("Fine Knock Learn");
            swap = 0;
        } else {
            swap = 1;
        }

        setParamList(test, 1);
    });
    timer->start();
}
