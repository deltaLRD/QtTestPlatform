#ifndef BALLSCENE_H
#define BALLSCENE_H

#include <QObject>

#include "ball.h"
#include "scene.h"
#include "sprite.h"
#include "resmanager.h"
#include "animsprite.h"
#include "audioplayer.h"

#include "SigSlot/sigslot.h"

class BallScene : public Scene, public SigSlotBase
{
    Q_OBJECT
public:
    explicit BallScene(Ball *ball, QObject *parent = nullptr);
    //重载绘制函数
    void render(Renderer *renderer, QMatrix4x4 inModel = QMatrix4x4()) override;

private:
    //持有逻辑球对象
    Ball *ball;
    //记录球位置、角度
    double x, y;
    int angle;
    //球飞行精灵
    Sprite *body;

signals:
    void explode(double x, double y);

};

#endif // BALLSCENE_H
