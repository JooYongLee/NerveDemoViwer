#ifndef SCENEBOX_H
#define SCENEBOX_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QWheelEvent>
#include <QDebug>
#include <QGraphicsLineItem>
#include "boundingbox.h"
#include "imgitem.h"



class SceneItems : public QGraphicsScene
{
    Q_OBJECT

public :
    enum Mode {NoMode, SelectObject, DrawLine};
    enum RectVerices {NoneVertex, TopLeft, TopRight, BottomLeft, BottomRight};
    SceneItems(QObject *parent = 0 );
    void setMode(Mode mode);

    ImgItem *pixmapitem;
    void Redraw(QString path);
    void ComputeBoxInImg();
    void wheelEvent(QWheelEvent* event);
    void createGuideLine();
signals:
    void valuechanged(QBoxitem*);

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
    QBoxitem _ConvertBoundingBox(BoundingBox *box);


    QRectF _GetBoundingRectOnImg();
    // get the scenerect of  graphicspixmapitem when drawing box
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
