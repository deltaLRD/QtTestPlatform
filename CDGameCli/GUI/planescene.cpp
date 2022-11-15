#include "planescene.h"

PlaneScene::PlaneScene(Plane *plane, const QString& color, QObject *parent) : Scene(parent), plane(plane),
    color(color), planeState(NORMAL), x(plane->getX()), y(plane->getY()), angle(plane->getAngle()), invincibleCount(0)
{
    //判断飞机是否为空
    if (!plane)
    {
        //为空则报错
        qDebug() << "ERROR::PlaneScene::Constructor::PlaneIsNull";
    }
    //连接飞机爆炸信号
    plane->explode.bind([this](){
        //设置飞机爆炸
        planeState = EXPLODED;
        emit explode(this->plane->getX(), this->plane->getY());
    }, this);
    //连接飞机启用无敌信号
    plane->invincibleUp.bind([this](){
        //设置飞机无敌
        planeState = INVINCIBLE;
        invincibleCount = 0;
    }, this);
    //连接飞机取消无敌信号
    plane->invincibleDown.bind([this](){
        //取消飞机无敌
        planeState = NORMAL;
    }, this);
    //连接开启护盾信号
    plane->shieldUp.bind([this](){
        //启用护盾
        shield->show();
        //播放启用护盾音效
        shield_up->play(false, qPow(10, -0.5));
    }, this);
    //连接关闭护盾信号
    plane->shieldDown.bind([this](){
        //隐藏护盾
        shield->hide();
        //播放护盾消失音效
        shield_down->play(false, qPow(10, -0.5));
    }, this);
    QColor colorRes(color);
    colorVec.setX(colorRes.redF());
    colorVec.setY(colorRes.greenF());
    colorVec.setZ(colorRes.blueF());
    //获取资源管理器
    ResManager *resManager = ResManager::getInstance();
    //获取能量条纹理并创建精灵
    mp = new Sprite(resManager->getTexture(":/textures/mp/white_mid"), this);
    //获取机身纹理并创建精灵
    body = new Sprite(resManager->getTexture(QString(":/textures/others/plane_%1").arg(color)), this);
    //获取护盾纹理并创建精灵
    shield = new Sprite(resManager->getTexture(":/textures/others/shield2"), this);
    //显示mp
    mp->show();
    //显示机身
    body->show();
    //添加机身、护盾至精灵List
    addSprite(body);
    addSprite(shield);
    //喷气纹理List
    QList<QOpenGLTexture *> textures;
    for (int i = 0; i < 3; i++)
    {
        textures.push_back(resManager->getTexture(QString(":/textures/fire/%1%2").arg(color).arg(i)));
    }
    //加载左右喷气动画精灵并设置模型矩阵最后加入动画精灵List
    fireLeft = new AnimSprite(textures, this);
    QMatrix4x4 fireLeftModel;
    fireLeftModel.translate(-25, 50);
    fireLeft->setModel(fireLeftModel);
    addAnimSprite(fireLeft);
    fireRight = new AnimSprite(textures, this);
    QMatrix4x4 fireRightModel;
    fireRightModel.translate(25, 50);
    fireRight->setModel(fireRightModel);
    addAnimSprite(fireRight);
    //获取声音引擎
    irrklang::ISoundEngine *engine = resManager->getISoundEngine();
    //获取启用护盾、护盾消失、飞行声源并创建相应播放器
    shield_up = new AudioPlayer(engine, resManager->getSound(":/audio/shield_up.wav"), this);
    shield_down = new AudioPlayer(engine, resManager->getSound(":/audio/shield_down.wav"), this);
    flight_sound = new AudioPlayer(engine, resManager->getSound(":/audio/flight_sound.ogg"), this);
}

PlaneScene::~PlaneScene()
{

}

void PlaneScene::render(Renderer *renderer, QMatrix4x4 inModel)
{    
    //飞机前进且喷气动画不在播放
    if (plane->getKeyPressed(FORWARD) && !fireLeft->isPlaying())
    {
        //播放喷气动画
        fireLeft->play(true, 5);
        fireRight->play(true, 5);
        //播放飞行音效
        flight_sound->play(true, qPow(10, -1.2));
    }
    //正在播放喷气动画且飞机不在前进
    else if (fireLeft->isPlaying() && !plane->getKeyPressed(FORWARD))
    {
        //停止喷气动画
        fireLeft->stop();
        fireRight->stop();
        //停止飞行音效
        flight_sound->stop();
    }
    //更新飞机位置信息
    x = plane->getX();
    y = plane->getY();
    angle = plane->getAngle();
    //更新飞机模型矩阵
    model.setToIdentity();
    model.translate(x, y);
    model.rotate(angle, 0, 0, -1);
    //如果飞机爆炸则不绘制
    if (planeState == EXPLODED) return;
    if (planeState == INVINCIBLE) {
        invincibleCount++;
        if (invincibleCount % 5 > 3) return;
    }
    //调用基类绘制函数
    Scene::render(renderer, inModel);
    //绘制MP条
    if (plane->getMp() > 0)
    {
        if (plane->getMp() != MAX_MP_PLANE)
            renderer->setSpriteColor(colorVec);
        float mpLength = 0.0017f * plane->getMp();
        QMatrix4x4 mpModel;
        mpModel.translate((mpLength - 1.7) * 7.5, 15);
        mpModel.scale(mpLength, 0.2f);
        mp->setModel(mpModel);
        mp->draw(renderer, inModel * model);
        renderer->setSpriteColor(QVector3D(1.0f, 1.0f, 1.0f));
    }
}
