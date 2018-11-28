#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QDebug>
#include <QUuid>



class QBoxitem
{

public:
    enum BoxClass { BACKGROUND, NERVE, LOWERCASE };
//    explicit QBoxitem(QObject *parent ):
//        QObject(parent)
//    {}
//    QBoxitem( QObject *parent = 0) : QObject(parent){}

    QBoxitem(qreal x_min = 0, qreal y_min = 0, qreal width = 0, qreal height= 0)
        : left(x_min),
          top(y_min),
          right(x_min+width-1),
          bottom(y_min+height-1),
          id_class(NERVE)
    {
//        uid_box = QUuid::createUuid();
//        qDebug()<<uid_box;
    }
    ~QBoxitem(){}
    void setID(QUuid id)        {  uid_box = id;}
    QUuid getID() const          {   return uid_box;}
    qreal left ;
    qreal right;
    qreal top;
    qreal bottom;
    int   id_class;
private:
    QUuid  uid_box;
};

class BoxManager
{
public:
    BoxManager(){}
    virtual ~BoxManager(){}
    // interface with unique number and the number of qlistwidget
    QMap<int,QBoxitem> boxmap;
    // image file name
    QString filename;
    int     id_filename;
};

class BoundingBox : public QGraphicsRectItem
{

//    Q_OBJECT

public:
    BoundingBox(QGraphicsItem *parent = 0);
    ~BoundingBox()
    {

    }
    void SetProperty(QRectF scene_rect, QRectF bounding_rect, QUuid uid, int box_id);
    void setSceneBoundingRect(QRectF rect);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void wheelEvent(QWheelEvent *event)
    {
        qDebug()<<__FUNCTION__;
    }


    int     id_class;

    //bool m_moving;
    //void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
private :

    QRectF  _boundingRect;

};

#endif // BOUNDINGBOX_H
