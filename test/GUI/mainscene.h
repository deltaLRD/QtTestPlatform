#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QtMath>
#include <QObject>
#include <QMatrix4x4>
#include <QOpenGLTexture>

#include "const.h"
#include "ball.h"
#include "plane.h"
#include "bullet.h"

#include "scene.h"
#include "ballscene.h"
#include "resmanager.h"
#include "planescene.h"
#include "bulletscene.h"
#include "audioplayer.h"
#include "explodescene.h"
#include "SigSlot/sigslot.h"

class Core;
class Player;

class MainScene : public Scene, public SigSlotBase
{
    Q_OBJECT
public:
    enum GameType
    {
        PVP            = 0x0001,
        PVE            = 0x0002,
    };
    explicit MainScene(QObject *parent = nullptr);
    ~MainScene();
    //更新窗口大小
    void resize(int w, int h);
    //设置开始游戏
    void startGame(Core *core);
    //设置结束游戏
    void gameOver();
    //添加球场景
    void newBall(Ball *ball);
    //添加飞机场景
    void newPlane(Plane *plane);
    //飞机分数改变
    void scoreChange(Plane *plane,int score);
    //绘制函数
    void render(Renderer *renderer, QMatrix4x4 inModel = QMatrix4x4()) override;

private:
    //背景音乐
    AudioPlayer *bgMusic;
    //窗口宽高
    int w, h;
    //游戏剩余时间
    long time;
    //游戏核心
    Core *core;
    //游戏播放器
    Player *player;
    //游戏模式
    GameType type;
    //两人的分数
    int p1Score, p2Score;
    //记录飞机指针
    Plane *planeOne, *planeTwo;

signals:

};

#endif // MAINSCENE_H
