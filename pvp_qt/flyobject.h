#ifndef FLYOBJECT_H
#define FLYOBJECT_H

#include <QObject>
#include"math.h"
#include"const.h"
class FlyObject : public QObject
{
    Q_OBJECT
public:
    explicit FlyObject(QObject *parent = nullptr);
    double getX();
    double getY();
    double getR();

protected:
    //坐标
    double x;
    double y;
    //速度
    double v_x;
    double v_y;
    //加速度
//    double a_x;
//    double a_y;
    //半径
    double r;
    //质量
    double m;

    void move();

    /*
    double getV_X();
    double getV_Y();
    double getA_X();
    double getA_Y();

    double getM();

    void setXY(double x,double y);
    void setX(double x);
    void setY(double y);
    void setV_X(double v_x);
    void setV_Y(double v_y);

    void setA_X(double a_x);
    void setA_Y(double a_y);
    void setR(double r);
    void setM(double m);
    */
    //得到两者间的距离
    double getDistance(FlyObject* fo);
    //检测碰撞
    bool collided(FlyObject* fo);
    //弹性碰撞
    void elasticCollide(FlyObject* fo);
    //非弹性碰撞
    void inelasticCollide(FlyObject* fo);

    void hitBorder();
private:
//    friend class FlyObject;
    friend class Plane;
    friend class Core;
    friend class Recording;
    friend class DataStruct;

signals:

    //位置改变
//    void postionChanged(double x, double y);

public slots:
};

#endif // FLYOBJECT_H
