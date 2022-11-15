#include "objectpool.h"

ObjectPool::ObjectPool(QObject *parent) : QObject(parent)
{
    this->index=0;
    this->maxObjs=0;
}
void ObjectPool::createPool()
{
    //ball和bullet的初始化不同，由子类来实现
}
PooledObject* ObjectPool::getFreeObj()
{
    for(int i=0;i<maxObjs;i++)
    {
        //循环查找
        index=(index+1)%maxObjs;
        PooledObject *poolObject=this->objArray[index];

        if(!poolObject->busy)
        {
            poolObject->busy=true;
            return  poolObject;
        }
    }
     return nullptr;
}
void ObjectPool::returnObj(PooledObject *obj)
{
    if(nullptr==this->objArray[0])
    {
        return;
    }

    int i=obj->index;
    PooledObject *o=this->objArray[i];
    o->busy=false;
    return;
}
void ObjectPool::closePool()
{
    if(nullptr==objArray[0])
    {
        return;
    }
    PooledObject *o=nullptr;
    for(int i=0;i<maxObjs;i++)
    {
        o=this->objArray[i];
        if(o->busy)
        {
            Sleep(5);
        }
        delete o->obj;
        delete o;
        objArray[i]=nullptr;
    }
}
