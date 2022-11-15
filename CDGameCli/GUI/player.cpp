#include "player.h"

Player::Player(const neb::CJsonObject &frameArray, QObject *parent) : Core(parent), frameArray(frameArray)
{

}

Player::~Player()
{

}

void Player::update()
{
    //增加帧偏移
    frame_offset += playSpeed;
    //更新当前帧
    while (frame_offset >= RECORDING_FREQUENCY)
    {
        //每5偏移对应1帧
        cur_frame++;
        frame_offset -= RECORDING_FREQUENCY;
        //清空jsonMap
        jsonMap.clear();
    }
    //当前帧越界
    if (cur_frame >= frameArray.GetArraySize() - 1)
    {
        cur_frame = frameArray.GetArraySize() - 1;
        frame_offset = 0;
    }
    //加载当前帧数据
    loadData(cur_frame, frame_offset);
    //加载经过帧信号
    while (next_frame < cur_frame + 1)
    {
        loadSignal(next_frame++);
    }
}

void Player::loadSignal(int sig_frame)
{
    neb::CJsonObject sig_obj = frameArray[sig_frame];
    //获取每个信号对象
    for (int i = 0; i < sig_obj["signals"].GetArraySize(); i++)
    {
        neb::CJsonObject &sig = sig_obj["signals"][i];
        uint sender = getUint(sig, "sender");
        uint param = getUint(sig, "param");
        int name = getInt(sig, "name");
        //发送信号
        switch (name) {
        case NEW_PLANE:
            //核心发送newPlane
            newPlane(getPlane(param));
            break;
        case NEW_BALL:
            //核心发送newBall
            newBall(getBall(param));
            break;
        case GAME_OVER:
            //核心发送gameOver
            timer->stop();
            gameOver();
            return;
        case SCORE_CHANGE:
            //核心发送scoreChange
            scoreChange(getPlane(sender), param);
            break;
        case EXPLODE:
        {
            //flyObj发送explode
            FlyObject *obj = getFlyObj(sender);
            obj->x = getDouble(sig, "x");
            obj->y = getDouble(sig, "y");
            obj->explode();
        }
            break;
        case DISAPPEAR:
            //flyObj发送disappear
            getFlyObj(sender)->disappear();
            break;
        case SHOOT_BULLET:
            //飞机发射子弹
            getPlane(sender)->shootBullet(getBullet(param));
            break;
        case SHIELD_UP:
            //飞机开启护盾
            getPlane(sender)->shieldUp();
            break;
        case SHIELD_DOWN:
            //飞机关闭护盾
            getPlane(sender)->shieldDown();
            break;
        case INVINCIBLE_UP:
            //飞机开启无敌
            getPlane(sender)->invincibleUp();
            break;
        case INVINCIBLE_DOWN:
            //飞机关闭无敌
            getPlane(sender)->invincibleDown();
            break;
        }
    }
}

void Player::loadData(int frame, int offset)
{
    //获取当前帧数据
    neb::CJsonObject cur_obj = frameArray[frame];
    //如果需要下一帧数据且未加载下一帧数据
    if (offset > 0 && jsonMap.empty())
    {
        //获取下一帧数据
        neb::CJsonObject next_obj = frameArray[frame + 1];
        //加载数据至jsonMap
        jsonMap.insert(0, next_obj["core"]);
        for (int i = 0; i < next_obj["balls"].GetArraySize(); i++)
        {
            neb::CJsonObject ballJson = next_obj["balls"][i];
            jsonMap.insert(getUint(ballJson, "id"), ballJson);
        }
        for (int i = 0; i < next_obj["planes"].GetArraySize(); i++)
        {
            neb::CJsonObject planeJson = next_obj["planes"][i];
            jsonMap.insert(getUint(planeJson, "id"), planeJson);
        }
        for (int i = 0; i < next_obj["bullets"].GetArraySize(); i++)
        {
            neb::CJsonObject bulletJson = next_obj["bullets"][i];
            jsonMap.insert(getUint(bulletJson, "id"), bulletJson);
        }
    }
    //读取游戏时间
    neb::CJsonObject coreJson = cur_obj["core"];
    slot = getInt(coreJson, "slot");
    if (offset > 0)
    {
        linearCalc(slot, (long)getInt(coreJson, "slot"), offset);
    }
    //设置每个球的坐标、半径、类型
    for (int i = 0; i < cur_obj["balls"].GetArraySize(); i++)
    {
        neb::CJsonObject ballJson = cur_obj["balls"][i];
        Ball *ball = getBall(getUint(ballJson, "id"));
        ball->x = getDouble(ballJson, "x");
        ball->y = getDouble(ballJson, "y");
        ball->r = getDouble(ballJson, "r");
        ball->type = getInt(coreJson, "type");
        if (offset > 0)
        {
            QMap<uint, neb::CJsonObject>::const_iterator nextBall = jsonMap.find(getUint(ballJson, "id"));
            if (nextBall != jsonMap.end())
            {
                linearCalc(ball->x, getDouble(nextBall.value(), "x"), offset);
                linearCalc(ball->y, getDouble(nextBall.value(), "y"), offset);
            }
        }
    }
    //设置每个飞机的坐标、半径、mp、角度、前进
    for (int i = 0; i < cur_obj["planes"].GetArraySize(); i++)
    {
        neb::CJsonObject planeJson = cur_obj["planes"][i];
        Plane *plane = getPlane(getUint(planeJson, "id"));
        plane->x = getDouble(planeJson, "x");
        plane->y = getDouble(planeJson, "y");
        plane->r = getDouble(planeJson, "r");
        plane->mp = getDouble(planeJson, "mp");
        plane->angle = getDouble(planeJson, "angle");
        plane->keyPressed[FORWARD] = getBool(planeJson, "keyPressed");
        if (offset > 0)
        {
            QMap<uint, neb::CJsonObject>::const_iterator nextPlane = jsonMap.find(getUint(planeJson, "id"));
            if (nextPlane != jsonMap.end())
            {
                linearCalc(plane->x, getDouble(nextPlane.value(), "x"), offset);
                linearCalc(plane->y, getDouble(nextPlane.value(), "y"), offset);
                linearCalc(plane->angle, getDouble(nextPlane.value(), "angle"), offset);
            }
        }
    }
    //设置每个子弹的坐标、半径、角度
    for (int i = 0; i < cur_obj["bullets"].GetArraySize(); i++)
    {
        neb::CJsonObject bulletJson = cur_obj["bullets"][i];
        Bullet *bullet = getBullet(getUint(bulletJson, "id"));
        bullet->x = getDouble(bulletJson, "x");
        bullet->y = getDouble(bulletJson, "y");
        bullet->r = getDouble(bulletJson, "r");
        bullet->angle = getDouble(bulletJson, "angle");
        if (offset > 0)
        {
            QMap<uint, neb::CJsonObject>::const_iterator nextBullet = jsonMap.find(getUint(bulletJson, "id"));
            if (nextBullet != jsonMap.end())
            {
                linearCalc(bullet->x, getDouble(nextBullet.value(), "x"), offset);
                linearCalc(bullet->y, getDouble(nextBullet.value(), "y"), offset);
                linearCalc(bullet->angle, getDouble(nextBullet.value(), "angle"), offset);
            }
        }
    }
}

Ball *Player::getBall(uint ball)
{
    //查找该球对象，不存在则创建
    QMap<uint, Ball *>::const_iterator i = balls.find(ball);
    if (i != balls.end())
    {
        return i.value();
    }
    balls[ball] = new Ball(this);
    return balls[ball];
}

Plane *Player::getPlane(uint plane)
{
    //查找该飞机对象，不存在则创建
    QMap<uint, Plane *>::const_iterator i = planes.find(plane);
    if (i != planes.end())
    {
        return i.value();
    }
    planes[plane] = new Plane(0, 0, 0, 0, 0, this);
    return planes[plane];
}

Bullet *Player::getBullet(uint bullet)
{
    //查找该子弹对象，不存在则创建
    QMap<uint, Bullet *>::const_iterator i = bullets.find(bullet);
    if (i != bullets.end())
    {
        return i.value();
    }
    bullets[bullet] = new Bullet(this);
    return bullets[bullet];
}

FlyObject *Player::getFlyObj(uint flyObj)
{
    //尝试在球中查找该对象
    QMap<uint, Ball *>::const_iterator b = balls.find(flyObj);
    if (b != balls.end())
    {
        return b.value();
    }
    //尝试在飞机中查找该对象
    QMap<uint, Plane *>::const_iterator p = planes.find(flyObj);
    if (p != planes.end())
    {
        return p.value();
    }
    //获取子弹对象兵返回
    return getBullet(flyObj);
}

void Player::initial(GameType type, bool useAI, time_t gameTime)
{
    Q_UNUSED(type);
    Q_UNUSED(useAI);
    //设置游戏倍速
    playSpeed = 1;
    //设置游戏时长
    this->gameTime = gameTime;
    //设置帧、帧偏移为0
    cur_frame = 0;
    next_frame = 0;
    frame_offset = 0;
    //初始化定时器
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Player::update);
    timer->setInterval(TIME_FRAME);
    timer->start();
}

void Player::decelerate()
{
    //减速最低为1倍
    if (playSpeed > 1)
    {
        playSpeed /= 2;
    }
}

void Player::accelerate()
{
    //加速最高为8倍
    if (playSpeed < 8)
    {
        playSpeed *= 2;
    }
}

void Player::suspend()
{
    if (timer->isActive())
    {
        timer->stop();
    }
    else
    {
        timer->start();
    }
}

int Player::getInt(const neb::CJsonObject &jsonObject, const std::string& strKey)
{
    jsonObject.Get(strKey, intRef);
    return intRef;
}

uint Player::getUint(const neb::CJsonObject &jsonObject, const std::string& strKey)
{
    jsonObject.Get(strKey, uintRef);
    return uintRef;
}

bool Player::getBool(const neb::CJsonObject &jsonObject, const std::string& strKey)
{
    jsonObject.Get(strKey, boolRef);
    return boolRef;
}

double Player::getDouble(const neb::CJsonObject &jsonObject, const std::string& strKey)
{
    jsonObject.Get(strKey, doubleRef);
    return doubleRef;
}
