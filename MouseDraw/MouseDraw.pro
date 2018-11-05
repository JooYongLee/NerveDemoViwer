#-------------------------------------------------
#
# Project created by QtCreator 2018-11-06T00:02:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MouseDraw
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    scene.cpp

HEADERS  += mainwindow.h \
    scene.h

FORMS    += mainwindow.ui


RESOURCES += \
    userdrawing.qrc
