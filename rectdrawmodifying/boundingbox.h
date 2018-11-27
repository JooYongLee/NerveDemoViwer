#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QDebug>
class QBoxitem : public QObject
{
    Q_OBJECT
public:
    enum BoxClass { BACKGROUND, NERVE, LOWERCASE };
    QBoxitem(qreal x_min = 0, qreal y_min = 0, qreal width = 0, qreal height = 0)
        :
          left(x_min),
          top(y_min),
          right(x_min+width-1),
          bottom(y_min+height-1),
          id_class(NERVE)

    {

    }
    ~QBoxitem(){}
    qreal left ;
    qreal right;
    qreal top;
    qreal bottom;
    int   id_class;


};

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


    int     id_class;

    QBoxitem TranserBox();

    //bool m_moving;
    //void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
private :
//    QBoxitem box;

    QRectF  _boundingRect;

};

#endif // BOUNDINGBOX_H
