#################################################
# Made by Jordan. Do not edit. Only for testing
# Date: December 10, 2012
#################################################

TARGET = UnivursaEditor
TEMPLATE = app

CONFIG += qt console

unix {
  LIBS += -lGLU
  LIBS += -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf
}
macx {
  INCLUDEPATH = "/usr/include"
  QMAKE_LFLAGS += -F/Library/Frameworks
  LIBS += -framework SDL2 -framework SDL2_image -framework SDL2_mixer -framework SDL2_ttf
  LIBS -= -lGLU
  LIBS -= -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf 
}
win32 {
  LIBS += -lmingw32 -lSDL2main
  LIBS += -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf
}

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXX_FLAGS_WARN_ON += -Wall
QT += opengl widgets #phonon network xml, core and gui are default

SOURCES += \
    src/Main.cc \
    src/Application.cc \
    src/EditorHelpers.cc \
    src/Database/EditorAction.cc \
    src/Database/EditorBubby.cc \
    src/Database/EditorCategory.cc \
    src/Database/EditorEquipment.cc \
    src/Database/EditorEvent.cc \
    src/Database/EditorItem.cc \
    src/Database/EditorMap.cc \
    src/Database/EditorMapPerson.cc \
    src/Database/EditorMapThing.cc \
    src/Database/EditorMatrix.cc \
    src/Database/EditorParty.cc \
    src/Database/EditorPerson.cc \
    src/Database/EditorSkill.cc \
    src/Database/EditorSkillset.cc \
    src/Database/EditorSprite.cc \
    src/Database/EditorTile.cc \
    src/Database/EditorTileSprite.cc \
    src/Database/GameDatabase.cc \
    src/Dialog/ConvoDialog.cc \
    src/Dialog/FrameDialog.cc \
    src/Dialog/FrameList.cc \
    src/Dialog/FrameView.cc \
    src/Dialog/MatrixDialog.cc \
    src/Dialog/SpriteDialog.cc \
    src/Dialog/ThingDialog.cc \
    src/View/EventView.cc \
    src/View/GameView.cc \
    src/View/MapControl.cc \
    src/View/MapDatabase.cc \
    src/View/MapRender.cc \
    src/View/MapView.cc \
    src/View/MatrixView.cc \
    src/View/RawImage.cc \
    src/View/RawImageList.cc \
    src/View/RawImageView.cc \
    src/View/SpriteView.cc \
    src/View/SpriteViewList.cc \
    src/View/ThingView.cc \
    ../Project/src/FileHandler.cc \
    ../Project/src/Frame.cc \
    ../Project/src/Helpers.cc \
    ../Project/src/Md5.cc \
    ../Project/src/Sprite.cc \
    ../Project/src/TinyXML.cc \
    ../Project/src/XmlData.cc \
    ../Project/src/Game/EventHandler.cc \
    ../Project/src/Game/Map/MapThing.cc \
    ../Project/src/Game/Map/SpriteMatrix.cc \
    ../Project/src/Game/Map/Tile.cc \
    ../Project/src/Game/Map/TileSprite.cc \
    ../Project/src/Game/Player/Action.cc \
    ../Project/src/Game/Battle/AIModule.cc \
    ../Project/src/Game/Player/AttributeSet.cc \
    ../Project/src/Game/Player/Bubby.cc \
    ../Project/src/Game/Player/Category.cc \
    ../Project/src/Game/Player/Equipment.cc \
    ../Project/src/Game/Player/Flavour.cc \
    ../Project/src/Game/Player/Item.cc \
    ../Project/src/Game/Player/Party.cc \
    ../Project/src/Game/Player/Person.cc \
    ../Project/src/Game/Player/Signature.cc \
    ../Project/src/Game/Player/Skill.cc \
    ../Project/src/Game/Player/SkillSet.cc

HEADERS += \
    include/Application.h \
    include/EditorEnumDb.h \
    include/EditorHelpers.h \
    include/Database/EditorAction.h \
    include/Database/EditorBubby.h \
    include/Database/EditorCategory.h \
    include/Database/EditorEquipment.h \
    include/Database/EditorEvent.h \
    include/Database/EditorItem.h \
    include/Database/EditorMap.h \
    include/Database/EditorMapPerson.h \
    include/Database/EditorMapThing.h \
    include/Database/EditorMatrix.h \
    include/Database/EditorParty.h \
    include/Database/EditorPerson.h \
    include/Database/EditorSkill.h \
    include/Database/EditorSkillset.h \
    include/Database/EditorSprite.h \
    include/Database/EditorTemplate.h \
    include/Database/EditorTile.h \
    include/Database/EditorTileSprite.h \
    include/Database/GameDatabase.h \
    include/Dialog/ConvoDialog.h \
    include/Dialog/FrameDialog.h \
    include/Dialog/FrameList.h \
    include/Dialog/FrameView.h \
    include/Dialog/MatrixDialog.h \
    include/Dialog/SpriteDialog.h \
    include/Dialog/ThingDialog.h \
    include/View/EventView.h \
    include/View/GameView.h \
    include/View/MapControl.h \
    include/View/MapDatabase.h \
    include/View/MapRender.h \
    include/View/MapView.h \
    include/View/MatrixView.h \
    include/View/RawImage.h \
    include/View/RawImageList.h \
    include/View/RawImageView.h \
    include/View/SpriteView.h \
    include/View/SpriteViewList.h \
    include/View/ThingView.h \
    ../Project/include/EnumDb.h \
    ../Project/include/EnumFlags.h \
    ../Project/include/FileHandler.h \
    ../Project/include/Frame.h \
    ../Project/include/Helpers.h \
    ../Project/include/Md5.h \
    ../Project/include/Sprite.h \
    ../Project/include/TinyXML.h \
    ../Project/include/XmlData.h \
    ../Project/include/Game/EventHandler.h \
    ../Project/include/Game/Map/MapThing.h \
    ../Project/include/Game/Map/SpriteMatrix.h \
    ../Project/include/Game/Map/Tile.h \
    ../Project/include/Game/Map/TileSprite.h \
    ../Project/include/Game/Battle/AIModule.h \
    ../Project/include/Game/Player/AttributeSet.h \
    ../Project/include/Game/Player/Action.h \
    ../Project/include/Game/Player/Bubby.h \
    ../Project/include/Game/Player/Category.h \
    ../Project/include/Game/Player/Equipment.h \
    ../Project/include/Game/Player/Flavour.h \
    ../Project/include/Game/Player/Item.h \
    ../Project/include/Game/Player/Party.h \
    ../Project/include/Game/Player/Person.h \
    ../Project/include/Game/Player/Signature.h \
    ../Project/include/Game/Player/Skill.h \
    ../Project/include/Game/Player/SkillSet.h

INCLUDEPATH += ./include
INCLUDEPATH += ../Project/include

RESOURCES += resources.qrc
