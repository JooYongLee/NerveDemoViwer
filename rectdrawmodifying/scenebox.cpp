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
inline QRectF ConstrainedRect(QRectF rect1, QRectF rect2)
{
    qreal left    = qMax(rect1.left()     ,rect2.left());
    qreal right   = qMin(rect1.right()    ,rect2.right());
    qreal top     = qMax(rect1.top()      ,rect2.top());
    qreal bottom  = qMin(rect1.bottom()   ,rect2.bottom());
    return QRectF(QPointF(left,top), QPointF(right,bottom));
}
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
    pixmapitem->setPos(-50,-50);

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
        BoundingBox *imgitem =  qgraphicsitem_cast<BoundingBox*>(item);
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
bool SceneItems::_isInsideImage(QPointF pressedPos)
{
    if(pixmapitem->sceneBoundingRect().contains(origPoint))
    {
        return true;
    }
    else
    {
        return false;
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

//        qDebug()<<this->sceneRect();

        qDebug()<<origPoint<<pixmapitem->sceneBoundingRect();





        if( !itemToDrawRect && _isInsideImage(origPoint))
        {
            itemToDrawRect = new BoundingBox;

            this->addItem(itemToDrawRect);

            itemToDrawRect->setRect(rect);
            QPen mypen(Qt::red);
            itemToDrawRect->setPen(mypen);
            itemToDrawRect->setPos(0,0);
            itemToDrawRect->setSceneBoundingRect(pixmapitem->sceneBoundingRect());


//            itemToDrawRect->setBrush(QBrush(QColor("#ffa07a")));
        }
    }
    else
    {
        QList<QGraphicsItem*> all_selected_items = selectedItems();
        if( all_selected_items.size() == 1  )
        {
            QGraphicsItem *items = all_selected_items[0];
            BoundingBox *selectBox= qgraphicsitem_cast<BoundingBox *>(items);

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
SceneItems::RectVerices SceneItems::_isSelectedVertex(BoundingBox *box, QPointF refpnt, double thresh)
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
void SceneItems::_ResizeBox(BoundingBox *box, QPointF dragPoint, RectVerices vertex)
{

    QRectF boxRect = box->rect();
    QRectF rect;
    QPointF box_cetner = box->sceneBoundingRect().center();
    QRectF imgRect = this->_GetPixmapRectOnDrawing(box_cetner);
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
    rect = ConstrainedRect(imgRect,rect);
    box->setRect(rect);
}
QRectF SceneItems::_GetPixmapRectOnDrawing(QPointF preedPoint)
{
    if( pixmapitem->sceneBoundingRect().contains(preedPoint))
    {
        return pixmapitem->sceneBoundingRect();
    }
    else
    {
        return QRect();
    }
}
QGraphicsItem* SceneItems::_GetItemOnDrawing(QPointF preedPoint)
{
    if( pixmapitem->sceneBoundingRect().contains(preedPoint))
    {
        return pixmapitem;
    }
    else
    {
        return nullptr;
    }
}



void SceneItems::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    QPointF dragPoint   =   event->scenePos();
    if(sceneMode == DrawLine)
    {
        qDebug()<<"buttonDownPos"<<event->scenePos();
        if( itemToDrawRect )
        {
            qreal left  = qMin(origPoint.x(), dragPoint.x());
            qreal right = qMax(origPoint.x(), dragPoint.x());
            qreal top   = qMin(origPoint.y(), dragPoint.y());
            qreal bottom= qMax(origPoint.y(), dragPoint.y());


            QRectF rectbox(QPointF(left,top), QPointF(right,bottom));
            QRectF imgRect = _GetPixmapRectOnDrawing(origPoint);
            rectbox = ConstrainedRect(rectbox, imgRect);

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
            BoundingBox *selectBox= qgraphicsitem_cast<BoundingBox *>(items);

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

//                    QRectF boxRect = selectBox->sceneBoundingRect();

//                    QPointF box_cetner = selectBox->sceneBoundingRect().center();


//                    QRectF imgRect = this->_GetPixmapRectOnDrawing(box_cetner);
//                    QGraphicsItem *drawingitem = this->_GetItemOnDrawing(box_cetner);

//                    qreal dx = boxRect.right() > imgRect.right() ? -1 : boxRect.left() < imgRect.left() ? +1:0;
//                    qreal dy = boxRect.bottom() > imgRect.bottom() ? -1 : boxRect.top() < imgRect.top() ? +1:0;
//                    qDebug()<<dx<<dy;

                    QGraphicsScene::mouseMoveEvent(event);

//                    qDebug()<<selectBox->pos()<<dragPoint<<dragPoint-boxRect.topLeft();
//                    selectBox->setPos(dragPoint+selectBox->pos());


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
            BoundingBox *rect = qgraphicsitem_cast<BoundingBox*>(item);
            if( rect != nullptr )
            {
                removeItem(item);
                delete item;
            }
        }
    else
        QGraphicsScene::keyPressEvent(event);
}
