#include "DataStruct.h"
#include "math.h"

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <windows.h>
#include <vector>

#define PI 3.1415926
#define UNUSED(x) (void)x;
#define random(x) rand() % (x)
#define SIGN(x) ((x < 0 ) ? 1 : 0 )

void timeoutFault(){ //������ʱ����
    Sleep(1000);
}

void crashFault(){ //������������
    exit(0);
}

void randGenFault(){//������ɹ���
    //������̬��ʼ����־
    static bool initFlag = false;
    //�������������
    if (!initFlag){
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

inline double angleConver(double angle){ //�Ƕ�����ϵת��
    return -90 - angle;
}

inline double vecMul(double x0, double y0, double x1, double y1){//��ȡ�������
    return x0 * x1 + y0 * y1;
}

inline double vecMod(double x, double y){//��ȡ����ȡģ
    return sqrt(x * x + y * y);
}

//ת��ĳ��
//������data����Ϸ���ݣ�x��y��Ŀ��㣬leftward��rightward�����ض���
//����ֵ��0������ת��1�����ת��-1��ת��ʧ��
inline int turnTo(DataStruct *data, float x, float y, int &leftward, int &rightward){
    //�ɻ���Ŀ�ĵصĽǶ�
    double angleTo, angleDiff;
    //����ɻ���Ŀ�ĵصĽǶȲ��ı�����ϵ
    angleTo = angleConver(atan2(y - data->plane1.y, x - data->plane1.x) * 180 / PI);
    //����ɻ�������ýǶ�֮��
    angleDiff = fmod(fmod(data->plane1.angle - angleTo, 360) + 360, 360);

    //���ݽǶȲ�ѡ�������ת����
    if (angleDiff < 2.25 || angleDiff > 357.75){
        return 1;
    }
    else if (angleDiff < 180){
        leftward = false;
        rightward = true;
    }
    else{
        leftward = true;
        rightward = false;
    }

    return 0;
}

//��׼ĳ����
//������ball_x��ball_y����ǰλ�ã�ball_v_x��ball_v_y����ǰ�ٶȣ�leftward��rightward�����ض���
//����ֵ��Ԥ������ʱ��
double AimBall(DataStruct *data, BallStruct &ball, int &leftward, int &rightward){
    float ball_x = ball.x, ball_y = ball.y,
            ball_v_x = ball.v_x, ball_v_y = ball.v_y;
    //�������ĵ�˳�����������ģ��������˶�����ͷɻ�����λ�������ļн�����
    double cosPosV = vecMul(ball_v_x, ball_v_y, data->plane1.x - ball_x, data->plane1.y - ball_y)
            / vecMod(ball_v_x, ball_v_y) / vecMod(data->plane1.x - ball_x, data->plane1.y - ball_y);

    float e1_x, e1_y; //Ԥ�ƻ��е������
    double est_t; //Ԥ���ӵ�����ʱ��
    //����λ�ù�ϵ��P-Plane, B-Ball����BB'^2+PB^2-PB'^2=2*PB*BB'*cosPosV
    //����vb���ӵ��ٶ�v0��ʱ��t��(vb^2-v0^2)*t^2-2*PB*cosPosV*vb*t+PB^2=0
    //Ax^2+Bx+C=0��A=vb^2-v0^2, B=-2*PB*cosPosV*vb, C=PB^2
    //�����ʽ��-B+sqrt(B^2-4AC)/2A, -B-sqrt(B^2-4AC)/2A
    double PB = vecMod(data->plane1.x-ball_x, data->plane1.y-ball_y);
    double vb = vecMod(ball_v_x, ball_v_y);
    double v0 = 2000.0;
    double A = (vb*vb - v0*v0);
    double B = -2 * PB * cosPosV * vb;
    double C = PB*PB;
    est_t = (-B-sqrt(B*B-4*A*C))/2/A; //��һ���Ǹ�������ȥ
    e1_x = ball_x + ball_v_x*est_t;
    e1_y = ball_y + ball_v_y*est_t;
    turnTo(data,e1_x,e1_y,leftward,rightward);

    double angleTo0, angleDiff0; //�ɻ���Ŀ�ĵصĽǶ�
    //����ɻ���Ŀ�ĵصĽǶȲ��ı�����ϵ
    angleTo0 = angleConver(atan2(e1_y - data->plane1.y, e1_x - data->plane1.x) * 180 / PI);
    //����ɻ�������ýǶ�֮��
    angleDiff0 = fmod(fmod(data->plane1.angle - angleTo0, 360) + 360, 360);
    //����ת������ʱ��
    double turnTime = angleDiff0 / 360;
    return /*est_t +*/ turnTime;

}

//��������������
void defend(DataStruct *data, int &leftward, int &rightward){
    int enemyIndex=0;
    int min_dist_sqr=INT_MAX;
    for (int i = 0; i < data->ball_size; i++){ //�ҳ������������
        float ball_x = data->ball[i].x, ball_y = data->ball[i].y;
        float pl_x = data->plane1.x, pl_y = data->plane1.y;
        int dist_sqr=(ball_x-pl_x)*(ball_x-pl_x)+(ball_y-pl_y)*(ball_y-pl_y);
        if(dist_sqr<min_dist_sqr){
            min_dist_sqr=dist_sqr;
            enemyIndex=i;
        }
    }
    std::cout<<"Defend against Ball["<<enemyIndex<<"], distance: "<<sqrt(min_dist_sqr)<<".\n";
    AimBall(data,data->ball[enemyIndex],leftward,rightward);
}

//Ԥ��ɻ�λ��
//������data����Ϸ���ݣ�x��y������λ�ã�time������ʱ��
//����ֵ��0��Ԥ��ɹ���-1��Ԥ��ʧ��
int prePlanePos(DataStruct *data, int time, float &x, float &y){
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

//�ƶ���ĳ��
//������data����Ϸ���ݣ�x��y��Ŀ��㣬forward��leftward��rightward�����ض���
//����ֵ��0�������ƶ���1������ƶ���-1���ƶ�ʧ��
int moveTo(DataStruct *data, float x, float y, int &forward, int &leftward, int &rightward, int precision = 1){
    //���㵱ǰ�㵽�յ����
    double dis = vecMod(y - data->plane1.y, x - data->plane1.x);
    //�ѵ���Ŀ�������ֹ����
    if (dis < precision){
        return 1;
    }

    //Ԥ��ɻ�λ��
    float pre_x, pre_y;
    prePlanePos(data, 3000, pre_x, pre_y);

    //����ת���򲻼���
    if (!turnTo(data, data->plane1.x + x - pre_x, data->plane1.y + y - pre_y, leftward, rightward)){
        forward = false;
        return 0;
    }

    dis = vecMod(y - pre_x, x - pre_y);
    //ͣ��ʱδ����Ŀ���
    if (dis >= precision){
        forward = true;
        leftward = false;
        rightward = false;
    }

    return 0;
}

//�ܵ���в
int threatened(DataStruct* data){
    for (int i = 0; i < data->ball_size; i++){ //Ԥ��ÿһ������ƶ�
        //��ȡ��ǰ�������
        BallStruct &ball = data->ball[i];
        //��ȡ��ǰ��İ뾶��������ٶ�
        float ball_r = ball.r;
        float ball_x = ball.x, ball_y = ball.y, ball_v_x = ball.v_x, ball_v_y = ball.v_y;
        //����һ��ʱ�����Ƿ���Σ��
        float timeUnits = 60;
        float est_x=ball_x+ball_v_x*timeUnits/100,
            est_y=ball_y+ball_v_y*timeUnits/100;
        float est_plane_x=data->plane1.x+data->plane1.v_x*timeUnits/100,
            est_plane_y=data->plane1.y+data->plane1.v_y*timeUnits/100;

        //Բ������
        float alertDist=500; //�������
        if( (est_x-est_plane_x)*(est_x-est_plane_x) +
                (est_y-est_plane_y)*(est_y-est_plane_y) <
                (alertDist+ball_r)*(alertDist+ball_r) ){
            return true;
        }
    }
    return false;
}

//Ѱ��������׼����
int acuteAngle_ball(DataStruct* data){
    double angleThreshold = PI/6; //�Ƕ�����
    float veloThreshold = 250.0; //�ٶ�����
    vector<int> vIndexes;
    for (int i = 0; i < data->ball_size; i++){
        float ball_x = data->ball[i].x, ball_y = data->ball[i].y;
        float ball_vx = data->ball[i].v_x, ball_vy = data->ball[i].v_y;
        float pl_x = data->plane1.x, pl_y = data->plane1.y;

        if( ball_vx*ball_vx + ball_vy*ball_vy < veloThreshold*veloThreshold )
            continue;

        float lineVec_x = pl_x - ball_x, lineVec_y = pl_y - ball_y;
        double vecProduct = vecMul(lineVec_x, lineVec_y, ball_vx, ball_vy);
        double lineVec_norm = vecMod(lineVec_x, lineVec_y);
        double veloVec_norm = vecMod(ball_vx, ball_vy);
        double cosAngle = vecProduct / lineVec_norm / veloVec_norm;
        double angle = acos(cosAngle);

        if( angle < angleThreshold || angle > PI-angleThreshold ){
            std::cout<<"Ball ["<<i<<"] ";
            std::cout<<"Angle: "<<angle/PI*180<<"deg ";
            std::cout<<"Velocity: "<<sqrt(ball_vx*ball_vx + ball_vy*ball_vy)<<".\n";
            vIndexes.push_back(i);
        }
    }
    int enemyIndex = 0;
    int min_dist_sqr=INT_MAX;
    for(int k=0;k<vIndexes.size();k++){
        float ball_x = data->ball[vIndexes[k]].x,
                ball_y = data->ball[vIndexes[k]].y;
        float pl_x = data->plane1.x, pl_y = data->plane1.y;
        int dist_sqr=(ball_x-pl_x)*(ball_x-pl_x)+(ball_y-pl_y)*(ball_y-pl_y);
        if(dist_sqr<min_dist_sqr){
            min_dist_sqr=dist_sqr;
            enemyIndex=vIndexes[k];
        }
    }
    return enemyIndex;
}

//�÷�����ж���в
//����ֵ����0��ӭ�����������±꣬-1�����������������뾯����룬-9��û����в
int threatened_pro(DataStruct* data){
    //������
    for (int i = 0; i < data->ball_size; i++){ //Ԥ��ÿһ������ƶ�
        //��ȡ��ǰ�������
        BallStruct &ball = data->ball[i];
        //��ȡ��ǰ��İ뾶��������ٶ�
        float ball_r = ball.r;
        float ball_x = ball.x, ball_y = ball.y, ball_v_x = ball.v_x, ball_v_y = ball.v_y;
        //����500ms���Ƿ���Σ��
        float est_x=ball_x+ball_v_x*50/100,
            est_y=ball_y+ball_v_y*50/100;
        float est_plane_x=data->plane1.x+data->plane1.v_x*50/100,
            est_plane_y=data->plane1.y+data->plane1.v_y*50/100;

        //Բ������
        float alertDist=500; //�������
        if( (est_x-est_plane_x)*(est_x-est_plane_x) +
                (est_y-est_plane_y)*(est_y-est_plane_y) <
                (alertDist+ball_r)*(alertDist+ball_r) ){
            return -1;
        }
    }


    //�����
    vector<int> vIndexes;
    for (int i = 0; i < data->ball_size; i++){
        float ball_x = data->ball[i].x, ball_y = data->ball[i].y;
        float ball_vx = data->ball[i].v_x, ball_vy = data->ball[i].v_y;
        float pl_x = data->plane1.x, pl_y = data->plane1.y;
        double vec_angle = atan( (pl_y-ball_y)/(pl_x-ball_x) ) +PI/2.0; //rad
        double direc_angle = atan( ball_vy/ball_vx ) +PI/2.0; //rad
        if(abs(vec_angle-direc_angle)<PI/36.0)
            vIndexes.push_back(i); //�ٶȷ��������߼н�5������
    }
    int enemyIndex=-10;
    int min_dist_sqr=INT_MAX;
    for(int k=0;k<vIndexes.size();k++){
        float ball_x = data->ball[vIndexes[k]].x,
                ball_y = data->ball[vIndexes[k]].y;
        float pl_x = data->plane1.x, pl_y = data->plane1.y;
        int dist_sqr=(ball_x-pl_x)*(ball_x-pl_x)+(ball_y-pl_y)*(ball_y-pl_y);
        if(dist_sqr<min_dist_sqr){
            min_dist_sqr=dist_sqr;
            enemyIndex=vIndexes[k];
        }
    }
    if(enemyIndex!=-10){
        std::cout<<"Ball ["<<enemyIndex<<"] on critical trail, distance: "<<sqrt(min_dist_sqr)<<".\n";
        return enemyIndex;
    }


    //����в
    return -9;
}

//K-means clustering
BallStruct clusterShoot(DataStruct* data){
    int K=4; //2*2����
    vector<int> clus[K];
    float clusCenter_x[K], clusCenter_y[K];
    //��ʼ����������
    clusCenter_x[0]=500.0;clusCenter_x[1]=1500.0;clusCenter_x[2]=500.0;clusCenter_x[3]=1500.0;
    clusCenter_y[0]=375.0;clusCenter_y[1]=375.0;clusCenter_y[2]=1125.0;clusCenter_y[3]=1125.0;

    for(int xx=0;xx<5;xx++){ //��������
        for(int p=0;p<data->ball_size;p++){ //��ÿ��������������ľ���
            BallStruct& ball=data->ball[p];
            double min_dist_sqr=INT_MAX;
            int indx=0;
            for(int i=0;i<K;i++){
                double dist_sqr = (ball.x-clusCenter_x[i])*(ball.x-clusCenter_x[i])
                        + (ball.y-clusCenter_y[i])*(ball.y-clusCenter_y[i]);
                if(dist_sqr < min_dist_sqr){
                    min_dist_sqr = dist_sqr;
                    indx=i;
                }
            }
            clus[indx].push_back(p);
        }
        for(int i=0;i<K;i++){ //���¾�����������
            if(clus[i].size()>0){
                double xsum=0, ysum=0;
                for(int jl=0;jl<clus[i].size();jl++){
                    xsum+=data->ball[clus[i][jl]].x;
                    ysum+=data->ball[clus[i][jl]].y;
                }
                clusCenter_x[i]=xsum/clus[i].size();
                clusCenter_y[i]=ysum/clus[i].size();
            }
        }
    }

    //�ҳ������ľ���
    int largestClusIndx=0;
    int max_clus_size=INT_MIN;
    for(int i=0;i<K;i++){
        if(clus[i].size()>max_clus_size){
            largestClusIndx = i;
            max_clus_size = clus[i].size();
        }
    }

    //����Ŀ��
    BallStruct ballCluster;
    ballCluster.x=clusCenter_x[largestClusIndx];
    ballCluster.y=clusCenter_y[largestClusIndx];
    ballCluster.v_x=0;ballCluster.v_y=0;
    ballCluster.r=100;
    //���ɴ����ƫ������Ŀ��
//    static bool having_rand_seed = false;
//    if (!having_rand_seed){
//        srand((int)time(NULL));
//        having_rand_seed = true;
//    }
//    int ranint_x=random(300), ranint_y=random(300);
//    BallStruct ballCluster;
//    ballCluster.x=clusCenter_x[largestClusIndx] + ranint_x;
//    ballCluster.y=clusCenter_y[largestClusIndx] + ranint_y;
//    ballCluster.v_x=0;ballCluster.v_y=0;
//    ballCluster.r=100;

    return ballCluster;
}

//ÿ20ms����һ��
void ai_pve(DataStruct*data,KeyStruct*key){

    //Ĭ�ϲ��ƶ��Һ㷢���ӵ�
    key->forward = false;
    key->rotate_left = false;
    key->rotate_right = false;
    key->shoot = true;

//    if( data->bullet1_size>0 && threatened(data) ){
//        //��������򿪷�����
//        key->shield=true;
//        //��Ҫ���ܣ���������
//        defend(data,key->rotate_left,key->rotate_right);
//        //��������ִ��
//        return;
//    }

    //�Ż��ķ��ط����㷨
    if(data->bullet1_size>0){
        int threatCode = threatened_pro(data);
        if(threatCode==-1){
            key->shield = true;
            defend(data, key->rotate_left, key->rotate_right);
            return;
        }
        if(threatCode>=0){
            AimBall(data, data->ball[threatCode], key->rotate_left, key->rotate_right);
            return;
        }
    }

    //������̬�ƶ���־
    static int moveFlag = 1, des_x, des_y;

    //���ݳ�ʼ�������ȥ�ĸ��ǣ�Ĭ�����½�
    static int base_x=1950-data->plane1.r, base_y=1450-data->plane1.r;
    if( data->ball_size==3 && data->ball[0].r==100&&data->ball[1].r==100&&data->ball[2].r==100 ){
        //��ֵ�㷨
//        int est_avr_x_sum=data->ball[0].x+data->ball[1].x+data->ball[2].x
//                + (data->ball[0].v_x+data->ball[1].v_x+data->ball[2].v_x)*10/100;
//        if(est_avr_x_sum>3*1000)
//            base_x=50+data->plane1.r; //������������ұߣ�ȥ���½�

        //ͳ���㷨
        int LCount=0,RCount=0;
        for(int i=0;i<3;i++){
            if(data->ball[i].x<950) LCount++;
            else RCount++;
        }
        if(LCount<RCount)
            base_x=50+data->plane1.r; //������������ұߣ�ȥ���½�
    }

    //���ڽ��䣬����ײ��
    if(base_x==50+data->plane1.r){
        if(data->plane1.x>base_x+200 || data->plane1.y<base_y-200){
            moveFlag=0;
            des_x=base_x;
            des_y=base_y;
        }
    }
    else{
        if(data->plane1.x<base_x-200 || data->plane1.y<base_y-200){
            moveFlag=0;
            des_x=base_x;
            des_y=base_y;
        }
    }

    //��־Ϊ0�������ƶ�����Ҫ�ƶ������ڷǹ���״̬�ƶ�
    if ( moveFlag==0 && data->bullet1_size==0 ){
        //�����ƶ��������ƶ����
        moveFlag = moveTo(data, des_x, des_y, key->forward, key->rotate_left, key->rotate_right);
        //���ر�־Ϊ-1��������
        if (moveFlag == -1)
            std::cout << "�ƶ���������" << std::endl;
        //��������ִ��
        return;
    }

    if(data->ball_size){
        //�Ż�Ŀ��ѡ��
        int tarIndx = acuteAngle_ball(data);
        AimBall(data, data->ball[tarIndx], key->rotate_left, key->rotate_right);
        std::cout<<"Target: Ball["<<tarIndx<<"].\n";

        //������е�һ���ӵ���ת���µ�Ŀ��
//        static int timeThreshold = 13; //����26��ʱ�䵥λ��ȷ���ɻ��ܹ���׼90�����ڵ�Ŀ��
//        static int timeSlotCount = 0; //�Ǳ�׼ʱ��֡��10ms������2����
//        static int targetIndex = 0;
//        //Ԥ������ʱ�䣨��λ���룩
//        double est_time =
//                AimBall(data, data->ball[targetIndex], key->rotate_left, key->rotate_right);
//        //����ʱ�䳤����ȴ�
//        if(est_time > timeThreshold*0.02)
//            timeThreshold = ceil(est_time/0.02);

//        if(timeSlotCount % timeThreshold == 0){
//            std::cout<<"Target: Ball "<<targetIndex<<", time: "<<timeThreshold*20<<"ms.\n";
//            targetIndex ++ ;
//            targetIndex = targetIndex % data->ball_size; //��ֹ�±�Խ��
//            timeThreshold = 13;
//        }
//        timeSlotCount ++ ;
    }
}

