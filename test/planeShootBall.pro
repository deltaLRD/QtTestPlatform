#-------------------------------------------------
#
# Project created by QtCreator 2020-12-15T12:27:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = planeShootBall
TEMPLATE = app

SOURCES += main.cpp\
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
    ballpool.cpp \
    bulletpool.cpp \
    objectpool.cpp \
    pooledobject.cpp \
    core.cpp \
    flyobject.cpp \
    plane.cpp \
    bullet.cpp \
    ball.cpp \
    ai_pve.cpp \
    pvpwidget.cpp \
    ai_pvp_ai2.cpp \
    ai_pvp_ai1.cpp

HEADERS  += \
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
    ballpool.h \
    bulletpool.h \
    const.h \
    core.h \
    flyobject.h \
    objectpool.h \
    plane.h \
    bullet.h \
    ball.h \
    pooledobject.h \
    pvpwidget.h

FORMS += \
    GUI/keywidget.ui \
    GUI/mainwidget.ui \
    pvpwidget.ui

RESOURCES += \
    res.qrc

LIBS += -L$$PWD/lib/ -lirrKlang
LIBS += -L$$PWD/lib/ -lfreetype

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

DISTFILES +=
