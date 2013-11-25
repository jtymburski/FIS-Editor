#################################################
# Made by Jordan. Do not edit. Only for testing
# Date: December 10, 2012
#################################################

TARGET = UnivursaEditor
TEMPLATE = app

CONFIG += qt
CONFIG += console
DESTDIR = ./bin
macx {
  INCLUDEPATH += "/Library/Frameworks/SDL.framework/Headers/"
}

win32 {
  QMAKE_CXXFLAGS += -std=c++11
  LIBS += -lmingw32 -lSDL2main
  win32:QMAKE_CXX_FLAGS_WARN_ON += -Wall
}
LIBS += -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf
unix:LIBS += -lGLU
QT += opengl widgets #phonon network xml, core and gui are default
#RESOURCES = Resources.qrc

SOURCES += \
    ./src/Main.cc \
    ./src/Application.cc \
    ./src/Map/MapEditor.cc \
    ./src/SpriteToolbox.cc \
	../Project/src/Helpers.cc \
	../Project/src/Frame.cc \
    ../Project/src/Sprite.cc \
    src/SpriteChoice.cc \
    src/EditorSprite.cc \
    src/EditorSpriteToolbox.cc \
    src/SpriteCreationDialog.cc

HEADERS += \
    ./include/Application.h \
    ./include/EnumDB.h \
    ./include/Map/MapEditor.h \
    ./include/SpriteToolbox.h \
	../Project/include/Helpers.h \
	../Project/include/Frame.h \
    ../Project/include/Sprite.h \
    include/SpriteChoice.h \
    include/EditorSprite.h \
    include/EditorSpriteToolbox.h \
    include/SpriteCreationDialog.h

INCLUDEPATH += ./include
INCLUDEPATH += ../Project/include
