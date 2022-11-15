#include "core.h"
#include<iostream>

Core::Core( )
{
    //初始化时间片
    slot=0;
    //初始化各个指针为空
    planeOfP1=nullptr;

    bulletObjPool=nullptr;
    ballObjPool=nullptr;
    //默认加速因子为1
    accelerator=1;
    aliveBall.clear();
    gameTime=0;
    offset=0;
    //if判断ai OR keyboard
}
void Core::FromObjToStruct(DataStruct&data,Plane*plane1,Plane*plane2,list<PooledObject*>ball){
     data.id=offset;
    //初始化ball、bullet的长度
    if(!ball.empty())
        data.ball_size=ball.size();
    else
        data.ball_size=0;
    if(nullptr!=plane1)
        data.bullet1_size=plane1->ownBullet.size();
    else
        data.bullet1_size=0;
    if(nullptr!=plane2)
        data.bullet2_size=plane2->ownBullet.size();
    else
        data.bullet2_size=0;
    if(nullptr!=plane1){
        data.plane1.id=1;
        data.plane1.m=plane1->m;
        data.plane1.r=plane1->r;
        data.plane1.x=plane1->x;
        data.plane1.y=plane1->y;
        data.plane1.v_x=plane1->v_x;
        data.plane1.v_y=plane1->v_y;
        data.plane1.angle=plane1->angle;
        data.plane1.a_x=plane1->a_x;
        data.plane1.a_y=plane1->a_y;
    }
    if(nullptr!=plane2){
        data.plane2.id=2;
        data.plane2.m=plane2->m;
        data.plane2.r=plane2->r;
        data.plane2.x=plane2->x;
        data.plane2.y=plane2->y;
        data.plane2.v_x=plane2->v_x;
        data.plane2.v_y=plane2->v_y;
        data.plane2.angle=plane2->angle;
        data.plane2.a_x=plane2->a_x;
        data.plane2.a_y=plane2->a_y;
    }
    //Ball* ba=nullptr;
    list<PooledObject*>::iterator iter;
    PooledObject* po;
    Ball* ba;
    int i=0;
    for(iter=ball.begin();iter!=ball.end();iter++,i++){
        //ba=(Ball*)ball->at(i)->obj;
        //data.ball[i].id=PTR_TO_UINT(ba);
        po = *iter;
        ba=(Ball*)po->obj;
        data.ball[i].x=ba->x;
        data.ball[i].y=ba->y;
        data.ball[i].r=ba->r;
        data.ball[i].m=ba->m;
        data.ball[i].v_x=ba->v_x;
        data.ball[i].v_y=ba->v_y;
        data.ball[i].type=ba->type;
        data.ball[i].hp=ba->hp;
        
    }
	
    i=0;
    Bullet*b1,*b2;
    for(iter=plane1->ownBullet.begin();iter!=plane1->ownBullet.end();i++,iter++){
        //b1=(Bullet*)plane1->ownBullet.at(i)->obj;
        po = *iter;
        b1=(Bullet*)po->obj;
        //data.bullet1[i].id=PTR_TO_UINT(b1);
        data.bullet1[i].m=b1->m;
        data.bullet1[i].r=b1->r;
        data.bullet1[i].x=b1->x;
        data.bullet1[i].y=b1->y;
        data.bullet1[i].v_x=b1->v_x;
        data.bullet1[i].v_y=b1->v_y;
        data.bullet1[i].angle=b1->angle;
        
        /*cout<<"bullet.m:"<<data.bullet1[i].m;
    	cout<<"bullet.r:"<<data.bullet1[i].r;
    	cout<<"bullet.x:"<<data.bullet1[i].x;
    	cout<<"bullet.y:"<<data.bullet1[i].y;
    	cout<<"bullet.v_x:"<<data.bullet1[i].v_x;
    	cout<<"bullet.v_y:"<<data.bullet1[i].v_y;
    	cout<<"bullet.angle:"<<data.bullet1[i].angle;
    	cout<<'\n';*/
    }
    if(nullptr!=plane2){
		for(i=0,iter=plane2->ownBullet.begin();i<data.bullet2_size;i++,iter++){
            //b2=(Bullet*)plane2->ownBullet.at(i)->obj;
            po =*iter;
			b2=(Bullet*)po->obj;
            //data.bullet2[i].id=PTR_TO_UINT(b2);
            data.bullet2[i].m=b2->m;
            data.bullet2[i].r=b2->r;
            data.bullet2[i].x=b2->x;
            data.bullet2[i].y=b2->y;
            data.bullet2[i].v_x=b2->v_x;
            data.bullet2[i].v_y=b2->v_y;
            data.bullet2[i].angle=b2->angle;
        }
    }
    /*cout<<"ball.size: "<<ball.size()<<'\n';
    cout<<"bullet1.size: "<<plane1->ownBullet.size()<<'\n';
    cout<<"bullet2.size: "<<plane2->ownBullet.size()<<'\n';
    cout<<"plane1.m:"<<data.plane1.m;
    cout<<"plane1.r:"<<data.plane1.r;
    cout<<"plane1.x:"<<data.plane1.x;
    cout<<"plane1.y:"<<data.plane1.y;
    cout<<"plane1.v_x:"<<data.plane1.v_x;
    cout<<"plane1.v_y:"<<data.plane1.v_y;
    cout<<"plane1.angle:"<<data.plane1.angle;
    cout<<"plane1.a_x:"<<data.plane1.a_x;
    cout<<"plane1.a_y:"<<data.plane1.a_y<<'\n';*/
}


void Core::setAccelerator(int accelerator)
{
    this->accelerator=accelerator;
}
long Core::getRestTime()
{
    return gameTime - slot * TIME_FRAME / 1000;
}
Core::~Core()
{
    //析构时delete
    if(nullptr!=planeOfP1)delete planeOfP1;
    if(nullptr!=planeOfP2)delete planeOfP2;
    if(!aliveBall.empty()) aliveBall.clear();
    //清空资源池，并释放所有对象

    if(nullptr!=bulletObjPool){
        bulletObjPool->closePool();
        delete bulletObjPool;
    }
    if(nullptr!=ballObjPool){
        ballObjPool->closePool();
        delete ballObjPool;
    }
}

//博弈核心初始化
void Core::initial(GameType type,int ai,bool useAI, time_t gameTime)//bool默认false ,gametime默认30
{
    //记录是否使用AI
    this->useAI = useAI;
    this->ai=ai;
    grade_ai1 = 0;
    grade_ai2 = 0;
    //实例化并创建对象池
    bulletObjPool=new BulletPool();
    bulletObjPool->createPool();
    //实例化并创建对象池
    ballObjPool=new BallPool();
    ballObjPool->createPool();
    Ball* ball=nullptr;

    this->gameTime=gameTime;


    int numOfBalls=3;

    list<int> x,y,v_x,v_y;
    //differentRanList(x,RADIUS_BALL,WIDTH_WIDGET-RADIUS_BALL,numOfBalls);
    //differentRanList(y,RADIUS_BALL,HEIGHT_WIDGET-RADIUS_BALL,numOfBalls);
    differentranList(x,y,RADIUS_BALL,HEIGHT_WIDGET-RADIUS_BALL,numOfBalls);
    differentvelocity(v_x,-INIT_VELOCITY_X_BALL,INIT_VELOCITY_X_BALL,numOfBalls);
    differentvelocity(v_y,-INIT_VELOCITY_X_BALL,INIT_VELOCITY_Y_BALL,numOfBalls);
    //    qDebug()<<"exist correct\n";
    list<int>::iterator ix=x.begin(),iy=y.begin(),iv_x=v_x.begin(),iv_y=v_y.begin();
    for(int i=0;i<numOfBalls;i++,ix++,iy++,iv_x++,iv_y++){
        //初始化生成第一个球
        PooledObject *pooledObject=ballObjPool[0].getFreeObj();
        if(nullptr!=pooledObject){
            ball=(Ball*)pooledObject->obj;
            //            qDebug()<<x.size()<<y.size()<<v_x.size()<<v_y.size();
            ball->init(*ix,*iy,*iv_x,*iv_y,RADIUS_BALL,QUALITY_BALL);
            pooledObject->busy=true;
            aliveBall.push_back(pooledObject);
            //不让球重叠
            sepatareBall();
        }
    }
    //实例化飞机c 
    if(type==PVP||type==PVE){
        planeOfP1=new Plane(WIDTH_WIDGET/2,HEIGHT_WIDGET-RADIUS_PLANE,RADIUS_PLANE,QUALITY_PLANE,0);
        planeOfP2=nullptr;
        if(type==PVP){
            planeOfP2=new Plane(WIDTH_WIDGET/2,0+RADIUS_PLANE,RADIUS_PLANE,QUALITY_PLANE,180);
        }
    }
   while (true)
    {
        update();
        /*if(slot==-2){
            cout<<ai1_name.toStdString()<<" process crashed !!\n";
            emit gameOver(2,grade_ai1,grade_ai2);
            break;
        }
        if(slot==-1){
            cout<<ai1_name.toStdString()<<" process timeout!!\n";
            emit gameOver(1,grade_ai1,grade_ai2);
            break;
        }*/
        if(slot*20>=gameTime*1000){
            //std::cout<<"ai1:"<<grade_ai1<<"ai2:"<<grade_ai2<<'\n';
            FILE *fp;
            fp=fopen("D:\\out.txt","w");
            int c=0;// 0��ƽ�� 1��ai1Ӯ 2��ai2Ӯ 
            if(grade_ai1>grade_ai2)
                c=1;
            if(grade_ai1<grade_ai2)
                c=2;
            fprintf(fp,"%d",c);
            fclose(fp);
            cout<<"ai1:"<<grade_ai1<<" ai2:"<<grade_ai2;
            break;
        }
    }
}
bool  Core::existInList(list<int>lis,int n)
{
	list<int>::iterator it;
    for(it=lis.begin();it!=lis.end();it++){
        //判断是否相同
        if(*it==n)
            return true;
    }
    return false;
}


void Core::differentRanList(list<int>&l,int min,int max,int num)
{
    int b,t=0;
    time_t nowtime;
    time(&nowtime);
    struct tm* p;
    p=localtime(&nowtime);
    srand(p->tm_hour*3600+p->tm_min*60+p->tm_sec);
    for(int i=0;i<num;i++){
        do{
            t=0;
            b=rand()%(max-min)+min;
            for(int j=0;j<=2*RADIUS_BALL;j++){
                if(!existInList(l,b-j))
                    t++;
                if(!existInList(l,b+j))
                    t++;
            }

        }while(t<=1+4*RADIUS_BALL);
        l.push_back(b);
    }
}
void Core::differentranList(list<int>&l,list<int>&m,int min,int max,int num)
{
    int b,c;
    bool t;
    time_t nowtime;
    time(&nowtime);
    struct tm* p;
    p=localtime(&nowtime);
    srand(p->tm_hour*3600+p->tm_min*60+p->tm_sec);
    for(int i=0;i<num;i++){
        do{
            t=true;
            b=rand()%(max-min)+min;
            c=rand()%(max-min)+min;
            list<int>::iterator it,i;
            for(it=l.begin(),i=m.begin();it!=l.end();it++,i++){
                if(sqrt((b-*it)*(b-*it)+(c-*i)*(c-*i))<200||((b<1100&&b>900)&&(c<100||c>1400))){
                    t=false;
                    break;
                }
            }
        }while(t==false);
        l.push_back(b);
        m.push_back(c);
    }
}
void Core::differentvelocity(list<int>&l,int min,int max,int num)
{
    int b;
    time_t nowtime;
    time(&nowtime);
    struct tm* p;
    p=localtime(&nowtime);
    srand(p->tm_hour*3600+p->tm_min*60+p->tm_sec);
    for(int i=0;i<num;i++){
        do{
            b=rand()%(max-min)+min;

        }while(existInList(l,b));
        l.push_back(b);
    }
}

void Core::sepatareBall()
{
    Ball*b1=nullptr;
    Ball*b2=nullptr;
    list<PooledObject*>::iterator it;
    list<PooledObject*>::iterator iter;
    it=aliveBall.begin();
    iter=aliveBall.begin();
	iter++;
	PooledObject* po;
    for(;it!=aliveBall.end()&&iter!=aliveBall.end();it++,iter++){
        //b1=(Ball*)aliveBall.at(i)->obj;
        //b2=(Ball*)aliveBall.at(i+1)->obj;
        po =*it;
        b1=(Ball*)po->obj;
        po=*iter;
        b2=(Ball*)po->obj;
        if(b1->collided(b2)){
            double tmp=b1->v_x;
            b1->v_x=b1->v_y;
            b1->v_y=tmp;
        }
    }
}

void Core::update()
{
    //     qDebug()<<"core::update start\n";
    //每十个时间片更新一次窗口
    for(int i=0;i<accelerator;i++){
        //时间片计数
        slot++;
        //std::cout<<slot<<'\n';
        updateBall();
        //更新飞机
        //        qDebug()<<"updateplane start\n";
        updatePlane();
        //        qDebug()<<"updateplane end\n";
        //碰撞检测
        collideDetect();
        //        w->update();
    }


    //更新画图窗口
    //    qDebug()<<"core::update end\n";

}
void Core::pve_setKeyPressedByAI(){

    //构造data结构体
    DataStruct data;
    KeyStruct key;
    FromObjToStruct(data,planeOfP1,planeOfP2,aliveBall);
    ai_pvp_ai1(&data,&key);
    key.id=offset;
    planeOfP1->keyPressed[FORWARD]=key.forward;
    planeOfP1->keyPressed[ROTATE_LEFT]=key.rotate_left;
    planeOfP1->keyPressed[ROTATE_RIGHT]=key.rotate_right;
    planeOfP1->keyPressed[SHOOT]=key.shoot;
    planeOfP1->keyPressed[SHIELD]=key.shield;
    offset++;
}
void Core::pvp_setplane1KeyPressedByAI(){
    //构造data结构体
    DataStruct data;
    KeyStruct key;
    FromObjToStruct(data,planeOfP1,planeOfP2,aliveBall);
    ai_pvp_ai1(&data,&key);
    key.id=offset;
    planeOfP1->keyPressed[FORWARD]=key.forward;
    planeOfP1->keyPressed[ROTATE_LEFT]=key.rotate_left;
    planeOfP1->keyPressed[ROTATE_RIGHT]=key.rotate_right;
    planeOfP1->keyPressed[SHOOT]=key.shoot;
    planeOfP1->keyPressed[SHIELD]=key.shield;
    offset++;
}
void Core::pvp_setplane2KeyPressedByAI(){//飞机2

    //构造data结构体
    DataStruct data;
    KeyStruct key;
    FromObjToStruct(data,planeOfP1,planeOfP2,aliveBall);
    ai_pvp_ai2(&data,&key);
	key.id=offset;
    planeOfP2->keyPressed[FORWARD]=key.forward;
    planeOfP2->keyPressed[ROTATE_LEFT]=key.rotate_left;
    planeOfP2->keyPressed[ROTATE_RIGHT]=key.rotate_right;
    planeOfP2->keyPressed[SHOOT]=key.shoot;
    planeOfP2->keyPressed[SHIELD]=key.shield;
    offset++;
}
void Core::updatePlane()
{
    //设置每一帧，两个飞机的按键情况
    //setKeyPressedByKeyBoard();
    //if判断ai OR keyboard
    pvp_setplane1KeyPressedByAI();
    pvp_setplane2KeyPressedByAI();
    //复活剩余时间不等于0时，不能执行任何操作
    //P1
    if(planeOfP1->reviveTime==0){
        planeOfP1->move();

        //在刚复活的无敌时间里不能发射子弹和用盾
        if(planeOfP1->invincibleTime==0){

            if(planeOfP1->keyPressed[SHOOT])
                planeOfP1->shoot(slot,bulletObjPool);

            if(planeOfP1->keyPressed[SHIELD])
                planeOfP1->shield(slot);
        }
    }
    planeOfP1->updateBullet(bulletObjPool);
    //更新护盾时间
    planeOfP1->updateShieldTime();
    //更新魔法值
    planeOfP1->updateMp();
    //更新复活时间
    planeOfP1->updateReviveTime();
    //更新无敌时间
    planeOfP1->updateInvincibleTime();



    //P2
    if(nullptr!=planeOfP2){
        if(planeOfP2->reviveTime==0){
            planeOfP2->move();

            //在刚复活的无敌时间里不能发射子弹和用盾
            if(planeOfP2->invincibleTime==0){

                if(planeOfP2->keyPressed[SHOOT])
                    planeOfP2->shoot(slot,bulletObjPool);

                if(planeOfP2->keyPressed[SHIELD])
                    planeOfP2->shield(slot);
            }
        }
        planeOfP2->updateBullet(bulletObjPool);
        //更新护盾时间
        planeOfP2->updateShieldTime();
        //更新魔法值
        planeOfP2->updateMp();
        //更新复活时间
        planeOfP2->updateReviveTime();
        //更新无敌时间
        planeOfP2->updateInvincibleTime();
    }

}
void Core::updateBall()
{
	list<PooledObject*>::iterator it;
	PooledObject* po;
    for(it=aliveBall.begin();it!=aliveBall.end();it++){
        po=*it;
		Ball*fo=(Ball*)po->obj;
        fo->move();
        fo->hitBorder();
    }
}
list<PooledObject*>* Core::getAliveBall(){
    return &(this->aliveBall);
}
void Core::collideDetect()
{

    //plane和ball的检测，可以写成单独的方法
    Ball *ball=new Ball();

    //检索飞机和球是否碰撞
	list<PooledObject*>::iterator it;
	PooledObject* pooledObj=new PooledObject();
    for(it=aliveBall.begin();it!=aliveBall.end();it++){
    	pooledObj=*it;
        ball=(Ball*)pooledObj->obj;
        if(ball->collided(planeOfP1)){
            //飞机未复活or处于无敌状态，无视体积，不发生碰撞
            if(planeOfP1->reviveTime>0||planeOfP1->invincibleTime>0){
                continue;
            }
            //判断飞机是否用盾
            if(planeOfP1->shieldTime<=0){
                //非弹性碰撞，飞机炸毁
                planeOfP1->inelasticCollide(ball);
                planeOfP1->resetState();
                //进入复活倒计时
                planeOfP1->reviveTimeCountDown();

            }else{
                //球和飞机弹性碰撞
                planeOfP1->elasticCollide(ball);
            }

        }
    }
    for(it=aliveBall.begin();it!=aliveBall.end();it++)
    {
        pooledObj=*it;
		ball=(Ball*)pooledObj->obj;
        if(nullptr!=planeOfP2){
            if(ball->collided(planeOfP2)){
                //飞机未复活or处于无敌状态，无视体积，不发生碰撞
                if(planeOfP2->reviveTime>0||planeOfP2->invincibleTime>0){
                    continue;
                }
                //判断飞机是否用盾
                if(planeOfP2->shieldTime<=0){
                    //非弹性碰撞，飞机炸毁
                    planeOfP2->inelasticCollide(ball);
                    planeOfP2->resetState();
                    //进入复活倒计时
                    planeOfP2->reviveTimeCountDown();

                }else{
                    //球和飞机弹性碰撞
                    planeOfP2->elasticCollide(ball);
                }

            }
        }


    }
    //bullet和ball的检测
    Bullet* bullet=new Bullet();

    Ball* left=nullptr;
    Ball* right=nullptr;
    PooledObject* leftChild=nullptr;
    PooledObject* rightChild=nullptr;
    for(it=aliveBall.begin();it!=aliveBall.end();it++){
        pooledObj=*it;

        //当该球已经消失时，跳过对下一个进行搜索
        if(pooledObj->busy==false)
            break;

        ball=(Ball*)pooledObj->obj;
        int initHp=ball->hp;
        int initType=ball->type;
        int termType=-1;

        list<PooledObject*>* oneBullet=&planeOfP1->ownBullet;
        list<PooledObject*>* twoBullet;
        if(nullptr!=planeOfP2){
            twoBullet=&planeOfP2->ownBullet;
        }

    	list<PooledObject*>::iterator j;
        for(j=oneBullet->begin();j!=oneBullet->end();j++){
            pooledObj=*j;
			bullet=(Bullet*)pooledObj->obj;

            if(ball->collided(bullet)){

                //非弹性碰撞，子弹炸毁
                ball->inelasticCollide(bullet);



                termType=ball->type;

                //达到分裂的或者消失的条件
                if(initHp==1||initHp==2||initHp==4||initHp==7){
                    termType--;
                    //球消失or分裂成小球，返回给对象池
                    if(termType!=initType&&termType!=-1){
                        //                        ball->explode();
                        //找到对象池中两个空闲的ball对象
                        leftChild=ballObjPool->getFreeObj();
                        rightChild=ballObjPool->getFreeObj();

                        if(leftChild==nullptr||rightChild==nullptr){
                        }else{
                            left=(Ball*)leftChild->obj;
                            left->leftSplit(ball);
                            right=(Ball*)rightChild->obj;
                            right->rightSplit(ball);
                            aliveBall.push_back(leftChild);
                            aliveBall.push_back(rightChild);
                        }


                    }
                    score(planeOfP1);
                    ballObjPool->returnObj(*it);
                    //aliveBall.removeAt(i);
                    aliveBall.erase(it);
                }else{
                    //子弹击中球，球的hp-1
                    ball->hpMinusOne();
                }
            }
        }
        if(nullptr!=planeOfP2&&nullptr!=twoBullet){
            for(j=twoBullet->begin();j!=twoBullet->end();j++){
                pooledObj=*j;
				bullet=(Bullet*)pooledObj->obj;

                if(ball->collided(bullet)){

                    //非弹性碰撞，子弹炸毁
                    ball->inelasticCollide(bullet);



                    termType=ball->type;

                    //达到分裂的或者消失的条件
                    if(initHp==1||initHp==2||initHp==4||initHp==7){
                        termType--;
                        //球消失or分裂成小球，返回给对象池
                        if(termType!=initType&&termType!=-1){
                            //                        ball->explode();
                            //找到对象池中两个空闲的ball对象
                            leftChild=ballObjPool->getFreeObj();
                            rightChild=ballObjPool->getFreeObj();

                            if(leftChild==nullptr||rightChild==nullptr){
                                //qDebug()<<"找不到两个空闲的球的封装对象\n";
                            }else{
                                left=(Ball*)leftChild->obj;
                                left->leftSplit(ball);
                                right=(Ball*)rightChild->obj;
                                right->rightSplit(ball);
                                aliveBall.push_back(leftChild);
                                aliveBall.push_back(rightChild);
                            }


                        }
                        score(planeOfP2);
                        ballObjPool->returnObj(*it);
                        aliveBall.erase(it);
                    }else{
                        //子弹击中球，球的hp-1
                        ball->hpMinusOne();
                    }
                }
            }
        }

    }
    //ball和ball的检测
    Ball* b1=nullptr;
    Ball* b2=nullptr;
	list<PooledObject*>::iterator jt;
	int i=1;
    for(it=aliveBall.begin();it!=aliveBall.end();i++,it++){
        pooledObj =*it;
		b1=(Ball*)pooledObj->obj;
        jt=aliveBall.begin();
        advance(jt,i);
        for(;jt!=aliveBall.end();jt++){
        	pooledObj=*jt;
            b2=(Ball*)pooledObj->obj;
            if(b1->collided(b2)){
                //弹性碰撞
                b1->elasticCollide(b2);
            }
        }
    }
    //plane和对方bullet的检测
    if(nullptr!=planeOfP2){
        list<PooledObject*>* oneBullet=&planeOfP1->ownBullet;
        list<PooledObject*>* twoBullet=&planeOfP2->ownBullet;
        list<PooledObject*>::iterator iter;
        for(iter=oneBullet->begin();iter!=oneBullet->end();iter++){
        	pooledObj=*iter;
            bullet=(Bullet*)pooledObj->obj;
            if(planeOfP2->collided(bullet)){
                //当飞机还没复活或者处于无敌状态时，无视体积碰撞
                if(planeOfP2->invincibleTime>0||planeOfP2->reviveTime>0){
//                    planeOfP2->elasticCollide(bullet);
                }else{
                    planeOfP2->elasticCollide(bullet);
                    if(planeOfP2->shieldTime==0){
                        planeOfP2->resetState();
                        //进入复活倒计时
                        planeOfP2->reviveTimeCountDown();
                    }

                }
            }
        }
        for(iter=twoBullet->begin();iter!=twoBullet->end();iter++){
        	pooledObj=*iter;
            bullet=(Bullet*)pooledObj->obj;
            if(planeOfP1->collided(bullet)){
                if(planeOfP1->invincibleTime>0||planeOfP1->reviveTime>0){
//                    planeOfP1->elasticCollide(bullet);
                }else{
                    planeOfP1->elasticCollide(bullet);
                    if(planeOfP1->shieldTime==0){
                        planeOfP1->resetState();
                        //进入复活倒计时
                        planeOfP1->reviveTimeCountDown();
                    }

                }
            }
        }
    }

    //双方飞机的碰撞检测
    if(nullptr!=planeOfP2){
        if(planeOfP1->collided(planeOfP2)){
            if(planeOfP1->invincibleTime==0&&planeOfP2->invincibleTime==0&&planeOfP1->reviveTime==0&&planeOfP2->reviveTime==0){
                planeOfP1->elasticCollide(planeOfP2);
                if(planeOfP1->shieldTime==0){
                    planeOfP1->resetState();
                    planeOfP1->reviveTimeCountDown();
                }
                if(planeOfP2->shieldTime==0){
                    planeOfP2->resetState();
                    planeOfP2->reviveTimeCountDown();
                }
            }

        }
    }

}
void Core::score(Plane*plane){
    if(nullptr!=planeOfP1&&plane==planeOfP1){
        grade_ai1++;
    }
    if(nullptr!=planeOfP2&&plane==planeOfP2){
        grade_ai2++;
    }
}
//void Core::split(Ball*leftChild,Ball*rightChild,Ball*parent){
//    double x=parent->getX();
//    double y=parent->getY();
//    double v_x=parent->getV_X();
//    double v_y=parent->getV_Y();
//    double r=parent->getR();
//    double m=parent->getM();
//    int hp=parent->getHP();
//    int type=parent->getType();
//    leftChild->init(x-r,y,v_x*0.5,v_y,r/2,m/4,hp-1,type-1);
//    rightChild->init(x+r,y,v_x*1.5,v_y,r/2,m/4,hp-1,type-1);
//}
