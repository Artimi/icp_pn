#-------------------------------------------------
#
# Project created by QtCreator 2012-04-27T13:25:55
#
#-------------------------------------------------

QT       += core
QT       += network
QT       -= gui

TARGET = pnserver
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    server.cpp \
    thread.cpp \
    servermain.cpp

HEADERS += \
    server.h \
    thread.h
