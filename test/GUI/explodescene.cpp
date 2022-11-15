#include "explodescene.h"

ExplodeScene::ExplodeScene(double x, double y, const QString &type, QObject *parent) : Scene(parent)
{
    //获取资源管理器
    ResManager *resManager = ResManager::getInstance();
    //爆炸纹理List
    QList<QOpenGLTexture *> textures;
    for (int i = 0; i < 9; i++)
    {
        textures.push_back(resManager->getTexture(QString(":/textures/explode/%1%2").arg(type).arg(i)));
    }
    //设置场景模型矩阵
    model.translate(x, y);
    model.scale(2, 2);
    //加载爆炸动画精灵并加入动画精灵List
    explodeSprite = new AnimSprite(textures, this);
    addAnimSprite(explodeSprite);
    //播放爆炸动画
    explodeSprite->play(false, 12);
    //播放完毕后释放this
    connect(explodeSprite, &AnimSprite::finished, [this](){
        this->deleteLater();
    });
    //获取声音引擎
    irrklang::ISoundEngine *engine = resManager->getISoundEngine();
    //获取爆炸声源并创建相应播放器
    explodePlayer = new AudioPlayer(engine, resManager->getSound(":/audio/plane_explode"), this);
    //播放爆炸音效
    explodePlayer->play(false, qPow(10, -0.5));
}
