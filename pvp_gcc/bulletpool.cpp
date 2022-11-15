#include "bulletpool.h"

BulletPool::BulletPool()
{
    this->maxObjs=MAX_BULLET_OBJECT_POOL;
    this->index=0;
}
void BulletPool::createPool()
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
            Bullet*bullet =new Bullet();
            poolObject->busy=false;
            poolObject->index=i;
            poolObject->obj=bullet;
            this->objArray[i]=poolObject;
        }
        this->index=0;
}
