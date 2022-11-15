#ifndef BALL_H
#define BALL_H

#include "flyobject.h"

class Ball : public FlyObject
{
public:
    //初始化参数赋值
    void init(double x,double y,double v_x,double v_y,double r,double m,int hp=10,int type=3);

//    int getHP();
    int getType();
    //使用已有的球进行赋值
//    void copy(Ball*ball);

private:
    friend class Core;
    friend class Recording;
    friend class DataStruct;
    //球剩余HP
    int hp;
    /* 球初始类型--HP
     * 3--[7,10]
     * 2--[4,6]
     * 1--[2,3]
     * 0--[1]
     */
    int type;

    void hitBorder();
    void move();
    void hpMinusOne();
    void leftSplit(Ball*ball);
    void rightSplit(Ball*ball);
};

#endif // BALL_H
