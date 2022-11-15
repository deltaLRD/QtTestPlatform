#ifndef SCENE_H
#define SCENE_H

#include <QList>
#include <QObject>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QtAlgorithms>

#include "sprite.h"
#include "animsprite.h"
#include "renderer.h"

class Scene : public QObject
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);
    ~Scene();
    //添加场景至场景List
    void addScene(Scene *scene);
    //添加精灵至精灵Lsit
    void addSprite(Sprite *sprite);
    //添加动画精灵至动画精灵List
    void addAnimSprite(AnimSprite *animSprite);
    //绘制场景
    virtual void render(Renderer *renderer, QMatrix4x4 inModel = QMatrix4x4());

protected:
    //场景模型矩阵
    QMatrix4x4 model;
    //场景、精灵、动画精灵List
    QList<Scene *> scenes;
    QList<Sprite *> sprites;
    QList<AnimSprite *> animSprites;

};

#endif // SCENE_H
