#-------------------------------------------------
#
# Project created by QtCreator 2017-04-29T21:31:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AudioTube
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serveur.cpp \
    automate.cpp

HEADERS  += mainwindow.h \
    serveur.h \
    automate.h

FORMS    += mainwindow.ui
