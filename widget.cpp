#include "widget.h"
#include "ui_widget.h"
#include "sprite.h"

#include <QPainter>
#include <QKeyEvent>
#include <QTimerEvent>
#include <QBitmap>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //размер окна
    m_width_window = 720;
    m_height_window = 480;
    resize(m_width_window, m_height_window);

    m_pixmap_apple.load(":/resource/apple");
    m_pixmap_apple.setMask(m_pixmap_apple.createMaskFromColor(QColor(252, 0, 252)));
    m_pSprite_apple = new Sprite(m_pixmap_apple);

    m_pSprite_apple->SetPosition(m_width_window/2 - m_pSprite_apple->GetPosition().width()/2,
                                 m_height_window/2 - m_pSprite_apple->GetPosition().height()/2);
    m_pSprite_apple->SetSpeed(1, 0);

    startTimer(8);
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()){
    case Qt::Key_Left:{
        m_pSprite_apple->SetSpeed(-1, 0);
    } break;

    case Qt::Key_Right:{
        m_pSprite_apple->SetSpeed(1, 0);
    } break;

    case Qt::Key_Up:{
        m_pSprite_apple->SetSpeed(0, -1);
    } break;

    case Qt::Key_Down:{
        m_pSprite_apple->SetSpeed(0, 1);
    } break;

    }
}

void Widget::timerEvent(QTimerEvent *e)
{
    m_pSprite_apple->SetPosition(m_pSprite_apple->GetPosition().left() + m_pSprite_apple->GetSpeed().x(),
                                 m_pSprite_apple->GetPosition().top() + m_pSprite_apple->GetSpeed().y());
    update();
}

void Widget::paintEvent(QPaintEvent *e)
{
    //вызывается каждый раз при изменении окна
    QPainter painter(this);
    painter.setWindow(0, 0, m_width_window, m_height_window);
    m_pSprite_apple->Draw(&painter);
}

Widget::~Widget()
{
    delete ui;
}
