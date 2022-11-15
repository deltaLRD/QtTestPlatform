#ifndef CONST_H
#define CONST_H

#define RECORDING_FREQUENCY 5
//录制信号
enum SIG{
    //Core Signal
    NEW_PLANE,
    NEW_BALL,
    GAME_OVER,
    SCORE_CHANGE,
    //FlyObj Signal
    EXPLODE,
    DISAPPEAR,
    //Plane Signal
    SHOOT_BULLET,
    SHIELD_UP,
    SHIELD_DOWN,
    INVINCIBLE_UP,
    INVINCIBLE_DOWN
};
#define PTR_TO_UINT(ptr) (uint)ptr
//初始化对象池的数量
#define MAX_BULLET_OBJECT_POOL 1000
#define MAX_BALL_OBJECT_POOL 100
//飞机满魔法值,每帧自增1
#define MAX_MP_PLANE 1000
//飞机左右前后的加速度 per second
//#define ACCELERATE_X 6000
#define ACCELERATE_Y 6000
//减速的加速度 per second
#define ACCELERATE_SLOWDOWN 4000
//飞机转向的角速度 per second 一秒九十度
#define VELOCITY_ANGLE 360
//飞机最大飞行速度 per second
#define MAX_VELOCITY_X 700
#define MAX_VELOCITY_Y 700
//子弹飞行初速度
#define INIT_VELOCITY_BULLET 2000
//球的初始x方向速度
#define INIT_VELOCITY_X_BALL 800
#define INIT_VELOCITY_Y_BALL 600
//窗口的宽和高 4:3 9寸10寸笔记本大概是1024*600
#define WIDTH_WIDGET 2000
#define HEIGHT_WIDGET 1500
//每一帧ms 一秒100帧
#define TIME_FRAME 10
//飞机、子弹、球质量
#define QUALITY_PLANE 10
#define QUALITY_BULLET 1
#define QUALITY_BALL 100
//飞机、子弹、球半径
#define RADIUS_PLANE 50
#define RADIUS_BULLET 5
#define RADIUS_BALL 100
//shoot、shield的间隔s
#define INTERVAL_SHOOT 0.2
#define INTERVAL_SHIELD 10
//shield持续s
#define SPAN_SHIELD 1
//复活时间秒数
#define SPAN_REVIVE 3
//无敌时间秒数
#define SPAN_INVINCIBLE 2
//每一个飞机遍历的按键个数
#define KEY_SUM 5
//每一个按键对应的数组下标
#define FORWARD 0
#define ROTATE_LEFT 1
#define ROTATE_RIGHT  2
#define SHOOT 3
#define SHIELD  4
#define PI 3.1415926

#endif // CONST_H

