#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsScene>

class BoundingBox : public QGraphicsRectItem
{
public:
    BoundingBox(QGraphicsItem *parent = 0);
    ~BoundingBox()
    {

    }
    void setSceneBoundingRect(QRectF rect);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    //bool m_moving;
    //void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
private :

    QRectF  _boundingRect;


};

#endif // BOUNDINGBOX_H
