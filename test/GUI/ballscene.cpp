#include "ballscene.h"

BallScene::BallScene(Ball *ball, QObject *parent) : Scene(parent), ball(ball), angle(0)
{
    //判断球是否为空
    if (!ball)
    {
        //为空则报错
        qDebug() << "ERROR::BallScene::Constructor::BallIsNull";
    }
    //连接球爆炸信号
    ball->explode.bind([this](){
        emit explode(this->ball->getX(), this->ball->getY());
        this->deleteLater();
    }, this);
    //连接球消失信号
    ball->disappear.bind([this](){
        this->deleteLater();
    }, this);
    //获取资源管理器
    ResManager *resManager = ResManager::getInstance();
    //获取球纹理并创建精灵
    body = new Sprite(resManager->getTexture(QString(":/textures/others/ball")), this);
    //设置球模型矩阵
    QMatrix4x4 bodyModel;
    bodyModel.scale(ball->getR() / 45.5);
    body->setModel(bodyModel);
    //加入精灵List
    body->show();
    addSprite(body);
}

void BallScene::render(Renderer *renderer, QMatrix4x4 inModel)
{
    //每次绘制进行旋转
    angle += 5;
    if (angle > 360)
    {
        angle = angle % 360;
    }
    //更新球的位置信息
    this->x = ball->getX();
    this->y = ball->getY();
    //更新飞机模型矩阵
    model.setToIdentity();
    model.translate(x, y);
    model.rotate(angle, 0, 0, -1);
    //调用基类绘制函数
    Scene::render(renderer, inModel);
}
