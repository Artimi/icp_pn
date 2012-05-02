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
    manipulatenet.cpp \
    ../pn/message.cpp \
    petrinet.cpp \
    petrinetitem.cpp \
    petrinetarrow.cpp \
    petrinetplace.cpp \
    petrinettransition.cpp \
    petrinetobject.cpp \
    xmlhandler.cpp

HEADERS += \
    server.h \
    thread.h \
    manipulatenet.h \
    ../pn/message.h \
    petrinet.h \
    petrinetitem.h \
    petrinetarrow.h \
    petrinetplace.h \
    petrinettransition.h \
    petrinetobject.h \
    xmlhandler.h
