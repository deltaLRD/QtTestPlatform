#ifndef BULLETPOOL_H
#define BULLETPOOL_H

#include"objectpool.h"

class BulletPool:public ObjectPool
{
public:
    BulletPool();
    void createPool();
};

#endif // BULLETPOOL_H
