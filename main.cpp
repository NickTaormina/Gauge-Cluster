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
#include <QSerialPort>
#include <QSerialPortInfo>
#include "serialhandler.h"
#include "ecucomm.h"
#include "fueleconomy.h"
#include <QtGlobal>

QString logPath;
QString framePath;


//debug log handler.
void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg){
    QString txt;
    QString frame;
    QByteArray local = msg.toLocal8Bit();
    int fr = 0;

    switch (type) {
    case QtDebugMsg:
        txt = QString("") + msg;
        if(msg.at(0)!="*"){
            fprintf(stdout, "%s\n", local.constData());
        }
        break;
    case QtInfoMsg: //save frame processing data as info into a sep file
        //frame = QString("") + QDateTime::currentDateTime().toString("mm.ss") + ": " + msg;
        frame = QString("") + msg;
        fr = 1;
        break;
    case QtWarningMsg:
        txt = QString("Warning: ") + (msg);
        fprintf(stdout, "%s\n", local.constData());
    break;
    case QtCriticalMsg:
        txt = QString("Critical: ")+(msg);
        fprintf(stdout, "%s\n", local.constData());
    break;
    case QtFatalMsg:
        txt = QString("Fatal: ")+(msg);
        fprintf(stdout, "%s\n", local.constData());
        abort();
    }

    if(fr == 1){
        QFile frameFile(framePath);
        frameFile.open(QIODevice::WriteOnly | QIODevice::Append);
          QTextStream fs(&frameFile);
          fs << frame << Qt::endl;
    }else {
        QFile outFile(logPath);
        outFile.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream ts(&outFile);
        ts << txt << Qt::endl;
    }


}


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


    if(!QDir("logs").exists()){
        QDir().mkdir("logs");
    }
    //error logging setup
    logPath = QCoreApplication::applicationDirPath() + "/logs/"+ "log-" + QDateTime::currentDateTime().toString("dd.MM.hh.mm.ss") + ".txt";
    framePath = QCoreApplication::applicationDirPath() + "/logs/"+ "frames-" + QDateTime::currentDateTime().toString("dd.MM.hh.mm.ss") + ".txt";
    setvbuf(stderr, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    qInstallMessageHandler(myMessageHandler);





    config* cfg = new config[5];
    configHandler hand(nullptr, cfg);
    hand.parseConfig();
    qDebug() << "*handler params: " <<hand.getParams();
    gear gr;
    hand.fillGear(&gr);
    trip tr;
    hand.fillTrip(&tr, "tripA");
    Definition def;
    defWindow defWin(nullptr, &def);
    fueleconomy fe;
    hand.fillEconomy(&fe);

    defWin.parseDefs();
    qDebug() << defWin.params();

    qDebug() << "*main can count: " << hand.getCanCount();
    canDef * _candef = new canDef[hand.getCanCount()];
    canData _canData(_candef, hand.getCanCount());
    QObject::connect(&hand, &configHandler::canFilled, [&_canData, &_candef, &hand](){
        qDebug() << "*fill can from connect";
        _canData.fillData(_candef, hand.getCanCount());
    } );
    hand.fillCan(_candef);


    serialHandler shand(nullptr, &cfg[config::CAN]);
    canbus can(nullptr, cfg, &shand);
    ecuComm _ecuComm(nullptr, &can);
    parameter* par;
    logger* log= new logger(nullptr, &can, par, &def);
    cel c(nullptr, &can, &cfg[config::LOGGER]);
    QObject::connect(&defWin, &defWindow::defsFilled, log, &logger::createParamArray);




    rootContext->setContextProperty("defClass", &defWin);
    rootContext->setContextProperty("logger", log);



    QList<QObject*> obj = engine.rootObjects();
    QObject* main = obj.at(0);
    QObject * statustext = main->findChild<QObject*>("statusText", Qt::FindChildrenRecursively);

    gauges* gauge = new gauges(nullptr, main, &gr, &cfg[config::GAUGES], &hand, &_canData, &defWin, &fe);
    QObject::connect(log, &logger::setParams, gauge, &gauges::setParamPointer);

    QObject::connect(&defWin, &defWindow::testSweep, gauge, &gauges::startTest);


    defWin.fillDefs();
    qDebug() << "*reponse length: " << def.getRxMessageLength();
    rootContext->setContextProperty("gauge", gauge); //sets access to gauge class in qml



    //cannects can data to gauges after the sweep
    QEventLoop loop;
    QTimer::singleShot(2400, &loop, SLOT(quit()));
    loop.exec();
    QObject::connect(&_canData, &canData::usefulIDsFound, &shand, &serialHandler::setUsefulIDs);
    _canData.getUsefulIDs();
    QObject::connect(log, &logger::paramUpdated, gauge, &gauges::updateParamDisplay);
    QObject::connect(&can, &canbus::messageRead, &_canData, &canData::receiveCanData);
    QObject::connect(gauge, &gauges::tripUpdated, &hand, &configHandler::storeTrip);
    QObject::connect(gauge, &gauges::odometerUpdated, &hand, &configHandler::storeOdometer);
    QObject::connect(gauge, &gauges::shiftThresholdChanged, &hand, &configHandler::storeShiftLightThreshold);
    QObject::connect(gauge, &gauges::shiftTimerChanged, &hand, &configHandler::storeShiftLightTimer);
    QObject::connect(gauge, &gauges::tripSwapped, &hand, &configHandler::swapTrip);
    QObject::connect(&can, &canbus::ecuResponse, log, &logger::combineECUResponse);
    QObject::connect(&_canData, &canData::paramValueChanged, gauge, &gauges::updateParamDisplay);
    QObject::connect(&_canData, &canData::valueChanged, gauge, &gauges::updateCANParam);
    QObject::connect(&shand, &serialHandler::serialFrameReceived, &can, &canbus::receiveSerialFrame);
    QObject::connect(&can, &canbus::ecuAck, &can, &canbus::sendQueuedMessage);
    QObject::connect(&defWin, &defWindow::paramsChanged, gauge, &gauges::redrawParamDisplay);
    //log->startLogging();

    //qDebug() << "main call: " << _ecuComm.getFuelLevel();


    //gauge->gaugeSweep();
    return app.exec();
}
