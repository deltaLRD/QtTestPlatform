#ifndef PLAYER_H
#define PLAYER_H

#include "core.h"
#include "ball.h"
#include "plane.h"
#include "bullet.h"
#include "JSON/CJsonObject.hpp"

#include <QMap>

class Player : public Core
{
    Q_OBJECT
public:
    explicit Player(const neb::CJsonObject &frameArray, QObject *parent = nullptr);
    ~Player();
    //重写更新函数
    void update() override;
    //重写初始化函数
    void initial(GameType type, bool useAI = false, time_t gameTime = 30) override;
    //播放录像减速
    void decelerate();
    //播放录像加速
    void accelerate();
    //设置暂停/播放
    void suspend();
    //获取当前倍速
    inline int getPlaySpeed()
    {
        return playSpeed;
    }

private:
    //读取每帧信息
    void loadData(int frame, int offset);
    void loadSignal(int sig_frame);
    //简化Json读取
    int getInt(const neb::CJsonObject &jsonObject, const std::string& strKey);
    uint getUint(const neb::CJsonObject &jsonObject, const std::string& strKey);
    bool getBool(const neb::CJsonObject &jsonObject, const std::string& strKey);
    double getDouble(const neb::CJsonObject &jsonObject, const std::string& strKey);
    //获取游戏对象
    Ball *getBall(uint ball);
    Plane *getPlane(uint plane);
    Bullet *getBullet(uint bullet);
    FlyObject *getFlyObj(uint flyObj);
    //线性计算数据
    template <typename T>
    void linearCalc(T &cur_value, T next_value, int offset)
    {
        cur_value += (next_value - cur_value) * offset / RECORDING_FREQUENCY;
    }

    //游戏对象Map<add, ptr>
    QMap<uint, Ball *> balls;
    QMap<uint, Plane *> planes;
    QMap<uint, Bullet *> bullets;
    QMap<uint, neb::CJsonObject> jsonMap;
    //Json数组对象
    int cur_frame, next_frame, frame_offset;
    neb::CJsonObject frameArray;
    //播放速度
    int playSpeed;
    //暂存读取信息
    int intRef;
    uint uintRef;
    bool boolRef;
    double doubleRef;

signals:

};

#endif // PLAYER_H
