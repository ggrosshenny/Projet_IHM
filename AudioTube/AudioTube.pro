#-------------------------------------------------
#
# Project created by QtCreator 2017-04-29T21:31:06
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AudioTube
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serveur.cpp \
    sendjsoncommandToMPV.cpp \
    sendjsoncommandtoserver.cpp

HEADERS  += mainwindow.h \
    serveur.h \
    sendjsoncommandtoserver.h \
    sendjsoncommandToMPV.h \
    codecommandes.h

FORMS    += mainwindow.ui
