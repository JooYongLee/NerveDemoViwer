#-------------------------------------------------
#
# Project created by QtCreator 2018-10-30T22:03:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dcmReader
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dcmcore.cpp

HEADERS  += mainwindow.h \
    dcmcore.h

FORMS    += mainwindow.ui

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/F:/workspace/dcmtk-3.6.3/build/DCMTK/Release/lib/ -ldcmdata
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/F:/workspace/dcmtk-3.6.3/build/DCMTK/Release/lib/ -ldcmdatad
#else:unix: LIBS += -L$$PWD/F:/workspace/dcmtk-3.6.3/build/DCMTK/Release/lib/ -ldcmdata

DEFINES -= UNICODE
DEFINES += _MBCS

INCLUDEPATH += F:/workspace/dcmtk-3.6.3/build/DCMTK/include
DEPENDPATH += F:/workspace/dcmtk-3.6.3/build/DCMTK/include
CONFIG(release ,debug|release)
{
LIBS += -L"F:/workspace/dcmtk-3.6.3/build/DCMTK/Release/bin"
LIBS += -ldcmdata
LIBS += -loflog
LIBS += -lofstd
LIBS += -ldcmimgle
LIBS += -ldcmimage

}
CONFIG(debug ,debug|release)
{
LIBS += -L"F:/workspace/dcmtk-3.6.3/build/DCMTK/Debug/bin/"
LIBS += -ldcmdata
LIBS += -loflog
LIBS += -lofstd
LIBS += -ldcmimgle
LIBS += -ldcmimage

}
