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
void pvp_ai1_timeoutFault()
{
    Sleep(1000);
}

//������������
void pvp_ai1_crashFault()
{
    exit(0);
}

//������ɹ���
void pvp_ai1_randGenFault()
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
        pvp_ai1_timeoutFault();
    case 1:
        pvp_ai1_crashFault();
    }
}

//�Ƕ�����ϵת��
inline double angleConver1(double angle)
{
    return -90 - angle;
}

//��ȡ�������
inline double vecMul1(double x0, double y0, double x1, double y1)
{
    return x0 * x1 + y0 * y1;
}

//��ȡ����ȡģ
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

/* ��׼ĳ����
 * ����ֵ��0����׼�ɹ���-1����׼ʧ��
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
    //��׼�ĽǶ�
    double angleTo, angleDiff;
    //���˶�����ͷɻ�����λ�������ļн�����
    double cosPosV;
    if(!plane_v_x && !plane_v_y )
    cosPosV=1;
    else
    cosPosV = vecMul1(plane_v_x, plane_v_y, data->plane1.x - plane_x, data->plane1.y - plane_y)
            / vecMod1(plane_v_x, plane_v_y) / vecMod1(data->plane1.x - plane_x, data->plane1.y - plane_y);
    //�������Ҷ���a/sinA=b/sinB�����sinֵ������������
    angleTo = angleConver1(
                (asin(sqrt(1 - cosPosV * cosPosV) * vecMod1(plane_v_x, plane_v_y) / 2000)
                 + atan2(plane_y - data->plane1.y,plane_x - data->plane1.x)) * 180 / PI);

    //����ɻ�������ýǶ�֮��
    angleDiff = fmod(fmod(data->plane1.angle - angleTo, 360) + 360, 360);

    //���ݽǶȲ�ѡ�������ת����
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



/* Ԥ��ɻ�λ��
 * ������data����Ϸ���ݣ�x��y������λ�ã�time������ʱ��
 * ����ֵ��0��Ԥ��ɹ���-1��Ԥ��ʧ��
 */
int pvp_ai1_prePlanePos(DataStruct *data, int time, float &x, float &y)
{
    double v_x0, v_y0, v_x1, v_y1, a_x, a_y;
    //��ȡ���ٶ�
    v_x0 = data->plane1.v_x;
    v_y0 = data->plane1.v_y;
    //���ٶ�������Ԥ��
    if (v_x0 == 0 && v_y0 == 0)
    {
        x = data->plane1.x;
        y = data->plane1.y;
        return 0;
    }
    //������ٶ�
    a_x = -v_x0 / vecMod1(v_x0, v_y0) * 4000;
    a_y = -v_y0 / vecMod1(v_x0, v_y0) * 4000;
    //����ĩ�ٶ�
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
    //����λ��
    x = data->plane1.x + (v_x1 * v_x1 - v_x0 * v_x0) / 2 / a_x;
    y = data->plane1.y + (v_y1 * v_y1 - v_y0 * v_y0) / 2 / a_y;
    return 0;
}

/* ת��ĳ��
 * ������data����Ϸ���ݣ�x��y��Ŀ��㣬leftward��rightward�����ض���
 * ����ֵ��0������ת��1�����ת��-1��ת��ʧ��
 */
int pvp_ai1_turnTo(DataStruct *data, float x, float y, int &leftward, int &rightward)
{
    //�ɻ���Ŀ�ĵصĽǶ�
    double angleTo, angleDiff;

    //����ɻ���Ŀ�ĵصĽǶȲ��ı�����ϵ
    angleTo = angleConver1(atan2(y - data->plane1.y, x - data->plane1.x) * 180 / PI);
    //����ɻ�������ýǶ�֮��
    angleDiff = fmod(fmod(data->plane1.angle - angleTo, 360) + 360, 360);

    //���ݽǶȲ�ѡ�������ת����
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

/* �ƶ���ĳ��
 * ������data����Ϸ���ݣ�x��y��Ŀ��㣬forward��leftward��rightward�����ض���
 * ����ֵ��0�������ƶ���1������ƶ���-1���ƶ�ʧ��
 */
int pvp_ai1_moveTo(DataStruct *data, float x, float y, int &forward, int &leftward, int &rightward, int precision = 1)
{
    //���㵱ǰ�㵽�յ����
    double dis = vecMod1(y - data->plane1.y, x - data->plane1.x);
    //�ѵ���Ŀ�������ֹ����
    if (dis < precision)
    {
        return 1;
    }

    //Ԥ��ɻ�λ��
    float pre_x, pre_y;
    pvp_ai1_prePlanePos(data, 3000, pre_x, pre_y);

    //����ת���򲻼���
    if (!pvp_ai1_turnTo(data, data->plane1.x + x - pre_x, data->plane1.y + y - pre_y, leftward, rightward))
    {
        forward = false;
        return 0;
    }

    dis = vecMod1(y - pre_x, x - pre_y);
    //ͣ��ʱδ����Ŀ���
    if (dis >= precision)
    {
        forward = true;
        leftward = false;
        rightward = false;
    }

    return 0;
}

/* �ж��Ƿ���Ҫ�ƶ�
 * ������data����Ϸ���ݡ�des_x��Ŀ�������ꡢdes_y��Ŀ���������
 * ����ֵ��0�������ƶ���1����Ҫ�ƶ�
 */
int pvp_ai1_moveNeeded(DataStruct *data, int &des_x, int &des_y, int preTime = 200)
{
    float min_t = INT_MAX;
    int j;
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
        float dis = fabs((A * data->plane1.y + B * data->plane1.x + C) /vecMod1(A, B));
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
        if (dis < data->plane1.r + ball.r + 10 && t < min_t)
        {
            j=i;
            //�����������в
            min_t = t;
            //���෴�����ƶ���뾶�ľ���
            des_x = data->plane1.x +  ball.r * (data->plane1.x - cross_x)
                    / vecMod1(data->plane1.x - cross_x, data->plane1.y - cross_y);
            des_y = data->plane1.y +  ball.r * (data->plane1.y - cross_y)
                    / vecMod1(data->plane1.x - cross_x, data->plane1.y - cross_y);
            //���xԽ��
            if (des_x < data->plane1.r || des_x > 2000 - data->plane1.r)
            {
                des_x = data->plane1.x;
                if (data->plane1.y - cross_y > 0)
                    des_y = cross_y + ball.r + data->plane1.r;
                else
                    des_y = cross_y - ball.r - data->plane1.r;
            }
            //���yԽ��
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

    //����ʱ������Σ��
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
    //Ĭ�ϲ��ƶ��Һ㷢���ӵ�
    key->forward = false;
    key->rotate_left = false;
    key->rotate_right = false;
    key->shoot = true;
    key->shield = false;
    shieldOpen1(data,key);
    //������̬�ƶ���־


    static int moveFlag = 1, des_x, des_y;
//    if(data->bullet2_size!=0)
//    {
//        static int i=0;
//        qDebug()<<i++;

//        pvp_ai1_Aimplane(data, key->rotate_left, key->rotate_right);
//    }


    //��־Ϊ0�������ƶ����鷢����Ҫ�ƶ�
    /*else*/ if (pvp_ai1_moveNeeded(data, des_x, des_y) || moveFlag == 0)
    {
        //�����ƶ��������ƶ����
//        if(data->bullet1_size==0)
//        moveFlag = pvp_ai1_moveTo(data, des_x, des_y, key->forward, key->rotate_left, key->rotate_right);
//        else
            pvp_ai1_AimBall(data, data->ball[pvp_ai1_moveNeeded(data, des_x, des_y)], key->rotate_left, key->rotate_right);
        //���ر�־Ϊ-1��������
        if (moveFlag == -1)
        {
            std::cout << "�ƶ���������" << std::endl;
        }
        //��������ִ��
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

