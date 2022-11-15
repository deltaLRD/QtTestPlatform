#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QDebug>
#include <QBitmap>
#include <QPainter>
#include <QFileDialog>
#include <QJsonDocument>

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    //设置标签字号
    QFont font;
    font.setPixelSize(80);
    ui->label->setFont(font);
    //设置标签颜色
    QPalette palette;
    palette.setColor(QPalette::WindowText,Qt::white);
    ui->label->setPalette(palette);

    //设置按钮字号
    font.setPixelSize(48);
    //设置按钮背景
    QPixmap pix = QPixmap(":/textures/ui/blueButton").scaledToHeight(50);
    //设置不规则按钮
    QPushButton *buttons[] = {ui->pushButton, ui->pushButton_2, ui->pushButton_3, ui->pushButton_4, ui->pushButton_5};
    for (QPushButton *btn : buttons)
    {
        btn->setFont(font);
        btn->setMask(pix.mask());
        btn->setFixedSize(pix.size());
        btn->setStyleSheet("border-image: url(:/textures/ui/blueButton)");
    }
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::resizeEvent(QResizeEvent *)
{
    w = width();
    h = height();
}

void MainWidget::paintEvent(QPaintEvent *)
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

void MainWidget::on_pushButton_2_clicked()
{
    emit startPVP();
}

void MainWidget::on_pushButton_clicked()
{
    emit exit();
}

void MainWidget::on_pushButton_3_clicked()
{
    emit setKey();
}

void MainWidget::on_pushButton_4_clicked()
{
    emit startPVE();
}

void MainWidget::on_pushButton_5_clicked()
{
    //获取output目录
    QDir dir;
    if (!dir.exists("output"))
    {
        dir.mkpath("output");
    }
    //对话框标题
    QString dlgTitle = "选择一个文件";
    //文件过滤器
    QString filter = "录像文件(*.pve *.pvp)";
    //获取文件
    QString fileName = QFileDialog::getOpenFileName(this, dlgTitle, "output", filter);
    //如果文件不为空
    if (!fileName.isEmpty())
    {
        //创建加载文件
        QFile loadFile(fileName);
        //只读打开加载文件
        if (!loadFile.open(QIODevice::ReadOnly)) {
            qDebug() << "ERROR::MainWidget::OpenRecordingFile::Failed";
            return;
        }
        //读取文件所有内容
        QByteArray saveData = loadFile.readAll();
        neb::CJsonObject oJson;
        if (oJson.Parse(saveData.toStdString()) ||
                oJson.Parse(QJsonDocument::fromBinaryData(saveData).toJson().toStdString()))
        {
            emit playRec(oJson);
        }
        else
        {
            qDebug() << "ERROR::MainWidget::ParseRecordingFile::Failed";
        }
        //关闭加载文件
        loadFile.close();
    }
}
