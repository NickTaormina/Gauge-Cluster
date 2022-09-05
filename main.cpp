#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QObject>
#include "defwindow.h"
#include "logger.h"
#include "gauges.h"



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
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    Definition def;
    defWindow defWin;
    defWin.setDefPointer(&def);
    defWin.parseDefs();
    qDebug() << defWin.params();



    canbus can;
    parameter* par;
    can.connectToCanDevice();
    logger* log= new logger(nullptr, &can, par, &def);
    QObject::connect(&defWin, &defWindow::defsFilled, log, &logger::createParamArray);




    QQmlContext * rootContext = engine.rootContext();
    rootContext->setContextProperty("defClass", &defWin);
    rootContext->setContextProperty("logger", log);

    QList<QObject*> obj = engine.rootObjects();
    QObject* main = obj.at(0);
    QObject * statustext = main->findChild<QObject*>("statusText", Qt::FindChildrenRecursively);

    gauges* gauge = new gauges(nullptr, main);
    QObject::connect(log, &logger::setParams, gauge, &gauges::setParamPointer);
    QObject::connect(&defWin, &defWindow::testSweep, gauge, &gauges::startTest);

    defWin.fillDefs();
    qDebug() << "reponse length: " << def.getRxMessageLength();





    //rootContext->setContextProperty("gaugeHandle", gauge);








    return app.exec();
}
