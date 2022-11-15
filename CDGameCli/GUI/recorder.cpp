#include "core.h"

#include "direct.h"
#include <ctime>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
using namespace neb;

void Core::saveVideo()
{
    //核心发出了gameOver信号无参数
    CJsonObject sig;
    sig.Add("sender", PTR_TO_UINT(this));
    sig.Add("name", GAME_OVER);
    sigArray.Add(sig);
    //创建输出目录
    char dir[] = "output";
    if (_access(dir, 0))
    {
        _mkdir(dir);
    }
    //最后存储一帧
    recordFrame();
    //获取当前时间
    time_t now = time(0);
    stringstream fileName;
    fileName << "output/" << now << (type == PVE ? ".pve" : ".pvp");
    //创建输出文件
    ofstream videoFile(fileName.str() , ios_base::out | ios_base::trunc);
    //只写打开输出文件
    if (!videoFile.is_open()) {
        cout << "saveVideo::OpenVideoFile::Failed" << endl;
    }
    else
    {
        videoFile << frameArray.ToString();
        videoFile.close();
    }
}

void Core::linkCore(GameType type)
{
    //记录游戏类型
    this->type = type;
    //记录gameOver并保存文件
    this->gameOver.bind(&Core::saveVideo, this);
    //创建新球时记录信号
    this->newBall.bind([this](Ball *ball)
    {
        //核心发出了newBall信号参数为ball
        CJsonObject sig;
        sig.Add("sender", PTR_TO_UINT(this));
        sig.Add("name" , NEW_BALL);
        sig.Add("param" , PTR_TO_UINT(ball));
        sigArray.Add(sig);
        //连接新生球的信号
        linkFlyObj(ball);
    }, this);
    //创建新飞机时记录信号
    this->newPlane.bind([this](Plane *plane)
    {
        //核心发出了newPlane信号参数为plane
        CJsonObject sig;
        sig.Add("sender", PTR_TO_UINT(this));
        sig.Add("name" , NEW_PLANE);
        sig.Add("param" , PTR_TO_UINT(plane));
        sigArray.Add(sig);
        //连接新生飞机的信号
        linkPlane(plane);
        linkFlyObj(plane);
    }, this);
    //分数改变时记录信号
    this->scoreChange.bind([this](Plane *plane, int score)
    {
        //核心发出了scoreChange信号参数为plane、soore
        CJsonObject sig;
        sig.Add("sender", PTR_TO_UINT(plane));
        sig.Add("name" , SCORE_CHANGE);
        sig.Add("param" , score);
        sigArray.Add(sig);
    }, this);
}

void Core::linkPlane(Plane *plane)
{
    //飞机发射子弹时记录信号
    plane->shootBullet.bind([this, plane](Bullet *bullet)
    {
        //plane发出了发射子弹信号参数为bullet
        CJsonObject sig;
        sig.Add("sender", PTR_TO_UINT(plane));
        sig.Add("name" , SHOOT_BULLET);
        sig.Add("param" , PTR_TO_UINT(bullet));
        sigArray.Add(sig);
        //连接新生子弹信号
        linkFlyObj(bullet);
    }, this);
    //飞机开启护盾时记录信号
    plane->shieldUp.bind([this, plane]()
    {
        //plane发出开启护盾信号无参数
        CJsonObject sig;
        sig.Add("sender", PTR_TO_UINT(plane));
        sig.Add("name" , SHIELD_UP);
        sigArray.Add(sig);
    }, this);
    //飞机关闭护盾时记录信号
    plane->shieldDown.bind([this, plane]()
    {
        //plane发出关闭护盾信号无参数
        CJsonObject sig;
        sig.Add("sender", PTR_TO_UINT(plane));
        sig.Add("name" , SHIELD_DOWN);
        sigArray.Add(sig);
    }, this);
    //飞机开启无敌时记录信号
    plane->invincibleUp.bind([this, plane]()
    {
        //plane发出开启无敌信号无参数
        CJsonObject sig;
        sig.Add("sender", PTR_TO_UINT(plane));
        sig.Add("name" , INVINCIBLE_UP);
        sigArray.Add(sig);
    }, this);
    //飞机关闭无敌时记录信号
    plane->invincibleDown.bind([this, plane]()
    {
        //plane发出关闭无敌信号无参数
        CJsonObject sig;
        sig.Add("sender", PTR_TO_UINT(plane));
        sig.Add("name" , INVINCIBLE_DOWN);
        sigArray.Add(sig);
    }, this);
}

void Core::linkFlyObj(FlyObject *obj)
{
    //连接飞行物的爆炸、消失信号
    obj->explode.bind([this, obj]()
    {
        //obj发出爆炸信号无参数
        CJsonObject sig;
        sig.Add("sender", PTR_TO_UINT(obj));
        sig.Add("name" , EXPLODE);
        sig.Add("x" , obj->x);
        sig.Add("y" , obj->y);
        sigArray.Add(sig);
    }, this);
    obj->disappear.bind([this, obj]()
    {
        //obj发出消失信号无参数
        CJsonObject sig;
        sig.Add("sender", PTR_TO_UINT(obj));
        sig.Add("name" , DISAPPEAR);
        sigArray.Add(sig);
    }, this);
}
void Core::recordBall(CJsonObject &frameObj)
{
    //创建球Json数组
    CJsonObject ballArray;
    CJsonObject ballJson;
    //遍历可用球集合
    for (int i = 0; i < aliveBall.size(); i++)
    {
        //记录每个球的id、坐标、半径、类型
        Ball *ball = (Ball *)aliveBall.at(i)->obj;
        ballJson.ReplaceAdd("id", PTR_TO_UINT(ball));
        ballJson.ReplaceAdd("x", ball->x);
        ballJson.ReplaceAdd("y", ball->y);
        ballJson.ReplaceAdd("r", ball->r);
        ballJson.ReplaceAdd("type", ball->type);
        ballArray.Add(ballJson);
    }
    frameObj.Add("balls", ballArray);
}

void Core::recordPlane(CJsonObject &frameObj)
{
    //创建飞机Json数组
    CJsonObject planeArray;
    //记录飞机的id、坐标、半径、mp、角度、前进
    CJsonObject planeOne;
    planeOne.ReplaceAdd("id", PTR_TO_UINT(planeOfP1));
    planeOne.ReplaceAdd("x", planeOfP1->x);
    planeOne.ReplaceAdd("y", planeOfP1->y);
    planeOne.ReplaceAdd("r", planeOfP1->r);
    planeOne.ReplaceAdd("mp", planeOfP1->mp);
    planeOne.ReplaceAdd("angle", planeOfP1->angle);
    planeOne.ReplaceAdd("keyPressed", planeOfP1->keyPressed[FORWARD]);
    planeArray.Add(planeOne);
    if (planeOfP2)
    {
        CJsonObject planeTwo;
        planeTwo.ReplaceAdd("id", PTR_TO_UINT(planeOfP2));
        planeTwo.ReplaceAdd("x", planeOfP2->x);
        planeTwo.ReplaceAdd("y", planeOfP2->y);
        planeTwo.ReplaceAdd("r", planeOfP2->r);
        planeTwo.ReplaceAdd("mp", planeOfP2->mp);
        planeTwo.ReplaceAdd("angle", planeOfP2->angle);
        planeTwo.ReplaceAdd("keyPressed", planeOfP2->keyPressed[FORWARD]);
        planeArray.Add(planeTwo);
    }
    frameObj.Add("planes", planeArray);
}


void Core::recordBullet(CJsonObject &frameObj)
{
    //创建子弹Json数组
    CJsonObject bulletArray;
    CJsonObject bulletJson;
    //遍历P1子弹集合
    for (int i = 0; i < planeOfP1->ownBullet.size(); i++)
    {
        //记录每个子弹的id、坐标、半径
        Bullet *bullet = (Bullet *)planeOfP1->ownBullet.at(i)->obj;
        bulletJson.ReplaceAdd("id", PTR_TO_UINT(bullet));
        bulletJson.ReplaceAdd("x", bullet->x);
        bulletJson.ReplaceAdd("y", bullet->y);
        bulletJson.ReplaceAdd("r", bullet->r);
        bulletJson.ReplaceAdd("angle", bullet->angle);
        bulletArray.Add(bulletJson);
    }
    if (planeOfP2)
    {
        //遍历P2子弹集合
        for (int i = 0; i < planeOfP2->ownBullet.size(); i++)
        {
            //记录每个子弹的id、坐标、半径
            Bullet *bullet = (Bullet *)planeOfP2->ownBullet.at(i)->obj;
            bulletJson.ReplaceAdd("id", PTR_TO_UINT(bullet));
            bulletJson.ReplaceAdd("x", bullet->x);
            bulletJson.ReplaceAdd("y", bullet->y);
            bulletJson.ReplaceAdd("r", bullet->r);
            bulletJson.ReplaceAdd("angle", bullet->angle);
            bulletArray.Add(bulletJson);
        }
    }
    frameObj.Add("bullets", bulletArray);
}

void Core::recordFrame()
{
    //创建记录帧对象
    CJsonObject frameObj;
    CJsonObject coreObj;
    coreObj.Add("slot", (int)slot);
    frameObj.Add("core", coreObj);
    frameObj.Add("signals", sigArray);
    recordBall(frameObj);
    recordPlane(frameObj);
    recordBullet(frameObj);
    //添加帧对象进帧数组
    frameArray.Add(frameObj);
    //清空信号数组
    sigArray.Clear();
}
