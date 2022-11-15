#ifndef PLANESCENE_H
#define PLANESCENE_H

#include <QColor>
#include <QObject>
#include <QMatrix4x4>
#include <QOpenGLTexture>

#include "scene.h"
#include "plane.h"
#include "sprite.h"
#include "resmanager.h"
#include "animsprite.h"
#include "audioplayer.h"

#include "SigSlot/sigslot.h"
class PlaneScene : public Scene, public SigSlotBase
{
    Q_OBJECT
public:
    explicit PlaneScene(Plane *plane, const QString& color, QObject *parent = nullptr);
    ~PlaneScene();
    //重载绘制函数
    void render(Renderer *renderer, QMatrix4x4 inModel = QMatrix4x4()) override;

private:
    enum PlaneState
    {
        NORMAL               = 0x0001,
        INVINCIBLE           = 0x0002,
        EXPLODED             = 0x0004,
    };
    //持有逻辑飞机对象
    Plane *plane;
    //飞机的颜色
    QString color;
    QVector3D colorVec;
    //飞机的状态
    PlaneState planeState;
    //记录飞机位置、角度
    double x, y, angle;
    //无敌状态闪烁
    int invincibleCount;
    //机身、护盾精灵
    Sprite *body, *shield, *mp;
    //左右喷气动画精灵
    AnimSprite *fireLeft, *fireRight;
    //启用护盾、护盾消失、飞行声音播放器
    AudioPlayer *shield_up, *shield_down, *flight_sound;

signals:
    void explode(double x, double y);

};

#endif // PLANESCENE_H
