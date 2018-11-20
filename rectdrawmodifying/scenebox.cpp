#include "scenebox.h"
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QPen>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QPushButton>
#include <QGraphicsWidget>
#include <QGraphicsLayout>
#include <QPushButton>
#define IMG_WIDTH 400
#define IMG_HEIGHT 400
class myImg : public QGraphicsPixmapItem
{
public:
    myImg(QGraphicsItem *parent = 0)
        :QGraphicsPixmapItem(parent)
    {
        this->setFlag(QGraphicsItem::ItemIsSelectable,false);
    }

};
void SceneItems::Redraw(QString path)
{
    img.load(path);
    img = img.scaled(IMG_WIDTH,IMG_HEIGHT);
    pixmapitem->setPixmap(img);

    qDebug()<<path;
}

SceneItems::SceneItems(QObject* parent):
    QGraphicsScene(parent),
    m_dragged(false),
    _selectedBoxVertex(NoneVertex)
{

    img.load("d:/yuna.jpg");
    img = img.scaled(IMG_WIDTH,IMG_HEIGHT);
    pixmapitem = new QGraphicsPixmapItem(img);
    pixmapitem->setFlag(QGraphicsItem::ItemIsSelectable,false);
    pixmapitem->setFlag(QGraphicsItem::ItemIsMovable,false);

    this->addItem(pixmapitem);


    sceneMode = NoMode;
    itemToDrawRect = 0;
}

void SceneItems::setMode(Mode mode)
{
    qDebug()<<__FUNCTION__<<mode;
    sceneMode = mode;
    QGraphicsView::DragMode vMode = QGraphicsView::NoDrag;

    if(mode == DrawLine){
        makeItemsControllable(false);
        vMode = QGraphicsView::NoDrag;
    }
    else if(mode == SelectObject){
        makeItemsControllable(true);
        vMode = QGraphicsView::RubberBandDrag;
    }
    QGraphicsView* mView = views().at(0);
    if(mView)
        mView->setDragMode(vMode);
}

void SceneItems::makeItemsControllable(bool areControllable){
    foreach(QGraphicsItem* item, items()){
        QGraphicsRectItem *imgitem =  qgraphicsitem_cast<QGraphicsRectItem*>(item);
        if( imgitem != nullptr )
        {
            item->setFlag(QGraphicsItem::ItemIsSelectable,
                          areControllable);
            item->setFlag(QGraphicsItem::ItemIsMovable,
                          areControllable);
        }
        else
        {
            qDebug()<<"---------------------------";

        }


    }
}
void SceneItems::ComputeBoxInImg()
{
    QList<QGraphicsItem*> colItems = collidingItems(pixmapitem);
    if(colItems.isEmpty())
    {
        qDebug()<<"empty colliding";
//        this->setPos(anchorPoint);
    }
    else
    {
        foreach(QGraphicsItem* item, colItems)
        {
            qDebug()<<item->sceneBoundingRect();
        }

        qDebug()<<"colliding item exists";
/*        QGraphicsItem* closestItem = colItems.at(0);
        qreal shortestDist = 100000;
        foreach(QGraphicsItem* item, colItems){
            QLineF line(item->sceneBoundingRect().center(),
                        this->sceneBoundingRect().center());
            if(line.length() < shortestDist){
                shortestDist = line.length();
                closestItem = item;
            }
        }
        this->setPos(closestItem->scenePos())*/;
    }

}

void SceneItems::mousePressEvent(QGraphicsSceneMouseEvent *event){
    m_dragged = true;
    origPoint = event->scenePos();
    if(sceneMode == DrawLine)
    {


        QRectF rect( origPoint.x(),
                     origPoint.y(),
                     0,0);

        qDebug()<<this->sceneRect();

        qDebug()<<origPoint;

        if( !itemToDrawRect)
        {
            itemToDrawRect = new QGraphicsRectItem;
            this->addItem(itemToDrawRect);

            itemToDrawRect->setRect(rect);
            QPen mypen(Qt::red);
            itemToDrawRect->setPen(mypen);
            itemToDrawRect->setPos(0,0);
//            itemToDrawRect->setBrush(QBrush(QColor("#ffa07a")));
        }
    }
    else
    {
        QList<QGraphicsItem*> all_selected_items = selectedItems();
        if( all_selected_items.size() == 1  )
        {
            QGraphicsItem *items = all_selected_items[0];
            QGraphicsRectItem *selectBox= qgraphicsitem_cast<QGraphicsRectItem *>(items);

            if(selectBox != nullptr)
            {
                const double rect_eidt_dist_thresh = 10.;
                _selectedBoxVertex = _isSelectedVertex(selectBox, origPoint, rect_eidt_dist_thresh);
                qDebug()<<"compute vertex";
            }
        }



    }
    QGraphicsScene::mousePressEvent(event);
}
double calc_distance(QPointF p1, QPointF p2)
{
    return sqrt((p1.x() - p2.x() )*(p1.x() - p2.x() ) + (p1.y() - p2.y())*(p1.y() - p2.y()));
}
SceneItems::RectVerices SceneItems::_isSelectedVertex(QGraphicsRectItem *box, QPointF refpnt, double thresh)
{
    QPointF MovedPnt = box->pos();

    QPointF TopLeftPnt = box->rect().topLeft();
    QPointF TopRihgtPnt = box->rect().topRight();
    QPointF BottomLeftPnt = box->rect().bottomLeft();
    QPointF BottomRightPnt = box->rect().bottomRight();

    TopLeftPnt += MovedPnt;
    TopRihgtPnt += MovedPnt;
    BottomLeftPnt += MovedPnt;
    BottomRightPnt += MovedPnt;

    double radisus_top_left = calc_distance(refpnt, TopLeftPnt);
    double radisus_top_right = calc_distance(refpnt, TopRihgtPnt);
    double radisus_bottom_left = calc_distance(refpnt, BottomLeftPnt);
    double radisus_bottom_right = calc_distance(refpnt, BottomRightPnt);

    int    selected_vertex = NoneVertex;

    if( radisus_top_left < thresh )              selected_vertex =   TopLeft;
    else if( radisus_top_right < thresh )        selected_vertex =   TopRight;
    else if( radisus_bottom_left < thresh )      selected_vertex =   BottomLeft;
    else if( radisus_bottom_right < thresh )     selected_vertex =   BottomRight;

    return SceneItems::RectVerices(selected_vertex);
}
void SceneItems::_ResizeBox(QGraphicsRectItem *box, QPointF dragPoint, RectVerices vertex)
{

    QRectF boxRect = box->rect();
    QRectF rect;
    if( vertex == TopLeft )
    {
//        views()[0]->setCursor(Qt::CrossCursor);
        rect = QRectF( QPointF(dragPoint.x(), dragPoint.y()),
                     QPointF(boxRect.right(), boxRect.bottom()));

    }
    else if( vertex == TopRight )
    {
        rect = QRectF( QPointF(boxRect.left(), dragPoint.y()),
                     QPointF(dragPoint.x(), boxRect.bottom()) );

    }
    else if( vertex == BottomLeft )
    {
        rect = QRectF( QPointF(dragPoint.x(), boxRect.top()),
                     QPointF(boxRect.right(), dragPoint.y()) );

    }
    else if( vertex == BottomRight )
    {

        rect = QRectF( QPointF(boxRect.left(), boxRect.top()),
                     QPointF(dragPoint.x(), dragPoint.y()) );
    }
    else
    {
        //return;
    }
    box->setRect(rect);
}
void SceneItems::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    QPointF dragPoint   =   event->scenePos();
    if(sceneMode == DrawLine)
    {
        qDebug()<<"buttonDownPos"<<event->scenePos();
        if( itemToDrawRect )
        {
            qreal left  = origPoint.x() < dragPoint.x() ? origPoint.x():dragPoint.x();
            qreal right = origPoint.x() > dragPoint.x() ? origPoint.x():dragPoint.x();
            qreal top   = origPoint.y() < dragPoint.y() ? origPoint.y():dragPoint.y();
            qreal bottom= origPoint.y() > dragPoint.y() ? origPoint.y():dragPoint.y();

            QRectF rectbox(QPointF(left,top), QPointF(right,bottom));

            itemToDrawRect->setRect(rectbox);
        }
    }
    else
    {
        QList<QGraphicsItem*> all_selected_items = selectedItems();
//        qDebug()<<"selected items"<<all_selected_items.size();
        if( all_selected_items.size() == 1  && m_dragged )
        {
            QGraphicsItem *items = all_selected_items[0];
            QGraphicsRectItem *selectBox= qgraphicsitem_cast<QGraphicsRectItem *>(items);

            if(selectBox != nullptr)
            {
                const double rect_eidt_dist_thresh = 10.;
//                 _vertex;


                if( _selectedBoxVertex!= NoneVertex)
                {
                    qDebug()<<"_isSelectedVertex"<<_selectedBoxVertex<<origPoint;
                    _ResizeBox(selectBox, dragPoint, _selectedBoxVertex);
                }
                else
                {
                     qDebug()<<"Not !!!!! _isSelectedVertex";
                   //  QGraphicsScene::mouseMoveEvent(event);
                }
            }
        }
        else
        {
            QGraphicsScene::mouseMoveEvent(event);
        }

    }

}

void SceneItems::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    m_dragged = false;
    _selectedBoxVertex = NoneVertex;
    itemToDrawRect = 0;
    qDebug()<<"releaseed!!!!!!!!!!!!!!!!!!!!";
//    views()[0]->setCursor(Qt::ArrowCursor);
//    removeItem(itemToHorizonDraw);
//    removeItem(itemToVerticalDraw);
//    itemToHorizonDraw = 0;
//    itemToVerticalDraw = 0;
    QGraphicsScene::mouseReleaseEvent(event);
}

void SceneItems::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Delete)
        foreach(QGraphicsItem* item, selectedItems()){
            QGraphicsRectItem *rect = qgraphicsitem_cast<QGraphicsRectItem*>(item);
            if( rect != nullptr )
            {
                removeItem(item);
                delete item;
            }
        }
    else
        QGraphicsScene::keyPressEvent(event);
}
