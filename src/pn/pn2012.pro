######################################################################
# Automatically generated by qmake (2.01a) Mon Apr 23 10:41:08 2012
######################################################################

TEMPLATE = app
TARGET = pn2012
QT       += network
DEPENDPATH += ../../../icp_pn .
INCLUDEPATH += .

# Input
HEADERS += arrow.h \
           diagramitem.h \
           diagramscene.h \
           mainwindow.h \
           place.h \
           transition.h \
           xmlhandler.h \
    netinformation.h \
    message.h \
    connect.h \
    loginwindow.h
FORMS += mainwindow.ui \
    netinformation.ui \
    connect.ui \
    loginwindow.ui
SOURCES += arrow.cpp \
           diagramitem.cpp \
           diagramscene.cpp \
           mainwindow.cpp \
           place.cpp \
           transition.cpp \
           xmlhandler.cpp \
    netinformation.cpp \
    clientmain.cpp \
    message.cpp \
    connect.cpp \
    loginwindow.cpp
