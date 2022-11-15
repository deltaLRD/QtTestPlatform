#ifndef BULLETPOOL_H
#define BULLETPOOL_H

#include <QObject>
#include"objectpool.h"

class BulletPool:public ObjectPool
{
    Q_OBJECT
public:
    explicit BulletPool(QObject*parent=nullptr);
    void createPool();
};

#endif // BULLETPOOL_H
