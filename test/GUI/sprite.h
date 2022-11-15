#ifndef SPRITE_H
#define SPRITE_H

#include <QDebug>
#include <QObject>
#include <QMatrix4x4>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>

#include "resmanager.h"
#include "renderer.h"

class Sprite : public QObject
{
    Q_OBJECT
public:
    explicit Sprite(QOpenGLTexture *texture, QObject *parent = nullptr);
    ~Sprite();
    //绘制精灵
    void draw(Renderer *renderer, QMatrix4x4 inModel = QMatrix4x4());
    //设置模型矩阵
    inline void setModel(QMatrix4x4 model)
    {
        this->model = model;
    }
    //显示精灵
    inline void show()
    {
        visable = true;
    }
    //隐藏精灵
    inline void hide()
    {
        visable = false;
    }
    //返回精灵可见性
    inline bool visiable()
    {
        return visable;
    }

private:
    //纹理
    QOpenGLTexture *texture;
    //可见性
    bool visable;
    //模型矩阵
    QMatrix4x4 model;

private:

signals:

};

#endif // SPRITE_H
