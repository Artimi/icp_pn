#-------------------------------------------------
#
# Project created by QtCreator 2012-04-27T13:25:55
#
#-------------------------------------------------

QT       += core
QT       += network
#QT       -= gui

TARGET = server2012
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    server.cpp \
    thread.cpp \
    servermain.cpp \
    ../pn/xmlhandler.cpp \
    ../pn/transition.cpp  \
    ../pn/place.cpp \
    ../pn/diagramscene.cpp \
    ../pn/diagramitem.cpp \
    ../pn/arrow.cpp \
    ../pn/message.cpp \
    manipulatenet.cpp

HEADERS += \
    server.h \
    thread.h \
    ../pn/xmlhandler.h \
    ../pn/transition.h  \
    ../pn/place.h \
    ../pn/diagramscene.h \
    ../pn/diagramitem.h \
    ../pn/arrow.h \
    ../pn/message.h \
    manipulatenet.h
