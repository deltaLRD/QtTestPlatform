#include "plane.h"

Plane::Plane(double x,double y,double r,double m,double angle,QObject *parent) : FlyObject(parent)
{
    //初始化坐标、质量、半径、角度，默认初速度为0
    this->x=x;
    this->y=y;
    this->r=r;
    this->m=m;
    this->angle=angle;
    this->v_x=0;
    this->v_y=0;
    this->a_x=0;
    this->a_y=0;
    //初始化盾剩余时间、无敌剩余时间、复活剩余时间为0
    shieldTime=0;
    invincibleTime=0;
    reviveTime=0;
    //让飞机一开始就可以发射子弹和使用护盾
    this->mp=MAX_MP_PLANE;
    this->lastShoot=(-1)*INTERVAL_SHOOT*1000;
    this->lastShield=(-1)*INTERVAL_SHIELD*1000;

    //初始化按键数组，按下释放时间数组，在每一帧中实际的起止时间的数组
    for(int i=0;i<KEY_SUM;i++)
    {
        keyPressed[i]=false;
    }
    //初始化子弹列表
    ownBullet.clear();
}

/*
 *按照系统核心进程时间来更新
 *
void Plane::updateKeyPress(time_t lastFetch,time_t fetch)
{
    for(int i=0;i<KEY_SUM;i++)
    {
        if(i==6||i==7){
            if(pressTime[i]>0){
                keyPressed[i]=true;
                continue;
            }
        }
        //判断是否有key按下
        if(pressTime[i]>0&&releseTime[i]>0){
            keyPressed[i]=true;
            pressUnitlNextFrame[i]=false;
            durBegin[i]=pressTime[i];
            durEnd[i]=releseTime[i];
            continue;
        }
        if(pressTime[i]>0&&releseTime[i]==0){
            keyPressed[i]=true;
            pressUnitlNextFrame[i]=true;
            durBegin[i]=pressTime[i];
            durEnd[i]=fetch;
            continue;
        }
        if(keyPressed[i]){
            if(releseTime[i]==0){
//                keyPressed[i]=true;
                pressUnitlNextFrame[i]=true;
                durBegin[i]=lastFetch;
                durEnd[i]=fetch;
                continue;
            }else{
//                keyPressed[i]=true;
                pressUnitlNextFrame[i]=false;
                durBegin[i]=lastFetch;
                durEnd[i]=releseTime[i];
            }
        }

    }
}
*/

/*
void Plane::move(time_t lastFetch,time_t fetch)
{
    //飞机在xy方向的速度位移正负量，角度的正负量
    double v_x_plus=0;
    double v_x_minus=0;
    double x_plus=0;
    double x_minus=0;
    double v_y_plus=0;
    double v_y_minus=0;
    double y_plus=0;
    double y_minus=0;
    double v_y_slowDown=0;
    double y_slownDown=0;
    double angle_plus=0;
    double angle_minus=0;

    if(true==keyPressed[0]&&durEnd[0]>=durBegin[0]&&durBegin[0]>=0)
    {

        v_y_minus=(-1)*ACCELERATE_Y*(durEnd[0]-durBegin[0])/SHRINK;
        y_minus=v_y*(durBegin[0]-lastFetch)/SHRINK+((v_y+v_y+v_y_minus)/2*(durEnd[0]-durBegin[0])/SHRINK)+(v_y+v_y_minus)*(fetch-durEnd[0])/SHRINK;

        qDebug()<<keyPressed[0]<<1<<v_y_minus<<y_minus<<endl;
    }
    if(true==keyPressed[1]&&durEnd[1]>=durBegin[1]&&durBegin[1]>=0)
    {

        v_y_plus=ACCELERATE_Y*(durEnd[1]-durBegin[1])/SHRINK;
        y_plus=v_y*(durBegin[1]-lastFetch)/SHRINK+((v_y+v_y+v_y_plus)/2*(durEnd[1]-durBegin[1])/SHRINK)+(v_y+v_y_plus)*(fetch-durEnd[1])/SHRINK;
        qDebug()<<3<<v_y_plus<<y_plus<<endl;
    }
    if(true==keyPressed[2]&&durEnd[2]>=durBegin[2]&&durBegin[2]>=0)
    {

        v_x_minus=(-1)*ACCELERATE_X*(durEnd[2]-durBegin[2])/SHRINK;
        x_minus=v_x*(durBegin[2]-lastFetch)/SHRINK+((v_x+v_x+v_x_minus)/2*(durEnd[2]-durBegin[2])/SHRINK)+(v_x+v_x_minus)*(fetch-durEnd[2])/SHRINK;
        qDebug()<<5<<v_x_minus<<x_minus<<endl;
    }
    if(true==keyPressed[3]&&durEnd[3]>=durBegin[3]&&durBegin[3]>=0)
    {

        v_x_plus=ACCELERATE_X*(durEnd[3]-durBegin[3])/SHRINK;
        x_plus=v_x*(durBegin[3]-lastFetch)/SHRINK+((v_x+v_x+v_x_plus)/2*(durEnd[3]-durBegin[3])/SHRINK)+(v_x+v_x_plus)*(fetch-durEnd[3])/SHRINK;
        qDebug()<<7<<v_x_plus<<x_plus<<endl;
    }
    if(!keyPressed[0]&&!keyPressed[1]&&v_y!=0){
        //没有加速度时，考虑阻尼
        int derection=v_y>0?(-1):1;
        double Fd_y=0.5*(m/(3.1415*4/3*r*r*r))*(v_y*v_y);
        double a_fd=Fd_y/m*derection;
        v_y_slowDown=v_y+a_fd*(fetch-lastFetch);
        y_slownDown=(v_y_slowDown+v_y)/2*(fetch-lastFetch);
    }
    v_x+=v_x_plus+v_x_minus;
    v_y+=v_y_plus+v_y_minus+v_y_slowDown;
    x+=x_plus+x_minus;
    y+=y_plus+y_minus+y_slownDown;
    qDebug()<<"xy速度 "<<v_x<<" "<<v_y<<endl;

    //判断最大飞行速度
    if(v_x>=MAX_VELOCITY_X)
    {
        v_x=MAX_VELOCITY_X;
    }
    if(v_x<=-MAX_VELOCITY_X)
    {
        v_x=-MAX_VELOCITY_X;
    }
    if(v_y>=MAX_VELOCITY_Y)
    {
        v_y=MAX_VELOCITY_Y;
    }
    if(v_y<=-MAX_VELOCITY_Y)
    {
        v_y=-MAX_VELOCITY_Y;
    }
    //判断边界,碰到边界后停下,对应速度为0
    if(x<=0)
    {
        x=0;
        v_x=-v_x;
    }
    if(x>=WIDTH_WIDGET)
    {
        x=WIDTH_WIDGET;
        v_x=-v_x;
    }
    if(y<=0)
    {
        y=0;
        v_y=-v_y;
    }
    if(y>=HEIGHT_WIDGET)
    {
        y=HEIGHT_WIDGET;
        v_y=-v_y;
    }


    //检测转角
    if(keyPressed[4])
    {
        angle_plus=VELOCITY_ANGLE*(durEnd[4]-durBegin[4])/SHRINK;
    }
    if(keyPressed[5])
    {
        angle_minus=(-1)*VELOCITY_ANGLE*(durEnd[5]-durBegin[5])/SHRINK;
    }
    angle+=angle_plus+angle_minus;
}
*/
/*
void Plane::resetKeyPressed()
{

    for(int i=KEY_MOVE_SUM;i<KEY_SUM;i++)
    {
        keyPressed[i]=false;
    }
    return ;


}
*/
/*
 *判断按键长时间按下需要更新的数组
 *
void Plane::resetPressAndReleseTime()
{
    for(int i=0;i<KEY_SUM;i++){
        keyPressed[i]=pressUnitlNextFrame[i];
        pressTime[i]=0;
        releseTime[i]=0;
    }
}
*/
void Plane::move()
{

    //    qDebug()<<keyPressed[FORWARD]<<keyPressed[BACKWORD]<<keyPressed[LEFT]<<keyPressed[RIGHT]<<endl;
    //先转动角度，再前进
    double t=1.0*TIME_FRAME/1000;

    if(keyPressed[ROTATE_LEFT]){
        angle+=VELOCITY_ANGLE*t;
    }
    if(keyPressed[ROTATE_RIGHT]){
        angle-=VELOCITY_ANGLE*t;
    }

    double cosAngle=cos(angle*PI/180);
    double sinAngle=sin(angle*PI/180);
    if(keyPressed[FORWARD]){
        a_y-=cosAngle*ACCELERATE_Y;
        a_x-=sinAngle*ACCELERATE_Y;
    }
    /*
    if(keyPressed[BACKWORD]){
        a_y+=cosAngle*ACCELERATE_Y*t;
        a_x+=sinAngle*ACCELERATE_Y*t;

    }
    if(keyPressed[LEFT]){
        a_y+=sinAngle*ACCELERATE_X*t;
        a_x-=cosAngle*ACCELERATE_X*t;
    }
    if(keyPressed[RIGHT]){
        a_y-=sinAngle*ACCELERATE_X*t;
        a_x+=cosAngle*ACCELERATE_X*t;
    }*/

    /*
     * 减速方案一：xy方向有各自的阻力不符合实际，而且影响飞机运动
    if(!keyPressed[FORWARD]&&!keyPressed[BACKWORD]&&v_y*v_x>0){
        if(v_y>0){
            a_y-=cosAngle*ACCELERATE_SLOWDOWN*t;
            a_x-=sinAngle*ACCELERATE_SLOWDOWN*t;
            qDebug()<<1<<endl;
        }else{
            a_y+=cosAngle*ACCELERATE_SLOWDOWN*t;
            a_x+=sinAngle*ACCELERATE_SLOWDOWN*t;
            qDebug()<<2<<endl;
        }
    }
    if(!keyPressed[LEFT]&&!keyPressed[RIGHT]&&v_x*v_y<0){
        if(v_x>0){
            a_y+=sinAngle*ACCELERATE_SLOWDOWN*t;
            a_x-=cosAngle*ACCELERATE_SLOWDOWN*t;
            qDebug()<<3<<endl;
        }else{
            a_y-=sinAngle*ACCELERATE_SLOWDOWN*t;
            a_x+=cosAngle*ACCELERATE_SLOWDOWN*t;
            qDebug()<<4<<endl;
        }
    }
    */
    //减速方案二：由于阻力和飞行速度的平方成正比容易“漂移”，所以改为固定阻力加在合速度的方向上
    //"-"与原来速度i方向相反
    double a_x_slowndown=0;
    double a_y_slowndown=0;
    if(v_y!=0||v_x!=0){
        a_x_slowndown=-v_x/sqrt(v_x*v_x+v_y*v_y)*ACCELERATE_SLOWDOWN;
        a_y_slowndown=-v_y/sqrt(v_x*v_x+v_y*v_y)*ACCELERATE_SLOWDOWN;
    }
    a_x+=a_x_slowndown;
    a_y+=a_y_slowndown;

    //x=0.5*(v0+v')*t
    x+=v_x*t*0.5;
    y+=v_y*t*0.5;
    int tmp_x = v_x, tmp_y = v_y;
    if((v_x+a_x*t)>=0){
        v_x=(v_x+a_x*t)<=MAX_VELOCITY_X?(v_x+a_x*t):MAX_VELOCITY_X;
    }else{
        v_x=(v_x+a_x*t)>=-MAX_VELOCITY_X?(v_x+a_x*t):-MAX_VELOCITY_X;
    }
    if((v_y+a_y*t)>0){
        v_y=(v_y+a_y*t)<=MAX_VELOCITY_Y?(v_y+a_y*t):MAX_VELOCITY_Y;
    }else{
        v_y=(v_y+a_y*t)>=-MAX_VELOCITY_Y?(v_y+a_y*t):-MAX_VELOCITY_Y;
    }
    if(tmp_x * v_x < 0)
        v_x = 0;
    if(tmp_y * v_y < 0)
        v_y=0;
    x+=v_x*t*0.5;
    y+=v_y*t*0.5;
    //加速度变为0，方便下一帧计算总的加速度
    a_x=0;
    a_y=0;

    hitBorder();
//    postionChanged(x, y);
//    angleChanged(angle);
}
void Plane::hitBorder()
{
    if(v_x<0&&x<=r){
        x=r;
        v_x=0;
    }
    if(v_x>0&&x+r>=WIDTH_WIDGET){
        x=WIDTH_WIDGET-r;
        v_x=0;
    }

    if(v_y<0&&y<=r){
        y=r;
        v_y=0;
    }
    if(v_y>0&&y+r>=HEIGHT_WIDGET){
        y=HEIGHT_WIDGET-r;
        v_y=0;
    }

}
void Plane::shoot(time_t currentFrame,ObjectPool* bulletObjectPool)
{
    double  interval=(currentFrame*1.0*TIME_FRAME-lastShoot)/1000;
    if(interval>=INTERVAL_SHOOT)
    {
        //从对象池获得一个空闲的对象
        PooledObject* poolObject=bulletObjectPool->getFreeObj();
        if(nullptr!=poolObject)
        {
            //获取里面的子弹对象的指针
            Bullet* bullet=(Bullet*)poolObject->obj;
            //子弹的速度等自身的初速度加上飞机当前的速度
            double b_v_x=-sin(angle*PI/180)*INIT_VELOCITY_BULLET+v_x;
            double b_v_y=-cos(angle*PI/180)*INIT_VELOCITY_BULLET+v_y;
            //根据飞机设置子弹的初速度和坐标
//            bullet->init(x,y,b_v_x,b_v_y,angle,RADIUS_BULLET,QUALITY_BULLET);
            bullet->init(x - r * sin(angle * PI /180),y - r * cos(angle * PI /180),
                         b_v_x,b_v_y,angle,RADIUS_BULLET,QUALITY_BULLET);
            //将对象状态设置为busy
            poolObject->busy=true;
            //把对象加到飞机的子弹列表中
            ownBullet.append(poolObject);
            //记录shoot时间
            lastShoot=currentFrame*TIME_FRAME;

            shootBullet(bullet);
//            bullet->angleChanged(angle);
        }

    }

}
void Plane::updateBullet(ObjectPool* bulletObjectPool)
{
    double t=1.0*TIME_FRAME/1000;
    for(int i=0;i<ownBullet.size();i++){
        FlyObject*fo=ownBullet.at(i)->obj;
        double x=fo->x+fo->v_x*t;
        double y=fo->y+fo->v_y*t;
        if(x>0&&x<WIDTH_WIDGET&&y>0&&y<HEIGHT_WIDGET){
            fo->x=x;
            fo->y=y;
//            fo->postionChanged(x ,y);
        }else{
            //还给对象池，不再使用，busy=false
            bulletObjectPool->returnObj(ownBullet.at(i));
            //将子弹从list中移除，不再绘制
            ownBullet.removeAt(i);
            fo->disappear();
        }
    }
}
void Plane::shield(time_t currentFrame)
{
    double  interval=(currentFrame*1.0*TIME_FRAME-lastShield)/1000;
    //    qDebug()<<"MP:"<<mp<<" 护盾持续时间："<<shieldTime<<endl;
    if(interval>=INTERVAL_SHIELD&&mp==MAX_MP_PLANE)
    {
        //魔法值用光，设为0
        mp=0;
        //盾持续时间为SPAN_SHIELD
        shieldTime=SPAN_SHIELD*1000;
        lastShield=currentFrame*TIME_FRAME;

        shieldUp();
    }
}
void Plane::updateShieldTime()
{
    if(shieldTime>0){
        shieldTime-=TIME_FRAME;
        if(shieldTime<=0){
            shieldTime=0;
            shieldDown();
        }
    }
}
void Plane::updateMp(){
    if(mp<MAX_MP_PLANE)
        mp++;
}
//复活时间
void Plane::reviveTimeCountDown(){
    reviveTime=SPAN_REVIVE*1000;
}
void Plane::updateReviveTime(){
    //    qDebug() << reviveTime;
    if(reviveTime>0){
        reviveTime-=TIME_FRAME;
        if(reviveTime<=0){
            reviveTime=0;
            //进入无敌状态
            invincibleUp();
            //无敌时间开始倒计时
            invincibleTimeCountDown();
        }
    }
}
//无敌时间
void Plane::invincibleTimeCountDown(){
    invincibleTime=SPAN_INVINCIBLE*1000;
}
void Plane::updateInvincibleTime(){
    if(invincibleTime>0){
        invincibleTime-=TIME_FRAME;
        if(invincibleTime<=0){
            //无敌时间结束
            invincibleTime=0;
            invincibleDown();
        }
    }

}
void Plane::resetState(){
//    this->angle=0;
    this->v_x=0;
    this->v_y=0;
    this->a_x=0;
    this->a_y=0;

//    this->mp=MAX_MP_PLANE;

    //初始化按键数组，按下释放时间数组，在每一帧中实际的起止时间的数组
    for(int i=0;i<KEY_SUM;i++)
    {
        keyPressed[i]=false;
    }
}
double Plane::getMp()
{
    return this->mp;
}
double Plane::getAngle()
{
    return this->angle;
}
bool Plane::getKeyPressed(int i)
{
    return this->keyPressed[i];
}
/*
void Plane::setPressTime(int i,time_t pressTime)
{
    this->pressTime[i]=pressTime;
}
void Plane::setReleseTime(int i,time_t releseTime)
{
    this->releseTime[i]=releseTime;
}

void Plane::setMP(double mp)
{
    this->mp=mp;
}
void Plane::setShiledTime(double shieldTime)
{
    this->shieldTime=shieldTime;
}
void Plane::setInvincibleTime(double invincibleTime)
{
    this->invincibleTime=invincibleTime;
}
void Plane::setReviveTime(double reviveTime)
{
    this->reviveTime=reviveTime;
}
void Plane::setAngle(double angle)
{
    this->angle=angle;
}
void Plane::setA_X(double a_x)
{
    this->a_x=a_x;
}
void Plane::setA_Y(double a_y)
{
    this->a_y=a_y;
}
void Plane::setKeyPressed(int i,bool hasPressed)
{
    this->keyPressed[i]=hasPressed;
}
double Plane::getShieldTime()
{
    return this->shieldTime;
}
double Plane::getReviveTime()
{
    return this->reviveTime;
}
QList<PooledObject*>* Plane::getOwnBullet()
{
    return &(this->ownBullet);
}
time_t Plane::getPressTime(int i)
{
    return this->pressTime[i];
}
time_t Plane::getReleseTime(int i)
{
    return this->releseTime[i];
}
*/
/*
double Plane::getA_X()
{
    return this->a_x;
}
double Plane::getA_Y()
{
    return this->a_y;
}
*/
