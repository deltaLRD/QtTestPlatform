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

#include <functional>
#include "SigSlot/sigslot.h"
#include "JSON/CJsonObject.hpp"

class Core : public QObject, public SigSlotBase
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
    Q_DECLARE_FLAGS(KeyPresseds, KeyPressed)
    explicit Core(QObject *parent = nullptr);
    ~Core();
    //获取游戏时间
    long getRestTime();
    //博弈核心初始化
    virtual void initial(GameType type,bool useAI=false,time_t gameTime=30);
    //设置按键状态
    void setKeyState(KeyPressed key, bool state);

    //博弈函数
    static std::function<void(DataStruct*data,KeyStruct*key)> ai1_pve;
    static std::function<void(DataStruct*data,KeyStruct*key)> ai1_pvp;

protected:
    //时间片
    long slot;
    //博弈核心定时器
    QTimer *timer;
    //对局时长
    time_t gameTime;

private:
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
    void differentRanList(QList<int>&l,int range,int num);
    void sepatareBall();
    //根据keyPressed设置两飞机的按键状态
    void setKeyPressedByKeyBoard();
    void setKeyPressedByAI();
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
    KeyPresseds keyPressed;
    int offset;
    int grade_ai1,grade_ai2;

private:
    //连接核心信号
    void linkCore(GameType type);
    //连接飞机信号
    void linkPlane(Plane *plane);
    //连接FlyObject信号
    void linkFlyObj(FlyObject *obj);
    //记录各对象信息
    void recordBall(neb::CJsonObject &frameObj);
    void recordPlane(neb::CJsonObject &frameObj);
    void recordBullet(neb::CJsonObject &frameObj);
    //保存游戏录像
    void saveVideo();
    //记录当前帧数据
    void recordFrame();
    //Json帧数组
    neb::CJsonObject frameArray, sigArray;
    //记录游戏类型
    GameType type;
    //是否使用AI
    bool useAI;

public:
    Signal<Plane *> newPlane;
    Signal<Ball *> newBall;
    Signal<> gameOver;
    Signal<Plane *, int> scoreChange;

public slots:    
    //博弈核心处理
    virtual void update();
};

#endif // CORE_H
