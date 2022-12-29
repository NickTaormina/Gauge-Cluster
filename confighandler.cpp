#include "confighandler.h"


/*
Reads xmls and passes values where needed
*/
configHandler::configHandler(QObject *parent)
    : QObject{parent}
{
    applicationDir = QCoreApplication::applicationDirPath();
    configPath = applicationDir + "/config/config.xml";
    ratioPath = applicationDir + "/config/ratios.xml"; //TODO: put this in the config instead of here
    canPath = applicationDir + "/config/can.xml";
    setDefPath();
}

configHandler::configHandler(QObject *parent, config *c)
{
    applicationDir = QCoreApplication::applicationDirPath();
    configPath = applicationDir + "/config/config.xml";
    ratioPath = applicationDir + "/config/ratios.xml"; //TODO: put this in the config instead of here
    setDefPath();
    canPath = applicationDir + "/config/can.xml";
    cfg = c;
}


QString configHandler::getResourcesPath()
{
    if(!resourcesPath.isNull()){
        return resourcesPath;
    }
    return NULL;
}

QString configHandler::getConfigPath()
{
    if(!configPath.isNull()){
        return configPath;
    }
    return NULL;
}

QString configHandler::getDefPath()
{
    return _defPath;
}


QStringList configHandler::getParams()
{
    qDebug() << "*reading definition file";
    QStringList _params;
    QFile defFile(_defPath);
    defXml.setContent(&defFile);
    defFile.close();
    //get the year from the file heading
    QDomElement root = defXml.documentElement();
    qDebug() << "*" << root.tagName();

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
                qDebug() << "*break";
                break;
            }
            if(param.attribute("name") == _selectedParams.at(i)){
                def->appendParamNames(param.attribute("name"));
                def->appendPID(param.attribute("id").toInt(nullptr, 10));
                QDomElement child = param.firstChild().toElement();
                if(child.tagName() == "address"){
                    qDebug() << "*found param: " << param.attribute("name");
                    QString addr = child.text();
                    addr.remove(' ');
                    qDebug() << "*address: " << addr.toLatin1();
                    def->appendTx(QByteArray::fromHex(addr.toLatin1()));
                    child = child.nextSibling().toElement();
                    if(child.tagName() == "response"){
                        qDebug() << "*Response length: " << child.attribute("length").toInt(nullptr, 10);
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
                                qDebug() <<"*conv: " << conversion.attribute("expr").toDouble(nullptr);
                            } else if(conversion.tagName() == "offset"){
                                def->appendOffset(conversion.text().toFloat(nullptr));
                                qDebug() << "*offset: " << conversion.text().toFloat(nullptr);
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
    qDebug() << "*defs filled";
    qDebug() << "*raw tx message: " << def->getTxBytes().toHex();
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
    qDebug() << "*ratio" << ratio.tagName();
    QList<float> ratios;
    while(!ratio.isNull()){
        if(ratio.tagName() == "one"){
            ratios.append(ratio.text().toFloat(nullptr));
            qDebug() << "*1st ratio: " << ratio.text().toFloat(nullptr);
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

void configHandler::fillTrip(trip *tr, QString tripName)
{

    QDomElement xt = configXml.firstChild().firstChild().toElement();
    while(!xt.isNull()){
        if(xt.tagName() == "trip"){
            QDomElement trElement = xt.firstChild().toElement();
            while(!trElement.isNull()){
                if(trElement.tagName() == tripName){
                    tr->setMilesTraveled(trElement.text().toFloat(nullptr));
                    qDebug() << "*trip " << trElement.tagName()<< " : " << trElement.text().toFloat(nullptr);
                    break;
                }
                trElement = trElement.nextSibling().toElement();
            }
        }
        xt = xt.nextSibling().toElement();
    }
}

void configHandler::storeTrip(QString trip, QString val)
{
    QFile f(configPath);
    f.open(QIODevice::ReadWrite);
    QDomElement xt = configXml.firstChild().firstChild().toElement();
    QDomElement tr;
    while(!xt.isNull()){
        if(xt.tagName() == "trip"){
            tr = xt.firstChild().toElement();
            while(!tr.isNull()){
                if(tr.tagName() == trip){
                    QDomElement newNode = configXml.createElement(trip);
                    QDomText newText = configXml.createTextNode(val);
                    newNode.appendChild(newText);
                    xt.replaceChild(newNode, tr);
                    break;
                } else {
                    tr = tr.nextSibling().toElement();
                }
            }
        }
        xt = xt.nextSibling().toElement();
    }
    f.resize(0);
    QTextStream stream;
    stream.setDevice(&f);
    configXml.save(stream, 4);
    f.close();

}



void configHandler::swapTrip(QString trip){
    QFile f(configPath);
    f.open(QIODevice::ReadWrite);
    QDomElement xt = configXml.firstChild().firstChild().toElement();
    while(!xt.isNull()){
        if(xt.tagName() == "gauges"){
            QDomElement parent = xt.firstChild().toElement();
            QDomElement trEl = parent.firstChild().toElement();
            while(!trEl.isNull()){
                if(trEl.tagName() == "activeTrip"){
                    QDomElement newNode = configXml.createElement("activeTrip");
                    QDomText newText = configXml.createTextNode(trip);
                    qDebug() << "*writing active trip: " << trip;
                    newNode.appendChild(newText);
                    parent.replaceChild(newNode, trEl);
                    break;
                }
                trEl = trEl.nextSibling().toElement();
            }
            break;
        }
        xt = xt.nextSibling().toElement();
    }
    QDomElement tr;
    f.resize(0);
    QTextStream stream;
    stream.setDevice(&f);
    configXml.save(stream, 4);
    f.close();
}

void configHandler::storeOdometer(QString val)
{
    QFile f(configPath);
    f.open(QIODevice::ReadWrite);
    QDomElement xt = configXml.firstChild().firstChild().toElement();
    while(!xt.isNull()){
        if(xt.tagName() == "gauges"){
            QDomElement parent = xt.firstChild().toElement();
            QDomElement trEl = parent.firstChild().toElement();
            while(!trEl.isNull()){
                if(trEl.tagName() == "odometer"){
                    QDomElement newNode = configXml.createElement("odometer");
                    QDomText newText = configXml.createTextNode(val);
                    qDebug() << "*writing odometer: " << val;
                    newNode.appendChild(newText);
                    parent.replaceChild(newNode, trEl);
                    break;
                }
                trEl = trEl.nextSibling().toElement();
            }
            break;
        }
        xt = xt.nextSibling().toElement();
    }
    QDomElement tr;
    f.resize(0);
    QTextStream stream;
    stream.setDevice(&f);
    configXml.save(stream, 4);
    f.close();
}

void configHandler::storeShiftLightThreshold(QString val)
{
    QFile f(configPath);
    f.open(QIODevice::ReadWrite);
    QDomElement xt = configXml.firstChild().firstChild().toElement();
    while(!xt.isNull()){
        if(xt.tagName() == "gauges"){
            QDomElement parent = xt.firstChild().toElement();
            QDomElement trEl = parent.firstChild().toElement();
            while(!trEl.isNull()){
                if(trEl.tagName() == "shiftLightThreshold"){
                    QDomElement newNode = configXml.createElement("shiftLightThreshold");
                    QDomText newText = configXml.createTextNode(val);
                    qDebug() << "*writing shift light thresh: " << val;
                    newNode.appendChild(newText);
                    parent.replaceChild(newNode, trEl);
                    break;
                }
                trEl = trEl.nextSibling().toElement();
            }
            break;
        }
        xt = xt.nextSibling().toElement();
    }
    QDomElement tr;
    f.resize(0);
    QTextStream stream;
    stream.setDevice(&f);
    configXml.save(stream, 4);
    f.close();
}

void configHandler::storeShiftLightTimer(QString val)
{
    QFile f(configPath);
    f.open(QIODevice::ReadWrite);
    QDomElement xt = configXml.firstChild().firstChild().toElement();
    while(!xt.isNull()){
        if(xt.tagName() == "gauges"){
            QDomElement parent = xt.firstChild().toElement();
            QDomElement trEl = parent.firstChild().toElement();
            while(!trEl.isNull()){
                if(trEl.tagName() == "shiftLightFlashTimer"){
                    QDomElement newNode = configXml.createElement("shiftLightFlashTimer");
                    QDomText newText = configXml.createTextNode(val);
                    qDebug() << "*writing shift light thresh: " << val;
                    newNode.appendChild(newText);
                    parent.replaceChild(newNode, trEl);
                    break;
                }
                trEl = trEl.nextSibling().toElement();
            }
            break;
        }
        xt = xt.nextSibling().toElement();
    }
    QDomElement tr;
    f.resize(0);
    QTextStream stream;
    stream.setDevice(&f);
    configXml.save(stream, 4);
    f.close();
}

//reads the config xml and stores into config map
void configHandler::parseConfig()
{
    QFile f(configPath);
     if(f.exists()){
     configXml.setContent(&f);
     f.close();
     }
     QDomElement root = configXml.documentElement();
     QDomElement category = root.firstChild().toElement();
     qDebug() << "*category: " << category.tagName();
     while(!category.isNull()){
         int cat;
         if(category.tagName() == "defs"){
             cat = config::DEFS;
         } else if (category.tagName() == "logger"){
            cat = config::LOGGER;
         } else if (category.tagName() == "trip"){
            cat = config::TRIP;
         } else if (category.tagName() == "gauges"){
            cat = config::GAUGES;
         } else if (category.tagName() == "canDevice"){
             cat = config::CAN;
         }
         QDomElement sub = category.firstChild().toElement();
         qDebug() << "*sub: " << sub.tagName();
         qDebug() << "*childNodes: " << sub.childNodes().count();
         while(!sub.isNull()){
             if(sub.childNodes().count() < 2){
                 qDebug() << "*configs[" << cat << "]: " << sub.tagName() << " val: " <<sub.text();
                 cfg[cat].setValue(sub.tagName(), sub.text());
             } else {
                 QDomElement child = sub.firstChild().toElement();
                 while(!child.isNull()){
                     if(child.childNodes().count() < 2){
                         QString tmp = child.tagName();
                         tmp.replace(0, 1, tmp[0].toUpper());
                         tmp.prepend(sub.tagName());
                         qDebug() << "*configc: " << tmp << " val: " <<child.text();
                        cfg[cat].setValue(tmp, child.text());
                     } else {
                         qDebug() << "*config xml error: too many layers";
                         break;
                     }
                     child = child.nextSibling().toElement();
                 }
             }
             sub = sub.nextSibling().toElement();
         }
         category = category.nextSibling().toElement();
     }

}

//fills the can def object from xml
void configHandler::fillCan(canDef *d)
{
    qDebug() << "*can path" << canPath;
    QDomDocument canXml;
    QFile f(canPath);
     if(f.exists()){
     canXml.setContent(&f);
     f.close();
     }
     QDomElement root = canXml.documentElement();
     qDebug() << "*can root: " << root.tagName();
     QDomElement param = root.firstChild().toElement();
     qDebug() << "*can param: " << param.tagName();

     canCount = 0;
     //gets number of params
     while(!param.isNull()){
         canCount = canCount + 1;
         param = param.nextSibling().toElement();
     }


     param = root.firstChild().toElement();
     int pos = 0;
     while(!param.isNull()){
         QDomElement sub = param.firstChild().toElement();
         while(!sub.isNull()){
             if(sub.tagName() == "id"){
                 qDebug() << "*setting can def: " << param.attribute("name");
                d[pos].setName(param.attribute("name"));
                d[pos].setFrameID(sub.text().toUInt(nullptr, 10));
             } else if (sub.tagName() == "byte"){
                 QString byte = sub.text();
                 QStringList tmp;
                 for(int x = 0; x<byte.length(); x++){
                    tmp.append(byte.at(x));
                    qDebug() << "*name: " << d[pos].getName() << " byte: " << byte.at(x);
                 }
                 d[pos].setBytes(tmp);
             }else if (sub.tagName() == "bits"){
                 QDomElement b = sub.firstChild().toElement();
                 QStringList tmp;
                 while(!b.isNull()){
                     tmp.append( b.text());
                     b = b.nextSibling().toElement();
                 }
                 d[pos].setBits(tmp);
             } else if (sub.tagName() == "conv"){
                d[pos].setConv(sub.text().toDouble(nullptr));
             }
             else if (sub.tagName() == "offset"){
                d[pos].setOffset(sub.text().toInt(nullptr, 10));
             }
             else if (sub.tagName() == "bigEndian"){
                d[pos].setEndian(sub.text().toInt(nullptr, 10));
             }
             else if (sub.tagName() == "targets"){
                QMap<uint, QString> tmp;
                QDomElement tar = sub.firstChild().toElement();
                while(!tar.isNull()){
                    tmp.insert(tar.text().toUInt(nullptr, 10), tar.attribute("name"));
                    qDebug() << "*target: " << tar.text().toUInt(nullptr, 10) << " name: " << tar.attribute("name");
                    tar = tar.nextSibling().toElement();
                }
                d[pos].setTargets(tmp);

             }
             sub = sub.nextSibling().toElement();
         }
         pos = pos + 1;
         param = param.nextSibling().toElement();
     }
     qDebug() << "*can filled";
     emit canFilled();

}

int configHandler::getCanCount()
{
    QDomDocument canXml;
    QFile f(canPath);
     if(f.exists()){
     canXml.setContent(&f);
     f.close();
     }
     QDomElement root = canXml.documentElement();
     QDomElement param = root.firstChild().toElement();

     canCount = 0;
     //gets number of params
     while(!param.isNull()){
         canCount = canCount + 1;
         param = param.nextSibling().toElement();
     }
    qDebug() << "*can count: " << canCount;
    return canCount;
}

void configHandler::clearConfigXml()
{
    configXml.clear();
}

void configHandler::setDefPath()
{

   QFile f(configPath);
    if(f.exists()){
    configXml.setContent(&f);
    f.close();
    QDomElement root = configXml.documentElement();
    QDomElement category = root.firstChild().toElement();
    qDebug() << "*" << category.tagName();
    while(!category.isNull()){
        qDebug() << "*" << category.tagName();
        if(category.tagName() == "defs"){
            category = category.firstChild().toElement();
            qDebug() << "*" << category.tagName();
            while(!category.isNull()){
                if(category.tagName() == "defFile"){
                    qDebug() << "*" << category.text();
                    _defPath = applicationDir + category.text();
                    break;
                }
                category = category.nextSibling().toElement();
            }
        }
        category = category.nextSibling().toElement();
    }
    qDebug() << "*set def path";
    } else {
        qDebug() << "no config.xml";
    }


}


