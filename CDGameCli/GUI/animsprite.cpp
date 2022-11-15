#include "animsprite.h"

AnimSprite::AnimSprite(QList<QOpenGLTexture *> textures, QObject *parent) : QObject(parent), textures(textures), FPS(5), playStat(false), playLooped(false), currentFrame(0), model()
{

}

AnimSprite::~AnimSprite()
{

}

void AnimSprite::play(bool playLooped, int FPS)

{
    //播放状态设置为正在播放
    playStat = true;
    //当前帧数置0
    currentFrame = 0;
    //设置播放帧数
    this->FPS = FPS;
    //设置循环播放
    this->playLooped = playLooped;
}

void AnimSprite::draw(Renderer *renderer, QMatrix4x4 inModel)
{
    //判断渲染器是否为空
    if (!renderer)
    {
        //为空则报错
        qDebug() << "ERROR::AnimSprite::draw::RendererIsNull";
        return;
    }
    //不在播放则返回
    if (!playStat) return;
    //设置模型矩阵
    renderer->setSpriteModel(inModel * model);
    //判断当前帧是否存在
    if (!textures[currentFrame * FPS / 60 % textures.length()])
    {
        //不存在则报错
        qDebug() << "ERROR::AnimSprite::Draw::CurrentTexIsNull";
    }
    else
    {
        //存在则绘制并令当前帧数加一
        renderer->drawSprite(textures[currentFrame++ * FPS / 60 % textures.length()]);
    }
    //如果不为循环播放且完成一次循环
    if (!playLooped && currentFrame * FPS / 60 >= textures.length())
    {
        stop();
        emit finished();
    }
}
