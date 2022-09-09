#include "confighandler.h"

configHandler::configHandler(QObject *parent)
    : QObject{parent}
{
    applicationDir = QCoreApplication::applicationDirPath();
    _defPath = applicationDir + "/config/cobb2.xml";

}


QString configHandler::getResourcesPath()
{
    if(!resourcesPath.isNull()){
        return resourcesPath;
    }
}

QString configHandler::getConfigPath()
{
    if(!configPath.isNull()){
        return configPath;
    }
}

QString configHandler::getDefPath()
{
    return _defPath;
}


QStringList configHandler::getParams()
{
    qDebug() << "reading definition file";
    QStringList _params;
    QFile defFile(_defPath);
    defXml.setContent(&defFile);
    defFile.close();
    //get the year from the file heading
    QDomElement root = defXml.documentElement();
    qDebug() <<root.tagName();

    //go one level in to read all params
    QDomElement param = root.firstChild().toElement();

    while(!param.isNull()){
        if(param.tagName() == "ecuparam"){
            QString paramName = param.attribute("name", "invalid parameter");
            _params.append(paramName);
            /*QListWidgetItem *item = new QListWidgetItem;
            item.setFlags(item.flags() | Qt::ItemIsUserCheckable);
            item.setCheckState(Qt::Unchecked);
            item.setText(paramName);
            ui.defList.addItem(item);*/
        }
        param = param.nextSibling().toElement();
    }
    return _params;
}

void configHandler::fillDefs(QStringList _selectedParams, Definition* def)
{
    QDomElement root = defXml.documentElement();
    QDomElement param = root.firstChild().toElement();
    def->clearAll();
    def->appendTx(QByteArray::fromHex(QString("A800").toLatin1()));
    for(int i = 0; i < _selectedParams.size(); i++){
        param = root.firstChild().toElement();
        while(!param.isNull()){
            if(i >= _selectedParams.size()){
                qDebug() << "break";
                break;
            }
            if(param.attribute("name") == _selectedParams.at(i)){
                def->appendParamNames(param.attribute("name"));
                def->appendPID(param.attribute("id").toInt(nullptr, 10));
                QDomElement child = param.firstChild().toElement();
                if(child.tagName() == "address"){
                    qDebug() << "found param: " << param.attribute("name");
                    QString addr = child.text();
                    addr.remove(' ');
                    qDebug() << "address: " << addr.toLatin1();
                    def->appendTx(QByteArray::fromHex(addr.toLatin1()));
                    child = child.nextSibling().toElement();
                    if(child.tagName() == "response"){
                        qDebug() << "Response length: " << child.attribute("length").toInt(nullptr, 10);
                        def->appendRx(child.attribute("length").toInt(nullptr, 10));
                    }
                    child = child.nextSibling().toElement();
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

                }
                break;
            }
            param = param.nextSibling().toElement();
        }
    }
    qDebug() << "defs filled";
    qDebug() << "raw tx message: " << def->getTxBytes().toHex();
    emit defsFilled();
}

void configHandler::fillGear(gear *g)
{
    QDomDocument ratioXml;
    QFile f(ratioPath);
    ratioXml.setContent(&f);
    f.close();
    QDomElement root = ratioXml.documentElement();
    QDomElement ratio = root.firstChild().toElement();
    QList<float> ratios;
    while(!ratio.isNull()){
        if(ratio.tagName() == "one"){
            ratios.append(ratio.text().toFloat(nullptr));
        } else if(ratio.tagName() == "two"){
            ratios.append(ratio.text().toFloat(nullptr));
        } else if(ratio.tagName() == "three"){
            ratios.append(ratio.text().toFloat(nullptr));
        } else if(ratio.tagName() == "four"){
           ratios.append(ratio.text().toFloat(nullptr));
        } else if(ratio.tagName() == "five"){
            ratios.append(ratio.text().toFloat(nullptr));
        } else if(ratio.tagName() == "six"){
            ratios.append(ratio.text().toFloat(nullptr));
        } else if(ratio.tagName() == "R"){
            g->setRatioR(ratio.text().toFloat(nullptr));
        } else if(ratio.tagName().toLower() == "final"){
            g->setFinalDrive(ratio.text().toFloat(nullptr));
        } else if(ratio.tagName().toLower() == "tire"){
            g->setTireDiameter(ratio.text().toFloat(nullptr));
        }
        ratio = ratio.nextSibling().toElement();
    }
    g->setRatios(ratios);
}

void configHandler::clearConfigXml()
{
    configXml.clear();
}

void configHandler::setDefPath()
{
    QFile f(configPath);
    configXml.setContent(&f);
    f.close();
    QDomElement root = configXml.documentElement();
    QDomElement category = root.firstChild().toElement();

    while(!category.isNull()){
        if(category.tagName() == "defs"){
            category = category.firstChild().toElement();
            while(!category.isNull()){
                if(category.tagName() == "defFile"){
                    _defPath = category.text();
                }
            }
        }
    }
    qDebug() << "set def path";
}


