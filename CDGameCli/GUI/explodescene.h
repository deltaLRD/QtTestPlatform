#ifndef EXPLODESCENE_H
#define EXPLODESCENE_H

#include <QObject>

#include "scene.h"
#include "resmanager.h"
#include "animsprite.h"
#include "audioplayer.h"

class ExplodeScene : public Scene
{
    Q_OBJECT
public:
    explicit ExplodeScene(double x, double y, const QString &type, QObject *parent = nullptr);

private:
    //爆炸动画精灵
    AnimSprite *explodeSprite;
    //爆炸声音播放器
    AudioPlayer *explodePlayer;

signals:

};

#endif // EXPLODESCENE_H
