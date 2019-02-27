#include "widget.h"
#include "ui_widget.h"
#include "sprite.h"

#include <QPainter>
#include <QKeyEvent>
#include <QTimerEvent>
#include <QBitmap>
#include <QTime>
#include <QMessageBox>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_col = 32; //кол-во колонок
    m_row = 24; //кол-во строк
    m_cell_size = 16; //размер клетки

    //размер окна
    m_width_window = m_col * m_cell_size; //512
    m_height_window = m_row * m_cell_size; //384
    resize(m_width_window, m_height_window);

    //random
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));


    //голова
    m_pixmap_head.load(":/images/images/head");
    m_pixmap_headUp.load(":/images/images/headUp");
    m_pixmap_headDown.load(":/images/images/headDown");
    m_pixmap_headLeft.load(":/images/images/headLeft");
    m_pSpriteHead = new Sprite(m_pixmap_head);

    m_pSpriteHead->SetPosition(0, m_height_window/2);
    m_pSpriteHead->SetSpeed(16, 0);
    Snake.push_back(m_pSpriteHead);

    //1 элемент тела
    m_pixmap_body.load(":/images/images/body");
    m_pixmap_bodyUpDown.load(":/images/images/up_down");
    m_pSpriteBody = new Sprite(m_pixmap_body);

    m_pSpriteBody->SetPosition(- m_pSpriteBody->GetPosition().width(), m_height_window/2);
    m_pSpriteBody->SetSpeed(16, 0);
    Snake.push_back(m_pSpriteBody);

    //Хвост
    m_pixmap_tail.load(":/images/images/tail");
    m_pixmap_tailUp.load(":/images/images/tailUp");
    m_pixmap_tailDown.load(":/images/images/tailDown");
    m_pixmap_tailLeft.load(":/images/images/tailLeft");
    m_pSpriteTail = new Sprite(m_pixmap_tail);

    m_pSpriteTail->SetPosition(- m_pSpriteTail->GetPosition().width()*2, m_height_window/2);
    m_pSpriteTail->SetSpeed(16, 0);
    Snake.push_back(m_pSpriteTail);

    m_pixmap_apple.load(":/images/images/apple");
    m_pSprite_apple = new Sprite(m_pixmap_apple);
    m_pSprite_apple->SetPosition(qrand() % m_col * m_cell_size, qrand() % m_row * m_cell_size);
    m_pSprite_apple->SetSpeed(0, 0);

    //проверка на быстрое нажатие клавиш
    m_test_key = false;

    startTimer(140);
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()){
    case Qt::Key_Left:{
        if ((Snake.front()->GetSpeed().y() != 0) && m_test_key)
        {
            Snake.front()->SetPixmap(m_pixmap_headLeft);
            Snake.front()->SetSpeed(-16, 0);
        }
    } break;

    case Qt::Key_Right:{
        if ((Snake.front()->GetSpeed().y() != 0) && m_test_key)
        {
            Snake.front()->SetPixmap(m_pixmap_head);
            Snake.front()->SetSpeed(16, 0);
        }
        } break;

    case Qt::Key_Up:{
        if ((Snake.front()->GetSpeed().x() != 0) && m_test_key)
        {
            Snake.front()->SetPixmap(m_pixmap_headUp);
            Snake.front()->SetSpeed(0, -16);
        }
    } break;

    case Qt::Key_Down:{
        if ((Snake.front()->GetSpeed().x() != 0) && m_test_key)
        {
            Snake.front()->SetPixmap(m_pixmap_headDown);
            Snake.front()->SetSpeed(0, 16);
        }
    } break;
//    case Qt::Key_Space:{
//        QMessageBox::information(this, "Пауза","Вы нажали на паузу,\n чтобы продолжить, нажмите ОК");
//    } break;

    }

    m_test_key = false;
}

void Widget::timerEvent(QTimerEvent *e)
{

    //перемещение змеи
    if ((Snake[Snake.size()-2]->GetSpeed().y() > 0) && ((Snake.back()->GetSpeed().y() == 0)))
        Snake.back()->SetPixmap(m_pixmap_tailDown);
    else if ((Snake[Snake.size()-2]->GetSpeed().y() < 0) && ((Snake.back()->GetSpeed().y() == 0)))
        Snake.back()->SetPixmap(m_pixmap_tailUp);
    else if ((Snake[Snake.size()-2]->GetSpeed().x() > 0) && ((Snake.back()->GetSpeed().x() == 0)))
        Snake.back()->SetPixmap(m_pixmap_tail);
    else if ((Snake[Snake.size()-2]->GetSpeed().x() < 0) && ((Snake.back()->GetSpeed().x() == 0)))
        Snake.back()->SetPixmap(m_pixmap_tailLeft);
    Snake.back()->SetPosition(Snake[Snake.size()-2]->GetPosition());
    Snake.back()->SetSpeed(Snake[Snake.size()-2]->GetSpeed());

    for (int i = Snake.size() - 2; i > 0; i--)
    {
        if ((Snake[i-1]->GetSpeed().x() == 0) && ((Snake[i]->GetSpeed().x() != 0)))
            Snake[i]->SetPixmap(m_pixmap_bodyUpDown);
        if ((Snake[i-1]->GetSpeed().y() == 0) && ((Snake[i]->GetSpeed().y() != 0)))
            Snake[i]->SetPixmap(m_pixmap_body);
        Snake[i]->SetPosition(Snake[i-1]->GetPosition());
        Snake[i]->SetSpeed(Snake[i-1]->GetSpeed());


    }
    Snake.front()->SetPosition(Snake.front()->GetPosition().left() + Snake.front()->GetSpeed().x(),
                               Snake.front()->GetPosition().top() + Snake.front()->GetSpeed().y());



    m_test_key = true;


    //выход за границы экрана
    if (Snake.front()->GetPosition().left() < 0)
    {
        Snake.front()->SetPosition(m_width_window - Snake.front()->GetPosition().width(),
                                   Snake.front()->GetPosition().top());
    } else if (Snake.front()->GetPosition().right() > m_width_window)
    {
        Snake.front()->SetPosition(0, Snake.front()->GetPosition().top());
    } else if (Snake.front()->GetPosition().bottom() > m_height_window)
    {
        Snake.front()->SetPosition(Snake.front()->GetPosition().left(), 0);
    } else if (Snake.front()->GetPosition().top() < 0)
    {
        Snake.front()->SetPosition(Snake.front()->GetPosition().left(),
                                     m_height_window);
    }

    //змея ест яблоко
    for (int i = 0; i < Snake.size(); i++) {
        if (m_pSprite_apple->GetPosition().contains(Snake[i]->GetPosition()))
        {
            m_pSpriteBody = new Sprite(Snake[Snake.size() - 2]->GetPixmap());
            m_pSpriteBody->SetSpeed(Snake[Snake.size() - 2]->GetSpeed());
            m_pSpriteBody->SetPosition(Snake[Snake.size() - 2]->GetPosition());

            m_pSprite_apple->SetPosition(qrand() % m_col * m_cell_size, qrand() % m_row * m_cell_size);

            Snake.insert(Snake.size() - 2, m_pSpriteBody);
            break;
        }


    }


    update();
}

void Widget::paintEvent(QPaintEvent *e)
{
    //вызывается каждый раз при изменении окна
    QPainter painter(this);
    painter.setWindow(0, 0, m_width_window, m_height_window);

    //рисуем еду
    m_pSprite_apple->Draw(&painter);

    //рисуем змею
    for (int i = 0; i < Snake.size(); i++)
    {
        Snake[i]->Draw(&painter);
    }
}

Widget::~Widget()
{
    delete ui;
}
