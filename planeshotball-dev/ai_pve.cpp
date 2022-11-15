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

//������ʱ����
void timeoutFault()
{
    Sleep(1000);
}

//������������
void crashFault()
{
    exit(0);
}

//������ɹ���
void randGenFault()
{
    //������̬��ʼ����־
    static bool initFlag = false;
    //�������������
    if (!initFlag)
    {
        srand((int)time(NULL));
        initFlag = true;
    }
    //������ɹ���
    switch (random(3)) {
    case 0:
        timeoutFault();
    case 1:
        crashFault();
    }
}




//�Ƕ�����ϵת��
inline double angleConver(double angle)
{
    return -90 - angle;
}

//��ȡ�������
inline double vecMul(double x0, double y0, double x1, double y1)
{
    return x0 * x1 + y0 * y1;
}

//��ȡ����ȡģ
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

/* ��׼ĳ����
 * ����ֵ��0����׼�ɹ���-1����׼ʧ��
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


/* Ԥ��ɻ�λ��
 * ������data����Ϸ���ݣ�x��y������λ�ã�time������ʱ��
 * ����ֵ��0��Ԥ��ɹ���-1��Ԥ��ʧ��
 */
//int prePlanePos(DataStruct *data, int time, float &x, float &y)
//{
//    double v_x0, v_y0, v_x1, v_y1, a_x, a_y;
//    //��ȡ���ٶ�
//    v_x0 = data->plane1.v_x;
//    v_y0 = data->plane1.v_y;
//    //���ٶ�������Ԥ��
//    if (v_x0 == 0 && v_y0 == 0)
//    {
//        x = data->plane1.x;
//        y = data->plane1.y;
//        return 0;
//    }
//    //������ٶ�
//    a_x = -v_x0 / vecMod(v_x0, v_y0) * 4000;
//    a_y = -v_y0 / vecMod(v_x0, v_y0) * 4000;
//    //����ĩ�ٶ�
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
//    //����λ��
//    x = data->plane1.x + (v_x1 * v_x1 - v_x0 * v_x0) / 2 / a_x;
//    y = data->plane1.y + (v_y1 * v_y1 - v_y0 * v_y0) / 2 / a_y;
//    return 0;
//}


/* �ж��Ƿ���Ҫ�ƶ�
 * ������data����Ϸ���ݡ�des_x��Ŀ�������ꡢdes_y��Ŀ���������
 * ����ֵ��0�������ƶ���1����Ҫ�ƶ�
 */
int shootNeeded(DataStruct *data, int preTime = 200)
{
    float min_t = INT_MAX;
    //Ԥ��ÿһ������ƶ�
    for (int i = 0; i < data->ball_size; i++)
    {
        //��ȡ��ǰ�������
        BallStruct &ball = data->ball[i];
        //��ȡ��ǰ���������ٶ�
        float ball_x = ball.x, ball_y = ball.y, ball_v_x = ball.v_x, ball_v_y = ball.v_y;
        //�������˶�ֱ�߷���
        float A, B, C;
        A = -1;
        B = ball_v_y / ball_v_x;
        C = ball_y - ball_x * ball_v_y / ball_v_x;
        //����ɻ���ֱ�߾���
        float dis = fabs((A * data->plane1.y + B * data->plane1.x + C) / vecMod(A, B));
        //���㴹������
        float cross_x = (data->plane1.y - ball_y + ball_v_y / ball_v_x * ball_x + ball_v_x / ball_v_y * data->plane1.x)
                / (ball_v_x / ball_v_y + ball_v_y / ball_v_x);
        float cross_y = (ball_v_y / ball_v_x) * (cross_x - ball_x) + ball_y;
        //���㵽�����ʱ��
        float t = (cross_x - ball_x) / ball_v_x * 100;
        //�����˶���ʱ���������Ը���
        if (t < 0 || t > preTime)
        {
            continue;
        }
        //�жϸ����Ƿ�����в
        if (dis < data->plane1.r + ball.r + 30 && t < min_t)
        {
            //�����������в
            min_t = t;
            if(data->bullet1_size!=0)
            return i;
        }
    }

    if (min_t != INT_MAX&&(data->bullet1_size==0))
    {
        return 1;
    }

    //����ʱ������Σ��
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
        //���㵽�����ʱ��
        float t = (cross_x - ball_x) / ball_v_x * 100;
        //�����˶���ʱ���������Ը���
        if (t < 0 || t > preTime)
        {
            continue;
        }
        //�жϸ����Ƿ�����в
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
    //Ĭ�ϲ��ƶ��Һ㷢���ӵ�
    key->forward = false;
    key->rotate_left = false;
    key->rotate_right = false;
    key->shoot = true;
    key->shield = false;
    //������̬�ƶ���־
    static int moveFlag = 1;
    //��־Ϊ0�������ƶ����鷢����Ҫ�ƶ�
    shieldOpen(data,key);

    if (shootNeeded(data) || moveFlag == 0)
    {
        AimBall(data, data->ball[shootNeeded(data)], key->rotate_left, key->rotate_right);
        if (moveFlag == -1)
        {
            std::cout << "�ƶ���������" << std::endl;
        }
        //��������ִ��
        return ;
    }
    //����������Ϊ��
    if (data->ball_size)
    {

        int i = perfect(data);
        AimBall(data, data->ball[i], key->rotate_left, key->rotate_right);
    }

}

