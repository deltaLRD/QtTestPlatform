#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include"pooledobject.h"
#include"const.h"
#include"windows.h"
#include"bullet.h"
#include"ball.h"
class ObjectPool 
{
public:
    ObjectPool();
    //对象池中的对象数目
    int maxObjs;
    //初始化对象的下标
    int index;
    //找到空闲的对象，否则返回nullptr
    PooledObject* getFreeObj();
    //初始化对象池
    void createPool();
    //关闭对象池，释放指针
    void closePool();
    //返还对象
    void returnObj(PooledObject *obj);
    //对象数组
    PooledObject**objArray;


};

#endif // OBJECTPOOL_H
