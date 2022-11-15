﻿#ifndef DATASTRUCT_H
#define DATASTRUCT_H
typedef struct PlaneStruct{
    int id;
    float x;
    float y;
    float v_x;
    float v_y;
    float a_x;
    float a_y;
    int r;
    int m;
    float angle;
}PLANESTRUCT;

typedef struct BallStruct{
    int id;
    float x;
    float y;
    float v_x;
    float v_y;
    int type;
    int hp;
    int r;
    int m;
}BALLSTRUCT;

typedef struct BulletStruct{
    int id;
    float x;
    float y;
    float v_x;
    float v_y;
    int r;
    int m;
    float angle;
}BULLETSTRUCT;
typedef struct DataStruct{
    int id;
    struct PlaneStruct plane1;
    struct PlaneStruct plane2;
    struct BulletStruct bullet1[50];
    struct BulletStruct bullet2[50];
    struct BallStruct ball[25];
    int bullet1_size,bullet2_size,ball_size;

} DATASTRUCT;
typedef struct KeyStruct{
    int id;
    int forward;
    int rotate_left;
    int rotate_right;
    int shoot;
    int shield;
}KEYSTRUCT;
/*
typedef struct FlagStruct{
    //0代表主进程，1是AI进程
    int id;
    //-1代表死亡，1代表存活
    int exist;

}FLAGSTRUCT;
*/
typedef struct ProcessInfoStruct{
    //0代表主进程，1、2是AI进程
    int id;
    //-1代表死亡，1代表存活
    int exist;
    //进程名称
    char name[20];
}PROCESSINFOSTRUCT;

#endif // DATASTRUCT_H
