#ifndef SCENEBOX_H
#define SCENEBOX_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QWheelEvent>
#include <QDebug>
#include <QGraphicsLineItem>
#include "boundingbox.h"
#define IMG_WIDTH 700
#define IMG_HEIGHT 800

class myImg : public QGraphicsPixmapItem
{    
public:
    QPixmap m_pixmap;
    qreal   m_dScaled;
    QRectF  m_dScaledRect;
    QRect GetScaledRect(QPointF pnt, qreal dVariant);
    myImg(QGraphicsItem *parent = 0)
        :QGraphicsPixmapItem(parent),
          m_dScaled(1.0)
    {
        qDebug()<<"initalized"<<m_dScaled;
        this->setFlag(QGraphicsItem::ItemIsSelectable,true);
    }
    myImg(QPixmap img)
        :QGraphicsPixmapItem(img),
          m_dScaled(1.0)
    {
        m_dScaledRect = QRectF(0,0,IMG_WIDTH,IMG_HEIGHT);
        m_pixmap = img.copy();
        this->setFlag(QGraphicsItem::ItemIsSelectable,true);
//        QRect r = boundingRect();
//        this->setShapeMode(ShapeMode::BoundingRectShape);


    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void wheelEvent(QWheelEvent *event);

};

class SceneItems : public QGraphicsScene
{
    Q_OBJECT

public :
    enum Mode {NoMode, SelectObject, DrawLine};
    enum RectVerices {NoneVertex, TopLeft, TopRight, BottomLeft, BottomRight};
    SceneItems(QObject *parent = 0 );
    void setMode(Mode mode);

    myImg *pixmapitem;
    void Redraw(QString path);
    void ComputeBoxInImg();
    void wheelEvent(QWheelEvent* event);
    void createGuideLine();
signals:
    void valuechanged();

public:
    bool _isInsideImage(QPointF pressedPos);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);
private:
    bool m_dragged;

    QGraphicsLineItem *GuideLineToHorizonDraw;
    QGraphicsLineItem *GuideLineToVerticalDraw;
    void DrawGuideLine(QPointF center);


    QRectF _GetBoundingRectOnImg();
    QRectF _GetPixmapRectOnDrawing(QPointF preedPoint);
    QGraphicsItem* _GetItemOnDrawing(QPointF preedPoint);
    RectVerices _isSelectedVertex(BoundingBox *box, QPointF refpnt, double thresh);
    void _ResizeBox(BoundingBox *box, QPointF refpnt, RectVerices vertex);
    void makeItemsControllable(bool areControllable);
    Mode sceneMode;
    QPointF origPoint;
    BoundingBox* itemBoundingBox;
    RectVerices _selectedBoxVertex;

};

#endif // SCENEBOX_H
