#ifndef RECTDRAGITEM_H
#define RECTDRAGITEM_H
#include <QGraphicsRectItem>
class BoundBoxItem : public QGraphicsRectItem
{

public:
    ///////////////////////////////////////
    /// \brief BoundBoxItem
    /// \param parent
    ///
    BoundBoxItem(QGraphicsItem *parent = 0);

//    QRectF	boundingRect() const;
protected:
    // void mouseMoveEvent(QGraphicsSceneMouseEvent *event);


};

#endif // RECTDRAGITEM_H
