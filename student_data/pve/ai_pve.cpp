#include "DataStruct.h"
//#include "const.h"

#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <array>
#include <windows.h>



#define PI 3.1415926
#define UNUSED(x) (void)x;
#define random(x) rand() % (x)
#define SIGN(x) ((x < 0 ) ? 1 : 0 )

struct Vec2{
    double x;
    double y;
};

//产生超时故障
void timeoutFault()
{
    Sleep(1000);
}

//产生崩溃故障
void crashFault()
{
    exit(0);
}

//随机生成故障
void randGenFault()
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
        timeoutFault();
    case 1:
        crashFault();
    }
}

//角度坐标系转换
inline double angleConver(double angle)
{
    return -90 - angle;
}

//抽取向量点乘
inline double vecMul(double x0, double y0, double x1, double y1)
{
    return x0 * x1 + y0 * y1;
}

//抽取向量取模
inline double vecMod(double x, double y)
{
    return sqrt(x * x + y * y);
}

/* 瞄准某个球
 * 参数：data：游戏数据，ball_x、ball_y：球当前位置，ball_v_x、ball_v_y：球当前速度，leftward、rightward：返回动作
 * 返回值：0：瞄准成功，-1：瞄准失败
 */
int AimBall(DataStruct *data, BallStruct &ball, int &leftward, int &rightward)
{
    float ball_x = ball.x, ball_y = ball.y, ball_v_x = ball.v_x, ball_v_y = ball.v_y;
    //瞄准的角度
    double angleTo, angleDiff;
    //球运动方向和飞机与球位置向量的夹角余弦
    double cosPosV;
    //两向量点乘除以两向量的模
    cosPosV = vecMul(ball_v_x, ball_v_y, data->plane1.x - ball_x, data->plane1.y - ball_y)
            / vecMod(ball_v_x, ball_v_y) / vecMod(data->plane1.x - ball_x, data->plane1.y - ball_y);
    //根据正弦定理（a/sinA=b/sinB）求出sin值再求得所需度数
    angleTo = angleConver(
                (asin(sqrt(1 - cosPosV * cosPosV) * vecMod(ball_v_x, ball_v_y) / 2000)
                 + atan2(ball_y - data->plane1.y, ball_x - data->plane1.x)) * 180 / PI);

    //计算飞机朝向与该角度之差
    angleDiff = fmod(fmod(data->plane1.angle - angleTo, 360) + 360, 360);

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
int prePlanePos(DataStruct *data, int time, float &x, float &y)
{
    double v_x0, v_y0, v_x1, v_y1, a_x, a_y;
    //获取初速度
    v_x0 = data->plane1.v_x;
    v_y0 = data->plane1.v_y;
    //无速度则无需预测
    if (v_x0 == 0 && v_y0 == 0)
    {
        x = data->plane1.x;
        y = data->plane1.y;
        return 0;
    }
    //计算加速度
    a_x = -v_x0 / vecMod(v_x0, v_y0) * 4000;
    a_y = -v_y0 / vecMod(v_x0, v_y0) * 4000;
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
    x = data->plane1.x + (v_x1 * v_x1 - v_x0 * v_x0) / 2 / a_x;
    y = data->plane1.y + (v_y1 * v_y1 - v_y0 * v_y0) / 2 / a_y;
    return 0;
}

/* 转向某点
 * 参数：data：游戏数据，x、y：目标点，leftward、rightward：返回动作
 * 返回值：0：正在转向，1：完成转向，-1：转向失败
 */
int turnTo(DataStruct *data, float x, float y, int &leftward, int &rightward)
{
    //飞机到目的地的角度
    double angleTo, angleDiff;

    //计算飞机到目的地的角度并改变坐标系
    angleTo = angleConver(atan2(y - data->plane1.y, x - data->plane1.x) * 180 / PI);
    //计算飞机朝向与该角度之差
    angleDiff = fmod(fmod(data->plane1.angle - angleTo, 360) + 360, 360);

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
int moveTo(DataStruct *data, float x, float y, int &forward, int &leftward, int &rightward, int precision = 1)
{
    //计算当前点到终点距离
    double dis = vecMod(y - data->plane1.y, x - data->plane1.x);
    //已到达目标点则终止动作
    if (dis < precision)
    {
        return 1;
    }

    //预测飞机位置
    float pre_x, pre_y;
    prePlanePos(data, 3000, pre_x, pre_y);

    //正在转向则不加速
    if (!turnTo(data, data->plane1.x + x - pre_x, data->plane1.y + y - pre_y, leftward, rightward))
    {
        forward = false;
        return 0;
    }

    dis = vecMod(y - pre_x, x - pre_y);
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
int moveNeeded(DataStruct *data, int &des_x, int &des_y, int preTime = 2000)
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
        float dis = fabs((A * data->plane1.y + B * data->plane1.x + C) / vecMod(A, B));
        //计算垂点坐标
        float cross_x = (data->plane1.y - ball_y + ball_v_y / ball_v_x * ball_x + ball_v_x / ball_v_y * data->plane1.x)
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
        if (dis < data->plane1.r + ball.r + 10 && t < min_t)
        {
            //设置最紧迫威胁
            min_t = t;
            //向相反方向移动球半径的距离
            des_x = data->plane1.x +  ball.r * (data->plane1.x - cross_x)
                    / vecMod(data->plane1.x - cross_x, data->plane1.y - cross_y);
            des_y = data->plane1.y +  ball.r * (data->plane1.y - cross_y)
                    / vecMod(data->plane1.x - cross_x, data->plane1.y - cross_y);
            //如果x越界
            if (des_x < data->plane1.r || des_x > 2000 - data->plane1.r)
            {
                des_x = data->plane1.x;
                if (data->plane1.y - cross_y > 0)
                    des_y = cross_y + ball.r + data->plane1.r;
                else
                    des_y = cross_y - ball.r - data->plane1.r;
            }
            //如果y越界
            if (des_y < data->plane1.r || des_y > 1500 - data->plane1.r)
            {
                des_y = data->plane1.y;
                if (data->plane1.x - cross_x > 0)
                    des_x = cross_x + ball.r + data->plane1.r;
                else
                    des_x = cross_x - ball.r - data->plane1.r;
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

void vecNormalize(double& x, double& y)
{
    auto vecl = vecMod(x,y);
    x = x/vecl;
    y = y/vecl;
}

double vecMod(Vec2 v2)
{
    return std::sqrt(v2.x*v2.x+v2.y*v2.y);
}

double vecdot(Vec2 v1, Vec2 v2)
{
    return v1.x*v2.x+v1.y*v2.y;
}

Vec2 vecNormalize(const Vec2& v2)
{
    auto vecl = vecMod(v2);
    Vec2 ans;
    ans.x = v2.x/vecl;
    ans.y = v2.y/vecl;
    return ans;
}

double vecAngle(const Vec2& v1, const Vec2& v2)
{
    return std::acos(vecdot(vecNormalize(v1), vecNormalize(v2)));
}

Vec2 vecRot(const Vec2& v, double angle)
{
    Vec2 ans;
    ans.x = std::cos(angle)*v.x + std::sin(angle)*v.y;
    ans.y = -std::sin(angle)*v.x + std::cos(angle)*v.y;
    return ans;
}

void preBallPos(const DataStruct* data, const BallStruct& ball, float& x, float& y)
{
    const PlaneStruct& plane1 = data->plane1;
    Vec2 VBA = {plane1.x-ball.x, plane1.y-ball.y};
    Vec2 VAB = {-plane1.x+ball.x, -plane1.y+ball.y};
    Vec2 BV = {ball.v_x, ball.v_y};
    double angleB = vecAngle(VBA, BV);
    double angleA = std::asin(sin(angleB)*vecMod(BV)/1500);
    //std::cout << sin(angleB)*INIT_VELOCITY_BULLET/vecMod(BV) << "\n";
    //std::cout << angleA * 180 / 3.1415926 << "\n";
    auto AC = vecRot(VAB, BV.y>0?angleA:-angleA);
    x = plane1.x + AC.x;
    y = plane1.y + AC.y;
    x = abs(x);
    y = abs(y);
    x = fmin(2000,x);
    y = fmin(1500,y);
}

void turnTime(DataStruct* data, float x, float y, float& t)
{
    //计算飞机到目的地的角度并改变坐标系
    auto angleTo = angleConver(atan2(y - data->plane1.y, x - data->plane1.x) * 180 / PI);
    //计算飞机朝向与该角度之差
    auto angleDiff = fmod(fmod(data->plane1.angle - angleTo, 360) + 360, 360);

    t = angleDiff / 360;
}




void ai_pve(DataStruct*data,KeyStruct*key){

    //默认不移动且恒发射子弹
    key->forward = false;
    key->rotate_left = false;
    key->rotate_right = false;
    key->shoot = true;
    key->shield = false;
    
    
    //创建静态移动标志
    static int moveFlag = 1, des_x, des_y;

    
    /*
    //标志为0则正在移动或检查发现需要移动
    if (moveNeeded(data, des_x, des_y) || moveFlag == 0)
    {
        //进行移动并返回移动结果
        moveFlag = moveTo(data, WIDTH_WIDGET-65, HEIGHT_WIDGET-65, key->forward, key->rotate_left, key->rotate_right);
        
        //返回标志为-1则发生错误
        if (moveFlag == -1)
        {
            std::cout << "移动发生错误" << std::endl;
        }
        //结束函数执行
        return;
    }
    */

    moveFlag = moveTo(data, 2000-50, 1500-50, key->forward, key->rotate_left, key->rotate_right);
    if(moveFlag!=1)
    {
        key->shield = true;
        return;
    } 

    //如果球个数不为零
    //data->ball_size
    if (data->ball_size==0)
    {
        //瞄准零号球
        //AimBall(data, data->ball[0], key->rotate_left, key->rotate_right);
        return;
    }
    
    // for(int i=0;i<data->ball_size;++i)
    // {
    //     data->ball[i].x -= data->ball[i].r;
    //     data->ball[i].y -= data->ball[i].r;
    // }

    float nx,ny;
    float px1,py1;
    BallStruct& ball = data->ball[0];
    BallStruct& target = data->ball[0];
    px1 = data->plane1.x;
    py1 = data->plane1.y;
    nx = ball.x;
    ny = ball.y;
    float dis = vecMod(nx-px1, ny-py1)-ball.r-data->plane1.r;
    /*
    float newdis=0.0;
    
    for(int i=0;i<data->ball_size;++i)
    {
        ball = data->ball[i];
        newdis = vecMod(ball.x-px1, ball.y-py1)-ball.r-data->plane1.r;
        if(newdis<dis)
        {
            nx = ball.x;
            ny = ball.y;
            target = ball;
        }
    }*/
    double v1x,v1y;
    double v2x,v2y;
    double v3x,v3y;
    double v4x,v4y;
    for(int i=0;i<data->ball_size;++i)
    {
        v1x = data->ball[i].x;
        v1y = data->ball[i].y;
        v2x = data->ball[i].v_x;
        v2y = data->ball[i].v_y;
        v3x = data->plane1.x;
        v3y = data->plane1.y;
        v4x = v3x-v1x;
        v4y = v3y-v1y;
        if(vecMod(v2x,v2y)<vecMod(target.v_x, target.v_y))
        {
            target = data->ball[i];
        }
        vecNormalize(v2x, v2y);
        vecNormalize(v4x, v4y);
        if(abs(v2x-v4x)<=1&&abs(v2y-v4y)<=1)
        {
            target = data->ball[i];
            key->shield = true;
            break;
        }
    }
    nx = target.x;
    ny = target.y;

    preBallPos(data, target, nx, ny);

    // std::cout << "(" << nx << "," << ny << ")\n";

    turnTo(data, nx, ny, key->rotate_left, key->rotate_right);

}
void ai1_pvp(DataStruct*data,KeyStruct*key){
    UNUSED(data)
    UNUSED(key)
}
