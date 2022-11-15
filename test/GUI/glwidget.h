#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QPushButton>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

#include "renderer.h"
#include "mainscene.h"
#include "SigSlot/sigslot.h"
#include "JSON/CJsonObject.hpp"

class Core;

class GLWidget : public QOpenGLWidget, public SigSlotBase, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    enum GameType
    {
        PVP,
        PVE,
    };
    explicit GLWidget(int (&settings)[15], QWidget *parent = nullptr);
    ~GLWidget();
    void startGame(GameType type, int ai=0,const neb::CJsonObject &jsonObject = neb::CJsonObject());

protected:
    //Qt键盘/鼠标事件
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    //QtOpenGL重载函数
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void gameOver();
    //默认按键
    int (&settings)[15];
    //游戏核心
    Core *core;
    //游戏播放器
    Player *player;
    //主场景
    MainScene *mainScene;
    //渲染器
    Renderer *renderer;

signals:
    //返回主界面
    void goBack();
    //退出游戏
    void exit();

};

#endif // GLWIDGET_H
