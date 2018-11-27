#-------------------------------------------------
#
# Project created by QtCreator 2018-11-18T12:33:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rectdrawmodifying
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    rectdragitem.cpp \
    scenebox.cpp \
    boundingbox.cpp \
    imgitem.cpp \
    filemanager.cpp

HEADERS  += mainwindow.h \    
    scenebox.h \
    boundingbox.h \
    imgitem.h \
    defineconfigure.h \
    filemanager.h \
    rectdragitem.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
