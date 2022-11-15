#ifndef BULLET_H
#define BULLET_H

#include <QObject>
#include "flyobject.h"

class Bullet : public FlyObject
{
    Q_OBJECT
public:
    explicit Bullet(QObject *parent = nullptr);
    void init(double x,double y,double v_x,double v_y,double angle,double r,double m);
    double getAngle();

private:
    friend class Core;
    friend class Recording;
    friend class DataStruct;
    double angle;
    void move();
signals:

public slots:
};

#endif // BULLET_H
