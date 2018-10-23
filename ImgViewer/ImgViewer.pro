#-------------------------------------------------
#
# Project created by QtCreator 2018-10-22T20:25:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImgViewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    filemanager.cpp \
    cameraviewbutton.cpp

HEADERS  += mainwindow.h \
    filemanager.h \
    cameraviewbutton.h

FORMS    += mainwindow.ui
