#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T17:53:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Controle
TEMPLATE = app

LIBS += -larmadillo

SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    signal.cpp \
    control.cpp \
    controller.cpp \
    conectar.cpp \
    sistemaO2.cpp \
    observador.cpp \
    seguidor.cpp \
    fuzzyUI.cpp

HEADERS  += mainwindow.h \
    quanser.h \
    qcustomplot.h \
    signal.h \
    control.h \
    controller.h \
    conectar.h \
    sistemaO2.h \
    tanque.h \
    filtrommv.h \
    observador.h \
    seguidor.h \
    matriz.h \
    fuzzyUI.h

FORMS    += mainwindow.ui \
    conectar.ui \
    fuzzy.ui

DISTFILES +=
