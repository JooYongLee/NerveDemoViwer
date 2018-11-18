#include "scenebox.h"
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QKeyEvent>
SceneItems::SceneItems(QObject* parent): QGraphicsScene(parent)
{
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
        item->setFlag(QGraphicsItem::ItemIsSelectable,
                      areControllable);
        item->setFlag(QGraphicsItem::ItemIsMovable,
                      areControllable);
    }
}
void SceneItems::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if(sceneMode == DrawLine)
    {

        origPoint = event->scenePos();
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
            itemToDrawRect->setPos(0,0);
//            itemToDrawRect->setBrush(QBrush(QColor("#ffa07a")));
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

    if( vertex == TopLeft )
    {
        QRectF rect( QPointF(dragPoint.x(), dragPoint.y()),
                     QPointF(boxRect.right(), boxRect.bottom()));
        box->setRect(rect);
    }
    else if( vertex == TopRight )
    {
        QRectF rect( QPointF(boxRect.left(), dragPoint.y()),
                     QPointF(dragPoint.x(), boxRect.bottom()) );
        box->setRect(rect);
    }
    else if( vertex == BottomLeft )
    {
        QRectF rect( QPointF(dragPoint.x(), boxRect.top()),
                     QPointF(boxRect.right(), dragPoint.y()) );
        box->setRect(rect);
    }
    else if( vertex == BottomRight )
    {
        QRectF rect( QPointF(boxRect.left(), boxRect.top()),
                     QPointF(dragPoint.x(), dragPoint.y()) );
        box->setRect(rect);
    }
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
        if( all_selected_items.size() == 1)
        {
            QGraphicsItem *items = all_selected_items[0];
            QGraphicsRectItem *selectBox= qgraphicsitem_cast<QGraphicsRectItem *>(items);

            const double rect_eidt_dist_thresh = 10.;
            RectVerices vertex;
            if( (vertex = _isSelectedVertex(selectBox, dragPoint, rect_eidt_dist_thresh)) != NoneVertex)
            {
                qDebug()<<"_isSelectedVertex"<<vertex;
                _ResizeBox(selectBox, dragPoint, vertex);
            }
            else
            {
                 qDebug()<<"Not !!!!! _isSelectedVertex";

            }


        }
        else
        {
            QGraphicsScene::mouseMoveEvent(event);

        }




    }

}

void SceneItems::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    itemToDrawRect = 0;
//    removeItem(itemToHorizonDraw);
//    removeItem(itemToVerticalDraw);
//    itemToHorizonDraw = 0;
//    itemToVerticalDraw = 0;
    QGraphicsScene::mouseReleaseEvent(event);
}

void SceneItems::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Delete)
        foreach(QGraphicsItem* item, selectedItems()){
            removeItem(item);
            delete item;
        }
    else
        QGraphicsScene::keyPressEvent(event);
}
