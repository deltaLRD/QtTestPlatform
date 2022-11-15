#ifndef PVPWIDGET_H
#define PVPWIDGET_H
#include <QDebug>
#include <QWidget>
#include <QBitmap>
#include <QPainter>
#include <QKeyEvent>
#include <QMetaEnum>
#include <QPushButton>
#include<QLabel>

namespace Ui {
class pvpwidget;
}

class pvpwidget : public QWidget
{
    Q_OBJECT

public:
    explicit pvpwidget(QWidget *parent = 0);
    int getAi();
    ~pvpwidget();
protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::pvpwidget *ui;
    int w, h;
    int ai=0;

signals:
    void begin();
};

#endif // PVPWIDGET_H
