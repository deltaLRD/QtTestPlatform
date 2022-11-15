#ifndef PLANE_H
#define PLANE_H

#include <list>
#include "bullet.h"
#include "flyobject.h"
#include "pooledobject.h"
#include "const.h"
#include "time.h"
#include"math.h"
#include "objectpool.h"
#include"bulletpool.h"
#include"ballpool.h"
using namespace std;

class Plane : public FlyObject
{
public:
    Plane(double x,double y,double r,double m,double angle);    
    double getMp();
    double getAngle();
    bool getKeyPressed(int i);

    /*
    //属性的get和set
    void setMP(double mp);
    void setShiledTime(double shieldTime);
    void setInvincibleTime(double invincibleTime);
    void setReviveTime(double reviveTime);
    void setAngle(double angle);
    void setPressTime(int i,time_t pressTime);
    void setReleseTime(int i,time_t releseTime);
    void setA_X(double a_x);
    void setA_Y(double a_y);
    //将按下的键写到keyPressed数组里
    void setKeyPressed(int i,bool hasPressed);

    double getShieldTime();
    double getReviveTime();
    double getA_X();
    double getA_Y();
    //获取飞机的子弹数组
    QList<PooledObject*>* getOwnBullet();
    */
    //每一个时间片结束之后，将按键数组重置
//    void  resetKeyPressed();
    //    void  resetPressAndReleseTime();
    //判断这一时间片中是否有按键按下存到keyPress中
    //    void  updateKeyPress(time_t currentFrame);
    /*
    //检查是否需要移动、射击、按盾
    bool checkMove();
    bool checkShoot();
    bool checkShield();
    */
    //根据物体的m和v计算出阻尼
    //    double getAirDrag(double m,double v);

private:

    friend class Core;
    friend class Recording;
    friend class DataStruct;
    //x、y方向上的加速度
    double a_x;
    double a_y;
    //魔法值
    double mp;
    //护盾剩余毫秒数
    time_t shieldTime;
    /* 无敌剩余毫秒数：
     * if(invincibleTime == 0)
     * {
     *      接收射击/护盾事件;
     * }
     */
    time_t invincibleTime;
    //复活剩余毫秒数
    time_t reviveTime;
    //飞机与y轴正方向的夹角（逆时针角度为正）
    double angle;

    /*
     * 考虑改成秒数
     * */
    //上次shoot的毫秒数
    time_t lastShoot;
    //上次shield的毫秒数
    time_t lastShield;

    /* 0：↑
     * 1：↖
     * 2：↗
     * 3：射击
     * 4：护盾
     * 5-9同理
     * 记录当前帧有没有按键按下
     */
    bool keyPressed[KEY_SUM];
    //    bool  pressUnitlNextFrame[KEY_SUM];

    //    time_t pressTime[8];
    //    time_t releseTime[8];
    //持有的子弹数组
    list<PooledObject*> ownBullet;
	list<int> l;
    //移动
    void move();
    //射击
    void shoot(time_t slot,ObjectPool* bulletObjectPool);
    //更新子弹的位置，超出屏幕的子弹还给对象池
    void updateBullet(ObjectPool* bulletObjectPool);
    //护盾
    void shield(time_t slot);

    //护盾持续时间更新
    void updateShieldTime();
    //持续增加魔法值
    void updateMp();
    //碰撞边界检测
    void hitBorder();
    //复活时间
    void reviveTimeCountDown();
    void updateReviveTime();
    //无敌时间
    void invincibleTimeCountDown();
    void updateInvincibleTime();
    //清空飞机所有状态
    void resetState();


};

#endif // PLANE_H
