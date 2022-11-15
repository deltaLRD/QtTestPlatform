#ifndef POOLEDOBJECT_H
#define POOLEDOBJECT_H

#include <QObject>
#include"flyobject.h"

class PooledObject : public QObject
{
    Q_OBJECT
public:
    explicit PooledObject(QObject *parent = nullptr);
    /*
    void setBusy(bool busy);
    bool getBusy();
    FlyObject* getObj();
    void setObj(FlyObject*obj);
    void setIndex(int index);
    int getIndex();
    */

private:
    friend class Core;
    friend class ObjectPool;
    friend class BulletPool;
    friend class BallPool;
    friend class Plane;
    friend class DataStruct;
    bool busy;
    int index;
    FlyObject* obj;

signals:

};

#endif // POOLEDOBJECT_H
