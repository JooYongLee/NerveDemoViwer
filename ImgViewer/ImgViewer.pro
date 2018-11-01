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
    cameraviewbutton.cpp \
    imgreader.cpp \
    dcmcore.cpp

HEADERS  += mainwindow.h \
    filemanager.h \
    cameraviewbutton.h \
    imgreader.h \
    dcmcore.h

FORMS    += mainwindow.ui

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
