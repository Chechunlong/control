#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T17:53:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Controle
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    signal.cpp \
    control.cpp \
    controller.cpp \
    conectar.cpp \
    sistemao2.cpp \
    ../../Downloads/Controle-master/tanque.cpp

HEADERS  += mainwindow.h \
    quanser.h \
    qcustomplot.h \
    signal.h \
    control.h \
    controller.h \
    conectar.h \
    sistemaO2.h \
    ../../Downloads/Controle-master/tanque.h \
    tanque.h

FORMS    += mainwindow.ui \
    conectar.ui

DISTFILES +=
