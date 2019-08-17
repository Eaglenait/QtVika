QT += core gui websockets network widgets

TARGET = Vika
TEMPLATE = app

CONFIG += c++11 console
CONFIG -= app_bundle

DEFINES = QZEROCONF_STATIC

RESOURCES = resources.qrc

include(libs/QtZeroConf/qtzeroconf.pri)

SOURCES += \
        action.cpp \
        device.cpp \
        devicemanager.cpp \
        main.cpp \
        mainwindow.cpp \
        syntaxhelper.cpp

HEADERS += \
    action.h \
    device.h \
    devicemanager.h \
    mainwindow.h \
    syntaxhelper.h \
    vikaword.h \

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
