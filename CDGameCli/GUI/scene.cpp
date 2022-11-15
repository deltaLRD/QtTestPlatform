#include "scene.h"

Scene::Scene(QObject *parent) : QObject(parent)
{

}

Scene::~Scene(){
    //删除管理的场景、精灵、动画精灵对象
    for (Scene *scene : scenes) {
        delete scene;
    }
    for (Sprite *sprite : sprites) {
        delete sprite;
    }
    for (AnimSprite *animSprite : animSprites) {
        delete animSprite;
    }
}

void Scene::addScene(Scene *scene)
{
    //判断场景是否为空
    if (!scene)
    {
        //为空则报错
        qDebug() << "ERROR::Scene::addScene::SceneIsNull";
        return;
    }
    //连接释放场景信号
    connect(scene, &QObject::destroyed, [this, scene]()
    {
        scenes.removeOne(scene);
    });
    //添加场景至场景List
    scenes.push_back(scene);
//    //按照场景绘制顺序进行排序
//    qSort(scenes.begin(), scenes.end(), [](Scene *s1, Scene *s2)->bool
//    {
//        return s1->getOrder() < s2->getOrder();
//    });
}

void Scene::addSprite(Sprite *sprite)
{
    //判读精灵是否为空
    if (!sprite)
    {
        //为空则报错
        qDebug() << "ERROR::Scene::addSprite::SpriteIsNull";
        return;
    }
    //添加精灵至精灵Lsit
    sprites.push_back(sprite);
}

void Scene::addAnimSprite(AnimSprite *animSprite)
{
    //判断动画精灵是否为空
    if (!animSprite)
    {
        //为空则报错
        qDebug() << "ERROR::Scene::addAnimSprite::AnimSpriteIsNull";
        return;
    }
    //添加动画精灵至动画精灵List
    animSprites.push_back(animSprite);
}

void Scene::render(Renderer *renderer, QMatrix4x4 inModel)
{
    //判断渲染器是否为空
    if (!renderer)
    {
        //为空则报错
        qDebug() << "ERROR::Scene::render::RendererIsNull";
    }
    //绘制每个精灵
    for (Sprite *sprite : sprites) {
        sprite->draw(renderer, inModel * model);
    }
    //绘制每个动画精灵
    for (AnimSprite *animSprite : animSprites) {
        animSprite->draw(renderer, inModel * model);
    }
    //绘制每个场景
    for (Scene * scene : scenes) {
        scene->render(renderer, inModel * model);
    }
}
