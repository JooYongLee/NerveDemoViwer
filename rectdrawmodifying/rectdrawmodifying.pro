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
    draggablerectitem.cpp \
    scenebox.cpp

HEADERS  += mainwindow.h \
    rectdragitem.h \
    draggablerectitem.h \
    scenebox.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
