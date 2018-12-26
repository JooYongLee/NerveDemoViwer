#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QDebug>
#include <QUuid>

#include "defineconfigure.h"


class QBoxitem
{

public:
    // box class
    enum BoxClass { BACKGROUND, NERVE, LOWERCASE };
    enum PointRegion { LEFTTOP, RIGHTTOP, LEFTBOTTOM, RIGHTBOTTOM, LEFT, RIGHT};


    static void init_map_box();
    static bool CheckBalanceBox(QList<QBoxitem> boxes);
    static bool CheckBalanceBox(QList<QBoxitem> boxes, ImgStatus &imgstatus);
    static bool CheckBalanceBox(QList<QBoxitem> boxes, ImgStatus &imgstatus, int &nerveCounter, int &lowercaseCounter);

    QBoxitem(qreal x_min = 0, qreal y_min = 0, qreal width = 0, qreal height= 0)
        : left(x_min),
          top(y_min),
          right(x_min+width-1),
          bottom(y_min+height-1),
          id_class(NERVE)
    {
        box = QRectF(x_min, y_min,width,height);
        uid_box = QUuid::createUuid();
//        uid_box = QUuid::createUuid();
//        qDebug()<<uid_box;
    }
    bool isValidBox()
    {
        if( right > left && bottom > top )
        {
            return true;
        }
        else
        {
            return false;
        }


    }

    ~QBoxitem(){}


//    QDebug operator <<(QDebug dbg, const QBoxitem &box);

    // unique id of the box, to used to check between drawing item and item of list
    void setID(QUuid id)            { uid_box = id;}
    QUuid getID() const             { return uid_box;}

    // set the BoxClass
    void setClass(BoxClass id)      { id_class = id;}
    int  getClass()                 { return id_class ;}

    double getAerea();
    bool isValidArea();

    QBoxitem copy();

    PointRegion WhereRegion(QSize imgsize);
    PointRegion WhereRegion(qreal img_width, qreal img_height);

    // boxes info
    qreal left ;
    qreal right;
    qreal top;
    qreal bottom;
    // it is the identical number with list from viewer.
    int   id_class;
    QRectF box;
private:

    QUuid  uid_box;
};

extern std::map<QBoxitem::BoxClass,QString> map_boxclas;
class BoxManager
{
public:
    BoxManager(){}
    virtual ~BoxManager(){}
    // interface with unique number and the number of qlistwidget
    QBoxitem GetBox(QBoxitem::PointRegion checkRegion, QBoxitem::BoxClass boxid, QSize imgSize);
    QList<QBoxitem> boxmap;
    QSize   imgSize;
    // image file name
    QString filename;
    int     id_filename;
    void sort();
    QList<QBoxitem> pop(QBoxitem::BoxClass boxid);
    int count(QBoxitem::BoxClass boxid);
};

class BoundingBox : public QGraphicsRectItem
{

//    Q_OBJECT

public:
    BoundingBox(QGraphicsItem *parent = 0, QBoxitem::BoxClass box_flag = QBoxitem::NERVE);
    ~BoundingBox()
    {

    }
    /*
     * scene_rect : box size, position within scene
     * bounding_rect : contrained region within image
     * uid : unique id
     * box_id : the number of qlistwidget of viewer, interface with viewer
     * _id_class : box class, nerve. lowercase..etc..
    */
    void SetProperty(QRectF scene_rect, QRectF bounding_rect, QUuid uid, int box_id, QBoxitem::BoxClass _id_class);
    void setSceneBoundingRect(QRectF rect);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void wheelEvent(QWheelEvent *event)
    {
        Q_UNUSED(event);
        qDebug()<<__FUNCTION__;
    }





    void    SetBoxClass(QBoxitem::BoxClass id)   {   id_class = id; }
    int     GetBoxClass()                       {   return id_class;}

    //bool m_moving;
    //void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
private :
    int     id_class;

    QRectF  _boundingRect;

};

#endif // BOUNDINGBOX_H
