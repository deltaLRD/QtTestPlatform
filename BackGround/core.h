#ifndef CORE_H
#define CORE_H

//#include "GUI/glwidget.h"
#include"time.h"
#include"plane.h"
#include"objectpool.h"
#include"bulletpool.h"
#include"ballpool.h"
#include"ballpool.h"
#include"const.h"
#include"DataStruct.h"
#include<iostream>
#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QtGlobal>
#include <qmath.h>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QProcess>
#include <QDir>

using namespace std;
void ai1_pve(DataStruct*data,int*key);
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
    Q_DECLARE_FLAGS(KeyPresseds, KeyPressed)
    explicit Core(QObject *parent = nullptr);
    ~Core();
    //获取游戏时间
    long getRestTime();
    //博弈核心初始化
    virtual void initial(GameType type,QString ai1_name,QString ai2_name=nullptr,time_t gameTime=30);
    //设置按键状态
    void setKeyState(KeyPressed key, bool state);

protected:
    //时间片
    long slot;
    //博弈核心定时器
    QTimer *timer;
    //对局时长
    time_t gameTime;

private:
    //连接飞机信号
    void linkPlane(Plane *plane);
    //连接FlyObject信号
    void linkFlyObj(FlyObject *obj);
    //记录各对象信息
    void recordBall(QJsonObject &frameObj);
    void recordPlane(QJsonObject &frameObj);
    void recordBullet(QJsonObject &frameObj);
    //记录游戏结束
    void beforeGameOver();
    //记录当前帧数据
    void recordFrame();
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
    void sendDataToAI();

    /*
    //封装一个json对象
    void packageAll(QJsonObject &data);
    void packageBall(QJsonObject &dataObj);
    void packageBullet(QJsonObject &dataObj);
    void packagePlane(QJsonObject &dataObj);*/

    //构造data结构体
    void FromObjToStruct(DataStruct&data,Plane*plane1,Plane*plane2,QList<PooledObject*>*ball);
    void score(Plane*plane);


//    //时钟
//    QTime *clock;
    //画图控件
//    GLWidget *w;
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
    //Json帧数组
    QJsonArray frameArray, sigArray;
//    AI1*ai1;
//    AI2*ai2;
    //记录游戏类型
    GameType type;
    //共享data文件和key文件的句柄
    HANDLE data_Handle,key_Handle;
    LPVOID data_ptr,key_ptr;
    //记录不响应的次数
    int dieOne,dieTwo;
//    char input_key[32];
    int offset;

    QString ai1_name,ai2_name;
    int grade_ai1,grade_ai2;
signals:
    void newPlane(Plane *plane);
    void newBall(Ball *ball);
    /* state
     * -1,对局没结束，还没写文件
     * 0,正常结束
     * 1,超时三次
    */
    void gameOver(int state,int ai1_score,int ai2_score);
    void scoreChange(Plane*plane,int score);

public slots:    
    //博弈核心处理
    virtual void update();
};

#endif // CORE_H
