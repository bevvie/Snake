#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QKeyEvent>
#include <QBitmap>
#include <QVector>
#include <QLabel>

#include "sprite.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:

    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);
    void keyPressEvent(QKeyEvent *);

    //void GameInit();

public slots:
    void GameInit();
    void ShowBanana();

signals:
    void FoodEated(int cost);
    void NewGame();
    void Pause();

private:
    Ui::Widget *ui;

    //переменные размера окна
    int m_width_window;
    int m_height_window;

    int m_col;
    int m_row;
    int m_cell_size;

    QTimer* timer;
    //int timerId;

    Sprite* m_pSpriteHead;
    Sprite* m_pSpriteBody;
    Sprite* m_pSpriteTail;
    QVector<Sprite*> Snake;

    QPixmap m_pixmap_head;
    QPixmap m_pixmap_headUp;
    QPixmap m_pixmap_headDown;
    QPixmap m_pixmap_headLeft;

    QPixmap m_pixmap_body;
    QPixmap m_pixmap_bodyUpDown;

    QPixmap m_pixmap_tail;
    QPixmap m_pixmap_tailUp;
    QPixmap m_pixmap_tailDown;
    QPixmap m_pixmap_tailLeft;

    QPixmap m_pixmap_lU_dR;
    QPixmap m_pixmap_rU_dL;
    QPixmap m_pixmap_uL_rD;
    QPixmap m_pixmap_uR_lD;

    Sprite* m_pSprite_apple;
    QPixmap m_pixmap_apple;

    Sprite* m_pSprite_banana;
    QPixmap m_pixmap_banana;

    bool m_test_key;
    int banana_timer;
    bool banana_on_widget;
};

#endif // WIDGET_H
