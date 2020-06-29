TARGET = FISEditor
TEMPLATE = app

QT += core gui widgets multimedia #opengl?

CONFIG += c++11 #console? (console output), static? (static binary)

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    src/Main.cc \
    src/Application.cc \
    src/EditorHelpers.cc \
    src/Database/EditorAction.cc \
    src/Database/EditorBattleScene.cc \
    src/Database/EditorCategory.cc \
    src/Database/EditorEvent.cc \
    src/Database/EditorEventSet.cc \
    src/Database/EditorItem.cc \
    src/Database/EditorLock.cc \
    src/Database/EditorMap.cc \
    src/Database/EditorMapIO.cc \
    src/Database/EditorMapItem.cc \
    src/Database/EditorMapNPC.cc \
    src/Database/EditorMapPerson.cc \
    src/Database/EditorMapThing.cc \
    src/Database/EditorMatrix.cc \
    src/Database/EditorNPCPath.cc \
    src/Database/EditorParty.cc \
    src/Database/EditorPerson.cc \
    src/Database/EditorSkill.cc \
    src/Database/EditorSkillset.cc \
    src/Database/EditorSound.cc \
    src/Database/EditorSoundDb.cc \
    src/Database/EditorSprite.cc \
    src/Database/EditorTile.cc \
    src/Database/EditorTileSprite.cc \
    src/Database/GameDatabase.cc \
    src/Dialog/ConvoDialog.cc \
    src/Dialog/EventDialog.cc \
    src/Dialog/InstanceDialog.cc \
    src/Dialog/FrameDialog.cc \
    src/Dialog/FrameList.cc \
    src/Dialog/FrameView.cc \
    src/Dialog/IODialog.cc \
    src/Dialog/ItemDialog.cc \
    src/Dialog/MatrixDialog.cc \
    src/Dialog/NodeDialog.cc \
    src/Dialog/PersonDialog.cc \
    src/Dialog/SpriteDialog.cc \
    src/Dialog/ThingDialog.cc \
    src/View/BattleSceneView.cc \
    src/View/EventSetView.cc \
    src/View/EventView.cc \
    src/View/GameView.cc \
    src/View/LockView.cc \
    src/View/MapBattleSceneView.cc \
    src/View/MapControl.cc \
    src/View/MapDatabase.cc \
    src/View/MapIOView.cc \
    src/View/MapItemView.cc \
    src/View/MapLayView.cc \
    src/View/MapMusicView.cc \
    src/View/MapNPCView.cc \
    src/View/MapPersonView.cc \
    src/View/MapRender.cc \
    src/View/MapThingView.cc \
    src/View/MapView.cc \
    src/View/MatrixView.cc \
    src/View/RawImage.cc \
    src/View/RawImageList.cc \
    src/View/RawImageView.cc \
    src/View/SoundView.cc \
    src/View/SpriteView.cc \
    src/View/SpriteViewList.cc \
    lib/fis-types/src/Foundation/FrameCore.cc \
    lib/fis-types/src/Foundation/SpriteCore.cc \
    lib/fis-types/src/Persistence/XmlData.cc \
    lib/fis-types/src/Persistence/XmlWriter.cc

HEADERS += \
    include/Application.h \
    include/EditorEnumDb.h \
    include/EditorHelpers.h \
    include/Database/EditorAction.h \
    include/Database/EditorBattleScene.h \
    include/Database/EditorCategory.h \
    include/Database/EditorEvent.h \
    include/Database/EditorEventSet.h \
    include/Database/EditorItem.h \
    include/Database/EditorLock.h \
    include/Database/EditorMap.h \
    include/Database/EditorMapIO.h \
    include/Database/EditorMapItem.h \
    include/Database/EditorMapNPC.h \
    include/Database/EditorMapPerson.h \
    include/Database/EditorMapThing.h \
    include/Database/EditorMatrix.h \
    include/Database/EditorNPCPath.h \
    include/Database/EditorParty.h \
    include/Database/EditorPerson.h \
    include/Database/EditorSkill.h \
    include/Database/EditorSkillset.h \
    include/Database/EditorSound.h \
    include/Database/EditorSoundDb.h \
    include/Database/EditorSprite.h \
    include/Database/EditorTemplate.h \
    include/Database/EditorTile.h \
    include/Database/EditorTileSprite.h \
    include/Database/GameDatabase.h \
    include/Dialog/ConvoDialog.h \
    include/Dialog/EventDialog.h \
    include/Dialog/InstanceDialog.h \
    include/Dialog/FrameDialog.h \
    include/Dialog/FrameList.h \
    include/Dialog/FrameView.h \
    include/Dialog/IODialog.h \
    include/Dialog/ItemDialog.h \
    include/Dialog/MatrixDialog.h \
    include/Dialog/NodeDialog.h \
    include/Dialog/PersonDialog.h \
    include/Dialog/SpriteDialog.h \
    include/Dialog/ThingDialog.h \
    include/View/BattleSceneView.h \
    include/View/EventSetView.h \
    include/View/EventView.h \
    include/View/GameView.h \
    include/View/LockView.h \
    include/View/MapBattleSceneView.h \
    include/View/MapControl.h \
    include/View/MapDatabase.h \
    include/View/MapIOView.h \
    include/View/MapItemView.h \
    include/View/MapLayView.h \
    include/View/MapMusicView.h \
    include/View/MapNPCView.h \
    include/View/MapPersonView.h \
    include/View/MapRender.h \
    include/View/MapThingView.h \
    include/View/MapView.h \
    include/View/MatrixView.h \
    include/View/RawImage.h \
    include/View/RawImageList.h \
    include/View/RawImageView.h \
    include/View/SoundView.h \
    include/View/SpriteView.h \
    include/View/SpriteViewList.h

INCLUDEPATH += \
    include \
    lib/fis-types/include

RESOURCES += resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
