#include "sprite.h"

Sprite::Sprite(QPixmap& pixmap)
{
    m_pixmap = pixmap;
    m_rcPosition.setRect(0, 0, pixmap.width(), pixmap.height());
    m_ptSpeed.setX(0);
    m_ptSpeed.setY(0);
}

void Sprite::Draw(QPainter *painter)
{
    painter->drawPixmap(m_rcPosition.left(), m_rcPosition.top(), m_pixmap);
}

void Sprite::SetPosition(QRect& rcPosition)
{
    m_rcPosition.moveTo(rcPosition.left(), rcPosition.top());
}

void Sprite::SetPosition(QPoint ptPosition)
{
    m_rcPosition.moveTo(ptPosition);
}

void Sprite::SetPosition(int x, int y)
{
    m_rcPosition.moveTo(x, y);
}

void Sprite::SetPixmap(QPixmap& pixmap)
{
    m_pixmap = pixmap;
}

//----

void Sprite::SetSpeed(QPoint ptPosition)
{
    m_ptSpeed.setX(ptPosition.x());
    m_ptSpeed.setY(ptPosition.y());
}

void Sprite::SetSpeed(int x, int y)
{
    m_ptSpeed.setX(x);
    m_ptSpeed.setY(y);
}
