#ifndef KEYWIDGET_H
#define KEYWIDGET_H

#include <QDebug>
#include <QWidget>
#include<QtWidgets>
#include <QBitmap>
#include <QPainter>
#include <QKeyEvent>
#include <QMetaEnum>
#include <QPushButton>

namespace Ui {
class KeyWidget;
}

class KeyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KeyWidget(int (&settings)[15], QWidget *parent = nullptr);
    ~KeyWidget();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private slots:
    void on_pushButton_11_clicked();

private:
    //弹起当前按下按钮
    void currentButtonBounce();

    Ui::KeyWidget *ui;
    //窗口宽高
    int w, h;
    //枚举转字符串
    QMetaEnum keyEnum;
    //记录按键
    int (&settings)[15];
    //记录按钮内容
    QString btnText[15];
    //记录所有按钮
    QList<QPushButton *> buttons;
    //记录当前按钮
    int currentButton = -1;

signals:
    void goBack();
};

#endif // KEYWIDGET_H
