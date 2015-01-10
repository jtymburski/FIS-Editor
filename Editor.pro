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
    src/View/MapRender.cc \
    ../Project/src/Helpers.cc \
    ../Project/src/Frame.cc \
    ../Project/src/Sprite.cc \
    ../Project/src/XmlData.cc \
    ../Project/src/Game/Map/Tile.cc \
    src/Database/EditorSprite.cc \
    ../Project/src/Game/EventHandler.cc \
    src/View/EditorSpriteList.cc \
    src/View/RawImageView.cc \
    src/View/RawImageList.cc \
    src/View/RawImage.cc \
    src/View/SpriteView.cc \
    src/View/TileRender.cc \
    src/Dialog/SpriteDialog.cc \
    src/Dialog/FrameList.cc \
    src/Dialog/FrameView.cc \
    src/Dialog/FrameDialog.cc \
    src/View/GameView.cc \
    src/View/MapView.cc \
    src/View/MapDatabase.cc \
    src/View/MapControl.cc \
    src/Database/GameDatabase.cc \
    src/Database/EditorMap.cc \
    src/Database/EditorPerson.cc \
    src/Database/EditorTile.cc \
    src/Database/EditorThing.cc \
    src/Database/EditorItem.cc \
    src/Database/EditorParty.cc \
    src/Database/EditorAction.cc \
    ../Project/src/Game/Player/AttributeSet.cc \
    ../Project/src/Game/Player/Action.cc \
    src/Database/EditorCategory.cc \
    src/Database/EditorSkillset.cc \
    src/Database/EditorSkill.cc \
    src/Database/EditorEquipment.cc \
    src/Database/EditorBubby.cc \
    ../Project/src/Game/Player/SkillSet.cc \
    ../Project/src/Game/Player/Skill.cc \
    ../Project/src/Game/Player/Person.cc \
    ../Project/src/Game/Player/Party.cc \
    ../Project/src/Game/Player/Item.cc \
    ../Project/src/Game/Player/Equipment.cc \
    ../Project/src/Game/Player/Category.cc \
    ../Project/src/Game/Player/Bubby.cc \
    ../Project/src/Game/Player/Signature.cc \
    ../Project/src/Game/Player/Flavour.cc \
    ../Project/src/Game/Battle/AIModule.cc \

HEADERS += \
    include/Application.h \
    include/EditorEnumDb.h \
    include/EditorHelpers.h \
    include/View/MapRender.h \
    ../Project/include/EnumDb.h \
    ../Project/include/Helpers.h \
    ../Project/include/Frame.h \
    ../Project/include/Sprite.h \
    ../Project/include/XmlData.h \
    ../Project/include/Game/Map/Tile.h \
    include/Database/EditorSprite.h \
    ../Project/include/Game/EventHandler.h \
    include/View/EditorSpriteList.h \
    include/View/RawImageView.h \
    include/View/RawImageList.h \
    include/View/RawImage.h \
    include/View/SpriteView.h \
    include/View/TileRender.h \
    include/Dialog/SpriteDialog.h \
    include/Dialog/FrameList.h \
    include/Dialog/FrameView.h \
    include/Dialog/FrameDialog.h \
    include/View/GameView.h \
    include/View/MapView.h \
    include/View/MapDatabase.h \
    include/View/MapControl.h \
    include/Database/EditorMap.h \
    include/Database/EditorPerson.h \
    include/Database/EditorThing.h \
    include/Database/EditorTile.h \
    include/Database/GameDatabase.h \
    include/Database/EditorItem.h \
    include/Database/EditorParty.h \
    include/Database/EditorAction.h \
    ../Project/include/EnumFlags.h \
    ../Project/include/Game/Player/AttributeSet.h \
    ../Project/include/Game/Player/Action.h \
    include/Database/EditorCategory.h \
    include/Database/EditorSkillset.h \
    include/Database/EditorSkill.h \
    include/Database/EditorEquipment.h \
    include/Database/EditorBubby.h \
    ../Project/include/Game/Player/SkillSet.h \
    ../Project/include/Game/Player/Skill.h \
    ../Project/include/Game/Player/Person.h \
    ../Project/include/Game/Player/Party.h \
    ../Project/include/Game/Player/Item.h \
    ../Project/include/Game/Player/Equipment.h \
    ../Project/include/Game/Player/Category.h \
    ../Project/include/Game/Player/Bubby.h \
    ../Project/include/Game/Battle/AIModule.h \
    ../Project/include/Game/Player/Signature.h \
    ../Project/include/Game/Player/Flavour.h \

INCLUDEPATH += ./include
INCLUDEPATH += ../Project/include

RESOURCES += \
    src/Resource_Pack.qrc
