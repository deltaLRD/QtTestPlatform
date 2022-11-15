#include "bullet.h"

void Bullet::init(double x,double y,double v_x,double v_y,double angle,double r,double m)
{
    this->x=x;
    this->y=y;
    this->r=r;
    this->m=m;
    this->angle=angle;
    this->v_x=v_x;
    this->v_y=v_y;
}
void Bullet::move(){
    double t=1.0*TIME_FRAME/1000;
    x=x+v_x*t;
    y=y+v_y*t;
}
double Bullet::getAngle(){
    return angle;
}
