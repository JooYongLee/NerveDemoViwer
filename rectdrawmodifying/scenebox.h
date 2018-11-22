#ifndef SCENEBOX_H
#define SCENEBOX_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "boundingbox.h"
class SceneItems : public QGraphicsScene
{
public :
    enum Mode {NoMode, SelectObject, DrawLine};
    enum RectVerices {NoneVertex, TopLeft, TopRight, BottomLeft, BottomRight};
    SceneItems(QObject *parent = 0 );
    void setMode(Mode mode);

    QGraphicsPixmapItem *pixmapitem;
    void Redraw(QString path);
    void ComputeBoxInImg();
protected:
    bool _isInsideImage(QPointF pressedPos);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
private:
    bool m_dragged;
    QPixmap img;
    QRectF _GetPixmapRectOnDrawing(QPointF preedPoint);
    QGraphicsItem* _GetItemOnDrawing(QPointF preedPoint);
    RectVerices _isSelectedVertex(BoundingBox *box, QPointF refpnt, double thresh);
    void _ResizeBox(BoundingBox *box, QPointF refpnt, RectVerices vertex);
    void makeItemsControllable(bool areControllable);
    Mode sceneMode;
    QPointF origPoint;
    BoundingBox* itemToDrawRect;
    RectVerices _selectedBoxVertex;
//    QGraphicsLineItem* itemToHorizonDraw;
//    QGraphicsLineItem* itemToVerticalDraw;

};

#endif // SCENEBOX_H
