#include "widget.h"
#include "ui_widget.h"
#include "sprite.h"

#include <QPainter>
#include <QKeyEvent>
#include <QTimerEvent>
#include <QBitmap>
#include <QTime>
#include <QMessageBox>
#include <QGraphicsTextItem>
#include <QMediaPlayer>
#include <QMediaPlaylist>

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

    m_pixmap_head.load(":/images/head");
    m_pixmap_headUp.load(":/images/headUp");
    m_pixmap_headDown.load(":/images/headDown");
    m_pixmap_headLeft.load(":/images/headLeft");

    m_pixmap_body.load(":/images/body");
    m_pixmap_bodyUpDown.load(":/images/up_down");

    m_pixmap_tail.load(":/images/tail");
    m_pixmap_tailUp.load(":/images/tailUp");
    m_pixmap_tailDown.load(":/images/tailDown");
    m_pixmap_tailLeft.load(":/images/tailLeft");

    m_pixmap_lU_dR.load(":/images/leftUp-downRight");
    m_pixmap_rU_dL.load(":/images/rightUp-downLeft");
    m_pixmap_uL_rD.load(":/images/upLeft-rightDown");
    m_pixmap_uR_lD.load(":/images/upRight-leftDown");

    m_pixmap_apple.load(":/images/apple");

    m_pixmap_banana.load(":/images/banana");

    GameInit();
}

void Widget::GameInit()
{
    emit NewGame();

    for (int i = 1; i < Snake.size(); i++)
    {
        delete Snake[i];
        Snake.clear();
    }

    //голова
    m_pSpriteHead = new Sprite(m_pixmap_head);
    m_pSpriteHead->SetPosition(0, m_height_window/2);
    m_pSpriteHead->SetSpeed(16, 0);
    Snake.push_back(m_pSpriteHead);

    //1 элемент тела
    m_pSpriteBody = new Sprite(m_pixmap_body);
    m_pSpriteBody->SetPosition(- m_pSpriteBody->GetPosition().width(), m_height_window/2);
    m_pSpriteBody->SetSpeed(16, 0);
    Snake.push_back(m_pSpriteBody);

    //Хвост
    m_pSpriteTail = new Sprite(m_pixmap_tail);
    m_pSpriteTail->SetPosition(- m_pSpriteTail->GetPosition().width()*2, m_height_window/2);
    m_pSpriteTail->SetSpeed(16, 0);
    Snake.push_back(m_pSpriteTail);


    m_pSprite_apple = new Sprite(m_pixmap_apple);
    m_pSprite_apple->SetPosition(qrand() % m_col * m_cell_size, qrand() % m_row * m_cell_size);
    m_pSprite_apple->SetSpeed(0, 0);

    m_pSprite_banana = new Sprite(m_pixmap_banana);
    m_pSprite_banana->SetPosition(qrand() % m_col * m_cell_size, qrand() % m_row * m_cell_size);
    m_pSprite_banana->SetSpeed(0, 0);

    //проверка на быстрое нажатие клавиш
    m_test_key = false;

    banana_on_widget = false;
    banana_timer = 0;
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()){
    case Qt::Key_Left:{
    case Qt::Key_A:
        if ((Snake.front()->GetSpeed().y() != 0) && m_test_key)
        {
            Snake.front()->SetPixmap(m_pixmap_headLeft);
            Snake.front()->SetSpeed(-16, 0);
        }
    } break;

    case (Qt::Key_Right):{
        case Qt::Key_D:
        if ((Snake.front()->GetSpeed().y() != 0) && m_test_key)
        {
            Snake.front()->SetPixmap(m_pixmap_head);
            Snake.front()->SetSpeed(16, 0);
        }
        } break;

    case Qt::Key_Up:{
        case Qt::Key_W:
        if ((Snake.front()->GetSpeed().x() != 0) && m_test_key)
        {
            Snake.front()->SetPixmap(m_pixmap_headUp);
            Snake.front()->SetSpeed(0, -16);
        }
    } break;

    case Qt::Key_Down:{
        case Qt::Key_S:
        if ((Snake.front()->GetSpeed().x() != 0) && m_test_key)
        {
            Snake.front()->SetPixmap(m_pixmap_headDown);
            Snake.front()->SetSpeed(0, 16);
        }
    } break;
    case Qt::Key_Space:{

        //пауза
        emit Pause();

    } break;

    }

    m_test_key = false;
}

void Widget::timerEvent(QTimerEvent *e)
{

    //перемещение змеи

    //хвост
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

    //тело
    for (int i = Snake.size() - 2; i > 0; i--)
    {
        if (i == 1)
        {
            if (((Snake.front()->GetPixmap() == m_pixmap_head)&&(Snake[i]->GetSpeed().y() > 0))
                || ((Snake.front()->GetPixmap() == m_pixmap_headUp)&&(Snake[i]->GetSpeed().x() < 0)))
                    Snake[i]->SetPixmap(m_pixmap_lU_dR);
            else if (((Snake.front()->GetPixmap() == m_pixmap_headUp)&&(Snake[i]->GetSpeed().x() > 0))
                     || ((Snake.front()->GetPixmap() == m_pixmap_headLeft)&&(Snake[i]->GetSpeed().y() > 0)))
                    Snake[i]->SetPixmap(m_pixmap_rU_dL);
            else if (((Snake.front()->GetPixmap() == m_pixmap_headLeft)&&(Snake[i]->GetSpeed().y() < 0))
                     || ((Snake.front()->GetPixmap() == m_pixmap_headDown)&&(Snake[i]->GetSpeed().x() > 0)))
                    Snake[i]->SetPixmap(m_pixmap_uL_rD);
            else if (((Snake.front()->GetPixmap() == m_pixmap_head)&&(Snake[i]->GetSpeed().y() < 0))
                     || ((Snake.front()->GetPixmap() == m_pixmap_headDown)&&(Snake[i]->GetSpeed().x() < 0)))
                    Snake[i]->SetPixmap(m_pixmap_uR_lD);
            else if (Snake[i]->GetSpeed().x() == 0)
                    Snake[i]->SetPixmap(m_pixmap_bodyUpDown);
            else if (Snake[i]->GetSpeed().y() == 0)
                    Snake[i]->SetPixmap(m_pixmap_body);
        }
        else
            Snake[i]->SetPixmap(Snake[i-1]->GetPixmap());

        Snake[i]->SetPosition(Snake[i-1]->GetPosition());
        Snake[i]->SetSpeed(Snake[i-1]->GetSpeed());


    }

    //голова
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
    for (int i = 0; i < Snake.size(); i++)
    {
        if (m_pSprite_apple->GetPosition().contains(Snake[i]->GetPosition()))
        {
            //воспроизведение звука
            QMediaPlayer * m_player = new QMediaPlayer(this);
            QMediaPlaylist * m_playlist = new QMediaPlaylist(m_player);

            m_playlist->addMedia(QUrl("qrc:/music/a_am.mp3"));
            m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
            m_player->setPlaylist(m_playlist);
            m_player->setVolume(30);
            m_player->play();


            m_pSpriteBody = new Sprite(Snake[Snake.size() - 2]->GetPixmap());
            m_pSpriteBody->SetSpeed(Snake[Snake.size() - 2]->GetSpeed());
            m_pSpriteBody->SetPosition(Snake[Snake.size() - 2]->GetPosition());

            m_pSprite_apple->SetPosition(qrand() % m_col * m_cell_size, qrand() % m_row * m_cell_size);

            Snake.insert(Snake.size() - 2, m_pSpriteBody);

            emit FoodEated(5);
            break;
        }
        else
            //змея ест банан
            if (banana_on_widget && m_pSprite_banana->GetPosition().contains(Snake[i]->GetPosition()))
            {
                //-------воспроизведение звукa------
                QMediaPlayer * m_player = new QMediaPlayer(this);
                QMediaPlaylist * m_playlist = new QMediaPlaylist(m_player);

                //выбор звука
                int tmp = qrand();
                if ((tmp % 7) % 2 == 0)
                    m_playlist->addMedia(QUrl("qrc:/music/b_tasty.mp3"));
                else
                    m_playlist->addMedia(QUrl("qrc:/music/b_nam.mp3"));

                m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
                m_player->setPlaylist(m_playlist);
                m_player->setVolume(30);
                m_player->play();

                //-----------------------------------

                m_pSpriteBody = new Sprite(Snake[Snake.size() - 2]->GetPixmap());
                m_pSpriteBody->SetSpeed(Snake[Snake.size() - 2]->GetSpeed());
                m_pSpriteBody->SetPosition(Snake[Snake.size() - 2]->GetPosition());

                banana_on_widget = false;

                Snake.insert(Snake.size() - 2, m_pSpriteBody);

                emit FoodEated(10);
                break;
            }
    }

    //змея ест себя
    for (int i = 1; i < Snake.size(); i++)
    {
        if (Snake.front()->GetPosition().contains(Snake[i]->GetPosition()))
        {
            //-------воспроизведение звукa------
            QMediaPlayer * m_player = new QMediaPlayer(this);
            QMediaPlaylist * m_playlist = new QMediaPlaylist(m_player);

            //выбор звука
            int tmp = qrand();
            if ((tmp % 7) % 2 == 0)
                m_playlist->addMedia(QUrl("qrc:/music/d_set.mp3"));
            else
                m_playlist->addMedia(QUrl("qrc:/music/d_skdch.mp3"));

            m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
            m_player->setPlaylist(m_playlist);
            m_player->setVolume(30);
            m_player->play();

            //-----------------------------------

            QMessageBox::about(0, "Snake", "Game over");

            delete Snake[i];
            Snake.clear();
            GameInit();
        }
    }

    if (banana_on_widget)
        ++banana_timer;
    if (banana_timer == 40)
        banana_on_widget = false;

    update();
}

void Widget::paintEvent(QPaintEvent *e)
{
    //вызывается каждый раз при изменении окна
    QPainter painter(this);
    painter.setWindow(0, 0, m_width_window, m_height_window);

    //рисуем еду
    m_pSprite_apple->Draw(&painter);

    if (banana_on_widget)
        m_pSprite_banana->Draw(&painter);

    //рисуем змею
    for (int i = 0; i < Snake.size(); i++)
    {
        Snake[i]->Draw(&painter);
    }
}

void Widget::ShowBanana()
{
    banana_on_widget = true;
    banana_timer = 0;
    m_pSprite_banana->SetPosition(qrand() % m_col * m_cell_size, qrand() % m_row * m_cell_size);
}

Widget::~Widget()
{
    delete ui;
}
