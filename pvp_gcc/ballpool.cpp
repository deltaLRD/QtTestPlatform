#include "ballpool.h"

BallPool::BallPool()
{
    this->index=0;
    this->maxObjs=MAX_BALL_OBJECT_POOL;
}
void BallPool::createPool()
{
        objArray=new PooledObject*[maxObjs];
        //初始化置为nullptr
        for(int i=0;i<maxObjs;i++){
            objArray[i]=nullptr;
        }
        //检查objArray是否已经初始化
        if(nullptr!=this->objArray[0]){
            return ;
        }
        for(int i=0;i<maxObjs;i++)
        {
            PooledObject *poolObject=new PooledObject();
            Ball* ball =new Ball();
            poolObject->busy=false;
            poolObject->index=i;
            poolObject->obj=ball;
            this->objArray[i]=poolObject;
        }
        this->index=0;
}
