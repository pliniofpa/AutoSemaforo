#-------------------------------------------------
#
# Project created by QtCreator 2014-09-19T22:32:07
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoSemaforo
TEMPLATE = app

win32{
RC_FILE     = resources.rc
}
SOURCES += main.cpp\
        mainwindow.cpp \
    about.cpp \
    tporta.cpp \
    mytimer.cpp \
    configuration.cpp

HEADERS  += mainwindow.h \
    about.h \
    tporta.h \
    version.h \
    mytimer.h \
    configuration.h

FORMS    += mainwindow.ui \
    about.ui \
    configuration.ui

RESOURCES += \
    res.qrc

OTHER_FILES +=
