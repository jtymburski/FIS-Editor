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
	../Project/src/XmlData.cc \
	../Project/src/Game/Map/Tile.cc \
    src/SpriteChoice.cc \
    src/EditorSprite.cc \
    src/EditorSpriteToolbox.cc \
    src/SpriteCreationDialog.cc \
    src/TileWrapper.cc \
    ../Project/src/Game/EventHandler.cc \
    src/ImageSelectionModule.cc \
    src/SizeSelector.cc \
    src/EditorSpriteList.cc \
    src/FrameManipulator.cc \
    src/FrameManipulationDialog.cc \
    src/Manipulabel.cc

HEADERS += \
    ./include/Application.h \
    ./include/EditorEnumDb.h \
    ./include/Map/MapEditor.h \
    ./include/SpriteToolbox.h \
	../Project/include/EnumDb.h \
	../Project/include/Helpers.h \
	../Project/include/Frame.h \
    ../Project/include/Sprite.h \
	../Project/include/XmlData.h \
	../Project/include/Game/Map/Tile.h \
    include/SpriteChoice.h \
    include/EditorSprite.h \
    include/EditorSpriteToolbox.h \
    include/SpriteCreationDialog.h \
    include/TileWrapper.h \
    ../Project/include/Game/EventHandler.h \
    include/ImageSelectionModule.h \
    include/SizeSelector.h \
    include/EditorSpriteList.h \
    include/FrameManipulator.h \
    include/FrameManipulationDialog.h \
    include/Manipulabel.h

INCLUDEPATH += ./include
INCLUDEPATH += ../Project/include

RESOURCES += \
    src/Resource_Pack.qrc
