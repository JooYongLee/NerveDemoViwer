#include "rectdragitem.h"

BoundBoxItem::BoundBoxItem(QGraphicsItem* parent)
    : QGraphicsRectItem(parent)

{
    setFlags(QGraphicsItem::ItemIsSelectable|
             QGraphicsItem::ItemIsMovable);

}
//void BoundBoxItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
//{
//    QGraphicsRectItem::mouseMoveEvent(event);
//}
//QRectF BoundBoxItem::boundingRect() const
//{
//    return QRectF(0,0,100,100);

//}
