#-------------------------------------------------
#
# Project created by QtCreator 2015-06-25T14:08:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = qt5-cpuload
TEMPLATE = app


SOURCES += main.cpp\
        cpuload.cpp \
    qcustomplot.cpp

HEADERS  += cpuload.h \
    qcustomplot.h

FORMS    += \
    cpuload.ui

#LIBS     += -lqcustomplot
