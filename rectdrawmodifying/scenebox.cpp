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

const double gloabl_rect_eidt_dist_thresh = 15.;



QRect myImg::GetScaledRect(QPointF pnt, qreal dVariant)
{    
    qreal  width = m_dScaledRect.width()  *(1+ dVariant);
    qreal  height = m_dScaledRect.height() *(1+ dVariant);
    qreal   left    =   pnt.x() - width / 2.0;
    qreal   top    =   pnt.y() - height / 2.0;

    qreal normalized_x = (pnt.x() -this->sceneBoundingRect().x() )/this->sceneBoundingRect().width();
    qreal normalized_y = (pnt.y() -this->sceneBoundingRect().y() )/this->sceneBoundingRect().height();


    if( m_dScaled < 1.0 )
    {

        m_dScaledRect.setX(m_dScaledRect.x() + m_dScaledRect.width() * normalized_x);
        m_dScaledRect.setY(m_dScaledRect.y() + m_dScaledRect.height() * normalized_y);

    }
    else
    {
        m_dScaledRect.setX(0);
        m_dScaledRect.setY(0);

    }

    m_dScaledRect.setWidth(width);
    m_dScaledRect.setHeight(height);
    qDebug()<<normalized_x<<normalized_y<<"--------"<<m_dScaled<<"---"<<m_dScaledRect;



    return QRect((int)m_dScaledRect.left(),
                 (int)m_dScaledRect.top(),
                 (int)m_dScaledRect.width(),
                 (int)m_dScaledRect.height());
}
void myImg::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
       this->setPos(-100,-100);


      painter->setBrush(QBrush(Qt::red));
      QRectF r = this->sceneBoundingRect();
      double mRadius = 10;
      r.setSize(r.size()+mRadius*QSizeF(1, 1));
      r.translate(-this->scenePos()-mRadius*QPointF(1, 1)*.5);//(mRadius*QPointF(1, 1)/2);
      qDebug()<<this->scenePos()<<r;
      painter->drawRect(r);
      QGraphicsPixmapItem::paint(painter, option, widget);

}

void myImg::wheelEvent(QWheelEvent *event)
{
    qreal dVariant = event->delta()/(125.*50);
    m_dScaled += dVariant;


        QCursor mousecursor = cursor();
        QPoint viewcursor = scene()->views().first()->mapFromGlobal(mousecursor.pos());
        QPointF scenepos = scene()->views().first()->mapToScene(viewcursor);

        qDebug()<<(scenepos - this->sceneBoundingRect().topLeft());
        qDebug()<<"mouse"<<mousecursor.pos()<<viewcursor<<scenepos;


        QPixmap img =  m_pixmap.copy(this->GetScaledRect(scenepos, dVariant));
        img = img.scaled(IMG_WIDTH,IMG_HEIGHT);
        this->setPixmap(img);
        qDebug()<<__FUNCTION__<<event->delta()<<this->offset()<<m_dScaled;

}







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
    QPixmap img;
    img.load(path);
    img = img.scaled(IMG_WIDTH,IMG_HEIGHT);
    pixmapitem->setPixmap(img);

    qDebug()<<path;
}

SceneItems::SceneItems(QObject* parent):
    QGraphicsScene(parent),
    m_dragged(false),
    _selectedBoxVertex(NoneVertex),
    GuideLineToHorizonDraw(nullptr),
    GuideLineToVerticalDraw(nullptr)
{
    QPixmap img;
    img.load("d:/dog.jpg");
    img = img.scaled(IMG_WIDTH,IMG_HEIGHT);
    pixmapitem = new myImg(img);
    pixmapitem->setFlag(QGraphicsItem::ItemIsSelectable,false);
    pixmapitem->setFlag(QGraphicsItem::ItemIsMovable,false);
//    pixmapitem->setPos(0,0);

    this->addItem(pixmapitem);
//    pixmapitem->paint();


    sceneMode = NoMode;
    itemBoundingBox = 0;


    createGuideLine();


}
void SceneItems::createGuideLine()
{
    if(!GuideLineToHorizonDraw){
        GuideLineToHorizonDraw = new QGraphicsLineItem;
        this->addItem(GuideLineToHorizonDraw);
        GuideLineToHorizonDraw->setPen(QPen(Qt::blue, 1, Qt::DashDotLine));
        GuideLineToHorizonDraw->setPos(QPointF(0,0));
    }
    if(!GuideLineToVerticalDraw){
        GuideLineToVerticalDraw = new QGraphicsLineItem;
        this->addItem(GuideLineToVerticalDraw);
        GuideLineToVerticalDraw->setPen(QPen(Qt::blue, 1, Qt::DashDotLine));
        GuideLineToVerticalDraw->setPos(QPointF(0,0));
    }
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
//            qDebug()<<"---------------------------";

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

        qDebug()<<origPoint<<pixmapitem->sceneBoundingRect();





        if( !itemBoundingBox && _isInsideImage(origPoint))
        {
            itemBoundingBox = new BoundingBox;

            this->addItem(itemBoundingBox);

            itemBoundingBox->setRect(rect);
            QPen mypen(Qt::red);
            itemBoundingBox->setPen(mypen);
            itemBoundingBox->setPos(0,0);
            itemBoundingBox->setSceneBoundingRect(pixmapitem->sceneBoundingRect());


//            itemBoundingBox->setBrush(QBrush(QColor("#ffa07a")));
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

                _selectedBoxVertex = _isSelectedVertex(selectBox, origPoint, gloabl_rect_eidt_dist_thresh);
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



    dragPoint.setX(dragPoint.x() - box->pos().x());
    dragPoint.setY(dragPoint.y() - box->pos().y());




    QRectF rect = boxRect;
    QPointF box_cetner = box->sceneBoundingRect().center();
    QRectF imgRect = this->_GetPixmapRectOnDrawing(box_cetner);
    if( vertex == TopLeft )
    {
        qDebug()<<"before"<<rect;
//        views()[0]->setCursor(Qt::CrossCursor);
        rect = QRectF( QPointF(dragPoint.x(), dragPoint.y()),
                     QPointF(boxRect.right(), boxRect.bottom()));

//        rect.setWidth(rect.width()+box->pos().x());
//        rect.setHeight(rect.height()+box->pos().y());
        qDebug()<<"after"<<rect;

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
//        rect.setWidth(rect.width()-box->pos().x());
//        rect.setHeight(rect.height()-box->pos().y());

    }
    else
    {
        //return;
    }

    rect = ConstrainedRect(imgRect,rect);
//    rect.moveTo(box->pos());


     box->setRect( rect);


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


void SceneItems::DrawGuideLine(QPointF center)
{
    qreal dTop    = 0;
    qreal dLeft   = 0;
    qreal dRight  = 0;
    qreal dBottom = 0;
    if( this->_GetItemOnDrawing(center) != nullptr)
    {
        QRectF imgRect = _GetPixmapRectOnDrawing(center);

        dTop    = qAbs( center.y()  - imgRect.top() );
        dLeft   = qAbs( center.x()  - imgRect.left());
        dRight  = qAbs( imgRect.right() - center.x()    ) ;
        dBottom = qAbs( imgRect.bottom()  - center.y()    );

        GuideLineToHorizonDraw->setLine(
                            center.x()-dLeft,
                            center.y(),
                            center.x()+dRight,
                            center.y());

        GuideLineToVerticalDraw->setLine(
                            center.x(),
                            center.y()-dTop,
                            center.x(),
                            center.y()+dBottom);
        GuideLineToHorizonDraw->show();
        GuideLineToVerticalDraw->show();

    }
    else
    {
        GuideLineToHorizonDraw->hide();
        GuideLineToVerticalDraw->hide();
    }
}

void SceneItems::mouseMoveEvent(QGraphicsSceneMouseEvent *event){


    DrawGuideLine(event->scenePos());

    QPointF dragPoint   =   event->scenePos();
    if(sceneMode == DrawLine)
    {
        qDebug()<<"buttonDownPos"<<event->scenePos();
        if( itemBoundingBox )
        {
            qreal left  = qMin(origPoint.x(), dragPoint.x());
            qreal right = qMax(origPoint.x(), dragPoint.x());
            qreal top   = qMin(origPoint.y(), dragPoint.y());
            qreal bottom= qMax(origPoint.y(), dragPoint.y());


            QRectF rectbox(QPointF(left,top), QPointF(right,bottom));
            QRectF imgRect = _GetPixmapRectOnDrawing(origPoint);
            rectbox = ConstrainedRect(rectbox, imgRect);

            itemBoundingBox->setRect(rectbox);
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

                //                 _vertex;


                if( _selectedBoxVertex!= NoneVertex)
                {
                    qDebug()<<"_isSelectedVertex"<<selectBox->sceneBoundingRect();
                    //                    selectBox->m_moving = false;
                    _ResizeBox(selectBox, dragPoint, _selectedBoxVertex);
                    //                    selectBox->m_moving = true;
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
                    qDebug()<<selectBox->sceneBoundingRect();

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
QRectF SceneItems::_GetBoundingRectOnImg()
{
    if( itemBoundingBox )
    {
        QRectF imgRect = this->_GetPixmapRectOnDrawing(this->origPoint);
        qreal left = imgRect.left() - origPoint.x();
        qreal top = imgRect.top() - origPoint.y();
        qreal right = imgRect.right() - itemBoundingBox->sceneBoundingRect().right();
        qreal bottom = imgRect.bottom() - itemBoundingBox->sceneBoundingRect().bottom();
        return QRectF(QPointF(left,top), QPointF(right,bottom));
    }
    else
    {
        return this->sceneRect();
    }
}
void SceneItems::wheelEvent(QWheelEvent* event)
{
    qDebug()<<__FUNCTION__;
    pixmapitem->wheelEvent(event);

    QList<QGraphicsItem*> colItems = collidingItems(pixmapitem);//, Qt::ItemSelectionMode::);




    ////        this->setPos(anchorPoint);
    //    }
    //    else
    //    {
    foreach(QGraphicsItem* item, colItems)
    {
        qDebug()<<"colliding item exists"<<pixmapitem->m_dScaled;
        if( pixmapitem->m_dScaled < 0.75)
        {
            item->setVisible(false);
//            item->setPos(100,100);
            qDebug()<<"hidden";
        }

        qDebug()<<item->sceneBoundingRect();
    }
//    qDebug()<<"empty colliding";
    foreach(QGraphicsItem* allitem, items())
    {
        BoundingBox *box = qgraphicsitem_cast<BoundingBox*>(allitem);
        if(box!=nullptr)
        {
            if( pixmapitem->m_dScaled >= 0.75)
            {
                box->setVisible(true);
                qDebug()<<"show";
            }

        }

    }

}

void SceneItems::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    m_dragged = false;
    _selectedBoxVertex = NoneVertex;
    QRectF boundaryRect = this->_GetBoundingRectOnImg();
    if( itemBoundingBox )
    {
        emit valuechanged(&QBoxitem());

        itemBoundingBox->setSceneBoundingRect(boundaryRect);
    }

    itemBoundingBox = 0;
    qDebug()<<"releaseed!!!!!!!!!!!!!!!!!!!!";

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
