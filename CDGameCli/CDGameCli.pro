QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    GUI/animsprite.cpp \
    GUI/audioplayer.cpp \
    GUI/ballscene.cpp \
    GUI/bulletscene.cpp \
    GUI/explodescene.cpp \
    GUI/glwidget.cpp \
    GUI/keywidget.cpp \
    GUI/mainscene.cpp \
    GUI/mainwidget.cpp \
    GUI/planescene.cpp \
    GUI/player.cpp \
    GUI/recorder.cpp \
    GUI/renderer.cpp \
    GUI/resmanager.cpp \
    GUI/scene.cpp \
    GUI/sprite.cpp \
    JSON/CJsonObject.cpp \
    JSON/cJSON.c \
    SigSlot/sigslot.cpp \
    ball.cpp \
    ballpool.cpp \
    bullet.cpp \
    bulletpool.cpp \
    cdgamecli.cpp \
    core.cpp \
    flyobject.cpp \
    objectpool.cpp \
    plane.cpp \
    pooledobject.cpp

HEADERS += \
    DataStruct.h \
    GUI/animsprite.h \
    GUI/audioplayer.h \
    GUI/ballscene.h \
    GUI/bulletscene.h \
    GUI/explodescene.h \
    GUI/glwidget.h \
    GUI/keywidget.h \
    GUI/mainscene.h \
    GUI/mainwidget.h \
    GUI/planescene.h \
    GUI/player.h \
    GUI/renderer.h \
    GUI/resmanager.h \
    GUI/scene.h \
    GUI/sprite.h \
    JSON/CJsonObject.hpp \
    JSON/cJSON.h \
    SigSlot/sigslot.h \
    ai1.h \
    ball.h \
    ballpool.h \
    bullet.h \
    bulletpool.h \
    cdgamecli.h \
    const.h \
    core.h \
    flyobject.h \
    globject.h \
    objectpool.h \
    plane.h \
    pooledobject.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

FORMS += \
    GUI/keywidget.ui \
    GUI/mainwidget.ui

RESOURCES += \
    res.qrc

LIBS += -L$$PWD/lib/ -lirrKlang
LIBS += -L$$PWD/lib/ -lfreetype

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
