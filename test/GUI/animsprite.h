#ifndef ANIMSPRITE_H
#define ANIMSPRITE_H

#include <QList>
#include <QDebug>
#include <QObject>
#include <QMatrix4x4>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>

#include "resmanager.h"
#include "renderer.h"

class AnimSprite : public QObject
{
    Q_OBJECT
public:
    explicit AnimSprite(QList<QOpenGLTexture *> textures, QObject *parent = nullptr);
    ~AnimSprite();
    //播放动画
    void play(bool playLooped = false, int FPS = 5);
    //绘制动画精灵
    void draw(Renderer *renderer, QMatrix4x4 inModel = QMatrix4x4());
    //设置模型矩阵
    inline void setModel(QMatrix4x4 model)
    {
        this->model = model;
    }
    //停止动画
    inline void stop()
    {
        playStat = false;
    }
    //返回动画播放状态
    inline bool isPlaying()
    {
        return playStat;
    }

private:
    //动画纹理List
    QList<QOpenGLTexture *> textures;
    //播放帧数
    int FPS;
    //是否在播放
    bool playStat;
    //是否循环播放
    bool playLooped;
    //当前动画帧数
    int currentFrame;
    //模型矩阵
    QMatrix4x4 model;

signals:
    void finished();

};

#endif // ANIMSPRITE_H
