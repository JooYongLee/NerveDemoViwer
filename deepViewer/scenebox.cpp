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
#include "defineconfigure.h"

const double gloabl_rect_eidt_dist_thresh = 15.;



inline QRectF ConstrainedRect(QRectF rect1, QRectF rect2)
{
    qreal left    = qMax(rect1.left()     ,rect2.left());
    qreal right   = qMin(rect1.right()    ,rect2.right());
    qreal top     = qMax(rect1.top()      ,rect2.top());
    qreal bottom  = qMin(rect1.bottom()   ,rect2.bottom());
    return QRectF(QPointF(left,top), QPointF(right,bottom));
}
void SceneItems::RedrawBox(QString path,ViewConfig viewConfig)
{
    QPixmap img;
    QSize imgSize;
    if( viewConfig.imgType == ImgType::DcmImg)
    {
//        qDebug()<<__FUNCTION__<<"--------------------"<<path<<path.toInt();
        this->dcmReader.ReadPixmapFromVolume(viewConfig.viewFlag, path.toInt(),img);
    }
    else
    {
        img.load(path);
    }

    if( !img.isNull())
    {
        pixmapitem->m_imgSize = img.size();
        img = img.scaled(IMG_WIDTH,IMG_HEIGHT);
        pixmapitem->setPixmap(img);
    }
}
void SceneItems::_DeleteAllBoxItems()
{
    foreach(QGraphicsItem* item, this->items()){
        BoundingBox *rect = qgraphicsitem_cast<BoundingBox*>(item);        
        if( rect != nullptr )
        {
            removeItem(item);
            delete item;
        }
    }
}
void SceneItems::RedrawBox(QList<QBoxitem> boxitems, bool boxVisible )
{
    _DeleteAllBoxItems();

    for( int ind = 0; ind < boxitems.count(); ind++)
    {
        QBoxitem box = boxitems.at(ind);
        QBoxitem::BoxClass box_class_id = QBoxitem::BoxClass(box.getClass());
        BoundingBox *newbox = new BoundingBox(nullptr, box_class_id);
        QUuid box_uid = box.getID();
        int     box_list_id = ind;

        QRectF sceneRect = _ConvertInverseBoundingBox(&box);

        QRectF bounding_rect = _GetBoundingRectOnImg(sceneRect, sceneRect.topLeft());
//        qDebug()<<__FUNCTION__<<box.getClass();

        newbox->setVisible(boxVisible);
//        newbox->setVisible(false);
        newbox->SetProperty(sceneRect, bounding_rect, box_uid, box_list_id, box_class_id );

        this->addItem(newbox);
    }
}

void SceneItems::RedrawBox(QString path, QList<QBoxitem> boxitems, ViewConfig viewConfig, bool boxVisible )
{
    RedrawBox(path, viewConfig);

    _DeleteAllBoxItems();    

    //_ResizeBox();
    for( int ind = 0; ind < boxitems.count(); ind++)
    {
        QBoxitem box = boxitems.at(ind);
        QBoxitem::BoxClass box_class_id = QBoxitem::BoxClass(box.getClass());
        BoundingBox *newbox = new BoundingBox(nullptr, box_class_id);
        QUuid box_uid = box.getID();
        int     box_list_id = ind;

        QRectF sceneRect = _ConvertInverseBoundingBox(&box);

        QRectF bounding_rect = _GetBoundingRectOnImg(sceneRect, sceneRect.topLeft());
//        qDebug()<<__FUNCTION__<<box.getClass();

        newbox->setVisible(boxVisible);
//        newbox->setVisible(false);
        newbox->SetProperty(sceneRect, bounding_rect, box_uid, box_list_id, box_class_id );

        this->addItem(newbox);
    }

}
SceneItems::SceneItems(QObject* parent):
    QGraphicsScene(parent),
    m_dragged(false),
    _selectedBoxVertex(NoneVertex),
    GuideLineToHorizonDraw(nullptr),
    GuideLineToVerticalDraw(nullptr),
    m_nNumBoundingBoxes(0),
    m_nBoxClass(QBoxitem::NERVE)
{

    QPixmap img;
    img.load("d:/dog.jpg");
//
    pixmapitem = new ImgItem(img);
    pixmapitem->setFlag(QGraphicsItem::ItemIsSelectable,false);
    pixmapitem->setFlag(QGraphicsItem::ItemIsMovable,false);


    this->addItem(pixmapitem);



    sceneMode = NoMode;
    itemBoundingBox = 0;


    createGuideLine();


}
int SceneItems::GetSliceSize(VIEW_FLAG typeflag)
{
    return dcmReader.GetSliceSize(typeflag);
}

bool SceneItems::LoadDcmVolume(QStringList imglist, QString basepath)
{
    dcmReader.SetDicomPath(imglist, basepath);
    dcmReader.GetVolumeBuffer();
    return true;
}
void SceneItems::createGuideLine()
{
    QColor darkred(255,210,210);
    if(!GuideLineToHorizonDraw){
        GuideLineToHorizonDraw = new QGraphicsLineItem;
        this->addItem(GuideLineToHorizonDraw);
        GuideLineToHorizonDraw->setPen(QPen(darkred, 2, Qt::DashDotLine));
        GuideLineToHorizonDraw->setPos(QPointF(0,0));
    }
    if(!GuideLineToVerticalDraw){
        GuideLineToVerticalDraw = new QGraphicsLineItem;
        this->addItem(GuideLineToVerticalDraw);
        GuideLineToVerticalDraw->setPen(QPen(darkred, 2, Qt::DashDotLine));
        GuideLineToVerticalDraw->setPos(QPointF(0,0));
    }
}

void SceneItems::setMode(Mode mode)
{
//    qDebug()<<__FUNCTION__<<mode;
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

        }


    }
}
void SceneItems::ComputeBoxInImg()
{
    QList<QGraphicsItem*> colItems = collidingItems(pixmapitem);
    if(colItems.isEmpty())
    {
//        qDebug()<<"empty colliding";
//        this->setPos(anchorPoint);
    }
    else
    {
//        foreach(QGraphicsItem* item, colItems)
//        {
////            qDebug()<<item->sceneBoundingRect();
//        }

//        qDebug()<<"colliding item exists";

    }

}
bool SceneItems::_isInsideImage(QPointF pressedPos)
{
    Q_UNUSED(pressedPos);
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

//        qDebug()<<origPoint<<pixmapitem->sceneBoundingRect();




//        qDebug()<<__FUNCTION__<<QBoxitem::NERVE<<m_nBoxClass;
        if( !itemBoundingBox && _isInsideImage(origPoint))
        {
            itemBoundingBox = new BoundingBox(nullptr,QBoxitem::BoxClass(m_nBoxClass));

            itemBoundingBox->setRect(rect);
            itemBoundingBox->setPos(0,0);
            itemBoundingBox->setSceneBoundingRect(pixmapitem->sceneBoundingRect());
            itemBoundingBox->SetBoxClass(QBoxitem::BoxClass(m_nBoxClass));
//            qDebug()<<__FUNCTION__<<QBoxitem::NERVE<<m_nBoxClass;

            this->addItem(itemBoundingBox);

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
//                qDebug()<<"compute vertex";
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
//        qDebug()<<"before"<<rect;

        rect = QRectF( QPointF(dragPoint.x(), dragPoint.y()),
                     QPointF(boxRect.right(), boxRect.bottom()));


//        qDebug()<<"after"<<rect;

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

//    qDebug()<<"=============================";

    QPointF dragPoint   =   event->scenePos();
    if(sceneMode == DrawLine)
    {
//        qDebug()<<"buttonDownPos"<<event->scenePos();
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
                if( _selectedBoxVertex!= NoneVertex)
                {
//                    qDebug()<<"_isSelectedVertex"<<selectBox->sceneBoundingRect();
                    //                    selectBox->m_moving = false;
                    _ResizeBox(selectBox, dragPoint, _selectedBoxVertex);
                    //                    selectBox->m_moving = true;
                }
                else
                {
                    QGraphicsScene::mouseMoveEvent(event);
                }
            }
        }
        else
        {

            QGraphicsScene::mouseMoveEvent(event);
        }
        UpdateBoxItems();

    }

    QPointF imgPos = _ConvertScenePos2ImgPos(&event->scenePos());
//    qDebug()<<__FUNCTION__<<event->scenePos()<<pixmapitem->scenePos()<<imgPos;
    emit cursorPos(&imgPos);
//    QGraphicsScene::mouseMoveEvent(event);

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
QRectF SceneItems::_GetBoundingRectOnImg(QRectF scenerect , QPointF pos)
{
    QRectF imgRect = pixmapitem->sceneBoundingRect();
    qreal left = imgRect.left() - pos.x();
    qreal top = imgRect.top() - pos.y();
    qreal right = imgRect.right() - scenerect.right();
    qreal bottom = imgRect.bottom() - scenerect.bottom();
    return QRectF(QPointF(left,top), QPointF(right,bottom));
}

void SceneItems::UpdateBoxItems()
{
    foreach(QGraphicsItem* item, selectedItems()){
        BoundingBox *boxe = qgraphicsitem_cast<BoundingBox*>(item);
        if( boxe != nullptr )
        {
            QBoxitem boxitems = _ConvertBoundingBox(boxe);
            boxitems.setID(boxe->data(QVariant::Type::Uuid).toUuid());
            emit updateItems(&boxitems);
        }
    }
}

void SceneItems::wheelEvent(QWheelEvent* event)
{
//    qDebug()<<__FUNCTION__;
//    pixmapitem->wheelEvent(event);

    QList<QGraphicsItem*> colItems = collidingItems(pixmapitem);//, Qt::ItemSelectionMode::);




    ////        this->setPos(anchorPoint);
    //    }
    //    else
    //    {
    foreach(QGraphicsItem* item, colItems)
    {
//        qDebug()<<"colliding item exists"<<pixmapitem->m_dScaled;
        if( pixmapitem->m_dScaled < 0.75)
        {
            item->setVisible(false);
//            item->setPos(100,100);
//            qDebug()<<"hidden";
        }

//        qDebug()<<item->sceneBoundingRect();
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
//                qDebug()<<"show";
            }

        }
    }

}
QSize SceneItems::GetCurrrentOriginImgsize()
{
    return pixmapitem->m_imgSize;
}

QRectF SceneItems::_ConvertInverseBoundingBox(QBoxitem *box)
{

    QSize origin_size = pixmapitem->m_imgSize;

    QRectF imgRect = pixmapitem->sceneBoundingRect();

    qreal widthScale = imgRect.width() / origin_size.width() ;
    qreal heightScale = imgRect.height() / origin_size.height();

    qreal x_min = box->left * widthScale + imgRect.left();
    qreal x_max = box->right * widthScale + imgRect.left();

    qreal y_min = box->top * heightScale + imgRect.top();
    qreal y_max = box->bottom * heightScale + imgRect.top();

    return QRectF( QPointF(x_min, y_min),
                   QPointF(x_max, y_max)
                  );
}
QPointF SceneItems::_ConvertScenePos2ImgPos(QPointF *scenPos)
{
    QRectF imgRect =  pixmapitem->sceneBoundingRect();
//    qDebug()<<__FUNCTION__<<imgRect;
    QPointF OriginImg = imgRect.topLeft();

    QSize origin_size = pixmapitem->m_imgSize;

    qreal scale_x = origin_size.width() / imgRect.width();
    qreal scale_y = origin_size.height() / imgRect.height();

    qreal norm_x_pos = (scenPos->x() - imgRect.left()) * scale_x;
    qreal norm_y_pos =  (scenPos->y() - imgRect.top()) * scale_y;

    qreal x_pos = qMin( qMax(norm_x_pos, 0.), (qreal)origin_size.width());
    qreal y_pos = qMin( qMax(norm_y_pos, 0.), (qreal)origin_size.height());

    return QPointF(x_pos, y_pos);
}

QBoxitem SceneItems::_ConvertBoundingBox(BoundingBox *box)
{
    QRectF view_domain_rect = box->sceneBoundingRect();
    QRectF imgRect = pixmapitem->sceneBoundingRect();

//    qDebug()<<"box rect"<<view_domain_rect;
//    qDebug()<<"img rect"<<imgRect;

    QSize origin_size = pixmapitem->m_imgSize;

    qreal x_min = view_domain_rect.left() - imgRect.left();
    qreal x_max = view_domain_rect.right() - imgRect.left();
    qreal y_min = view_domain_rect.top() - imgRect.top();
    qreal y_max = view_domain_rect.bottom() - imgRect.top();

    x_min = x_min / imgRect.width() * origin_size.width();
    x_max = x_max / imgRect.width() * origin_size.width();
    y_min = y_min / imgRect.height() * origin_size.height();
    y_max = y_max / imgRect.height() * origin_size.height();



//    qDebug()<<"origin size"<<origin_size;
//    qDebug()<<"img"<<imgRect;


    QBoxitem src_domain_box(x_min,y_min,x_max-x_min+1,y_max-y_min+1);

    src_domain_box.setClass( QBoxitem::BoxClass(box->GetBoxClass()));

//    qDebug()<<__FUNCTION__<<src_domain_box.getClass()<<box->GetBoxClass();

    return src_domain_box;
}

void SceneItems::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    m_dragged = false;
    _selectedBoxVertex = NoneVertex;
    QRectF boundaryRect = this->_GetBoundingRectOnImg();

    if( itemBoundingBox )
    {
        if( itemBoundingBox->rect().width() == 0 ||\
                itemBoundingBox->rect().height() == 0)
        {
            removeItem(itemBoundingBox);
            delete itemBoundingBox;
//            qDebug()<<__FUNCTION__<<"so tiny!!!!!!!!!!!!!!!!!!!11";
        }
        else
        {
//            qDebug()<<__FUNCTION__<<origPoint<<event->scenePos();
//            qDebug()<<__FUNCTION__<<itemBoundingBox->sceneBoundingRect();
//            qDebug()<<__FUNCTION__<<itemBoundingBox->rect();

            QUuid uid = QUuid::createUuid();
            QBoxitem boxitems = _ConvertBoundingBox(itemBoundingBox);

            boxitems.setID(uid);
            itemBoundingBox->setSceneBoundingRect(boundaryRect);

            itemBoundingBox->setData(QVariant::Type::Uuid, uid);
            itemBoundingBox->setData(QVariant::Type::Int, _GetBoxCountAndIncreaseCount());
            emit valuechanged(&boxitems);
        }


        //        qDebug()<<__FUNCTION__<<itemBoundingBox->data(QVariant::Type::Uuid);
        //        qDebug()<<__FUNCTION__<<itemBoundingBox->data(QVariant::Type::Int);
    }
    itemBoundingBox = 0;

    QGraphicsScene::mouseReleaseEvent(event);
}

void SceneItems::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Delete)
        foreach(QGraphicsItem* item, selectedItems()){
            BoundingBox *rect = qgraphicsitem_cast<BoundingBox*>(item);
            if( rect != nullptr )
            {                
//                QUuid id = ();
                QUuid id = rect->data(QVariant::Type::Uuid).toUuid();
//                QUuid d = QMetaType::QUuid(a);
//                qDebug()<<__FUNCTION__<<;
                emit deletedItems(&id);
                removeItem(item);
                delete item;
            }
        }
    else
        QGraphicsScene::keyPressEvent(event);
}

//void SceneItems::changeBoxClass(QBoxitem::BoxClass id_class)
//{
//    qDebug()<<__FUNCTION__<<QBoxitem::BoxClass(id_class);
////    itemBoundingBox->SetBoxClass(id_class);
//}
