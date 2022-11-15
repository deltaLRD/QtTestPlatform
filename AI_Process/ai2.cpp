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
    int divByZero = 10 / 0;
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

/* ��׼ĳ����
 * ������data����Ϸ���ݣ�ball_x��ball_y����ǰλ�ã�ball_v_x��ball_v_y����ǰ�ٶȣ�leftward��rightward�����ض���
 * ����ֵ��0����׼�ɹ���-1����׼ʧ��
 */
int AimBall(DataStruct *data, BallStruct &ball, int &leftward, int &rightward)
{
    float ball_x = ball.x, ball_y = ball.y, ball_v_x = ball.v_x, ball_v_y = ball.v_y;
    //��׼�ĽǶ�
    double angleTo, angleDiff;
    //���˶�����ͷɻ�����λ�������ļн�����
    double cosPosV;
    //��������˳�����������ģ
    cosPosV = vecMul(ball_v_x, ball_v_y, data->plane1.x - ball_x, data->plane1.y - ball_y)
            / vecMod(ball_v_x, ball_v_y) / vecMod(data->plane1.x - ball_x, data->plane1.y - ball_y);
    //�������Ҷ���a/sinA=b/sinB�����sinֵ������������
    angleTo = angleConver(
                (asin(sqrt(1 - cosPosV * cosPosV) * vecMod(ball_v_x, ball_v_y) / 2000)
                 + atan2(ball_y - data->plane1.y, ball_x - data->plane1.x)) * 180 / PI);

    //����ɻ�������ýǶ�֮��
    angleDiff = fmod(fmod(data->plane1.angle - angleTo, 360) + 360, 360);

    //���ݽǶȲ�ѡ�������ת����
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

/* Ԥ��ɻ�λ��
 * ������data����Ϸ���ݣ�x��y������λ�ã�time������ʱ��
 * ����ֵ��0��Ԥ��ɹ���-1��Ԥ��ʧ��
 */
int prePlanePos(DataStruct *data, int time, float &x, float &y)
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
    a_x = -v_x0 / vecMod(v_x0, v_y0) * 4000;
    a_y = -v_y0 / vecMod(v_x0, v_y0) * 4000;
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
int turnTo(DataStruct *data, float x, float y, int &leftward, int &rightward)
{
    //�ɻ���Ŀ�ĵصĽǶ�
    double angleTo, angleDiff;

    //����ɻ���Ŀ�ĵصĽǶȲ��ı�����ϵ
    angleTo = angleConver(atan2(y - data->plane1.y, x - data->plane1.x) * 180 / PI);
    //����ɻ�������ýǶ�֮��
    angleDiff = fmod(fmod(data->plane1.angle - angleTo, 360) + 360, 360);

    //���ݽǶȲ�ѡ�������ת����
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

/* �ƶ���ĳ��
 * ������data����Ϸ���ݣ�x��y��Ŀ��㣬forward��leftward��rightward�����ض���
 * ����ֵ��0�������ƶ���1������ƶ���-1���ƶ�ʧ��
 */
int moveTo(DataStruct *data, float x, float y, int &forward, int &leftward, int &rightward, int precision = 1)
{
    //���㵱ǰ�㵽�յ����
    double dis = vecMod(y - data->plane1.y, x - data->plane1.x);
    //�ѵ���Ŀ�������ֹ����
    if (dis < precision)
    {
        return 1;
    }

    //Ԥ��ɻ�λ��
    float pre_x, pre_y;
    prePlanePos(data, 3000, pre_x, pre_y);

    //����ת���򲻼���
    if (!turnTo(data, data->plane1.x + x - pre_x, data->plane1.y + y - pre_y, leftward, rightward))
    {
        forward = false;
        return 0;
    }

    dis = vecMod(y - pre_x, x - pre_y);
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
int moveNeeded(DataStruct *data, int &des_x, int &des_y, int preTime = 200)
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
        if (dis < data->plane1.r + ball.r + 10 && t < min_t)
        {
            //�����������в
            min_t = t;
            //���෴�����ƶ���뾶�ľ���
            des_x = data->plane1.x +  ball.r * (data->plane1.x - cross_x)
                    / vecMod(data->plane1.x - cross_x, data->plane1.y - cross_y);
            des_y = data->plane1.y +  ball.r * (data->plane1.y - cross_y)
                    / vecMod(data->plane1.x - cross_x, data->plane1.y - cross_y);
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
        return 1;
    }

    //����ʱ������Σ��
    return 0;
}

void ai_pve(DataStruct*data,KeyStruct*key){
    //Ĭ�ϲ��ƶ��Һ㷢���ӵ�
    key->forward = false;
    key->rotate_left = false;
    key->rotate_right = false;
    key->shoot = true;

    //������̬�ƶ���־
    static int moveFlag = 1, des_x, des_y;

    //��־Ϊ0�������ƶ����鷢����Ҫ�ƶ�
    if (moveNeeded(data, des_x, des_y) || moveFlag == 0)
    {
        //�����ƶ��������ƶ����
        moveFlag = moveTo(data, des_x, des_y, key->forward, key->rotate_left, key->rotate_right);
        //���ر�־Ϊ-1��������
        if (moveFlag == -1)
        {
            std::cout << "�ƶ���������" << std::endl;
        }
        //��������ִ��
        return;
    }

    //����������Ϊ��
    if (data->ball_size)
    {
        //��׼�����
        AimBall(data, data->ball[0], key->rotate_left, key->rotate_right);
    }
}
void ai1_pvp(DataStruct*data,KeyStruct*key){
    UNUSED(data)
    UNUSED(key)
}

