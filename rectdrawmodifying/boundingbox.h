#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QDebug>
class BoundingBox : public QGraphicsRectItem
{

//    Q_OBJECT
public:
    BoundingBox(QGraphicsItem *parent = 0);
    ~BoundingBox()
    {

    }
    void setSceneBoundingRect(QRectF rect);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void wheelEvent(QWheelEvent *event)
    {
        qDebug()<<__FUNCTION__;
    }

    //bool m_moving;
    //void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
private :

    QRectF  _boundingRect;




};

#endif // BOUNDINGBOX_H
