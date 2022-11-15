#include "ball.h"
#include"math.h"

Ball::Ball(QObject *parent) : FlyObject(parent)
{

}
void Ball::init(double x,double y,double v_x,double v_y,double r,double m,int hp,int type)
{
    this->x=x;
    this->y=y;
    this->r=r;
    this->m=m;
    this->v_x=v_x;
    this->v_y=v_y;
    this->hp=hp;
    this->type=type;
}
void Ball::move(){
    double t=1.0*TIME_FRAME/1000;
    x=x+v_x*t;
    y=y+v_y*t;
//    postionChanged(x, y);
}
void Ball::hitBorder(){
    if(v_x<0&&x<=r){
        x=r;
        v_x=-v_x;
    }
    if(v_x>0&&x+r>=WIDTH_WIDGET){
        x=WIDTH_WIDGET-r;
        v_x=-v_x;
    }

    if(v_y<0&&y<=r){
        y=r;
        v_y=-v_y;
    }
    if(v_y>0&&y+r>=HEIGHT_WIDGET){
        y=HEIGHT_WIDGET-r;
        v_y=-v_y;
    }
}
void Ball::hpMinusOne(){
    if(hp>0){
        hp=hp-1;
    }
}
/*
int Ball::getHP(){
    return hp;
}
*/
int Ball::getType(){
    return type;
}
/*
void Ball::copy(Ball *ball){
    x=ball->getX();
    y=ball->getY();
    r=ball->getR();
    m=ball->getM();
    v_x=ball->getV_X();
    v_y=ball->getV_Y();
    hp=ball->getHP();
    type=ball->getType();
}*/

void Ball::leftSplit(Ball *ball){
    int v_x_s=1;
    if(ball->v_x<0)
        v_x_s=-1;
    int v_y_s=1;
    if(ball->v_y<0)
        v_y_s=-1;
    r=ball->r*0.5;
    m=ball->m*0.25;
    v_x=ball->v_x-200*sin(abs(atan(ball->v_y/ball->v_x))*180/PI);
    v_y=ball->v_y+v_y_s*v_x_s*200*cos(abs(atan(ball->v_y/ball->v_x))*180/PI);
    hp=ball->hp-1;
    type=ball->type-1;
    x=ball->x-r*sin(abs(atan(ball->v_y/ball->v_x))*180/PI);
    y=ball->y+v_y_s*v_x_s*r*cos(abs(atan(ball->v_y/ball->v_x))*180/PI);
    r=r-(1e-5);
}

void Ball::rightSplit(Ball *ball){
    int v_x_s=1;
    if(ball->v_x<0)
        v_x_s=-1;
    int v_y_s=1;
    if(ball->v_y<0)
        v_y_s=-1;
    r=ball->r*0.5;
    m=ball->m*0.25;
    v_x=ball->v_x+200*sin(abs(atan(ball->v_y/ball->v_x))*180/PI);
    v_y=ball->v_y-v_y_s*v_x_s*200*cos(abs(atan(ball->v_y/ball->v_x))*180/PI);
    hp=ball->hp-1;
    type=ball->type-1;
    x=ball->x+r*sin(abs(atan(ball->v_y/ball->v_x))*180/PI);
    y=ball->y-v_y_s*v_x_s*r*cos(abs(atan(ball->v_y/ball->v_x))*180/PI);
    r=r-(1e-5);
}

