#include "sprite.h"

Sprite::Sprite(QOpenGLTexture *texture, QObject *parent) : QObject(parent), texture(texture), visable(false), model()
{
    //判断纹理是否为空
    if (!texture)
    {
        //为空则报错
        qDebug() << "ERROR::Sprite::Constructor::TextureIsNull";
    }
}

Sprite::~Sprite()
{

}

void Sprite::draw(Renderer *renderer, QMatrix4x4 inModel)
{
    //判断渲染器是否为空
    if (!renderer)
    {
        //为空则报错
        qDebug() << "ERROR::Sprite::draw::RendererIsNull";
        return;
    }
    //不可见则返回
    if (!visable) return;
    //设置渲染器模型矩阵
    renderer->setSpriteModel(inModel * model);
    //绘制精灵
    renderer->drawSprite(texture);
}
