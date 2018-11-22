
#include "boundingbox.h"
#include <QDebug>
BoundingBox::BoundingBox(QGraphicsItem *parent )
    :QGraphicsRectItem(parent), _boundingRect(QRectF(-100,-100,200,200))
{
    this->setFlags(QGraphicsItem::ItemIsMovable
                   | QGraphicsItem::ItemIsSelectable
                   | QGraphicsItem::ItemSendsScenePositionChanges);

//    _boundingRect \= scene()->sceneRect();

}
//void BoundingBox::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
//{
////    QGraphicsScene::mouseMoveEvent(event);
//    QGraphicsItem::mouseMoveEvent(event);
////    QGraphicsScene::mouseMoveEvent(event);
//}
void BoundingBox::setSceneBoundingRect(QRectF rect)
{
   this->_boundingRect = rect;
}
void BoundingBox::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
    QGraphicsItem::mouseMoveEvent(event);

//    qDebug()<<scene()->width()<<scene()->height();
    qDebug()<<boundingRect();
    qDebug()<<scene()->sceneRect();
//    qDebug()<<pos();
//    setPos(x(),y());
//    if( !_boundingRect.contains(this->sceneBoundingRect()))
//    {
//        setPos(_boundingRect.right() - this->sceneBoundingRect().right(),
//               _boundingRect.bottom() - this->sceneBoundingRect().bottom());
//    }
    if (x() < 0)
    {
        setPos(0, y());
    }
    else if (x() + boundingRect().right() > scene()->width())
    {
        setPos(scene()->width() - boundingRect().width(), y());
    }

    if (y() < 0)
    {
        setPos(x(), 0);
    }
    else if ( y()+ boundingRect().bottom() > scene()->height())
    {
        setPos(x(), scene()->height() - boundingRect().height());
    }
}

//QVariant BoundingBox::itemChange(GraphicsItemChange change, const QVariant &value)
//{
//    if (change == ItemPositionChange && scene()) {
//        // value is the new position.
//        QPointF newPos = value.toPointF();

//        QRectF diffRect = QRectF( QPointF(_boundingRect.left()-sceneBoundingRect().left()*1.3,
//                                          _boundingRect.top()-sceneBoundingRect().top())*1.3,
//                                  QPointF(_boundingRect.right()-sceneBoundingRect().right(),
//                                            _boundingRect.bottom()-sceneBoundingRect().bottom()));

//        QRectF rect = _boundingRect;
//        rect.setLeft(rect.left()-30);
//        rect.setTop(rect.top()-30);

//        qDebug()<<newPos;
//        qDebug()<<rect;
////        qDebug()<<_boundingRect;


//        if (!rect.contains(newPos)) {
//            // Keep the item inside the scene rect.
//            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
//            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
//            return newPos;
//        }
//    }
//    return QGraphicsItem::itemChange(change, value);
//}
