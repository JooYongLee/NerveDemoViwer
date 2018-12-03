#include "rectdragitem.h"

BoundBoxItem::BoundBoxItem(QGraphicsItem* parent)
    : QGraphicsRectItem(parent)

{
    setFlags(QGraphicsItem::ItemIsSelectable|
             QGraphicsItem::ItemIsMovable);

}
