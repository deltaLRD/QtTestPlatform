#ifndef BALLPOOL_H
#define BALLPOOL_H

#include"objectpool.h"

class BallPool:public ObjectPool
{
public:
    explicit BallPool();
    void createPool();
};

#endif // BALLPOOL_H
