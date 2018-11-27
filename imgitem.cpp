#include "imgitem.h"

QRect ImgItem::GetScaledRect(QPointF pnt, qreal dVariant)
{
    qreal  width = m_dScaledRect.width()  *(1+ dVariant);
    qreal  height = m_dScaledRect.height() *(1+ dVariant);
    qreal   left    =   pnt.x() - width / 2.0;
    qreal   top    =   pnt.y() - height / 2.0;

    qreal normalized_x = (pnt.x() -this->sceneBoundingRect().x() )/this->sceneBoundingRect().width();
    qreal normalized_y = (pnt.y() -this->sceneBoundingRect().y() )/this->sceneBoundingRect().height();


    if( m_dScaled < 1.0 )
    {
        m_dScaledRect.setX(m_dScaledRect.x() + m_dScaledRect.width() * normalized_x);
        m_dScaledRect.setY(m_dScaledRect.y() + m_dScaledRect.height() * normalized_y);
    }
    else
    {
        m_dScaledRect.setX(0);
        m_dScaledRect.setY(0);
    }

    m_dScaledRect.setWidth(width);
    m_dScaledRect.setHeight(height);
    qDebug()<<normalized_x<<normalized_y<<"--------"<<m_dScaled<<"---"<<m_dScaledRect;

    return QRect((int)m_dScaledRect.left(),
                 (int)m_dScaledRect.top(),
                 (int)m_dScaledRect.width(),
                 (int)m_dScaledRect.height());
}
void ImgItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
       this->setPos(-100,-100);

      painter->setBrush(QBrush(QColor(192,255,0)));
      QRectF r = this->sceneBoundingRect();
      double mRadius = 10;
      r.setSize(r.size()+mRadius*QSizeF(1, 1));
      r.translate(-this->scenePos()-mRadius*QPointF(1, 1)*.5);//(mRadius*QPointF(1, 1)/2);
      qDebug()<<this->scenePos()<<r;
      painter->drawRect(r);
      QGraphicsPixmapItem::paint(painter, option, widget);
}
