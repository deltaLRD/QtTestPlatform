#include "pvpwidget.h"
#include "ui_pvpwidget.h"
#include "GUI/glwidget.h"

pvpwidget::pvpwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::pvpwidget)
{
    ui->setupUi(this);

    //设置按钮字号
    QFont font;
    font.setPixelSize(60);
    //设置按钮背景
    QPixmap pix = QPixmap(":/textures/ui/blueButton").scaledToHeight(50);
    ui->pushButton->setFont(font);
    ui->pushButton->setMask(pix.mask());
    ui->pushButton->setFixedSize(pix.size());
    ui->pushButton->setStyleSheet("border-image: url(:/textures/ui/blueButton)");
    ui->pushButton_2->setFont(font);
    ui->pushButton_2->setMask(pix.mask());
    ui->pushButton_2->setFixedSize(pix.size());
    ui->pushButton_2->setStyleSheet("border-image: url(:/textures/ui/blueButton)");
}
void pvpwidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    //清空背景
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.drawRect(rect());

    //绘制背景
    QPixmap pix;
    pix.load(":/textures/others/background");
    painter.drawPixmap((w - h * 4 / 3) / 2, 0, h * 4 / 3, h, pix);
}

pvpwidget::~pvpwidget()
{
    delete ui;
}
void pvpwidget::resizeEvent(QResizeEvent *)
{
    w = width();
    h = height();
}
void pvpwidget::on_pushButton_clicked()
{
    emit begin();
}

void pvpwidget::on_pushButton_2_clicked()
{
    ai=(ai+1)%3;
    if(ai==2)
    ui->pushButton_2->setText("2-ai");
    if(ai==1)
    ui->pushButton_2->setText("1-ai");
    if(ai==0)
    ui->pushButton_2->setText("0-ai");
}
int pvpwidget::getAi(){
    return this->ai;
}
