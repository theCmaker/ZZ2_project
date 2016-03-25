#-------------------------------------------------
#
# Project created by QtCreator 2016-03-06T14:36:25
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = BRKGA_test
CONFIG   += console
CONFIG   += c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ../data.cpp \
    ../decoder.cpp \
    ../front.cpp \
    ../geometry.cpp \
    ../Population.cpp \
    ../solution.cpp \
    ../pfront.cpp \
    ../Solutions.cpp

HEADERS += \
    ../BRKGA.h \
    ../data.h \
    ../decoder.h \
    ../front.h \
    ../geometry.h \
    ../MTRand.h \
    ../Population.h \
    ../solution.h \
    ../Point.h \
    ../pfront.h \
    ../Solutions.h
