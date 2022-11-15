#include "bulletscene.h"

BulletScene::BulletScene(Bullet *bullet, const QString& color, QObject *parent) : Scene(parent), bullet(bullet), color(color), bulletState(NORMAL)
{
    //判断子弹是否为空
    if (!bullet)
    {
        //为空则报错
        qDebug() << "ERROR::BulletScene::Constructor::BulletIsNull";
    }
    //连接子弹爆炸信号
    bullet->explode.bind(&BulletScene::explode, this);
    //连接子弹消失信号
    bullet->disappear.bind(&BulletScene::disappear, this);
    //获取资源管理器
    ResManager *resManager = ResManager::getInstance();
    //获取子弹纹理并创建精灵
    body = new Sprite(resManager->getTexture(QString(":/textures/laser/%012").arg(color)), this);
    //设置模型并加入精灵List
    QMatrix4x4 bodyModel;
    bodyModel.translate(0, 28);
    body->setModel(bodyModel);
    addSprite(body);
    body->show();
//    //子弹发射纹理List
//    QList<QOpenGLTexture *> textures;
//    for (int i = 0; i < 2; i++)
//    {
//        textures.push_back(resManager->getTexture(QString(":/textures/laser/%1%2").arg(color).arg(i)));
//    }
//    //加载子弹发射动画精灵
//    shootAnim = new AnimSprite(textures, this);
//    //设置模型矩阵并加入动画精灵List
//    QMatrix4x4 shootAnimModel;
//    shootAnimModel.translate(0, 45);
//    shootAnim->setModel(shootAnimModel);
//    addAnimSprite(shootAnim);
//    //播放子弹发射动画
//    shootAnim->play(false, 20);
//    //播放完毕后改为正常飞行
//    connect(shootAnim, &AnimSprite::finished, [this](){
//        body->show();
//    });
    //获取声音引擎
    irrklang::ISoundEngine *engine = resManager->getISoundEngine();
    //获取子弹发射声源并创建相应播放器
    shootAudio = new AudioPlayer(engine, resManager->getSound(":/audio/laser_shoot"), this);
    //播放子弹发射音效
    shootAudio->play(false, qPow(10, -0.5));
}

void BulletScene::render(Renderer *renderer, QMatrix4x4 inModel)
{
    if (bulletState != EXPLODED)
    {
        //更新子弹位置信息
        x = bullet->getX();
        y = bullet->getY();
        angle = bullet->getAngle();
    }
    //更新子弹模型矩阵
    model.setToIdentity();
    model.translate(x, y);
    model.rotate(angle, 0, 0, -1);
    //调用基类绘制函数
    Scene::render(renderer, inModel);
}

void BulletScene::disappear()
{
    this->deleteLater();
}

void BulletScene::explode()
{
    //更新子弹位置信息
    x = bullet->getX();
    y = bullet->getY();
    //设置场景状态
    bulletState = EXPLODED;
    //隐藏子弹
    body->hide();
//    shootAnim->stop();
    //获取资源管理器
    ResManager *resManager = ResManager::getInstance();
    //获取声音引擎
    irrklang::ISoundEngine *engine = resManager->getISoundEngine();
    //获取子弹爆炸声源并创建相应播放器
    shootAudio = new AudioPlayer(engine, resManager->getSound(":/audio/laser_explode"), this);
    //播放子弹爆炸音效
    shootAudio->play(false, qPow(10, -0.5));
    //子弹爆炸纹理List
    QList<QOpenGLTexture *> textures;
    for (int i = 0; i < 2; i++)
    {
        textures.push_back(resManager->getTexture(QString(":/textures/laser/%1%2").arg(color).arg(i + 5)));
    }
    //加载子弹爆炸动画精灵并加入动画精灵List
    explodeAnim = new AnimSprite(textures, this);
    addAnimSprite(explodeAnim);
    //播放子弹爆炸动画
    explodeAnim->play(false, 10);
    //播放完毕后释放this
    connect(explodeAnim, &AnimSprite::finished, [this](){
        this->deleteLater();
    });
}
