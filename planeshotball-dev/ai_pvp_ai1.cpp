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
void pvp_ai1_timeoutFault()
{
    Sleep(1000);
}

//产生崩溃故障
void pvp_ai1_crashFault()
{
    exit(0);
}

//随机生成故障
void pvp_ai1_randGenFault()
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
        pvp_ai1_timeoutFault();
    case 1:
        pvp_ai1_crashFault();
    }
}

//角度坐标系转换
inline double angleConver1(double angle)
{
    return -90 - angle;
}

//抽取向量点乘
inline double vecMul1(double x0, double y0, double x1, double y1)
{
    return x0 * x1 + y0 * y1;
}

//抽取向量取模
inline double vecMod1(double x, double y)
{
    return sqrt(x * x + y * y);
}

inline double distance1(DataStruct *data,int x)
{
    return ( (data->plane1.x-data->ball[x].x)*(data->plane1.x-data->ball[x].x)+(data->plane1.y-data->ball[x].y)*(data->plane1.y-data->ball[x].y));
}

inline double distanceBullet1(DataStruct *data,int x)
{

    return ( (data->plane1.x-data->bullet2[x].x)*(data->plane1.x-data->bullet2[x].x)+(data->plane1.y-data->bullet2[x].y)*(data->plane1.y-data->bullet2[x].y));

}


 double calangleto1(DataStruct *data, BallStruct &ball)
{
    double cosPosV/*,angleto,angle*/;
    cosPosV = vecMul1(ball.v_x, ball.v_y, data->plane1.x - ball.x, data->plane1.y - ball.y)
            / vecMod1(ball.v_x, ball.v_y) / vecMod1(data->plane1.x - ball.x, data->plane1.y - ball.y);

    if(ball.v_x<0/*-data->plane1.x<0*/)
    return angleConver1((-asin(sqrt(1 - cosPosV * cosPosV) * vecMod1(ball.v_x, ball.v_y) / 2000)
                             + atan2(ball.y - data->plane1.y, ball.x - data->plane1.x)) * 180 / PI);
    if(ball.x>0/*-data->plane1.x>=0*/)
    return angleConver1((asin(sqrt(1 - cosPosV * cosPosV) * vecMod1(ball.v_x, ball.v_y) / 2000)
                             + atan2(ball.y - data->plane1.y, ball.x - data->plane1.x)) * 180 / PI);


}


 inline double calangleDiff1(DataStruct *data, BallStruct &ball)
 {

     return fmod(fmod(data->plane1.angle - calangleto1(data,ball), 360) + 360, 360);

 }


 double changeangleto1(DataStruct *data, BallStruct &ball)
 {
     double cosPosV;
     cosPosV = vecMul1(ball.v_x, ball.v_y, data->plane1.x - ball.x, data->plane1.y - ball.y)
             / vecMod1(ball.v_x, ball.v_y) / vecMod1(data->plane1.x - ball.x, data->plane1.y - ball.y);
     if(calangleDiff1(data,ball) < 180/*&&ball.v_x>0*/)
     {
         //qDebug() <<"yyy";
         return angleConver1((asin(sqrt(1 - cosPosV * cosPosV) * vecMod1(ball.v_x, ball.v_y) / 2000)
                              + atan2(ball.y - data->plane1.y, ball.x - data->plane1.x)) * 180 / PI);}

     else /*if(ball.v_x<0)*/
     {
         //qDebug() <<"zzzzzzzzz";
         return angleConver1((-asin(sqrt(1 - cosPosV * cosPosV) * vecMod1(ball.v_x, ball.v_y) / 2000)
                              + atan2(ball.y - data->plane1.y, ball.x - data->plane1.x)) * 180 / PI);}

 }


 double countangleDiff1(DataStruct *data, BallStruct &ball)
 {
     return fmod(fmod(data->plane1.angle - changeangleto1(data,ball), 360) + 360, 360);

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

inline double v1(BallStruct &ball)
{
    return  (ball.v_x*ball.v_x +ball.v_y*ball.v_y)/10000;

}

/* 瞄准某个球
 * 返回值：0：瞄准成功，-1：瞄准失败
 */
int pvp_ai1_AimBall(DataStruct *data, BallStruct &ball, int &leftward, int &rightward)
{

    if (countangleDiff1(data,ball) < 1.8 || countangleDiff1(data,ball) > 358.2)
    {
        return 1;
    }
    else if (countangleDiff1(data,ball) < 180)
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




int pvp_ai1_Aimplane(DataStruct *data, int &leftward, int &rightward)
{
    float plane_x = data->plane2.x, plane_y = data->plane2.y, plane_v_x = data->plane2.v_x, plane_v_y = data->plane2.v_y;
    //瞄准的角度
    double angleTo, angleDiff;
    //球运动方向和飞机与球位置向量的夹角余弦
    double cosPosV;
    if(!plane_v_x && !plane_v_y )
    cosPosV=1;
    else
    cosPosV = vecMul1(plane_v_x, plane_v_y, data->plane1.x - plane_x, data->plane1.y - plane_y)
            / vecMod1(plane_v_x, plane_v_y) / vecMod1(data->plane1.x - plane_x, data->plane1.y - plane_y);
    //根据正弦定理（a/sinA=b/sinB）求出sin值再求得所需度数
    angleTo = angleConver1(
                (asin(sqrt(1 - cosPosV * cosPosV) * vecMod1(plane_v_x, plane_v_y) / 2000)
                 + atan2(plane_y - data->plane1.y,plane_x - data->plane1.x)) * 180 / PI);

    //计算飞机朝向与该角度之差
    angleDiff = fmod(fmod(data->plane1.angle - angleTo, 360) + 360, 360);

    //根据角度差选择更优旋转方向
    if (angleDiff < 1.8 || angleDiff > 358.2)
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
int pvp_ai1_prePlanePos(DataStruct *data, int time, float &x, float &y)
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
    a_x = -v_x0 / vecMod1(v_x0, v_y0) * 4000;
    a_y = -v_y0 / vecMod1(v_x0, v_y0) * 4000;
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
int pvp_ai1_turnTo(DataStruct *data, float x, float y, int &leftward, int &rightward)
{
    //飞机到目的地的角度
    double angleTo, angleDiff;

    //计算飞机到目的地的角度并改变坐标系
    angleTo = angleConver1(atan2(y - data->plane1.y, x - data->plane1.x) * 180 / PI);
    //计算飞机朝向与该角度之差
    angleDiff = fmod(fmod(data->plane1.angle - angleTo, 360) + 360, 360);

    //根据角度差选择更优旋转方向
//    if (angleDiff < 3.6 || angleDiff > 356.4)
//    {
//        return 1;
//    }
/* else */ if (angleDiff < 180)
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
int pvp_ai1_moveTo(DataStruct *data, float x, float y, int &forward, int &leftward, int &rightward, int precision = 1)
{
    //计算当前点到终点距离
    double dis = vecMod1(y - data->plane1.y, x - data->plane1.x);
    //已到达目标点则终止动作
    if (dis < precision)
    {
        return 1;
    }

    //预测飞机位置
    float pre_x, pre_y;
    pvp_ai1_prePlanePos(data, 3000, pre_x, pre_y);

    //正在转向则不加速
    if (!pvp_ai1_turnTo(data, data->plane1.x + x - pre_x, data->plane1.y + y - pre_y, leftward, rightward))
    {
        forward = false;
        return 0;
    }

    dis = vecMod1(y - pre_x, x - pre_y);
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
int pvp_ai1_moveNeeded(DataStruct *data, int &des_x, int &des_y, int preTime = 200)
{
    float min_t = INT_MAX;
    int j;
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
        float dis = fabs((A * data->plane1.y + B * data->plane1.x + C) /vecMod1(A, B));
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
            j=i;
            //设置最紧迫威胁
            min_t = t;
            //向相反方向移动球半径的距离
            des_x = data->plane1.x +  ball.r * (data->plane1.x - cross_x)
                    / vecMod1(data->plane1.x - cross_x, data->plane1.y - cross_y);
            des_y = data->plane1.y +  ball.r * (data->plane1.y - cross_y)
                    / vecMod1(data->plane1.x - cross_x, data->plane1.y - cross_y);
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
        return j;
    }

    //给定时间内无危险
    return 0;
}



int ai1perfect(DataStruct*data)
{
    int max=0;
    float j;
    for(int i = 0; i < data->ball_size; i++)
    {
//       int flag=0;
//       if(data->ball[max].hp>data->ball[i].hp)

//       {flag=1;max=i;}
//       if(flag&&distance1(data,max)>distance1(data,i))
//           max=i;
//       if(flag&&data->ball[max].r<data->ball[i].r)
//           max=i;
        float f=0;
        f+=distance1(data,i)*(-2)+(-20)*data->ball[i].hp+2*data->ball[i].r+fabs(v1(data->ball[i])-40)*(-2);//+fabs(calangleDiff(data,data->ball[i])-180)*(-0.08);
        //qDebug()<<f;
        if(i==0)
            j=f;
        if(f>j)
        {
            j=f;
            max=i;
        }

    }
  //qDebug()<<max;
  return  max;


}

void shieldOpen1(DataStruct *data,KeyStruct *key,int preTime = 200)
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
        float dis = fabs((A * data->plane1.y + B * data->plane1.x + C) / vecMod1(A, B));
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
            if(pow(distance1(data,i),0.5)<=data->plane1.r + ball.r + 120)
            {
                //qDebug()<<3;
                key->shield = true;
            }


        }

    }


    for(int j = 0; j < data->bullet2_size; j++)
    {
        if(pow(distanceBullet1(data,j),0.5)<= 80)
        {
            //qDebug()<<3;
            key->shield = true;
        }

    }


}



void ai_pvp_ai1(DataStruct*data,KeyStruct*key){
    //默认不移动且恒发射子弹
    key->forward = false;
    key->rotate_left = false;
    key->rotate_right = false;
    key->shoot = true;
    key->shield = false;
    shieldOpen1(data,key);
    //创建静态移动标志


    static int moveFlag = 1, des_x, des_y;
//    if(data->bullet2_size!=0)
//    {
//        static int i=0;
//        qDebug()<<i++;

//        pvp_ai1_Aimplane(data, key->rotate_left, key->rotate_right);
//    }


    //标志为0则正在移动或检查发现需要移动
    /*else*/ if (pvp_ai1_moveNeeded(data, des_x, des_y) || moveFlag == 0)
    {
        //进行移动并返回移动结果
//        if(data->bullet1_size==0)
//        moveFlag = pvp_ai1_moveTo(data, des_x, des_y, key->forward, key->rotate_left, key->rotate_right);
//        else
            pvp_ai1_AimBall(data, data->ball[pvp_ai1_moveNeeded(data, des_x, des_y)], key->rotate_left, key->rotate_right);
        //返回标志为-1则发生错误
        if (moveFlag == -1)
        {
            std::cout << "移动发生错误" << std::endl;
        }
        //结束函数执行
        return;
    }

    if(data->bullet2_size!=0)
    {

        pvp_ai1_Aimplane(data, key->rotate_left, key->rotate_right);
    }

    else
    {

        int i = ai1perfect(data);
        pvp_ai1_AimBall(data, data->ball[i], key->rotate_left, key->rotate_right);
    }


}

