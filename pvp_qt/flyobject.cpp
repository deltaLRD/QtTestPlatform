#include "flyobject.h"
FlyObject::FlyObject(QObject *parent) : QObject(parent)
{

}

bool FlyObject::collided(FlyObject *fo)
{
    double distance=this->getDistance(fo);
    if(distance<=this->r+fo->r)
        return true;
    else
        return false;
}
void FlyObject::hitBorder()
{
    //do some test and act differently
}
double FlyObject::getDistance(FlyObject *fo)
{
    double fo_x=fo->getX();
    double fo_y=fo->getY();
    double distance;
    distance=std::sqrt((x-fo_x)*(x-fo_x)+(y-fo_y)*(y-fo_y));
    return distance;
}
void FlyObject::elasticCollide(FlyObject* fo)
{
    double vx1=v_x;
    double vx2=fo->v_x;
    double vy1=v_y;
    double vy2=fo->v_y;
    double m1=m;
    double m2=fo->m;
    v_x=((m1-m2)*vx1+2*m2*vx2)/(m1+m2);
    fo->v_x=(vx1+v_x-vx2);
    v_y=((m1-m2)*vy1+2*m2*vy2)/(m1+m2);
    fo->v_y=(vy1+v_y-vy2);
}
void FlyObject::inelasticCollide(FlyObject* fo)
{
    //先用弹性碰撞代替非弹性碰撞
    double vx1=v_x;
    double vx2=fo->v_x;
    double vy1=v_y;
    double vy2=fo->v_y;
    double m1=m;
    double m2=fo->m;
    v_x=((m1-m2)*vx1+2*m2*vx2)/(m1+m2);
    fo->v_x=(vx1+v_x-vx2);
    v_y=((m1-m2)*vy1+2*m2*vy2)/(m1+m2);
    fo->v_y=(vy1+v_y-vy2);
}
/*
double FlyObject::getM()
{
    return this->m;
}
*/
double FlyObject::getX()
{
    return this->x;
}
double FlyObject::getY()
{
    return this->y;
}
double FlyObject::getR()
{
    return this->r;
}
/*
double FlyObject::getV_X()
{
    return this->v_x;
}
double FlyObject::getV_Y()
{
    return this->v_y;
}

double FlyObject::getA_X()
{
    return this->a_x;
}
double FlyObject::getA_Y()
{
    return this->a_y;
}

void FlyObject::setXY(double x,double y)
{
    this->x=x;
    this->y=y;
}
void FlyObject::setM(double m)
{
    this->m=m;
}
void FlyObject::setR(double r)
{
    this->r=r;
}
void FlyObject::setX(double x)
{
    this->x=x;
}
void FlyObject::setY(double y)
{
    this->y=y;
}
void FlyObject::setV_X(double v_x)
{
    this->v_x=v_x;
}
void FlyObject::setV_Y(double v_y)
{
    this->v_y=v_y;
}
//void FlyObject::setA_X(double a_x)
//{
//    this->a_x=a_x;
//}
//void FlyObject::setA_Y(double a_y)
//{
//    this->a_y=a_y;
//}
*/
