
#include "boundingbox.h"
#include <QDebug>
class BoxClass
{
public:
    BoxClass();
    ~BoxClass();
};

BoundingBox::BoundingBox(QGraphicsItem *parent )
    :QGraphicsRectItem(parent), _boundingRect(QRectF(-100,-100,200,200))
{
    this->setFlags(QGraphicsItem::ItemIsMovable
                   | QGraphicsItem::ItemIsSelectable
                   | QGraphicsItem::ItemSendsGeometryChanges
                   //| QGraphicsItem::ItemSendsScenePositionChanges
                   );
}

void BoundingBox::setSceneBoundingRect(QRectF rect)
{
   this->_boundingRect = rect;
}

QVariant BoundingBox::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene()  ) {
        QPointF newPos = value.toPointF();
        QRectF rect = _boundingRect;

        if (!rect.contains(newPos)) {
            // Keep the item inside the scene rect.
            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
            return newPos;
        }

    }

//    if( change == ItemPositionChange ||
//            change == ItemScaleHasChanged ||
//            change == ItemTransformChange ||
//            change == ItemScaleChange )
//    {
//        qDebug()<<"_______________________________________"<<__FUNCTION__;

//    }
//    qDebug()<<change<<"_______________________________________"<<__FUNCTION__;


    return QGraphicsItem::itemChange(change, value);
}

//QBoxitem BoundingBox::TranserBox()
//{
//    box = QBoxitem( this->sceneBoundingRect().left(),
//                    this->sceneBoundingRect().top(),
//                    this->sceneBoundingRect().width(),
//                    this->sceneBoundingRect().height());

//    return box;
//}
