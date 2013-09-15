#-------------------------------------------------
#
# Project created by QtCreator 2013-09-07T12:31:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ZynAddSubFX
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    addsynthwidget.cpp \
    lfowidget.cpp \
    zyndial.cpp \
    Connection.cpp \
    NSM.cpp \
    MasterUI.cpp

HEADERS  += mainwindow.h \
    addsynthwidget.h \
    lfowidget.h \
    zyndial.h \
    models.h \
    MasterUI.h \
    NSM.h

FORMS    += mainwindow.ui \
    addsynthwidget.ui \
    lfowidget.ui \
    zyndial.ui

RESOURCES += \
    ressources.qrc


# added for compilation
QMAKE_CXXFLAGS += -std=c++0x
INCLUDEPATH += ../../rtosc/include ../src ../..
#LIBS += -Labs/build/src/ -lzynaddsubfx_core

