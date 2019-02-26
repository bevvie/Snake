#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QKeyEvent>
#include <QBitmap>
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

private:
    Ui::Widget *ui;
    //переменные размера окна
    int m_width_window;
    int m_height_window;

    Sprite* m_pSprite_apple;
    QPixmap m_pixmap_apple;
};

#endif // WIDGET_H
