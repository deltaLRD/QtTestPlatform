#ifndef CORE_H
#define CORE_H

#include "GUI/glwidget.h"
#include"time.h"
#include"plane.h"
#include"objectpool.h"
#include"bulletpool.h"
#include"ballpool.h"
#include"ballpool.h"
#include"const.h"
#include"DataStruct.h"
#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QtGlobal>
#include <qmath.h>

#include "SigSlot/sigslot.h"
#include "JSON/CJsonObject.hpp"

void ai_pve(DataStruct*data,KeyStruct*key);
void ai_pvp_ai2(DataStruct*data,KeyStruct*key);
void ai_pvp_ai1(DataStruct*data,KeyStruct*key);
class Core : public QObject, public SigSlotBase
{
    Q_OBJECT
public:
    //娓告垙绫诲瀷
    enum GameType
    {
        PVP            = 0x0001,
        PVE            = 0x0002,
    };
    enum KeyPressed
    {
        P1_FORWARD         = 0x0001,
        P1_ROTATE_LEFT            = 0x0002,
        P1_ROTATE_RIGHT           = 0x0004,
        P1_SHOOT           = 0x0008,
        P1_SHIELD          = 0x0010,
        P2_FORWARD         = 0x0020,
        P2_ROTATE_LEFT            = 0x0040,
        P2_ROTATE_RIGHT           = 0x0080,
        P2_SHOOT           = 0x0100,
        P2_SHIELD          = 0x0200,
    };
    Q_DECLARE_FLAGS(KeyPresseds, KeyPressed)
    explicit Core(QObject *parent = nullptr);
    ~Core();
    //鑾峰彇娓告垙鏃堕棿
    long getRestTime();
    //鍗氬紙鏍稿績鍒濆鍖�
    virtual void initial(GameType type,int ai=0,bool useAI=false,time_t gameTime=30);
    //璁剧疆鎸夐敭鐘舵��
    void setKeyState(KeyPressed key, bool state);

protected:
    //鏃堕棿鐗�
    long slot;
    //鍗氬紙鏍稿績瀹氭椂鍣�
    QTimer *timer;
    //瀵瑰眬鏃堕暱
    time_t gameTime;

private:
    //鏇存柊椋炴満鐨勭姸鎬�
    void updatePlane();
    //鏇存柊瀛愬脊鐨勭姸鎬�
    void updateBullet();
    //鏇存柊鐞冪殑鐘舵��
    void updateBall();
    void setAccelerator(int accelerator);
    QList<PooledObject*>* getAliveBall();
    //纰版挒妫�娴�
    void collideDetect();
    //鍒ゆ柇鐢熸垚涓嶅悓鐨勯殢鏈烘暟
    bool existInList(QList<int>list,int n);
    void differentRanList(QList<int>&l,int min,int max,int num);
    void differentranList(QList<int>&l,QList<int>&m,int min,int max,int num);
    void differentvelocity(QList<int>&l,int min,int max,int num);
    void sepatareBall();
    //鏍规嵁keyPressed璁剧疆涓ら鏈虹殑鎸夐敭鐘舵��
    void setKeyPressedByKeyBoard();
    void pve_setKeyPressedByAI();
    void pvp_setplane1KeyPressedByAI();
    void pvp_setplane2KeyPressedByAI();
    //鐞冭鍑讳腑鐖嗙偢寰椾竴鍒�
    void score(Plane*plane);

    /*
    //灏佽涓�涓猨son瀵硅薄
    QJsonObject packageAll();
    void packageBall(QJsonObject &dataObj);
    void packageBullet(QJsonObject &dataObj);
    void packagePlane(QJsonObject &dataObj);*/

    //鏋勯�燿ata缁撴瀯浣�
    void FromObjToStruct(DataStruct&data,Plane*plane1,Plane*plane2,QList<PooledObject*>*ball);


//    //鏃堕挓
//    QTime *clock;
    //鐢诲浘鎺т欢
    GLWidget *w;
    //瀛愬脊瀵硅薄姹�
    BulletPool *bulletObjPool;
    //鐞冨璞℃睜
    BallPool *ballObjPool;
    //鎸佹湁鐨勭悆鏁扮粍
    QList<PooledObject*> aliveBall;
    //椋炴満
    Plane*planeOfP1;
    Plane*planeOfP2;
    //鍔犻�熷洜瀛�
    int accelerator;
    //鍚勬寜閿姸鎬�
    KeyPresseds keyPressed;
    int offset;
    int grade_ai1,grade_ai2;

private:
    //杩炴帴鏍稿績淇″彿
    void linkCore(GameType type);
    //杩炴帴椋炴満淇″彿
    void linkPlane(Plane *plane);
    //杩炴帴FlyObject淇″彿
    void linkFlyObj(FlyObject *obj);
    //璁板綍鍚勫璞′俊鎭�
    void recordBall(neb::CJsonObject &frameObj);
    void recordPlane(neb::CJsonObject &frameObj);
    void recordBullet(neb::CJsonObject &frameObj);
    //淇濆瓨娓告垙褰曞儚
    void saveVideo();
    //璁板綍褰撳墠甯ф暟鎹�
    void recordFrame();
    //Json甯ф暟缁�
    neb::CJsonObject frameArray, sigArray;
    //璁板綍娓告垙绫诲瀷
    GameType type;
    //pve鏄惁浣跨敤AI
    bool useAI;
    //pvpai鏁伴噺
    int ai;

public:
    Signal<Plane *> newPlane;
    Signal<Ball *> newBall;
    Signal<> gameOver;
    Signal<Plane *, int> scoreChange;

public slots:
    //鍗氬紙鏍稿績澶勭悊
    virtual void update();
};

#endif // CORE_H
