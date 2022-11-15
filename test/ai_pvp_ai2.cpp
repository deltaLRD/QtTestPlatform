#include"DataStruct.h"
#include "math.h"

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <windows.h>

#define PI 3.1415926
#define UNUSED(x) (void)x;
#define random(x) rand() % (x)
#define SIGN(x) ((x < 0 ) ? 1 : 0 )

//产生超时故障
void pvp_ai2_timeoutFault()
{
    Sleep(1000);
}

//产生崩溃故障
void pvp_ai2_crashFault()
{
    exit(0);
}

//随机生成故障
void pvp_ai2_randGenFault()
{
    //创建静态初始化标志
    static bool initFlag = false;
    //产生随机数种子
    if (!initFlag)
    {
        srand((int)time(NULL));
        initFlag = true;
    }
    //随机生成故障
    switch (random(3)) {
    case 0:
        pvp_ai2_timeoutFault();
    case 1:
        pvp_ai2_crashFault();
    }
}

//角度坐标系转换
inline double pvp_ai2_angleConver(double angle)
{
    return -90 - angle;
}

//抽取向量点乘
inline double pvp_ai2_vecMul(double x0, double y0, double x1, double y1)
{
    return x0 * x1 + y0 * y1;
}

//抽取向量取模
inline double pvp_ai2_vecMod(double x, double y)
{
    return sqrt(x * x + y * y);
}

/* 瞄准某个球
 * 参数：data：游戏数据，ball_x、ball_y：球当前位置，ball_v_x、ball_v_y：球当前速度，leftward、rightward：返回动作
 * 返回值：0：瞄准成功，-1：瞄准失败
 */
int pvp_ai2_AimBall(DataStruct *data, BallStruct &ball, int &leftward, int &rightward)
{
    float ball_x = ball.x, ball_y = ball.y, ball_v_x = ball.v_x, ball_v_y = ball.v_y;
    //瞄准的角度
    double angleTo, angleDiff;
    //球运动方向和飞机与球位置向量的夹角余弦
    double cosPosV;
    //两向量点乘除以两向量的模
    cosPosV = pvp_ai2_vecMul(ball_v_x, ball_v_y, data->plane2.x - ball_x, data->plane2.y - ball_y)
            / pvp_ai2_vecMod(ball_v_x, ball_v_y) / pvp_ai2_vecMod(data->plane2.x - ball_x, data->plane2.y - ball_y);
    //根据正弦定理（a/sinA=b/sinB）求出sin值再求得所需度数
    angleTo = pvp_ai2_angleConver(
                (asin(sqrt(1 - cosPosV * cosPosV) * pvp_ai2_vecMod(ball_v_x, ball_v_y) / 2000)
                 + atan2(ball_y - data->plane2.y, ball_x - data->plane2.x)) * 180 / PI);

    //计算飞机朝向与该角度之差
    angleDiff = fmod(fmod(data->plane2.angle - angleTo, 360) + 360, 360);

    //根据角度差选择更优旋转方向
    if (angleDiff < 3.6 || angleDiff > 356.4)
    {
        return 1;
    }
    else if (angleDiff < 180)
    {
        leftward = false;
        rightward = true;
    }
    else
    {
        leftward = true;
        rightward = false;
    }

    return 0;
}

/* 预测飞机位置
 * 参数：data：游戏数据，x、y：返回位置，time：给定时间
 * 返回值：0：预测成功，-1：预测失败
 */
int pvp_ai2_prePlanePos(DataStruct *data, int time, float &x, float &y)
{
    double v_x0, v_y0, v_x1, v_y1, a_x, a_y;
    //获取初速度
    v_x0 = data->plane2.v_x;
    v_y0 = data->plane2.v_y;
    //无速度则无需预测
    if (v_x0 == 0 && v_y0 == 0)
    {
        x = data->plane2.x;
        y = data->plane2.y;
        return 0;
    }
    //计算加速度
    a_x = -v_x0 / pvp_ai2_vecMod(v_x0, v_y0) * 4000;
    a_y = -v_y0 / pvp_ai2_vecMod(v_x0, v_y0) * 4000;
    //计算末速度
    v_x1 = v_x0 + a_x * time / 100;
    if (SIGN(v_x1) != SIGN(v_x0))
    {
        v_x1 = 0;
    }
    v_y1 = v_y0 + a_y * time / 100;
    if (SIGN(v_y1) != SIGN(v_y0))
    {
        v_y1 = 0;
    }
    //计算位置
    x = data->plane2.x + (v_x1 * v_x1 - v_x0 * v_x0) / 2 / a_x;
    y = data->plane2.y + (v_y1 * v_y1 - v_y0 * v_y0) / 2 / a_y;
    return 0;
}

/* 转向某点
 * 参数：data：游戏数据，x、y：目标点，leftward、rightward：返回动作
 * 返回值：0：正在转向，1：完成转向，-1：转向失败
 */
int pvp_ai2_turnTo(DataStruct *data, float x, float y, int &leftward, int &rightward)
{
    //飞机到目的地的角度
    double angleTo, angleDiff;

    //计算飞机到目的地的角度并改变坐标系
    angleTo = pvp_ai2_angleConver(atan2(y - data->plane2.y, x - data->plane2.x) * 180 / PI);
    //计算飞机朝向与该角度之差
    angleDiff = fmod(fmod(data->plane2.angle - angleTo, 360) + 360, 360);

    //根据角度差选择更优旋转方向
    if (angleDiff < 3.6 || angleDiff > 356.4)
    {
        return 1;
    }
    else if (angleDiff < 180)
    {
        leftward = false;
        rightward = true;
    }
    else
    {
        leftward = true;
        rightward = false;
    }

    return 0;
}

/* 移动至某点
 * 参数：data：游戏数据，x、y：目标点，forward、leftward、rightward：返回动作
 * 返回值：0：正在移动，1：完成移动，-1：移动失败
 */
int pvp_ai2_moveTo(DataStruct *data, float x, float y, int &forward, int &leftward, int &rightward, int precision = 1)
{
    //计算当前点到终点距离
    double dis = pvp_ai2_vecMod(y - data->plane2.y, x - data->plane2.x);
    //已到达目标点则终止动作
    if (dis < precision)
    {
        return 1;
    }

    //预测飞机位置
    float pre_x, pre_y;
    pvp_ai2_prePlanePos(data, 3000, pre_x, pre_y);

    //正在转向则不加速
    if (!pvp_ai2_turnTo(data, data->plane2.x + x - pre_x, data->plane2.y + y - pre_y, leftward, rightward))
    {
        forward = false;
        return 0;
    }

    dis = pvp_ai2_vecMod(y - pre_x, x - pre_y);
    //停下时未到达目标点
    if (dis >= precision)
    {
        forward = true;
        leftward = false;
        rightward = false;
    }

    return 0;
}

/* 判断是否需要移动
 * 参数：data：游戏数据、des_x：目标点横坐标、des_y：目标点纵坐标
 * 返回值：0：无需移动，1：需要移动
 */
int pvp_ai2_moveNeeded(DataStruct *data, int &des_x, int &des_y, int preTime = 200)
{
    float min_t = INT_MAX;
    //预测每一个球的移动
    for (int i = 0; i < data->ball_size; i++)
    {
        //获取当前球的引用
        BallStruct &ball = data->ball[i];
        //获取当前球的坐标和速度
        float ball_x = ball.x, ball_y = ball.y, ball_v_x = ball.v_x, ball_v_y = ball.v_y;
        //计算球运动直线方程
        float A, B, C;
        A = -1;
        B = ball_v_y / ball_v_x;
        C = ball_y - ball_x * ball_v_y / ball_v_x;
        //计算飞机到直线距离
        float dis = fabs((A * data->plane2.y + B * data->plane2.x + C) / pvp_ai2_vecMod(A, B));
        //计算垂点坐标
        float cross_x = (data->plane2.y - ball_y + ball_v_y / ball_v_x * ball_x + ball_v_x / ball_v_y * data->plane2.x)
                / (ball_v_x / ball_v_y + ball_v_y / ball_v_x);
        float cross_y = (ball_v_y / ball_v_x) * (cross_x - ball_x) + ball_y;
        //计算到垂点的时间
        float t = (cross_x - ball_x) / ball_v_x * 100;
        //反向运动或时间过久则忽略该球
        if (t < 0 || t > preTime)
        {
            continue;
        }
        //判断该球是否有威胁
        if (dis < data->plane2.r + ball.r + 10 && t < min_t)
        {
            //设置最紧迫威胁
            min_t = t;
            //向相反方向移动球半径的距离
            des_x = data->plane2.x +  ball.r * (data->plane2.x - cross_x)
                    / pvp_ai2_vecMod(data->plane2.x - cross_x, data->plane2.y - cross_y);
            des_y = data->plane2.y +  ball.r * (data->plane2.y - cross_y)
                    / pvp_ai2_vecMod(data->plane2.x - cross_x, data->plane2.y - cross_y);
            //如果x越界
            if (des_x < data->plane2.r || des_x > 2000 - data->plane2.r)
            {
                des_x = data->plane2.x;
                if (data->plane2.y - cross_y > 0)
                    des_y = cross_y + ball.r + data->plane2.r;
                else
                    des_y = cross_y - ball.r - data->plane2.r;
            }
            //如果y越界
            if (des_y < data->plane2.r || des_y > 1500 - data->plane2.r)
            {
                des_y = data->plane2.y;
                if (data->plane2.x - cross_x > 0)
                    des_x = cross_x + ball.r + data->plane2.r;
                else
                    des_x = cross_x - ball.r - data->plane2.r;
            }
        }
    }

    if (min_t != INT_MAX)
    {
        return 1;
    }

    //给定时间内无危险
    return 0;
}

void ai_pvp_ai2(DataStruct*data,KeyStruct*key){
    //默认不移动且恒发射子弹
    key->forward = false;
    key->rotate_left = false;
    key->rotate_right = false;
    key->shoot = true;

    //创建静态移动标志
    static int moveFlag = 1, des_x, des_y;

    //标志为0则正在移动或检查发现需要移动
    if (pvp_ai2_moveNeeded(data, des_x, des_y) || moveFlag == 0)
    {
        //进行移动并返回移动结果
        moveFlag = pvp_ai2_moveTo(data, des_x, des_y, key->forward, key->rotate_left, key->rotate_right);
        //返回标志为-1则发生错误
        if (moveFlag == -1)
        {
            std::cout << "移动发生错误" << std::endl;
        }
        //结束函数执行
        return;
    }

    //如果球个数不为零
    if (data->ball_size)
    {
        //瞄准零号球
        pvp_ai2_AimBall(data, data->ball[0], key->rotate_left, key->rotate_right);
    }
}
