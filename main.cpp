#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QObject>
#include "defwindow.h"
#include "logger.h"
#include "gauges.h"
#include "trip.h"
#include <QtXml>
#include "confighandler.h"
#include "config.h"
#include "cel.h"



int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QLoggingCategory::setFilterRules("qt.canbus.plugins.passthru=false");
    QGuiApplication app(argc, argv);
    QCoreApplication::setOrganizationDomain("Hentai Racing");
    QCoreApplication::setOrganizationDomain("hentairacing.com");

    QQmlApplicationEngine engine;
    QQmlContext * rootContext = engine.rootContext();
    QString rootPath;
    rootPath = QCoreApplication::applicationDirPath();
    rootContext->setContextProperty("applicationDirPath", QCoreApplication::applicationDirPath());
    const QUrl url(QStringLiteral("qrc:/resources/ui/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    config* cfg = new config[5];
    configHandler hand(nullptr, cfg);
    hand.parseConfig();
    qDebug() <<"test: " <<hand.getParams();
    gear gr;
    hand.fillGear(&gr);
    trip tr;
    hand.fillTrip(&tr);
    Definition def;
    defWindow defWin(nullptr, &def);

    defWin.parseDefs();
    qDebug() << defWin.params();



    canbus can;
    parameter* par;
    can.connectToCanDevice();
    logger* log= new logger(nullptr, &can, par, &def);
    cel c(nullptr, &can, &cfg[config::LOGGER]);
    QObject::connect(&defWin, &defWindow::defsFilled, log, &logger::createParamArray);





    rootContext->setContextProperty("defClass", &defWin);
    rootContext->setContextProperty("logger", log);



    QList<QObject*> obj = engine.rootObjects();
    QObject* main = obj.at(0);
    QObject * statustext = main->findChild<QObject*>("statusText", Qt::FindChildrenRecursively);

    gauges* gauge = new gauges(nullptr, main, &gr, &tr, &cfg[config::GAUGES]);
    QObject::connect(log, &logger::setParams, gauge, &gauges::setParamPointer);
    QObject::connect(&defWin, &defWindow::testSweep, gauge, &gauges::startTest);
    QObject::connect(gauge, &gauges::tripUpdated, &hand, &configHandler::storeTrip);
    defWin.fillDefs();
    qDebug() << "reponse length: " << def.getRxMessageLength();
    rootContext->setContextProperty("gauge", gauge);


    QDomDocument xml;
    qDebug() << "reading definition file";
    QFile defFile("C:/Users/admin/OneDrive - University of Florida/Documents/_Tuning/Gauge Cluster/GaugeCluster/config/config.xml");
    xml.setContent(&defFile);
    defFile.close();
    QDomElement x = xml.firstChild().firstChild().toElement();
    QDomElement tri;
    while(!x.isNull()){
        if(x.tagName() ==  "trip"){
            tri = x;
        }
        x = x.nextSibling().toElement();
    }


    //gauge->gaugeSweep();
    return app.exec();
}
