#ifndef BULLETSCENE_H
#define BULLETSCENE_H

#include <QObject>

#include "scene.h"
#include "bullet.h"
#include "sprite.h"
#include "resmanager.h"
#include "animsprite.h"
#include "audioplayer.h"
#include "SigSlot/sigslot.h"

class BulletScene : public Scene, public SigSlotBase
{
    Q_OBJECT
public:
    explicit BulletScene(Bullet *bullet, const QString& color, QObject *parent = nullptr);
    //重载绘制函数
    void render(Renderer *renderer, QMatrix4x4 inModel = QMatrix4x4()) override;

    //子弹消失函数
    void disappear();
    //子弹爆炸函数
    void explode();

private:
    enum BulletState
    {
        NORMAL               = 0x0001,
        EXPLODED             = 0x0004,
    };
    //持有逻辑子弹对象
    Bullet *bullet;
    //子弹的颜色
    QString color;
    //子弹的状态
    BulletState bulletState;
    //记录子弹位置、角度
    double x, y, angle;
    //子弹飞行精灵
    Sprite *body;
    //子弹发射动画精灵
    AnimSprite *shootAnim, *explodeAnim;
    //子弹发射声音播放器
    AudioPlayer *shootAudio, *explodeAudio;

signals:

};

#endif // BULLETSCENE_H
