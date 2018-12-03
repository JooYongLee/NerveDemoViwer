
#include "boundingbox.h"
#include <QDebug>
class BoxClass
{
public:
    BoxClass();
    ~BoxClass();
};
std::map<QBoxitem::BoxClass,QString> map_boxclas;

void QBoxitem::init_map_box()
{
    map_boxclas[BACKGROUND] = "barckground";
    map_boxclas[NERVE] = "nerve";
    map_boxclas[LOWERCASE] = "lowercase";
}

void BoundingBox::SetProperty(QRectF scene_rect, QRectF bounding_rect, QUuid uid, int box_id, QBoxitem::BoxClass _id_class)
{
    this->setSceneBoundingRect(bounding_rect);
    this->setRect(scene_rect);
    this->setData(QVariant::Type::Uuid, uid);
    this->setData(QVariant::Type::Int, box_id);
    this->SetBoxClass(_id_class);
}

BoundingBox::BoundingBox(QGraphicsItem *parent, QBoxitem::BoxClass box_flag )
    :QGraphicsRectItem(parent), _boundingRect(QRectF(-100,-100,200,200)),
      id_class(box_flag)

{
    this->setFlags(QGraphicsItem::ItemIsMovable
                   | QGraphicsItem::ItemIsSelectable
                   | QGraphicsItem::ItemSendsGeometryChanges
                   //| QGraphicsItem::ItemSendsScenePositionChanges
                   );

    QPen mypen;
    if( id_class == QBoxitem::NERVE )
        mypen = QPen(QColor(130, 224, 170),2);
    else
        mypen = QPen(QColor(210, 180, 222),2);
    this->setPen(mypen);
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
