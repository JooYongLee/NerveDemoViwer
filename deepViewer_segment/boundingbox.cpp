
#include "boundingbox.h"
#include <QDebug>
#define VALID_BOX_AREA_THRESHOLD        10
class BoxClass
{
public:
    BoxClass();
    ~BoxClass();
};
std::map<QBoxitem::BoxClass,QString> map_boxclas;

bool QBoxitem::CheckBalanceBox(QList<QBoxitem> boxes)
{
    ImgStatus _;
    bool status = CheckBalanceBox(boxes, _);
    return status;
}
QBoxitem::PointRegion QBoxitem::WhereRegion(QSize imgsize)
{
    return this->WhereRegion((qreal)imgsize.width(), (qreal)imgsize.height());
}
//QDebug QBoxitem::operator <<(QDebug dbg, const QBoxitem &box)
//{
////        dbg.nospace()<<QString("left,top,right,bottom:%1,%2,%3,%4")
////             .arg(box.left)
////             .arg(box.top)
////             .arg(box.right)
////             .arg(box.bottom);
//    return dbg.maybeSpace();
//}
QBoxitem QBoxitem::copy()
{
    QBoxitem newbox(this->left,
                    this->top,
                    this->right - this->left + 1,
                    this->bottom - this->top + 1
                );
    newbox.setClass(QBoxitem::BoxClass(this->getClass()));
    return newbox;
}

QBoxitem::PointRegion QBoxitem::WhereRegion(qreal img_width, qreal img_height)
{
    qreal box_center_x = (left + right)/2.0;
    qreal box_center_y = (top + bottom)/2.0;
    qreal img_center_x = (img_width)/2.0;
    qreal img_center_y = (img_height)/2.0;
    if( box_center_x < img_center_x )
    {
        return QBoxitem::PointRegion::LEFT;
//        if( box_center_y < img_center_y)
//        {
//            return QBoxitem::PointRegion::LEFTTOP;
//        }
//        else
//        {
//            return QBoxitem::PointRegion::LEFTBOTTOM;
//        }
    }
    else
    {
        return QBoxitem::PointRegion::RIGHT;
//        if( box_center_y < img_center_y )
//        {
//            return QBoxitem::PointRegion::RIGHTTOP;
//        }
//        else
//        {
//            return QBoxitem::PointRegion::RIGHTBOTTOM;
//        }
    }
}
bool QBoxitem::CheckBalanceBox(QList<QBoxitem> boxes, ImgStatus &imgstatus)
{
    int nerveCounter = 0;
    int lowercaseCounter = 0;
    return CheckBalanceBox(boxes, imgstatus, nerveCounter, lowercaseCounter);
}

bool QBoxitem::CheckBalanceBox(QList<QBoxitem> boxes, ImgStatus &imgstatus, int &nerveCounter, int &lowercaseCounter)
{
    nerveCounter = 0;
    lowercaseCounter = 0;
    int invalidAreaCounter = 0;
    double AverageNerveArea = 0.;
    double AverageLowercaseArea = 0.;
    foreach (QBoxitem box, boxes) {
        if( !box.isValidArea())
            invalidAreaCounter++;

        if( box.getClass() == NERVE )
        {
            AverageNerveArea += box.getAerea();
            nerveCounter++;
        }
        else
        {
            AverageLowercaseArea += box.getAerea();
            lowercaseCounter++;
        }
    }
    if( lowercaseCounter == 2 && nerveCounter <= 2 && invalidAreaCounter == 0)
    {
        if( nerveCounter > 0 )
        {

            AverageNerveArea /= nerveCounter;
            AverageLowercaseArea /= lowercaseCounter;
            qDebug()<<__FUNCTION__<<"nerve"<<AverageNerveArea<<"lowercase"<<AverageLowercaseArea;
            if( AverageNerveArea < AverageLowercaseArea )
            {
                imgstatus = ImgStatus::GOOD;
                return true;
            }
            else
            {
                imgstatus = ImgStatus::BAD;
                return false;
            }

        }
        else
        {
            imgstatus = ImgStatus::GOOD;
            return true;
        }

    }
    else
    {
        if( lowercaseCounter == 0 && nerveCounter == 0 )
            imgstatus = ImgStatus::NONE;
        else
            imgstatus = ImgStatus::BAD;

        return false;
    }
}
double QBoxitem::getAerea()
{
    qreal width = qMax(right-left,0.);
    qreal height = qMax(bottom - top,0.);
    return width * height;
}
bool QBoxitem::isValidArea()
{
    double area = this->getAerea();
    if( area > VALID_BOX_AREA_THRESHOLD)
        return true;
    else
        return false;
}

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
QBoxitem BoxManager::GetBox(QBoxitem::PointRegion checkRegion, QBoxitem::BoxClass boxid, QSize imgSize)
{
    QBoxitem box;
    foreach (QBoxitem iBox, boxmap) {
        if( iBox.getClass()==boxid)
        {
            if( iBox.WhereRegion(imgSize) == checkRegion )
            {
                return iBox;
            }
        }
    }
    return box;
}
void BoxManager::sort()
{
    int low,high;
//    QList<QBoxitem> copybox(boxmap);
    for( low = 0; low < boxmap.count()-1; low++)
    {
        for(high = low+1; high < boxmap.count();high++)
        {
            if(     boxmap[low].getClass() == QBoxitem::BoxClass::NERVE &&
                    boxmap[high].getClass() == QBoxitem::BoxClass::LOWERCASE )
            {
                boxmap.swap(low,high);
            }
        }
    }
}
 QList<QBoxitem> BoxManager::pop(QBoxitem::BoxClass boxid)
 {
     QList<QBoxitem> ext_box;
     QList<QBoxitem>::iterator it = boxmap.begin();
     while( it != boxmap.end())
     {
         if((*it).getClass() == boxid)
         {
             ext_box.append(*it);
             it = boxmap.erase(it);
         }
         else
         {
             ++it;
         }
     }
     return ext_box;
 }
int BoxManager::count(QBoxitem::BoxClass boxid)
{
    int counter = 0;
    foreach (QBoxitem box, boxmap) {
        if( box.getClass()==boxid)
        {
            counter++;
        }
    }
    return counter;
}
