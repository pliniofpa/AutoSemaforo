#-------------------------------------------------
#
# Project created by QtCreator 2011-08-20T16:33:59
#
#-------------------------------------------------

QT       += core gui serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = AutoSemaforo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tporta.cpp \
    configuracao.cpp \
    about.cpp

HEADERS  += mainwindow.h \
    tporta.h \
    configuracao.h \
    about.h \
    version.h

FORMS    += mainwindow.ui \
    configuracao.ui \
    about.ui

RESOURCES += \
    res.qrc

RC_FILE = resources.rc

#LIBS += -lQt5ExtSerialPort

#INCLUDEPATH += C:\qextserialport-1.2rc\src

#unix:DEFINES   = _TTY_POSIX_
#win32:DEFINES  = _TTY_WIN_

OTHER_FILES += \
    resources.rc
