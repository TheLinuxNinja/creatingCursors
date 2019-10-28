#-------------------------------------------------
#
# Project created by QtCreator 2016-12-13T12:17:07
#
#-------------------------------------------------

QT       += core gui x11extras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mouseLang
TEMPLATE = app

LIBS += -lXfixes  -lX11

SOURCES += main.cpp\
        mainwindow.cpp \
    XKeyboard.cpp

HEADERS  += mainwindow.h \
    XKeyboard.h \
    X11Exception.h

FORMS    += mainwindow.ui

RESOURCES += \
    flags.qrc
