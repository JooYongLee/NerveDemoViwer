#ifndef IMGITEM_H
#define IMGITEM_H
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>
#include "defineconfigure.h"
class ImgItem : public QGraphicsPixmapItem
{
public:
    QPixmap m_pixmap;
    QSize   m_imgSize;
    qreal   m_dScaled;
    QRectF  m_dScaledRect;
    QRect GetScaledRect(QPointF pnt, qreal dVariant);
    ImgItem(QGraphicsItem *parent = 0)
        :QGraphicsPixmapItem(parent),
          m_dScaled(1.0)
    {
        this->setFlag(QGraphicsItem::ItemIsSelectable,true);
    }
    ImgItem(QPixmap img)
        : QGraphicsPixmapItem(img.scaled(IMG_WIDTH,IMG_HEIGHT)),
          m_dScaled(1.0)
    {
        m_imgSize = img.size();
        m_dScaledRect = QRectF(0,0,IMG_WIDTH,IMG_HEIGHT);
        m_pixmap = img.copy();
        this->setFlag(QGraphicsItem::ItemIsSelectable,true);
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
//    void wheelEvent(QWheelEvent *event);

};
#endif // IMGITEM_H
