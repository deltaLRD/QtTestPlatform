#include "cdgamecli.h"

#include "core.h"

#include "GUI/glwidget.h"
#include "GUI/keywidget.h"
#include "GUI/mainwidget.h"

#include <QMessageBox>
#include <QApplication>
#include <QFontDatabase>
#include <QStackedWidget>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(type);
    Q_UNUSED(context);
    QMessageBox::critical(NULL, "DEBUG", msg.toLocal8Bit());
}

CDGameCli::CDGameCli(int argc, char *argv[], std::function<void(DataStruct*data,KeyStruct*key)> aiPve,
                     std::function<void(DataStruct*data,KeyStruct*key)> aiPvp)
{
    Core::ai1_pve = aiPve;
    Core::ai1_pvp = aiPvp;

    QApplication a(argc, argv);

    //注册消息处理器
//    qInstallMessageHandler(myMessageOutput);

    //记录默认字体
    QFont defaultFont = a.font();

    //QT导入外部字体
    int nIndex = QFontDatabase::addApplicationFont(":/fonts/KenVector Future");
    if (nIndex != -1)
    {
        QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));
        if (strList.count() > 0)
        {
            QFont impFont(strList.at(0));
            a.setFont(impFont);
        }
    }

    int settings[15] = {
        Qt::Key_S, Qt::Key_A, Qt::Key_D, Qt::Key_J, Qt::Key_K,
        Qt::Key_Down, Qt::Key_Left, Qt::Key_Right, Qt::Key_1, Qt::Key_2,
        Qt::Key_F1, Qt::Key_F3, Qt::Key_F2, Qt::Key_F4, 1
    };

    //创建堆栈窗体
    QStackedWidget stackedWidget;
    //创建游戏界面
    GLWidget glWidget(settings);
    //设置按键界面
    KeyWidget keyWdiget(settings);
    //创建菜单界面
    MainWidget mainWidget;

    //连接游戏界面信号
    QObject::connect(&glWidget, &GLWidget::exit, &stackedWidget, &QWidget::close);
    QObject::connect(&glWidget, &GLWidget::goBack,[&stackedWidget, &glWidget]() {
        stackedWidget.setCurrentIndex(1);
        stackedWidget.grabKeyboard();
        glWidget.releaseMouse();
    });
    //连接菜单界面信号
    QObject::connect(&mainWidget, &MainWidget::exit, &stackedWidget, &QWidget::close);
    QObject::connect(&mainWidget, &MainWidget::startPVP,[&stackedWidget, &glWidget]() {
        stackedWidget.setCurrentIndex(0);
        glWidget.startGame(GLWidget::PVP);
        glWidget.grabKeyboard();
        glWidget.grabMouse();
    });
    QObject::connect(&mainWidget, &MainWidget::startPVE,[&stackedWidget, &glWidget]() {
        stackedWidget.setCurrentIndex(0);
        glWidget.startGame(GLWidget::PVE);
        glWidget.grabKeyboard();
        glWidget.grabMouse();
    });
    QObject::connect(&mainWidget, &MainWidget::playRec,[&stackedWidget, &glWidget](const neb::CJsonObject &jsonObject) {
        stackedWidget.setCurrentIndex(0);
        glWidget.startGame(GLWidget::PVE, jsonObject);
        glWidget.grabKeyboard();
        glWidget.grabMouse();
    });
    QObject::connect(&mainWidget, &MainWidget::setKey,[&stackedWidget, &keyWdiget]() {
        stackedWidget.setCurrentIndex(2);
        keyWdiget.grabKeyboard();
    });
    //连接按键界面信号
    QObject::connect(&keyWdiget, &KeyWidget::goBack,[&stackedWidget]() {
        stackedWidget.setCurrentIndex(1);
        stackedWidget.grabKeyboard();
    });

    //添加界面至堆栈窗体
    stackedWidget.addWidget(&glWidget);
    stackedWidget.addWidget(&mainWidget);
    stackedWidget.addWidget(&keyWdiget);

    //设置显示菜单界面
    stackedWidget.setCurrentIndex(1);
    //设置堆栈窗体边距
    stackedWidget.setContentsMargins(0, 0, 0, 0);
    //设置堆栈窗体全屏
    stackedWidget.setWindowState(Qt::WindowFullScreen);
    //显示游戏界面
    glWidget.show();
    //设置堆栈窗体可见
    stackedWidget.show();

    a.exec();
}
