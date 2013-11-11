#################################################
# Made by Jordan. Do not edit. Only for testing
# Date: December 10, 2012
#################################################

TARGET = UnivursaEditor
TEMPLATE = app

CONFIG += qt
DESTDIR = ./bin
macx {
  INCLUDEPATH += "/Library/Frameworks/SDL.framework/Headers/"
}

win32 {
  LIBS += -lmingw32 -l SDLmain
  win32:QMAKE_CXX_FLAGS_WARN_ON += -Wall
}
LIBS += -lSDL -lSDL_mixer
unix:LIBS += -lGLU
QT += opengl widgets #phonon network xml, core and gui are default
#RESOURCES = Resources.qrc

SOURCES += \
    ./src/Main.cc \
    ./src/Application.cc \
    ./src/Map/MapEditor.cc \
    ../Project/src/Sound.cc

HEADERS += \
    ./include/Application.h \
    ./include/Map/MapEditor.h \
    ../Project/include/Sound.h

INCLUDEPATH += ./include
INCLUDEPATH += ../Project/include
