QT += quick core xml serialbus serialport location
QTPLUGINS += osm mapboxgl

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    canbus.cpp \
    candata.cpp \
    candef.cpp \
    cel.cpp \
    config.cpp \
    confighandler.cpp \
    definition.cpp \
    defwindow.cpp \
    ecucomm.cpp \
    frames.cpp \
    fueleconomy.cpp \
    gauges.cpp \
    gear.cpp \
    logger.cpp \
    parameter.cpp \
    paramdisplay.cpp \
    serialhandler.cpp \
    trip.cpp \
    weather.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = resources/ui/

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    canbus.h \
    candata.h \
    candef.h \
    cel.h \
    config.h \
    confighandler.h \
    definition.h \
    defwindow.h \
    ecucomm.h \
    frames.h \
    fueleconomy.h \
    gauges.h \
    gear.h \
    logger.h \
    parameter.h \
    paramdisplay.h \
    serialhandler.h \
    trip.h \
    weather.h


CONFIG += DESTDIR
CONFIG += file_copies
DESTDIR = $$OUT_PWD/debug/
configfiles.path = $$DESTDIR
configfiles.files += config/
resourcefiles.path=$$DESTDIR
resourcefiles.files += resources/
COPIES += configfiles
COPIES += resourcefiles
