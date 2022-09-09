#include "defwindow.h"

defWindow::defWindow(QObject *parent)
    : QObject{parent}
{
    //_defPath = "C:/Users/admin/OneDrive - University of Florida/Documents/_Tuning/Gauge Cluster/GaugeCluster/config/cobb2.xml";


}

defWindow::defWindow(QObject *parent, Definition *d)
{
     def = d;
     _selectedParams.append("AF Correction 1");
     _selectedParams.append("AF Learning 1");
     _selectedParams.append("AF Ratio");
     _selectedParams.append("Engine Speed");
     _selectedParams.append("Vehicle Speed");
     _selectedParams.append("Odometer");
     _selectedParams.append("Feedback Knock Correction");
     _selectedParams.append("Fine Knock Learn");
     _selectedParams.append("Dynamic Advance Multiplier");

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
    _params = handle.getParams();

}

void defWindow::fillDefs(){
    handle.fillDefs(_selectedParams, def);
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
