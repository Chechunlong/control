#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T17:53:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = controle
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    signal.cpp

HEADERS  += mainwindow.h \
    quanser.h \
    qcustomplot.h \
    signal.h

FORMS    += mainwindow.ui
