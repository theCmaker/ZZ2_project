#-------------------------------------------------
#
# Project created by QtCreator 2015-11-12T16:15:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Pareto
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        mainwindow.cpp \
    Solutions.cpp \
    qcustomplot.cpp \
    qcp_hover.cpp \
    style.cpp

HEADERS  += mainwindow.h \
    Solutions.h \
    debug_stream.h \
    qcustomplot.h \
    Point.h \
    qcp_hover.h \
    style.h

FORMS    += mainwindow.ui
