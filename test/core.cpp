#include "core.h"
#include <cstdlib>
#include <iostream>

Core::Core( QObject *parent) : QObject(parent), planeOfP2(nullptr), keyPressed(0x0000)
{
    //鍒濆鍖栨椂闂寸墖
    slot=0;
    //鍒濆鍖栧悇涓寚閽堜负绌�
    w=nullptr;
    planeOfP1=nullptr;
    timer=nullptr;
    bulletObjPool=nullptr;
    ballObjPool=nullptr;
    //榛樿鍔犻�熷洜瀛愪负1
    accelerator=1;
    aliveBall.clear();
    gameTime=0;
    offset=0;
    //if鍒ゆ柇ai OR keyboard
}
void Core::FromObjToStruct(DataStruct&data,Plane*plane1,Plane*plane2,QList<PooledObject*>*ball){
     data.id=offset;
    //鍒濆鍖朾all銆乥ullet鐨勯暱搴�
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
    //鍒涘缓鐞僇son鏁扮粍
    QJsonArray ballArray;
    QJsonObject ballJson;
    //閬嶅巻鍙敤鐞冮泦鍚�
    for (int i = 0; i < aliveBall.size(); i++)
    {
        //璁板綍姣忎釜鐞冪殑id銆佸潗鏍囥�佸崐寰勩�佺被鍨�
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
    //鍒涘缓瀛愬脊Json鏁扮粍
    QJsonArray bulletArray;
    QJsonObject bulletJson;
    //閬嶅巻P1瀛愬脊闆嗗悎
    for (int i = 0; i < planeOfP1->ownBullet.size(); i++)
    {
        //璁板綍姣忎釜瀛愬脊鐨刬d銆佸潗鏍囥�佸崐寰�
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
        //閬嶅巻P2瀛愬脊闆嗗悎
        for (int i = 0; i < planeOfP2->ownBullet.size(); i++)
        {
            //璁板綍姣忎釜瀛愬脊鐨刬d銆佸潗鏍囥�佸崐寰�
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
    //鍒涘缓椋炴満Json鏁扮粍
    QJsonArray planeArray;
    //璁板綍椋炴満鐨刬d銆佸潗鏍囥�佸崐寰勩�乵p銆佽搴︺�佸墠杩�
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
    //鏋愭瀯鏃禿elete
    if(nullptr!= w) delete w;
    if(nullptr!=planeOfP1)delete planeOfP1;
    if(nullptr!=planeOfP2)delete planeOfP2;
    if(!aliveBall.empty()) aliveBall.clear();
    if(nullptr!=timer)delete timer;
    //娓呯┖璧勬簮姹狅紝骞堕噴鏀炬墍鏈夊璞�

    if(nullptr!=bulletObjPool){
        bulletObjPool->closePool();
        delete bulletObjPool;
    }
    if(nullptr!=ballObjPool){
        ballObjPool->closePool();
        delete ballObjPool;
    }
}

//鍗氬紙鏍稿績鍒濆鍖�
void Core::initial(GameType type,int ai,bool useAI, time_t gameTime)//bool榛樿false ,gametime榛樿30
{
    //杩炴帴鏍稿績淇″彿
    linkCore(type);
    //璁板綍鏄惁浣跨敤AI
    this->useAI = useAI;
    this->ai=ai;
    grade_ai1 = 0;
    grade_ai2 = 0;
    //瀹炰緥鍖栧苟鍒涘缓瀵硅薄姹�
    bulletObjPool=new BulletPool(this);
    bulletObjPool->createPool();
    //瀹炰緥鍖栧苟鍒涘缓瀵硅薄姹�
    ballObjPool=new BallPool(this);
    ballObjPool->createPool();
    Ball* ball=nullptr;

    this->gameTime=gameTime;


    int numOfBalls=4;

    QList<int> x,y,v_x,v_y;
    //differentRanList(x,RADIUS_BALL,WIDTH_WIDGET-RADIUS_BALL,numOfBalls);
    //differentRanList(y,RADIUS_BALL,HEIGHT_WIDGET-RADIUS_BALL,numOfBalls);
    differentranList(x,y,RADIUS_BALL,HEIGHT_WIDGET-RADIUS_BALL,numOfBalls);
    differentvelocity(v_x,-INIT_VELOCITY_X_BALL,INIT_VELOCITY_X_BALL,numOfBalls);
    differentvelocity(v_y,-INIT_VELOCITY_X_BALL,INIT_VELOCITY_Y_BALL,numOfBalls);
    //    qDebug()<<"exist correct\n";

    for(int i=0;i<numOfBalls;i++){
        //鍒濆鍖栫敓鎴愮涓�涓悆
        PooledObject *pooledObject=ballObjPool[0].getFreeObj();
        if(nullptr!=pooledObject){
            ball=(Ball*)pooledObject->obj;
            //            qDebug()<<x.size()<<y.size()<<v_x.size()<<v_y.size();
            ball->init(x.at(i),y.at(i),v_x.at(i),v_y.at(i),RADIUS_BALL,QUALITY_BALL);
            pooledObject->busy=true;
            aliveBall.append(pooledObject);
            //涓嶈鐞冮噸鍙�
            sepatareBall();
            newBall(ball);
        }
    }
    //瀹炰緥鍖栭鏈�
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

    //    //鍒濆鍖栨牳蹇冪殑鏃堕挓
    //    clock=new QTime();
    //    clock->start();

    //鍒濆鍖栫敾鍥剧獥鍙�
    //    w = new GLWidget(this);
    //    w->show();



    //鍒濆鍖栧畾鏃跺櫒
    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    connect(timer, &QTimer::timeout, this, &Core::update);
    timer->start(TIME_FRAME);
}
bool  Core::existInList(QList<int>list,int n)
{
    for(int i=0;i<list.size();i++){
        //鍒ゆ柇鏄惁鐩稿悓
        if(list.at(i)==n)
            return true;
    }
    return false;
}


void Core::differentRanList(QList<int>&l,int min,int max,int num)
{
    int b,t=0;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(int i=0;i<num;i++){
        do{
            t=0;
            b=qrand()%(max-min)+min;
            for(int j=0;j<=2*RADIUS_BALL;j++){
                if(!existInList(l,b-j))
                    t++;
                if(!existInList(l,b+j))
                    t++;
            }

        }while(t<=1+4*RADIUS_BALL);
        l.append(b);
    }
}
void Core::differentranList(QList<int>&l,QList<int>&m,int min,int max,int num)
{
    int b,c;
    bool t;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(int i=0;i<num;i++){
        do{
            t=true;
            b=qrand()%(max-min)+min;
            c=qrand()%(max-min)+min;
            if(sqrt((b-1000)*(b-1000)+(c-RADIUS_PLANE)*(c-RADIUS_PLANE))<500||sqrt((b-1000)*(b-1000)+(c-HEIGHT_WIDGET-RADIUS_PLANE)*(c-HEIGHT_WIDGET-RADIUS_PLANE))<500)
            {
                t=false;
                continue;
            }
            for(int j=0;j<l.size();j++){
                if(sqrt((b-l.at(j))*(b-l.at(j))+(c-m.at(j))*(c-m.at(j)))<200){
                    t=false;
                    break;
                }
            }
        }while(t==false);
        l.append(b);
        m.append(c);
        std::cout<<b<<"  "<<c<<"  "<<sqrt((b-1000)*(b-1000)+(c-RADIUS_PLANE)*(c-RADIUS_PLANE))<<"  "<<sqrt((b-1000)*(b-1000)+(c-HEIGHT_WIDGET-RADIUS_PLANE)*(c-HEIGHT_WIDGET-RADIUS_PLANE))<<'\n';
    }
}
void Core::differentvelocity(QList<int>&l,int min,int max,int num)
{
    int b;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(int i=0;i<num;i++){
        do{
            b=qrand()%(max-min)+min;

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
    //姣忓崄涓椂闂寸墖鏇存柊涓�娆＄獥鍙�
    for(int i=0;i<accelerator;i++){
        //鏃堕棿鐗囪鏁�
        slot++;
        updateBall();

        //鏇存柊椋炴満
        //        qDebug()<<"updateplane start\n";
        updatePlane();
        //        qDebug()<<"updateplane end\n";
        //纰版挒妫�娴�
        collideDetect();
        //        w->update();
    }


    if (slot*TIME_FRAME>=gameTime*1000){
        //鍋滄璁℃椂
        timer->stop();
        gameOver();
    }
    else if (slot % RECORDING_FREQUENCY == 0) {
        //璁板綍璇ュ抚瀵硅薄淇℃伅
        recordFrame();
    }
    //鏇存柊鐢诲浘绐楀彛
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
void Core::pve_setKeyPressedByAI(){

    //鏋勯�燿ata缁撴瀯浣�
    DataStruct data;
    KeyStruct key;
    FromObjToStruct(data,planeOfP1,planeOfP2,&aliveBall);
    ai_pve(&data,&key);
    key.id=offset;
    planeOfP1->keyPressed[FORWARD]=key.forward;
    planeOfP1->keyPressed[ROTATE_LEFT]=key.rotate_left;
    planeOfP1->keyPressed[ROTATE_RIGHT]=key.rotate_right;
    planeOfP1->keyPressed[SHOOT]=key.shoot;
    planeOfP1->keyPressed[SHIELD]=key.shield;
    offset++;
}
void Core::pvp_setplane1KeyPressedByAI(){

    //鏋勯�燿ata缁撴瀯浣�
    DataStruct data;
    KeyStruct key;
    FromObjToStruct(data,planeOfP1,planeOfP2,&aliveBall);
    ai_pvp_ai1(&data,&key);
    key.id=offset;
    planeOfP1->keyPressed[FORWARD]=key.forward;
    planeOfP1->keyPressed[ROTATE_LEFT]=key.rotate_left;
    planeOfP1->keyPressed[ROTATE_RIGHT]=key.rotate_right;
    planeOfP1->keyPressed[SHOOT]=key.shoot;
    planeOfP1->keyPressed[SHIELD]=key.shield;
    offset++;
}
void Core::pvp_setplane2KeyPressedByAI(){//椋炴満2

    //鏋勯�燿ata缁撴瀯浣�
    DataStruct data;
    KeyStruct key;
    FromObjToStruct(data,planeOfP1,planeOfP2,&aliveBall);
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
    //璁剧疆姣忎竴甯э紝涓や釜椋炴満鐨勬寜閿儏鍐�
    setKeyPressedByKeyBoard();
    //if鍒ゆ柇ai OR keyboard
    if(type==PVE){
        if (useAI)
        {
            pve_setKeyPressedByAI();
        }
    }
    if(type==PVP){
        if(ai>=1)
        pvp_setplane1KeyPressedByAI();
        if(ai==2)
        pvp_setplane2KeyPressedByAI();
    }
    //澶嶆椿鍓╀綑鏃堕棿涓嶇瓑浜�鏃讹紝涓嶈兘鎵ц浠讳綍鎿嶄綔
    //P1
    if(planeOfP1->reviveTime==0){
        planeOfP1->move();

        //鍦ㄥ垰澶嶆椿鐨勬棤鏁屾椂闂撮噷涓嶈兘鍙戝皠瀛愬脊鍜岀敤鐩�
        if(planeOfP1->invincibleTime==0){

            if(planeOfP1->keyPressed[SHOOT])
                planeOfP1->shoot(slot,bulletObjPool);

            if(planeOfP1->keyPressed[SHIELD])
                planeOfP1->shield(slot);
        }
    }
    planeOfP1->updateBullet(bulletObjPool);
    //鏇存柊鎶ょ浘鏃堕棿
    planeOfP1->updateShieldTime();
    //鏇存柊榄旀硶鍊�
    planeOfP1->updateMp();
    //鏇存柊澶嶆椿鏃堕棿
    planeOfP1->updateReviveTime();
    //鏇存柊鏃犳晫鏃堕棿
    planeOfP1->updateInvincibleTime();



    //P2
    if(nullptr!=planeOfP2){
        if(planeOfP2->reviveTime==0){
            planeOfP2->move();

            //鍦ㄥ垰澶嶆椿鐨勬棤鏁屾椂闂撮噷涓嶈兘鍙戝皠瀛愬脊鍜岀敤鐩�
            if(planeOfP2->invincibleTime==0){

                if(planeOfP2->keyPressed[SHOOT])
                    planeOfP2->shoot(slot,bulletObjPool);

                if(planeOfP2->keyPressed[SHIELD])
                    planeOfP2->shield(slot);
            }
        }
        planeOfP2->updateBullet(bulletObjPool);
        //鏇存柊鎶ょ浘鏃堕棿
        planeOfP2->updateShieldTime();
        //鏇存柊榄旀硶鍊�
        planeOfP2->updateMp();
        //鏇存柊澶嶆椿鏃堕棿
        planeOfP2->updateReviveTime();
        //鏇存柊鏃犳晫鏃堕棿
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

    //plane鍜宐all鐨勬娴嬶紝鍙互鍐欐垚鍗曠嫭鐨勬柟娉�
    Ball *ball=new Ball(this);

    PooledObject* pooledObj=new PooledObject(this);
    //妫�绱㈤鏈哄拰鐞冩槸鍚︾鎾�
    for(int i=0;i<aliveBall.size();i++){
        pooledObj=aliveBall.at(i);
        ball=(Ball*)pooledObj->obj;
        //鍙戝皠椋炴満鐖嗙偢explode淇″彿
        if(ball->collided(planeOfP1)){
            //椋炴満鏈娲籵r澶勪簬鏃犳晫鐘舵�侊紝鏃犺浣撶Н锛屼笉鍙戠敓纰版挒
            if(planeOfP1->reviveTime>0||planeOfP1->invincibleTime>0){
                continue;
            }
            //鍒ゆ柇椋炴満鏄惁鐢ㄧ浘
            if(planeOfP1->shieldTime<=0){
                //闈炲脊鎬х鎾烇紝椋炴満鐐告瘉
                planeOfP1->inelasticCollide(ball);
                planeOfP1->explode();
                planeOfP1->resetState();
                //杩涘叆澶嶆椿鍊掕鏃�
                planeOfP1->reviveTimeCountDown();

            }else{
                //鐞冨拰椋炴満寮规�х鎾�
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
                //椋炴満鏈娲籵r澶勪簬鏃犳晫鐘舵�侊紝鏃犺浣撶Н锛屼笉鍙戠敓纰版挒
                if(planeOfP2->reviveTime>0||planeOfP2->invincibleTime>0){
                    continue;
                }
                //鍒ゆ柇椋炴満鏄惁鐢ㄧ浘
                if(planeOfP2->shieldTime<=0){
                    //闈炲脊鎬х鎾烇紝椋炴満鐐告瘉
                    planeOfP2->inelasticCollide(ball);
                    planeOfP2->explode();
                    planeOfP2->resetState();
                    //杩涘叆澶嶆椿鍊掕鏃�
                    planeOfP2->reviveTimeCountDown();

                }else{
                    //鐞冨拰椋炴満寮规�х鎾�
                    planeOfP2->elasticCollide(ball);
                }

            }
        }


    }

    //bullet鍜宐all鐨勬娴�
    Bullet* bullet=new Bullet(this);

    Ball* left=nullptr;
    Ball* right=nullptr;
    PooledObject* leftChild=nullptr;
    PooledObject* rightChild=nullptr;
    for(int i=0;i<aliveBall.size();i++){
        pooledObj=aliveBall.at(i);

        //褰撹鐞冨凡缁忔秷澶辨椂锛岃烦杩囧涓嬩竴涓繘琛屾悳绱�
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

                //闈炲脊鎬х鎾烇紝瀛愬脊鐐告瘉
                ball->inelasticCollide(bullet);
                bullet->explode();



                termType=ball->type;

                //杈惧埌鍒嗚鐨勬垨鑰呮秷澶辩殑鏉′欢
                if(initHp==1||initHp==2||initHp==4||initHp==7){
                    termType--;
                    //鐞冩秷澶眔r鍒嗚鎴愬皬鐞冿紝杩斿洖缁欏璞℃睜
                    if(termType!=initType&&termType!=-1){
                        //                        ball->explode();
                        //鎵惧埌瀵硅薄姹犱腑涓や釜绌洪棽鐨刡all瀵硅薄
                        leftChild=ballObjPool->getFreeObj();
                        rightChild=ballObjPool->getFreeObj();

                        if(leftChild==nullptr||rightChild==nullptr){
                            qDebug()<<"鎵句笉鍒颁袱涓┖闂茬殑鐞冪殑灏佽瀵硅薄\n";
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
                    //瀛愬脊鍑讳腑鐞冿紝鐞冪殑hp-1
                    ball->hpMinusOne();
                }
            }
        }
        if(nullptr!=planeOfP2&&nullptr!=twoBullet){
            for(int j=0;j<twoBullet->size();j++){
                bullet=(Bullet*)twoBullet->at(j)->obj;

                if(ball->collided(bullet)){

                    //闈炲脊鎬х鎾烇紝瀛愬脊鐐告瘉
                    ball->inelasticCollide(bullet);
                    bullet->explode();



                    termType=ball->type;

                    //杈惧埌鍒嗚鐨勬垨鑰呮秷澶辩殑鏉′欢
                    if(initHp==1||initHp==2||initHp==4||initHp==7){
                        termType--;
                        //鐞冩秷澶眔r鍒嗚鎴愬皬鐞冿紝杩斿洖缁欏璞℃睜
                        if(termType!=initType&&termType!=-1){
                            //                        ball->explode();
                            //鎵惧埌瀵硅薄姹犱腑涓や釜绌洪棽鐨刡all瀵硅薄
                            leftChild=ballObjPool->getFreeObj();
                            rightChild=ballObjPool->getFreeObj();

                            if(leftChild==nullptr||rightChild==nullptr){
                                qDebug()<<"鎵句笉鍒颁袱涓┖闂茬殑鐞冪殑灏佽瀵硅薄\n";
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
                        //瀛愬脊鍑讳腑鐞冿紝鐞冪殑hp-1
                        ball->hpMinusOne();
                    }
                }
            }
        }

    }

    //ball鍜宐all鐨勬娴�
    Ball* b1=nullptr;
    Ball* b2=nullptr;

    for(int i=0;i<aliveBall.size();i++){
        b1=(Ball*)aliveBall.at(i)->obj;
        for(int j=i+1;j<aliveBall.size();j++){
            b2=(Ball*)aliveBall.at(j)->obj;
            if(b1->collided(b2)){
                //寮规�х鎾�
                b1->elasticCollide(b2);
            }
        }
    }
    //plane鍜屽鏂筨ullet鐨勬娴�
    if(nullptr!=planeOfP2){
        QList<PooledObject*>* oneBullet=&planeOfP1->ownBullet;
        QList<PooledObject*>* twoBullet=&planeOfP2->ownBullet;
        for(int i=0;i<oneBullet->size();i++){
            bullet=(Bullet*)oneBullet->at(i)->obj;
            if(planeOfP2->collided(bullet)){
                //褰撻鏈鸿繕娌″娲绘垨鑰呭浜庢棤鏁岀姸鎬佹椂锛屾棤瑙嗕綋绉鎾�
                if(planeOfP2->invincibleTime>0||planeOfP2->reviveTime>0){
//                    planeOfP2->elasticCollide(bullet);
                }else{
                    planeOfP2->elasticCollide(bullet);
                    if(planeOfP2->shieldTime==0){
                        planeOfP2->explode();
                        planeOfP2->resetState();
                        //杩涘叆澶嶆椿鍊掕鏃�
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
                        //杩涘叆澶嶆椿鍊掕鏃�
                        planeOfP1->reviveTimeCountDown();
                    }

                    bullet->explode();
                }
            }
        }
    }

    //鍙屾柟椋炴満鐨勭鎾炴娴�
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
