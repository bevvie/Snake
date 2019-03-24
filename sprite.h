#ifndef SPRITE_H
#define SPRITE_H

#include <QPixmap>
#include <QPainter>


class Sprite
{
public:
    Sprite(QPixmap& pixmap);

    void Draw (QPainter* painter);

    void SetPosition(QRect& rcPosition);
    void SetPosition(QPoint ptPosition);
    void SetPosition(int x, int y);
    //возвращает координаты прямоугольника
    QRect& GetPosition() { return m_rcPosition; }

    void SetPixmap(QPixmap& pixmap);
    QPixmap& GetPixmap() { return m_pixmap; }

    void SetSpeed(QPoint ptPosition);
    void SetSpeed(int x, int y);
    //возвращает скорость
    QPoint& GetSpeed() { return m_ptSpeed; }

private:
    QRect m_rcPosition;

    //картинка в прямоугольнике
    QPixmap m_pixmap;

    //скорость прямоугольника
    QPoint m_ptSpeed;
};

#endif // SPRITE_H
