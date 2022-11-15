#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "JSON/CJsonObject.hpp"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWidget *ui;
    //窗口宽高
    int w, h;

signals:
    //双人游戏游戏
    void startPVP();
    //单人游戏
    void startPVE();
    //播放录像
    void playRec(const neb::CJsonObject &jsonObject);
    //设置按键
    void setKey();
    //退出游戏
    void exit();
};

#endif // MAINWIDGET_H
