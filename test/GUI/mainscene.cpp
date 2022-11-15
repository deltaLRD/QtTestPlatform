#include "mainscene.h"

#include "core.h"
#include "player.h"

MainScene::MainScene(QObject *parent) : Scene(parent), core(nullptr), player(nullptr), type(PVE), p1Score(0), p2Score(0),
    planeOne(nullptr), planeTwo(nullptr)
{
    //获取资源管理器
    ResManager *resManager = ResManager::getInstance();
    //获取背景纹理
    QOpenGLTexture *bgTexture = resManager->getTexture(":/textures/others/background");
    //创建背景精灵
    Sprite *bgSprite = new Sprite(bgTexture, this);
    //显示背景精灵
    bgSprite->show();
    //背景精灵模型矩阵
    QMatrix4x4 bgModel;
    bgModel.translate(WIDTH_WIDGET / 2, HEIGHT_WIDGET / 2);
    bgModel.scale((float)WIDTH_WIDGET / bgTexture->width());
    //设置模型矩阵
    bgSprite->setModel(bgModel);
    //加入背景精灵至精灵List
    addSprite(bgSprite);
    //创建背景音乐播放器
    bgMusic = new AudioPlayer(resManager->getISoundEngine(), resManager->getSound(":/audio/SimpleBeat.ogg"), this);
    //循环播放背景音乐
    bgMusic->play(true, qPow(10, -0.5));
    bgMusic->setPaused(true);
}

MainScene::~MainScene()
{

}

void MainScene::startGame(Core *core)
{
    //设置分数
    p1Score = 0;
    p2Score = 0;
    //设置核心
    this->core = core;
    //尝试获取播放器
    player = dynamic_cast<Player *>(core);
    //播放音乐
    bgMusic->setPaused(false);
}

void MainScene::gameOver()
{
    //核心置空
    core = nullptr;
    //播放器置空
    player = nullptr;
    //清空飞机
    planeOne = nullptr;
    planeTwo = nullptr;
    //清空场景
    for (Scene *scene : scenes) {
        scene->deleteLater();
    }
    //停止播放
    bgMusic->setPaused(true);
}

void MainScene::render(Renderer *renderer, QMatrix4x4 inModel)
{
    //判断渲染器是否为空
    if (!renderer)
    {
        //为空则报错
        qDebug() << "ERROR::MainScene::render::RendererIsNull";
    }
    //如果不是四比三
    if (w * 3 != h * 4)
    {
        //设置视口
        renderer->setViewport(w/ 2 - h * 2 / 3 , 0, h * 4 / 3, h);
    }
    //精灵透视矩阵
    QMatrix4x4 spriteProjection;
    spriteProjection.ortho(0, WIDTH_WIDGET, HEIGHT_WIDGET, 0, -1, 1);
    //设置精灵透视矩阵
    renderer->setSpriteProjection(spriteProjection);
    //调用基类绘制函数
    Scene::render(renderer, inModel);
    //如果不是四比三
    if (w * 3 != h * 4)
    {
        //设置视口
        renderer->setViewport(0, 0, w, h);
    }
    //设置字体颜色
    renderer->setTextColor(QVector3D(1.0f, 1.0f, 1.0f));
    //绘制游戏剩余时间
    if (core)
    {
        time = core->getRestTime();
    }
    static Character charH = ResManager::getInstance()->getCharacter((ushort)'H');
    renderer->drawText(QString("%1").arg(time, 2, 10, QLatin1Char('0')), w / 2, 0, 1, Renderer::CENTER);
    //游戏结束提示返回
    if (!core)
    {
        renderer->drawText("Click to return",
                           w / 2, h / 2 - charH.size.y() / 2, 1, Renderer::CENTER);
    }
    //绘制回放中提示
    if (player)
    {
        renderer->drawText(QString("Replaying"), w, 0, 1, Renderer::RIGHT);
        renderer->drawText(QString("X%1").arg(player->getPlaySpeed()),
                           w, charH.size.y() + 10, 1, Renderer::RIGHT);
    }
    renderer->drawText("score:", 0, 0, 1);
    //绘制各玩家分数
    renderer->drawText(QString("p1:%1")
                       .arg(p1Score, 2, 10, QLatin1Char('0')),
                       0, charH.size.y() + 10, 1, Renderer::LEFT, false);
    if (type == PVP)
    {
        renderer->drawText(QString("p2:%1")
                           .arg(p2Score, 2, 10, QLatin1Char('0')),
                           0, (charH.size.y() + 10) * 2, 1, Renderer::LEFT, false);
    }
}

void MainScene::resize(int w, int h)
{
    //更新窗口宽高
    this->w = w;
    this->h = h;
}

void MainScene::newBall(Ball *ball)
{
    //创建球场景
    BallScene *ballScene = new BallScene(ball, this);
    //添加球场景至场景List
    addScene(ballScene);
    //连接球爆炸场景
    connect(ballScene, &BallScene::explode, [this](double x, double y){
        addScene(new ExplodeScene(x, y, "regular", this));
    });
}

void MainScene::newPlane(Plane *plane)
{
    QString color;
    //记录飞机并确定飞机颜色、游戏模式
    if (!planeOne)
    {
        color = "red";
        type = PVE;
        planeOne = plane;
    }
    else
    {
        color = "blue";
        type = PVP;
        planeTwo = plane;
    }
    //创建飞机场景
    PlaneScene *planeScene = new PlaneScene(plane, color, this);
    //添加飞机场景至场景List
    addScene(planeScene);
    //连接飞机发射子弹
    plane->shootBullet.bind([this, color](Bullet* bullet){
        addScene(new BulletScene(bullet, color, this));
    }, this);
    //连接飞机爆炸场景
    connect(planeScene, &PlaneScene::explode, [this](double x, double y){
        addScene(new ExplodeScene(x, y, "sonic", this));
    });
}

void MainScene::scoreChange(Plane *plane,int score)
{
    //改变对应飞机的分数
    if (plane == planeOne)
    {
        p1Score = score;
    }
    else if (plane == planeTwo)
    {
        p2Score = score;
    }
}
