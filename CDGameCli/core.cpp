#include "core.h"

std::function<void(DataStruct*data,KeyStruct*key)> Core::ai1_pve = [](DataStruct*data,KeyStruct*key){};
std::function<void(DataStruct*data,KeyStruct*key)> Core::ai1_pvp = [](DataStruct*data,KeyStruct*key){};

Core::Core( QObject *parent) : QObject(parent), planeOfP2(nullptr), keyPressed(0x0000)
{
    //初始化时间片
    slot=0;
    //初始化各个指针为空
    planeOfP1=nullptr;
    timer=nullptr;
    bulletObjPool=nullptr;
    ballObjPool=nullptr;
    //默认加速因子为1
    accelerator=1;
    aliveBall.clear();
    gameTime=0;
    offset=0;
    //if判断ai OR keyboard
}
void Core::FromObjToStruct(DataStruct&data,Plane*plane1,Plane*plane2,QList<PooledObject*>*ball){
     data.id=offset;
    //初始化ball、bullet的长度
    if(nullptr!=ball)
        data.ball_size=ball->size();
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
    Ball* ba=nullptr;
    for(int i=0;i<data.ball_size;i++){
        ba=(Ball*)ball->at(i)->obj;
        data.ball[i].id=PTR_TO_UINT(ba);
        data.ball[i].x=ba->x;
        data.ball[i].y=ba->y;
        data.ball[i].r=ba->r;
        data.ball[i].m=ba->m;
        data.ball[i].v_x=ba->v_x;
        data.ball[i].v_y=ba->v_y;
        data.ball[i].type=ba->type;
        data.ball[i].hp=ba->hp;
    }

    Bullet*b1=nullptr,*b2=nullptr;
    for(int i=0;i<data.bullet1_size;i++){
        b1=(Bullet*)plane1->ownBullet.at(i)->obj;
        data.bullet1[i].id=PTR_TO_UINT(b1);
        data.bullet1[i].m=b1->m;
        data.bullet1[i].r=b1->r;
        data.bullet1[i].x=b1->x;
        data.bullet1[i].y=b1->y;
        data.bullet1[i].v_x=b1->v_x;
        data.bullet1[i].v_y=b1->v_y;
        data.bullet1[i].angle=b1->angle;
    }
    if(nullptr!=plane2){
        for(int i=0;i<data.bullet2_size;i++){
            b2=(Bullet*)plane2->ownBullet.at(i)->obj;
            data.bullet2[i].id=PTR_TO_UINT(b2);
            data.bullet2[i].m=b2->m;
            data.bullet2[i].r=b2->r;
            data.bullet2[i].x=b2->x;
            data.bullet2[i].y=b2->y;
            data.bullet2[i].v_x=b2->v_x;
            data.bullet2[i].v_y=b2->v_y;
            data.bullet2[i].angle=b2->angle;
        }
    }
}
/*
void Core::packageBall(QJsonObject &dataObj)
{
    //创建球Json数组
    QJsonArray ballArray;
    QJsonObject ballJson;
    //遍历可用球集合
    for (int i = 0; i < aliveBall.size(); i++)
    {
        //记录每个球的id、坐标、半径、类型
        Ball *ball = (Ball *)aliveBall.at(i)->obj;
        ballJson["id"] = PTR_TO_UINT(ball);
        ballJson["x"] = ball->x;
        ballJson["y"] = ball->y;
        ballJson["v_x"] = ball->v_x;
        ballJson["v_y"] = ball->v_y;
        ballJson["r"] = ball->r;
        ballJson["m"] = ball->m;
        ballJson["type"] = ball->type;
        ballJson["hp"]=ball->hp;
        ballArray.append(ballJson);
    }
    dataObj["balls"] = ballArray;
}
void Core::packageBullet(QJsonObject &dataObj)
{
    //创建子弹Json数组
    QJsonArray bulletArray;
    QJsonObject bulletJson;
    //遍历P1子弹集合
    for (int i = 0; i < planeOfP1->ownBullet.size(); i++)
    {
        //记录每个子弹的id、坐标、半径
        Bullet *bullet = (Bullet *)planeOfP1->ownBullet.at(i)->obj;
        bulletJson["id"] = PTR_TO_UINT(bullet);
        bulletJson["own"]=1;
        bulletJson["x"] = bullet->x;
        bulletJson["y"] = bullet->y;
        bulletJson["v_x"] = bullet->v_x;
        bulletJson["v_y"] = bullet->v_y;
        bulletJson["r"] = bullet->r;
        bulletJson["m"] = bullet->m;
        bulletJson["angle"] = bullet->angle;
        bulletArray.append(bulletJson);
    }
    if (planeOfP2)
    {
        //遍历P2子弹集合
        for (int i = 0; i < planeOfP2->ownBullet.size(); i++)
        {
            //记录每个子弹的id、坐标、半径
            Bullet *bullet = (Bullet *)planeOfP2->ownBullet.at(i)->obj;
            bulletJson["id"] = PTR_TO_UINT(bullet);
            bulletJson["own"]=2;
            bulletJson["x"] = bullet->x;
            bulletJson["y"] = bullet->y;
            bulletJson["v_x"] = bullet->v_x;
            bulletJson["v_y"] =bullet->v_y;
            bulletJson["r"] = bullet->r;
            bulletJson["m"] = bullet->m;
            bulletJson["angle"] = bullet->angle;
            bulletArray.append(bulletJson);
        }
    }
    dataObj["bullets"] = bulletArray;
}
void Core::packagePlane(QJsonObject &dataObj)
{
    //创建飞机Json数组
    QJsonArray planeArray;
    //记录飞机的id、坐标、半径、mp、角度、前进
    QJsonObject planeOne;
    planeOne["id"] = 1;
    planeOne["x"]  = planeOfP1->x;
    planeOne["y"]  = planeOfP1->y;
    planeOne["v_x"] = planeOfP1->v_x;
    planeOne["v_y"] = planeOfP1->v_y;
    planeOne["r"]  = planeOfP1->r;
    planeOne["m"] = planeOfP1->m;
    planeOne["mp"]  = planeOfP1->mp;
    planeOne["angle"]  = planeOfP1->angle;
//    planeOne["keyPressed"]  = planeOfP1->keyPressed[FORWARD];
    planeArray.append(planeOne);
    if (planeOfP2)
    {
        QJsonObject planeTwo;
        planeTwo["id"] = 2;
        planeTwo["x"]  = planeOfP2->x;
        planeTwo["y"]  = planeOfP2->y;
        planeTwo["v_x"] = planeOfP1->v_x;
        planeTwo["v_y"] = planeOfP1->v_y;
        planeTwo["r"]  = planeOfP1->r;
        planeTwo["m"] = planeOfP1->m;
        planeTwo["mp"]  = planeOfP2->mp;
        planeTwo["angle"]  = planeOfP2->angle;
//        planeTwo["keyPressed"]  = planeOfP2->keyPressed[FORWARD];
        planeArray.append(planeTwo);
    }
    dataObj["planes"] = planeArray;
}
*/

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
    if(nullptr!=timer)delete timer;
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
void Core::initial(GameType type,bool useAI, time_t gameTime)
{
    //连接核心信号
    linkCore(type);
    //记录是否使用AI
    this->useAI = useAI;
    grade_ai1 = 0;
    grade_ai2 = 0;
    //实例化并创建对象池
    bulletObjPool=new BulletPool(this);
    bulletObjPool->createPool();
    //实例化并创建对象池
    ballObjPool=new BallPool(this);
    ballObjPool->createPool();
    Ball* ball=nullptr;

    this->gameTime=gameTime;


    int numOfBalls=3;

    QList<int> x,y,v_x,v_y;
    differentRanList(x,WIDTH_WIDGET,numOfBalls);
    differentRanList(y,HEIGHT_WIDGET,numOfBalls);
    differentRanList(v_x,INIT_VELOCITY_X_BALL,numOfBalls);
    differentRanList(v_y,INIT_VELOCITY_Y_BALL,numOfBalls);
    //    qDebug()<<"exist correct\n";

    for(int i=0;i<numOfBalls;i++){
        //初始化生成第一个球
        PooledObject *pooledObject=ballObjPool[0].getFreeObj();
        if(nullptr!=pooledObject){
            ball=(Ball*)pooledObject->obj;
            //            qDebug()<<x.size()<<y.size()<<v_x.size()<<v_y.size();
            ball->init(x.at(i),y.at(i),v_x.at(i),v_y.at(i),RADIUS_BALL,QUALITY_BALL);
            pooledObject->busy=true;
            aliveBall.append(pooledObject);
            //不让球重叠
            sepatareBall();
            newBall(ball);
        }
    }
    //实例化飞机
    if(type==PVP||type==PVE){
        planeOfP1=new Plane(WIDTH_WIDGET/2,HEIGHT_WIDGET-RADIUS_PLANE,RADIUS_PLANE,QUALITY_PLANE,0,this);
        newPlane(planeOfP1);
        planeOfP2=nullptr;
        if(type==PVP){
            planeOfP2=new Plane(WIDTH_WIDGET/2,0+RADIUS_PLANE,RADIUS_PLANE,QUALITY_PLANE,180,this);
            newPlane(planeOfP2);
        }
    }else{
        qDebug()<<"wrong game type!!!\n";
    }

    //    //初始化核心的时钟
    //    clock=new QTime();
    //    clock->start();

    //初始化画图窗口
    //    w = new GLWidget(this);
    //    w->show();



    //初始化定时器
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Core::update);
//    timer->start(TIME_FRAME);
    timer->start(TIME_FRAME);
}
bool  Core::existInList(QList<int>list,int n)
{
    for(int i=0;i<list.size();i++){
        //判断是否相同
        if(list.at(i)==n)
            return true;
    }
    return false;
}


void Core::differentRanList(QList<int>&l,int range,int num)
{
    int b;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(int i=0;i<num;i++){
        do{
            b=qrand()%range;

        }while(existInList(l,b));
        l.append(b);
    }
}
void Core::sepatareBall()
{
    Ball*b1=nullptr;
    Ball*b2=nullptr;
    for(int i=0;i<aliveBall.size()-1;i++){
        b1=(Ball*)aliveBall.at(i)->obj;
        b2=(Ball*)aliveBall.at(i+1)->obj;
        if(b1->collided(b2)){
            double tmp=b1->v_x;
            b1->v_x=b1->v_y;
            b1->v_y=tmp;
        }
    }
}
void Core::setKeyState(KeyPressed key, bool state)
{
    keyPressed.setFlag(key, state);
}

void Core::update()
{
    //     qDebug()<<"core::update start\n";
    //每十个时间片更新一次窗口
    for(int i=0;i<accelerator;i++){
        //时间片计数
        slot++;
        //碰撞检测
        collideDetect();
        //更新飞机
        //        qDebug()<<"updateplane start\n";
        updatePlane();
        //        qDebug()<<"updateplane end\n";
        updateBall();
        //        w->update();
    }


    if (slot*TIME_FRAME>=gameTime*1000){
        //停止计时
        timer->stop();
        gameOver();
    }
    else if (slot % RECORDING_FREQUENCY == 0) {
        //记录该帧对象信息
        recordFrame();
    }
    //更新画图窗口
    //    qDebug()<<"core::update end\n";

}
void Core::setKeyPressedByKeyBoard()
{
    planeOfP1->keyPressed[FORWARD]=keyPressed.testFlag(P1_FORWARD);
    planeOfP1->keyPressed[ROTATE_LEFT]=keyPressed.testFlag(P1_ROTATE_LEFT);
    planeOfP1->keyPressed[ROTATE_RIGHT]=keyPressed.testFlag(P1_ROTATE_RIGHT);
    planeOfP1->keyPressed[SHOOT]=keyPressed.testFlag(P1_SHOOT);
    planeOfP1->keyPressed[SHIELD]=keyPressed.testFlag(P1_SHIELD);

    if(nullptr!=planeOfP2){
        planeOfP2->keyPressed[FORWARD]=keyPressed.testFlag(P2_FORWARD);
        planeOfP2->keyPressed[ROTATE_LEFT]=keyPressed.testFlag(P2_ROTATE_LEFT);
        planeOfP2->keyPressed[ROTATE_RIGHT]=keyPressed.testFlag(P2_ROTATE_RIGHT);
        planeOfP2->keyPressed[SHOOT]=keyPressed.testFlag(P2_SHOOT);
        planeOfP2->keyPressed[SHIELD]=keyPressed.testFlag(P2_SHIELD);
    }
}
void Core::setKeyPressedByAI(){

    //构造data结构体
    DataStruct data;
    KeyStruct key;
    FromObjToStruct(data,planeOfP1,planeOfP2,&aliveBall);
    ai1_pve(&data,&key);
    key.id=offset;
    planeOfP1->keyPressed[FORWARD]=key.forward;
    planeOfP1->keyPressed[ROTATE_LEFT]=key.rotate_left;
    planeOfP1->keyPressed[ROTATE_RIGHT]=key.rotate_right;
    planeOfP1->keyPressed[SHOOT]=key.shoot;
    planeOfP1->keyPressed[SHIELD]=key.shield;
    offset++;
}
void Core::updatePlane()
{
    //设置每一帧，两个飞机的按键情况
    setKeyPressedByKeyBoard();
    //if判断ai OR keyboard
    if (useAI)
    {
        setKeyPressedByAI();
    }
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
    for(int i=0;i<aliveBall.size();i++){
        Ball*fo=(Ball*)aliveBall.at(i)->obj;
        fo->move();
        fo->hitBorder();
    }
}
QList<PooledObject*>* Core::getAliveBall(){
    return &(this->aliveBall);
}
void Core::collideDetect()
{

    //plane和ball的检测，可以写成单独的方法
    Ball *ball=new Ball(this);

    PooledObject* pooledObj=new PooledObject(this);
    //检索飞机和球是否碰撞
    for(int i=0;i<aliveBall.size();i++){
        pooledObj=aliveBall.at(i);
        ball=(Ball*)pooledObj->obj;
        //发射飞机爆炸explode信号
        if(ball->collided(planeOfP1)){
            //飞机未复活or处于无敌状态，无视体积，不发生碰撞
            if(planeOfP1->reviveTime>0||planeOfP1->invincibleTime>0){
                continue;
            }
            //判断飞机是否用盾
            if(planeOfP1->shieldTime<=0){
                //非弹性碰撞，飞机炸毁
                planeOfP1->inelasticCollide(ball);
                planeOfP1->explode();
                planeOfP1->resetState();
                //进入复活倒计时
                planeOfP1->reviveTimeCountDown();

            }else{
                //球和飞机弹性碰撞
                planeOfP1->elasticCollide(ball);
            }

        }
    }
    for(int i=0;i<aliveBall.size();i++)
    {
        pooledObj=aliveBall.at(i);
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
                    planeOfP2->explode();
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
    Bullet* bullet=new Bullet(this);

    Ball* left=nullptr;
    Ball* right=nullptr;
    PooledObject* leftChild=nullptr;
    PooledObject* rightChild=nullptr;
    for(int i=0;i<aliveBall.size();i++){
        pooledObj=aliveBall.at(i);

        //当该球已经消失时，跳过对下一个进行搜索
        if(pooledObj->busy==false)
            break;

        ball=(Ball*)pooledObj->obj;
        int initHp=ball->hp;
        int initType=ball->type;
        int termType=-1;

        QList<PooledObject*>* oneBullet=&planeOfP1->ownBullet;
        QList<PooledObject*>* twoBullet=nullptr;
        if(nullptr!=planeOfP2){
            twoBullet=&planeOfP2->ownBullet;
        }


        for(int j=0;j<oneBullet->size();j++){
            bullet=(Bullet*)oneBullet->at(j)->obj;

            if(ball->collided(bullet)){

                //非弹性碰撞，子弹炸毁
                ball->inelasticCollide(bullet);
                bullet->explode();



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
                            qDebug()<<"找不到两个空闲的球的封装对象\n";
                        }else{
                            left=(Ball*)leftChild->obj;
                            left->leftSplit(ball);
                            newBall(left);
                            right=(Ball*)rightChild->obj;
                            right->rightSplit(ball);
                            newBall(right);
                            aliveBall.append(leftChild);
                            aliveBall.append(rightChild);
                        }


                    }
                    score(planeOfP1);
                    ballObjPool->returnObj(aliveBall.at(i));
                    aliveBall.removeAt(i);
                    ball->explode();
                }else{
                    //子弹击中球，球的hp-1
                    ball->hpMinusOne();
                }
            }
        }
        if(nullptr!=planeOfP2&&nullptr!=twoBullet){
            for(int j=0;j<twoBullet->size();j++){
                bullet=(Bullet*)twoBullet->at(j)->obj;

                if(ball->collided(bullet)){

                    //非弹性碰撞，子弹炸毁
                    ball->inelasticCollide(bullet);
                    bullet->explode();



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
                                qDebug()<<"找不到两个空闲的球的封装对象\n";
                            }else{
                                left=(Ball*)leftChild->obj;
                                left->leftSplit(ball);
                                newBall(left);
                                right=(Ball*)rightChild->obj;
                                right->rightSplit(ball);
                                newBall(right);
                                aliveBall.append(leftChild);
                                aliveBall.append(rightChild);
                            }


                        }
                        score(planeOfP2);
                        ballObjPool->returnObj(aliveBall.at(i));
                        aliveBall.removeAt(i);
                        ball->explode();
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

    for(int i=0;i<aliveBall.size();i++){
        b1=(Ball*)aliveBall.at(i)->obj;
        for(int j=i+1;j<aliveBall.size();j++){
            b2=(Ball*)aliveBall.at(j)->obj;
            if(b1->collided(b2)){
                //弹性碰撞
                b1->elasticCollide(b2);
            }
        }
    }
    //plane和对方bullet的检测
    if(nullptr!=planeOfP2){
        QList<PooledObject*>* oneBullet=&planeOfP1->ownBullet;
        QList<PooledObject*>* twoBullet=&planeOfP2->ownBullet;
        for(int i=0;i<oneBullet->size();i++){
            bullet=(Bullet*)oneBullet->at(i)->obj;
            if(planeOfP2->collided(bullet)){
                //当飞机还没复活或者处于无敌状态时，无视体积碰撞
                if(planeOfP2->invincibleTime>0||planeOfP2->reviveTime>0){
//                    planeOfP2->elasticCollide(bullet);
                }else{
                    planeOfP2->elasticCollide(bullet);
                    if(planeOfP2->shieldTime==0){
                        planeOfP2->explode();
                        planeOfP2->resetState();
                        //进入复活倒计时
                        planeOfP2->reviveTimeCountDown();
                    }

                    bullet->explode();
                }
            }
        }
        for(int i=0;i<twoBullet->size();i++){
            bullet=(Bullet*)twoBullet->at(i)->obj;
            if(planeOfP1->collided(bullet)){
                if(planeOfP1->invincibleTime>0||planeOfP1->reviveTime>0){
//                    planeOfP1->elasticCollide(bullet);
                }else{
                    planeOfP1->elasticCollide(bullet);
                    if(planeOfP1->shieldTime==0){
                        planeOfP1->explode();
                        planeOfP1->resetState();
                        //进入复活倒计时
                        planeOfP1->reviveTimeCountDown();
                    }

                    bullet->explode();
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
                    planeOfP1->explode();
                    planeOfP1->resetState();
                    planeOfP1->reviveTimeCountDown();
                }
                if(planeOfP2->shieldTime==0){
                    planeOfP2->explode();
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
        scoreChange(plane,grade_ai1);
    }
    if(nullptr!=planeOfP2&&plane==planeOfP2){
        grade_ai2++;
        scoreChange(plane,grade_ai2);
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
