#include "glwidget.h"

#include "core.h"
#include "player.h"
#include "resmanager.h"

#include <QDebug>
#include <windows.h>

Core::KeyPressed keyPre[10] = {Core::P1_FORWARD, Core::P1_ROTATE_LEFT, Core::P1_ROTATE_RIGHT, Core::P1_SHOOT, Core::P1_SHIELD,
                Core::P2_FORWARD, Core::P2_ROTATE_LEFT, Core::P2_ROTATE_RIGHT, Core::P2_SHOOT, Core::P2_SHIELD};

GLWidget::GLWidget(int (&settings)[15], QWidget *parent) : QOpenGLWidget(parent),
    settings(settings), core(nullptr), player(nullptr)
{
    //设置请求格式
    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setSamples(4);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);
}

GLWidget::~GLWidget()
{
    makeCurrent();
    {
        //释放OpenGL相关资源
        ResManager::freeGLOBJ();
    }
    doneCurrent();
}

void GLWidget::startGame(GameType type, const neb::CJsonObject &jsonObject)
{
    //如果传入Json数组则播放录像
    if (!jsonObject.IsEmpty())
    {
        core = new Player(jsonObject, this);
    }
    else
    {
        core = new Core(this);
    }
    //尝试获取播放器
    player = dynamic_cast<Player *>(core);
    //设置开始游戏
    mainScene->startGame(core);
    //连接创建球信号和槽
    core->newBall.bind(&MainScene::newBall, mainScene);
    //连接创建飞机信号和槽
    core->newPlane.bind(&MainScene::newPlane, mainScene);
    //链接飞机分数信号和槽
    core->scoreChange.bind(&MainScene::scoreChange, mainScene);
    //连接游戏结束信号和槽
    core->gameOver.bind(&GLWidget::gameOver, this);
    //开始游戏
    core->initial(type == PVE ? Core::PVE : Core::PVP, settings[14]);
}

void GLWidget::gameOver()
{
    if(core)
    {
        core->deleteLater();
        core = nullptr;
    }
    player = nullptr;
    mainScene->gameOver();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    //自动重复则忽略
    if (event->isAutoRepeat()) return;
    //判断按下的按键
    if (event->key() == settings[10])
    {
        gameOver();
        //按Esc退出游戏
        emit goBack();
        return;
    }
    if (core)
    {
        //游戏时接收飞机按键
        for (int i = 0; i < 10 ; i++)
        {
            //设置按键按下状态
            if (event->key() == settings[i])
            {
                core->setKeyState(keyPre[i], true);
                return;
            }
        }
    }
    if (player)
    {
        if (event->key() == settings[11])
        {
            player->suspend();
        }
        else if (event->key() == settings[12])
        {
            player->decelerate();
        }
        else if (event->key() == settings[13])
        {
            player->accelerate();
        }
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    //自动重复则忽略
    if (event->isAutoRepeat()) return;
    //判断释放的按键
    if (core)
    {
        //游戏时接收飞机按键
        for (int i = 0; i < 10 ; i++)
        {
            //设置按键按下状态
            if (event->key() == settings[i])
            {
                core->setKeyState(keyPre[i], false);
                return;
            }
        }
    }
}
void GLWidget::mousePressEvent(QMouseEvent *)
{
    //游戏结束点击返回
    if (!core)
    {
        emit goBack();
    }
}

void GLWidget::initializeGL()
{
    //初始化OpenGLFunctions
    initializeOpenGLFunctions();
    //启用多采样
    glEnable(GL_MULTISAMPLE);
    //启用面剔除
    glEnable(GL_CULL_FACE);
//    //启用深度测试
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LEQUAL);
    //启用混合
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //配置背景颜色
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    //加载渲染器
    renderer = new Renderer(this);
    //创建主场景
    mainScene = new MainScene(this);
    this->hide();
}

void GLWidget::resizeGL(int w, int h)
{
    renderer->resize(w, h);
    mainScene->resize(w, h);
}

void GLWidget::paintGL(){
    //清空颜色缓冲
    glClear(GL_COLOR_BUFFER_BIT/* | GL_DEPTH_BUFFER_BIT*/);
    //渲染器开始绘制
    renderer->begin();
    {
        //绘制主场景
        mainScene->render(renderer);
    }
    //渲染器结束绘制
    renderer->end();
    //安排绘制
    update();
}
