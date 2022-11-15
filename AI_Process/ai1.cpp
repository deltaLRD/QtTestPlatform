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

void timeoutFault(){ //产生超时故障
    Sleep(1000);
}

void crashFault(){ //产生崩溃故障
    exit(0);
}

void randGenFault(){//随机生成故障
    //创建静态初始化标志
    static bool initFlag = false;
    //产生随机数种子
    if (!initFlag){
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

inline double angleConver(double angle){ //角度坐标系转换
    return -90 - angle;
}

inline double vecMul(double x0, double y0, double x1, double y1){//抽取向量点乘
    return x0 * x1 + y0 * y1;
}

inline double vecMod(double x, double y){//抽取向量取模
    return sqrt(x * x + y * y);
}

//转向某点
//参数：data：游戏数据，x、y：目标点，leftward、rightward：返回动作
//返回值：0：正在转向，1：完成转向，-1：转向失败
inline int turnTo(DataStruct *data, float x, float y, int &leftward, int &rightward){
    //飞机到目的地的角度
    double angleTo, angleDiff;
    //计算飞机到目的地的角度并改变坐标系
    angleTo = angleConver(atan2(y - data->plane1.y, x - data->plane1.x) * 180 / PI);
    //计算飞机朝向与该角度之差
    angleDiff = fmod(fmod(data->plane1.angle - angleTo, 360) + 360, 360);

    //根据角度差选择更优旋转方向
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

//瞄准某个球
//参数：ball_x、ball_y：球当前位置，ball_v_x、ball_v_y：球当前速度，leftward、rightward：返回动作
//返回值：预计所需时间
double AimBall(DataStruct *data, BallStruct &ball, int &leftward, int &rightward){
    float ball_x = ball.x, ball_y = ball.y,
            ball_v_x = ball.v_x, ball_v_y = ball.v_y;
    //两向量的点乘除以两向量的模，求出球运动方向和飞机与球位置向量的夹角余弦
    double cosPosV = vecMul(ball_v_x, ball_v_y, data->plane1.x - ball_x, data->plane1.y - ball_y)
            / vecMod(ball_v_x, ball_v_y) / vecMod(data->plane1.x - ball_x, data->plane1.y - ball_y);

    float e1_x, e1_y; //预计击中点的坐标
    double est_t; //预计子弹飞行时间
    //几何位置关系（P-Plane, B-Ball）：BB'^2+PB^2-PB'^2=2*PB*BB'*cosPosV
    //球速vb，子弹速度v0，时间t：(vb^2-v0^2)*t^2-2*PB*cosPosV*vb*t+PB^2=0
    //Ax^2+Bx+C=0：A=vb^2-v0^2, B=-2*PB*cosPosV*vb, C=PB^2
    //求根公式：-B+sqrt(B^2-4AC)/2A, -B-sqrt(B^2-4AC)/2A
    double PB = vecMod(data->plane1.x-ball_x, data->plane1.y-ball_y);
    double vb = vecMod(ball_v_x, ball_v_y);
    double v0 = 2000.0;
    double A = (vb*vb - v0*v0);
    double B = -2 * PB * cosPosV * vb;
    double C = PB*PB;
    est_t = (-B-sqrt(B*B-4*A*C))/2/A; //另一根是负数，舍去
    e1_x = ball_x + ball_v_x*est_t;
    e1_y = ball_y + ball_v_y*est_t;
    turnTo(data,e1_x,e1_y,leftward,rightward);

    double angleTo0, angleDiff0; //飞机到目的地的角度
    //计算飞机到目的地的角度并改变坐标系
    angleTo0 = angleConver(atan2(e1_y - data->plane1.y, e1_x - data->plane1.x) * 180 / PI);
    //计算飞机朝向与该角度之差
    angleDiff0 = fmod(fmod(data->plane1.angle - angleTo0, 360) + 360, 360);
    //计算转向所需时间
    double turnTime = angleDiff0 / 360;
    return /*est_t +*/ turnTime;

}

//向距离最近的球开炮
void defend(DataStruct *data, int &leftward, int &rightward){
    int enemyIndex=0;
    int min_dist_sqr=INT_MAX;
    for (int i = 0; i < data->ball_size; i++){ //找出距离最近的球
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

//预测飞机位置
//参数：data：游戏数据，x、y：返回位置，time：给定时间
//返回值：0：预测成功，-1：预测失败
int prePlanePos(DataStruct *data, int time, float &x, float &y){
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

//移动至某点
//参数：data：游戏数据，x、y：目标点，forward、leftward、rightward：返回动作
//返回值：0：正在移动，1：完成移动，-1：移动失败
int moveTo(DataStruct *data, float x, float y, int &forward, int &leftward, int &rightward, int precision = 1){
    //计算当前点到终点距离
    double dis = vecMod(y - data->plane1.y, x - data->plane1.x);
    //已到达目标点则终止动作
    if (dis < precision){
        return 1;
    }

    //预测飞机位置
    float pre_x, pre_y;
    prePlanePos(data, 3000, pre_x, pre_y);

    //正在转向则不加速
    if (!turnTo(data, data->plane1.x + x - pre_x, data->plane1.y + y - pre_y, leftward, rightward)){
        forward = false;
        return 0;
    }

    dis = vecMod(y - pre_x, x - pre_y);
    //停下时未到达目标点
    if (dis >= precision){
        forward = true;
        leftward = false;
        rightward = false;
    }

    return 0;
}

//受到威胁
int threatened(DataStruct* data){
    for (int i = 0; i < data->ball_size; i++){ //预测每一个球的移动
        //获取当前球的引用
        BallStruct &ball = data->ball[i];
        //获取当前球的半径、坐标和速度
        float ball_r = ball.r;
        float ball_x = ball.x, ball_y = ball.y, ball_v_x = ball.v_x, ball_v_y = ball.v_y;
        //估算一定时间内是否有危险
        float timeUnits = 60;
        float est_x=ball_x+ball_v_x*timeUnits/100,
            est_y=ball_y+ball_v_y*timeUnits/100;
        float est_plane_x=data->plane1.x+data->plane1.v_x*timeUnits/100,
            est_plane_y=data->plane1.y+data->plane1.v_y*timeUnits/100;

        //圆形区域
        float alertDist=500; //警戒距离
        if( (est_x-est_plane_x)*(est_x-est_plane_x) +
                (est_y-est_plane_y)*(est_y-est_plane_y) <
                (alertDist+ball_r)*(alertDist+ball_r) ){
            return true;
        }
    }
    return false;
}

//寻找容易瞄准的球
int acuteAngle_ball(DataStruct* data){
    double angleThreshold = PI/6; //角度容限
    float veloThreshold = 250.0; //速度容限
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

//用方向角判断威胁
//返回值：≥0：迎面飞来的球的下标，-1：有球从其他方向进入警戒距离，-9：没有威胁
int threatened_pro(DataStruct* data){
    //距离检查
    for (int i = 0; i < data->ball_size; i++){ //预测每一个球的移动
        //获取当前球的引用
        BallStruct &ball = data->ball[i];
        //获取当前球的半径、坐标和速度
        float ball_r = ball.r;
        float ball_x = ball.x, ball_y = ball.y, ball_v_x = ball.v_x, ball_v_y = ball.v_y;
        //估算500ms内是否有危险
        float est_x=ball_x+ball_v_x*50/100,
            est_y=ball_y+ball_v_y*50/100;
        float est_plane_x=data->plane1.x+data->plane1.v_x*50/100,
            est_plane_y=data->plane1.y+data->plane1.v_y*50/100;

        //圆形区域
        float alertDist=500; //警戒距离
        if( (est_x-est_plane_x)*(est_x-est_plane_x) +
                (est_y-est_plane_y)*(est_y-est_plane_y) <
                (alertDist+ball_r)*(alertDist+ball_r) ){
            return -1;
        }
    }


    //方向角
    vector<int> vIndexes;
    for (int i = 0; i < data->ball_size; i++){
        float ball_x = data->ball[i].x, ball_y = data->ball[i].y;
        float ball_vx = data->ball[i].v_x, ball_vy = data->ball[i].v_y;
        float pl_x = data->plane1.x, pl_y = data->plane1.y;
        double vec_angle = atan( (pl_y-ball_y)/(pl_x-ball_x) ) +PI/2.0; //rad
        double direc_angle = atan( ball_vy/ball_vx ) +PI/2.0; //rad
        if(abs(vec_angle-direc_angle)<PI/36.0)
            vIndexes.push_back(i); //速度方向与连线夹角5度以内
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


    //无威胁
    return -9;
}

//K-means clustering
BallStruct clusterShoot(DataStruct* data){
    int K=4; //2*2分类
    vector<int> clus[K];
    float clusCenter_x[K], clusCenter_y[K];
    //初始化聚类中心
    clusCenter_x[0]=500.0;clusCenter_x[1]=1500.0;clusCenter_x[2]=500.0;clusCenter_x[3]=1500.0;
    clusCenter_y[0]=375.0;clusCenter_y[1]=375.0;clusCenter_y[2]=1125.0;clusCenter_y[3]=1125.0;

    for(int xx=0;xx<5;xx++){ //迭代次数
        for(int p=0;p<data->ball_size;p++){ //将每个球归入距离最近的聚类
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
        for(int i=0;i<K;i++){ //更新聚类中心坐标
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

    //找出球最多的聚类
    int largestClusIndx=0;
    int max_clus_size=INT_MIN;
    for(int i=0;i<K;i++){
        if(clus[i].size()>max_clus_size){
            largestClusIndx = i;
            max_clus_size = clus[i].size();
        }
    }

    //生成目标
    BallStruct ballCluster;
    ballCluster.x=clusCenter_x[largestClusIndx];
    ballCluster.y=clusCenter_y[largestClusIndx];
    ballCluster.v_x=0;ballCluster.v_y=0;
    ballCluster.r=100;
    //生成带随机偏移量的目标
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

//每20ms调用一次
void ai_pve(DataStruct*data,KeyStruct*key){

    //默认不移动且恒发射子弹
    key->forward = false;
    key->rotate_left = false;
    key->rotate_right = false;
    key->shoot = true;

//    if( data->bullet1_size>0 && threatened(data) ){
//        //遇到来球打开防护盾
//        key->shield=true;
//        //不要逃跑，向它开炮
//        defend(data,key->rotate_left,key->rotate_right);
//        //结束函数执行
//        return;
//    }

    //优化的防守反击算法
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

    //创建静态移动标志
    static int moveFlag = 1, des_x, des_y;

    //根据初始情况决定去哪个角，默认右下角
    static int base_x=1950-data->plane1.r, base_y=1450-data->plane1.r;
    if( data->ball_size==3 && data->ball[0].r==100&&data->ball[1].r==100&&data->ball[2].r==100 ){
        //均值算法
//        int est_avr_x_sum=data->ball[0].x+data->ball[1].x+data->ball[2].x
//                + (data->ball[0].v_x+data->ball[1].v_x+data->ball[2].v_x)*10/100;
//        if(est_avr_x_sum>3*1000)
//            base_x=50+data->plane1.r; //如果多数球在右边，去左下角

        //统计算法
        int LCount=0,RCount=0;
        for(int i=0;i<3;i++){
            if(data->ball[i].x<950) LCount++;
            else RCount++;
        }
        if(LCount<RCount)
            base_x=50+data->plane1.r; //如果多数球在右边，去左下角
    }

    //躲在角落，减少撞击
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

    //标志为0则正在移动或需要移动，仅在非攻击状态移动
    if ( moveFlag==0 && data->bullet1_size==0 ){
        //进行移动并返回移动结果
        moveFlag = moveTo(data, des_x, des_y, key->forward, key->rotate_left, key->rotate_right);
        //返回标志为-1则发生错误
        if (moveFlag == -1)
            std::cout << "移动发生错误" << std::endl;
        //结束函数执行
        return;
    }

    if(data->ball_size){
        //优化目标选择
        int tarIndx = acuteAngle_ball(data);
        AimBall(data, data->ball[tarIndx], key->rotate_left, key->rotate_right);
        std::cout<<"Target: Ball["<<tarIndx<<"].\n";

        //打出必中的一发子弹后转向新的目标
//        static int timeThreshold = 13; //最少26个时间单位，确保飞机能够瞄准90度以内的目标
//        static int timeSlotCount = 0; //是标准时间帧（10ms）数的2倍！
//        static int targetIndex = 0;
//        //预计所需时间（单位：秒）
//        double est_time =
//                AimBall(data, data->ball[targetIndex], key->rotate_left, key->rotate_right);
//        //所需时间长，则等待
//        if(est_time > timeThreshold*0.02)
//            timeThreshold = ceil(est_time/0.02);

//        if(timeSlotCount % timeThreshold == 0){
//            std::cout<<"Target: Ball "<<targetIndex<<", time: "<<timeThreshold*20<<"ms.\n";
//            targetIndex ++ ;
//            targetIndex = targetIndex % data->ball_size; //防止下标越界
//            timeThreshold = 13;
//        }
//        timeSlotCount ++ ;
    }
}

