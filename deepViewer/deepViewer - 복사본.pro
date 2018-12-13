#-------------------------------------------------
#
# Project created by QtCreator 2018-12-03T22:27:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = deepViewer
TEMPLATE = app

DEFINES -= UNICODE
DEFINES += _MBCS

INCLUDEPATH += D:/Library/dcmtk/include
DEPENDPATH += D:/Library/dcmtk/include

SOURCES += main.cpp\
        mainwindow.cpp \
    boundingbox.cpp \
    classtoolbarbutton.cpp \
    filemanager.cpp \
    imgitem.cpp \
    qjsonbox.cpp \
    rectdragitem.cpp \
    scenebox.cpp \
    dcmreader.cpp

HEADERS  += mainwindow.h \
    boundingbox.h \
    classtoolbarbutton.h \
    defineconfigure.h \
    filemanager.h \
    imgitem.h \
    qjsonbox.h \
    rectdragitem.h \
    scenebox.h \
    dcmreader.h

CONFIG(release ,debug|release)
{
LIBS += -L"D:/Library/dcmtk/release"
LIBS += -ldcmdata
LIBS += -loflog
LIBS += -lofstd
LIBS += -ldcmimgle
LIBS += -ldcmimage

}
CONFIG(debug ,debug|release)
{
LIBS += -L"D:/Library/dcmtk/debug"
LIBS += -ldcmdata
LIBS += -loflog
LIBS += -lofstd
LIBS += -ldcmimgle
LIBS += -ldcmimage
}


FORMS    += mainwindow.ui

RESOURCES += \
    resource/resource.qrc
