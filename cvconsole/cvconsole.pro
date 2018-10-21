QT += core
QT -= gui

CONFIG += c++11

TARGET = cvconsole
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp


#INCLUDEPATH += -IF:\lib\opencv\build\include
#INCLUDEPATH += $$PWD\F:\lib\opencv\build\include
#INCLUDEPATH += -I"F:\lib\opencv\build\include"
INCLUDEPATH += F:\lib\opencv\build\include
CONFIG(release ,debug|release)
{
LIBS += -L"F:\lib\opencv\build\x64\vc14\bin"
LIBS += -lopencv_world342
}
CONFIG(debug ,debug|release)
{
LIBS += -L"F:\lib\opencv\build\x64\vc14\bin"
LIBS += -lopencv_world342d
}


#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/F:/lib/opencv/build/x64/vc14/bin/ -lopencv_world342d
#else:unix: LIBS += -L$$PWD/F:/lib/opencv/build/x64/vc14/bin/ -lopencv_world342

#INCLUDEPATH += $$PWD/F:/lib/opencv/build/include
#DEPENDPATH += $$PWD/F:/lib/opencv/build/include

