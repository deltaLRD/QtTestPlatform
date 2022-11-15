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

inline double distance(DataStruct *data,int x)
{
    return ( (data->plane1.x-data->ball[x].x)*(data->plane1.x-data->ball[x].x)+(data->plane1.y-data->ball[x].y)*(data->plane1.y-data->ball[x].y));
}


 double calangleto(DataStruct *data, BallStruct &ball)
{
    double cosPosV/*,angleto,angle*/;
    cosPosV = vecMul(ball.v_x, ball.v_y, data->plane1.x - ball.x, data->plane1.y - ball.y)
            / vecMod(ball.v_x, ball.v_y) / vecMod(data->plane1.x - ball.x, data->plane1.y - ball.y);

    if(ball.v_x<0/*-data->plane1.x<0*/)
    return angleConver((-asin(sqrt(1 - cosPosV * cosPosV) * vecMod(ball.v_x, ball.v_y) / 2000)
                             + atan2(ball.y - data->plane1.y, ball.x - data->plane1.x)) * 180 / PI);
    if(ball.x>0/*-data->plane1.x>=0*/)
    return angleConver((asin(sqrt(1 - cosPosV * cosPosV) * vecMod(ball.v_x, ball.v_y) / 2000)
                             + atan2(ball.y - data->plane1.y, ball.x - data->plane1.x)) * 180 / PI);


}


 inline double calangleDiff(DataStruct *data, BallStruct &ball)
 {

     return fmod(fmod(data->plane1.angle - calangleto(data,ball), 360) + 360, 360);

 }


 double changeangleto(DataStruct *data, BallStruct &ball)
 {
     double cosPosV;
     cosPosV = vecMul(ball.v_x, ball.v_y, data->plane1.x - ball.x, data->plane1.y - ball.y)
             / vecMod(ball.v_x, ball.v_y) / vecMod(data->plane1.x - ball.x, data->plane1.y - ball.y);
     if(calangleDiff(data,ball) < 180)
     {
         //qDebug() <<"yyy";
         return angleConver((asin(sqrt(1 - cosPosV * cosPosV) * vecMod(ball.v_x, ball.v_y) / 2000)
                              + atan2(ball.y - data->plane1.y, ball.x - data->plane1.x)) * 180 / PI);}

     else
     {
         //qDebug() <<"zzzzzzzzz";
         return angleConver((-asin(sqrt(1 - cosPosV * cosPosV) * vecMod(ball.v_x, ball.v_y) / 2000)
                              + atan2(ball.y - data->plane1.y, ball.x - data->plane1.x)) * 180 / PI);}

 }


 double countangleDiff(DataStruct *data, BallStruct &ball)
 {
     return fmod(fmod(data->plane1.angle - changeangleto(data,ball), 360) + 360, 360);

 }

// double changeangleDiff(DataStruct *data, BallStruct &ball)
// {
//     if(calangleDiff(data,ball) < 180&&ball.v_x>500)

//         return fmod(fmod(data->plane1.angle - changeangleto(data,ball)+1.7, 360) + 360, 360);

//     else if(calangleDiff(data,ball) < 180&&ball.v_x<-500)

//         return fmod(fmod(data->plane1.angle - changeangleto(data,ball)-1.7, 360) + 360, 360);
//     else if(ball.v_x>500)
//         return fmod(fmod(data->plane1.angle - changeangleto(data,ball)+1.7, 360) + 360, 360);

//     else if(ball.v_x<-500)
//         return fmod(fmod(data->plane1.angle - changeangleto(data,ball)-1.7, 360) + 360, 360);
//     else
//       return fmod(fmod(data->plane1.angle - changeangleto(data,ball), 360) + 360, 360);


// }

inline double v(BallStruct &ball)
{
    return  (ball.v_x*ball.v_x +ball.v_y*ball.v_y)/10000;

}

/* 瞄准某个球
 * 返回值：0：瞄准成功，-1：瞄准失败
 */
int AimBall(DataStruct *data, BallStruct &ball, int &leftward, int &rightward)
{

    if (countangleDiff(data,ball) < 1.8 || countangleDiff(data,ball) > 358.2)
    {
        return 1;
    }
    else if (countangleDiff(data,ball) < 180)
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
//int prePlanePos(DataStruct *data, int time, float &x, float &y)
//{
//    double v_x0, v_y0, v_x1, v_y1, a_x, a_y;
//    //获取初速度
//    v_x0 = data->plane1.v_x;
//    v_y0 = data->plane1.v_y;
//    //无速度则无需预测
//    if (v_x0 == 0 && v_y0 == 0)
//    {
//        x = data->plane1.x;
//        y = data->plane1.y;
//        return 0;
//    }
//    //计算加速度
//    a_x = -v_x0 / vecMod(v_x0, v_y0) * 4000;
//    a_y = -v_y0 / vecMod(v_x0, v_y0) * 4000;
//    //计算末速度
//    v_x1 = v_x0 + a_x * time / 100;
//    if (SIGN(v_x1) != SIGN(v_x0))
//    {
//        v_x1 = 0;
//    }
//    v_y1 = v_y0 + a_y * time / 100;
//    if (SIGN(v_y1) != SIGN(v_y0))
//    {
//        v_y1 = 0;
//    }
//    //计算位置
//    x = data->plane1.x + (v_x1 * v_x1 - v_x0 * v_x0) / 2 / a_x;
//    y = data->plane1.y + (v_y1 * v_y1 - v_y0 * v_y0) / 2 / a_y;
//    return 0;
//}


/* 判断是否需要移动
 * 参数：data：游戏数据、des_x：目标点横坐标、des_y：目标点纵坐标
 * 返回值：0：无需移动，1：需要移动
 */
int shootNeeded(DataStruct *data, int preTime = 200)
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
        if (dis < data->plane1.r + ball.r + 30 && t < min_t)
        {
            //设置最紧迫威胁
            min_t = t;
            if(data->bullet1_size!=0)
            return i;
        }
    }

    if (min_t != INT_MAX&&(data->bullet1_size==0))
    {
        return 1;
    }

    //给定时间内无危险
    return 0;
}

void shieldOpen(DataStruct *data,KeyStruct *key,int preTime = 200)
{
    for (int i = 0; i < data->ball_size; i++)
    {
        //qDebug()<<1;

        float min_t = INT_MAX;
        BallStruct &ball = data->ball[i];
        float ball_x = ball.x, ball_y = ball.y, ball_v_x = ball.v_x, ball_v_y = ball.v_y;
        float A, B, C;
        A = -1;
        B = ball_v_y / ball_v_x;
        C = ball_y - ball_x * ball_v_y / ball_v_x;
        float dis = fabs((A * data->plane1.y + B * data->plane1.x + C) / vecMod(A, B));
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
        if (dis < data->plane1.r + ball.r + 30&& t < min_t)
        {



            //qDebug()<<pow(distance(data,i),0.5);
            if(pow(distance(data,i),0.5)<=data->plane1.r + ball.r + 120)
            {
                //qDebug()<<3;
                key->shield = true;
            }


        }

    }


}



int perfect(DataStruct*data)
{
    int max=0;
    float j;
    for(int i = 0; i < data->ball_size; i++)
    {


        float flag=0;
        flag+=distance(data,i)*(-2)+(-0.6)*data->ball[i].hp+2*data->ball[i].r+fabs(v(data->ball[i])-40)*(-2);//+fabs(calangleDiff(data,data->ball[i])-180)*(-0.08);
        //qDebug()<<f;
        if(i==0)
            j=flag;
        if(flag>j)
        {
            j=flag;
            max=i;
        }

    }
  return  max;


}

void ai_pve(DataStruct *data,KeyStruct *key){
    //默认不移动且恒发射子弹
    key->forward = false;
    key->rotate_left = false;
    key->rotate_right = false;
    key->shoot = true;
    key->shield = false;
    //创建静态移动标志
    static int moveFlag = 1;
    //标志为0则正在移动或检查发现需要移动
    shieldOpen(data,key);

    if (shootNeeded(data) || moveFlag == 0)
    {
        AimBall(data, data->ball[shootNeeded(data)], key->rotate_left, key->rotate_right);
        if (moveFlag == -1)
        {
            std::cout << "移动发生错误" << std::endl;
        }
        //结束函数执行
        return ;
    }
    //如果球个数不为零
    if (data->ball_size)
    {

        int i = perfect(data);
        AimBall(data, data->ball[i], key->rotate_left, key->rotate_right);
    }

}

