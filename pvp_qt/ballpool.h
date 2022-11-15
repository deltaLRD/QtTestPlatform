#ifndef BALLPOOL_H
#define BALLPOOL_H

#include <QObject>
#include"objectpool.h"

class BallPool:public ObjectPool
{
    Q_OBJECT
public:
    explicit BallPool(QObject*parent=nullptr);
    void createPool();
};

#endif // BALLPOOL_H
