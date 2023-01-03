#include "defwindow.h"

/*
The definition selector window. Responsible for setting the params to request from ecu
*/
defWindow::defWindow(QObject *parent)
    : QObject{parent}
{

}

defWindow::defWindow(QObject *parent, Definition *d)
{
     def = d;

     //list of default params to monitor. rpm, speed, and odometer are required
     //TODO: move these to an initializastion function that can be used in clear
     //_selectedParams.append("AF Correction 1");
     _selectedParams.append("AF Learning 1");
     _selectedParams.append("AF Ratio");
     _selectedParams.append("Intake Manifold Temperature");
     //_selectedParams.append("Engine Speed");
    // _selectedParams.append("Vehicle Speed");
     _selectedParams.append("Odometer");
     _selectedParams.append("Feedback Knock Correction");
     _selectedParams.append("Fine Knock Learn");
    // _selectedParams.append("Battery");
     //_selectedParams.append("Dynamic Advance Multiplier");


}

//returns definition file used
QString defWindow::defPath(){
    return _defPath;
}

//set the definitoin file to use
void defWindow::setDefPath(QString path){
    if(_defPath != path){
        _defPath = path;
        emit defPathChanged();
    }
}

//read definition file to get param names
void defWindow::parseDefs()
{
    _params = handle.getParams();

}

//create definition object with selected parameter information
void defWindow::fillDefs(){
    handle.fillDefs(_selectedParams, def);
    emit defsFilled();

}

//Checks operation of gauges using dummy values
void defWindow::startTestSweep()
{
    emit testSweep();
}

//tells window where to store the definition info for selected params
void defWindow::setDefPointer(Definition *definition)
{
    def = definition;
}

//returns true if given parameter was selected by user
bool defWindow::isSelected(QString string)
{
    if(_selectedParams.contains(string)){
        return true;
    } else {
        return false;
    }
}

//removes a parameter from the selected param list
void defWindow::removeSelectedParam(QString str)
{
    if(isSelected(str)){
        int i = _selectedParams.indexOf(str);
        qDebug() << "removing: " << _selectedParams.at(i);
        _selectedParams.removeAt(i);
    }
}

//outputs all selected params
void defWindow::printSelectedParams()
{
    qDebug() << "selected params: " << _selectedParams;
}

//clears the selected parameter list
void defWindow::clearSelected()
{
    _selectedParams.clear();
}

//returns all displayed parameters
QStringList defWindow::params(){
    return _params;
}

//returns all selected params
QStringList defWindow::selectedParams()
{
    return _selectedParams;
}

//set the parameters to display
void defWindow::setParams(QList<QString> p){
    _params = p;
}

//append parameter to display
void defWindow::appendParams(QString p){
    _params.append(p);
}

//get count of all displayed params
int defWindow::getParamLength(){
    return _params.length();
}

//gets parameter name
QString defWindow::getParam(int i){
    return _params.at(i);
}

//adds parameter to selected list
void defWindow::setSelectedParams(QString param)
{
    _selectedParams.append(param);
}
