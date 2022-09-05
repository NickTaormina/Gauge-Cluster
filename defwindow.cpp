#include "defwindow.h"

defWindow::defWindow(QObject *parent)
    : QObject{parent}
{
    _defPath = "C:/Users/admin/OneDrive - University of Florida/Documents/_Tuning/Gauge Cluster/GaugeCluster/cobb2.xml";
}

QString defWindow::defPath(){
    return _defPath;
}

void defWindow::setDefPath(QString path){
    if(_defPath != path){
        _defPath = path;
        emit defPathChanged();
    }
}

void defWindow::parseDefs()
{
    QDomDocument xml;
    qDebug() << "parse called";
    QFile defFile(_defPath);
    xml.setContent(&defFile);
    defFile.close();
    //get the year from the file heading
    QDomElement root = xml.documentElement();

    //go one level in to read all params
    QDomElement param = root.firstChild().toElement();

    while(!param.isNull()){
        if(param.tagName() == "ecuparam"){
            QString paramName = param.attribute("name", "invalid parameter");
            _params.append(paramName);
            /*QListWidgetItem *item = new QListWidgetItem;
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(Qt::Unchecked);
            item->setText(paramName);
            ui->defList->addItem(item);*/
        }
        param = param.nextSibling().toElement();
    }

}

void defWindow::fillDefs(){
    QDomDocument xml;
    qDebug() << "parse called";
    QFile defFile(_defPath);
    xml.setContent(&defFile);
    defFile.close();
    QDomElement root = xml.documentElement();
    QDomElement param = root.firstChild().toElement();
    def->appendTx(QByteArray::fromHex(QString("A800").toLatin1()));
    int i = 0;
    while(!param.isNull()){
        if(i >= _selectedParams.size()){
            qDebug() << "break";
            break;
        }
        qDebug() << _selectedParams.size();

        if(param.attribute("name") == _selectedParams.at(i)){
            def->appendParamNames(param.attribute("name"));
            def->appendPID(param.attribute("id").toInt(nullptr, 10));
            QDomElement child = param.firstChild().toElement();
            qDebug() << child.tagName();
            if(child.tagName() == "address"){
                qDebug() << "found addr " << param.attribute("name");
                QString addr = child.text();
                addr.remove(' ');
                qDebug() << addr.toLatin1();
                def->appendTx(QByteArray::fromHex(addr.toLatin1()));
                child = child.nextSibling().toElement();
                if(child.tagName() == "response"){
                    qDebug() << "Response length " << child.attribute("length").toInt(nullptr, 10);
                    def->appendRx(child.attribute("length").toInt(nullptr, 10));
                }
                child = child.nextSibling().toElement();
                qDebug() << "going to conv element";
                if(child.tagName() == "conversions"){
                    QDomElement conversion = child.firstChild().toElement();
                    while(!conversion.isNull()){
                        if(conversion.tagName()=="conversion"){
                            def->appendConv(conversion.attribute("expr").toDouble(nullptr));
                            def->appendUnits(conversion.attribute("units"));
                            if(conversion.attribute("format").length() > 1){
                                def->appendFormat(conversion.attribute("format").length()-1);
                            } else{
                                def->appendFormat(conversion.attribute("format").length());
                            }
                            if(conversion.hasAttribute("invert")){
                                def->appendInvert(1);
                            } else {
                                def->appendInvert(0);
                            }
                            qDebug() <<"conv: " << conversion.attribute("expr").toDouble(nullptr);
                        } else if(conversion.tagName() == "offset"){
                            def->appendOffset(conversion.text().toFloat(nullptr));
                            qDebug() << "offset: " << conversion.text().toFloat(nullptr);
                        }
                        conversion = conversion.nextSibling().toElement();
                    }
                }
                i++;
            }
        }
        param = param.nextSibling().toElement();
    }
    emit defsFilled();

}

void defWindow::startTestSweep()
{
    emit testSweep();
}

void defWindow::setDefPointer(Definition *definition)
{
    def = definition;
}
bool defWindow::isSelected(QString string)
{
    if(_selectedParams.contains(string)){
        return true;
    } else {
        return false;
    }
}

void defWindow::removeSelectedParam(QString str)
{
    if(isSelected(str)){
        int i = _selectedParams.indexOf(str);
        qDebug() << "removing: " << _selectedParams.at(i);
        _selectedParams.removeAt(i);
    }
}

void defWindow::printSelectedParams()
{
    qDebug() << "selected params: " << _selectedParams;
}

void defWindow::clearSelected()
{
    _selectedParams.clear();
}

QStringList defWindow::params(){
    //qDebug() << _params;
    return _params;
}

QStringList defWindow::selectedParams()
{
    return _selectedParams;
}

void defWindow::setParams(QList<QString> p){
    _params = p;
}

void defWindow::appendParams(QString p){
    _params.append(p);
}

int defWindow::getParamLength(){
    return _params.length();
}

QString defWindow::getParam(int i){
    return _params.at(i);
}

void defWindow::setSelectedParams(QString param)
{
    _selectedParams.append(param);
}
