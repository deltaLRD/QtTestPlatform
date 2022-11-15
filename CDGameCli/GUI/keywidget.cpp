#include "keywidget.h"
#include "ui_keywidget.h"

KeyWidget::KeyWidget(int (&settings)[15], QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeyWidget),
    settings(settings)
{
    ui->setupUi(this);

    //设置按钮字号
    QFont font;
    font.setPixelSize(48);
    //设置按钮背景
    QPixmap pix = QPixmap(":/textures/ui/blueButton").scaledToHeight(50);
    ui->pushButton_11->setFont(font);
    ui->pushButton_11->setMask(pix.mask());
    ui->pushButton_11->setFixedSize(pix.size());
    ui->pushButton_11->setStyleSheet("border-image: url(:/textures/ui/blueButton)");

    //存储所有按钮
    buttons.push_back(ui->pushButton);
    buttons.push_back(ui->pushButton_2);
    buttons.push_back(ui->pushButton_3);
    buttons.push_back(ui->pushButton_4);
    buttons.push_back(ui->pushButton_5);
    buttons.push_back(ui->pushButton_6);
    buttons.push_back(ui->pushButton_7);
    buttons.push_back(ui->pushButton_8);
    buttons.push_back(ui->pushButton_9);
    buttons.push_back(ui->pushButton_10);
    buttons.push_back(ui->pushButton_12);
    buttons.push_back(ui->pushButton_13);
    buttons.push_back(ui->pushButton_14);
    buttons.push_back(ui->pushButton_15);
    //设置按钮字号
    font.setPixelSize(30);
    //枚举转String
    keyEnum = QMetaEnum::fromType<Qt::Key>();
    //设置按钮样式，连接事件
    for (int i = 0; i < buttons.length(); i++)
    {
        buttons[i]->setFixedSize(600, 80);
        buttons[i]->setFont(font);
        btnText[i] = buttons[i]->text();
        buttons[i]->setText(btnText[i].arg(keyEnum.valueToKey(settings[i])).replace("Key_", ""));
        //添加按钮单击事件
        connect(buttons[i], &QPushButton::clicked, [this, i, &settings]()
        {
            //如果转换按钮则还原原按钮
            if (currentButton != -1)
            {
                buttons[currentButton]->setText(
                            btnText[currentButton].arg(keyEnum.valueToKey(settings[currentButton])).replace("Key_", ""));
            }
            //记录单击按钮
            currentButton = i;
            buttons[i]->setText("enter hotkey. esc to cancel.");
        });
    }
    ui->pushButton_16->setFixedSize(600, 80);
    ui->pushButton_16->setFont(font);
    btnText[14] = ui->pushButton_16->text();
    ui->pushButton_16->setText(btnText[14].arg("enable"));
    //添加按钮单击事件
    connect(ui->pushButton_16, &QPushButton::clicked, [this, &settings]()
    {
        QPushButton *btn = ui->pushButton_16;
        if (btn->isChecked())
        {
            settings[14] = 0;
            btn->setText(btnText[14].arg("disable"));
        }
        else
        {
            settings[14] = 1;
            btn->setText(btnText[14].arg("enable"));
        }
    });
    //设置布局样式
    ui->verticalLayout->setSpacing(0);
    ui->verticalLayout->setContentsMargins(0, 0, 0, 0);
    ui->verticalLayout_2->setSpacing(0);
    ui->verticalLayout_2->setContentsMargins(0, 0, 0, 0);
}

KeyWidget::~KeyWidget()
{
    delete ui;
}

void KeyWidget::resizeEvent(QResizeEvent *)
{
    w = width();
    h = height();
}

void KeyWidget::keyPressEvent(QKeyEvent *event)
{
    //自动重复则忽略
    if (event->isAutoRepeat()) return;
    //如果有按钮被选中
    if (currentButton != -1)
    {
        //当前选中按钮弹起
        currentButtonBounce();
        //如果为Esc则不改变按键
        if (event->key() != Qt::Key_Escape)
        {
            //检查按键是否重复
            int i;
            for (i = 0; i < 14; i++)
            {
                //重复则跳出循环
                if (settings[i] == event->key())
                {
                    break;
                }
            }
            //如果按键未重复
            if (i == 14)
            {
                //设置改变按键
                settings[currentButton] = event->key();
            }
        }
        //设置按钮文本
        buttons[currentButton]->setText(
                    btnText[currentButton].arg(keyEnum.valueToKey(settings[currentButton])).replace("Key_", ""));
        //设置无按钮选中
        currentButton = -1;
    }
}

void KeyWidget::paintEvent(QPaintEvent *)
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

void KeyWidget::on_pushButton_11_clicked()
{
    if (currentButton != -1)
    {
        //当前选中按钮弹起
        currentButtonBounce();

        //设置按钮文本
        buttons[currentButton]->setText(
                    btnText[currentButton].arg(keyEnum.valueToKey(settings[currentButton])).replace("Key_", ""));
        //设置无按钮选中
        currentButton = -1;
    }
    emit goBack();
}

void KeyWidget::currentButtonBounce()
{
    buttons[currentButton]->setDown(false);
    buttons[currentButton]->setAutoExclusive(false);
    buttons[currentButton]->setChecked(false);
    buttons[currentButton]->setAutoExclusive(true);
}
