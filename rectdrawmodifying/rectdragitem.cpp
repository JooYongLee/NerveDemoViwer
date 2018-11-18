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
