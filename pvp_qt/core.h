#ifndef CORE_H
#define CORE_H

#include"time.h"
#include"plane.h"
#include"objectpool.h"
#include"bulletpool.h"
#include"ballpool.h"
#include"ballpool.h"
#include"const.h"
#include"DataStruct.h"
#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QtGlobal>
#include <qmath.h>
#include<iostream>


void ai_pve(DataStruct*data,KeyStruct*key);
void ai_pvp_ai2(DataStruct*data,KeyStruct*key);
void ai_pvp_ai1(DataStruct*data,KeyStruct*key);
class Core : public QObject
{
    Q_OBJECT
public:
    //游戏类型
    enum GameType
    {
        PVP            = 0x0001,
        PVE            = 0x0002,
    };
    enum KeyPressed
    {
        P1_FORWARD         = 0x0001,
        P1_ROTATE_LEFT            = 0x0002,
        P1_ROTATE_RIGHT           = 0x0004,
        P1_SHOOT           = 0x0008,
        P1_SHIELD          = 0x0010,
        P2_FORWARD         = 0x0020,
        P2_ROTATE_LEFT            = 0x0040,
        P2_ROTATE_RIGHT           = 0x0080,
        P2_SHOOT           = 0x0100,
        P2_SHIELD          = 0x0200,
    };

    explicit Core(QObject *parent = nullptr);
    ~Core();
    //获取游戏时间
    long getRestTime();
    //博弈核心初始化
    virtual void initial(GameType type,int ai=2,bool useAI=true,time_t gameTime=30);
    //设置按键状态
protected:
    //时间片
    long slot;
    //博弈核心定时器
    //QTimer *timer;
    //对局时长
    time_t gameTime;

private:
    void update();
    //更新飞机的状态
    void updatePlane();
    //更新子弹的状态
    void updateBullet();
    //更新球的状态
    void updateBall();
    void setAccelerator(int accelerator);
    QList<PooledObject*>* getAliveBall();
    //碰撞检测
    void collideDetect();
    //判断生成不同的随机数
    bool existInList(QList<int>list,int n);
    void differentRanList(QList<int>&l,int min,int max,int num);
    void differentranList(QList<int>&l,QList<int>&m,int min,int max,int num);
    void differentvelocity(QList<int>&l,int min,int max,int num);
    void sepatareBall();
    //根据keyPressed设置两飞机的按键状态
    void setKeyPressedByKeyBoard();
    void pve_setKeyPressedByAI();
    void pvp_setplane1KeyPressedByAI();
    void pvp_setplane2KeyPressedByAI();
    //球被击中爆炸得一分
    void score(Plane*plane);

    /*
    //封装一个json对象
    QJsonObject packageAll();
    void packageBall(QJsonObject &dataObj);
    void packageBullet(QJsonObject &dataObj);
    void packagePlane(QJsonObject &dataObj);*/

    //构造data结构体
    void FromObjToStruct(DataStruct&data,Plane*plane1,Plane*plane2,QList<PooledObject*>*ball);


//    //时钟
//    QTime *clock;
    //画图控件
    //子弹对象池
    BulletPool *bulletObjPool;
    //球对象池
    BallPool *ballObjPool;
    //持有的球数组
    QList<PooledObject*> aliveBall;
    //飞机
    Plane*planeOfP1;
    Plane*planeOfP2;
    //加速因子
    int accelerator;
    //各按键状态
    int offset;
    int grade_ai1,grade_ai2;

private:


    //记录各对象信息
    //记录游戏类型
    GameType type;
    //pve是否使用AI
    bool useAI;
    //pvpai数量
    int ai;

public:

public slots:

};

#endif // CORE_H
