#ifndef SCENEBOX_H
#define SCENEBOX_H

#include <QGraphicsScene>

class SceneItems : public QGraphicsScene
{
public :
    enum Mode {NoMode, SelectObject, DrawLine};
    enum RectVerices {NoneVertex, TopLeft, TopRight, BottomLeft, BottomRight};
    SceneItems(QObject *parent = 0 );
    void setMode(Mode mode);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
private:
    RectVerices _isSelectedVertex(QGraphicsRectItem *box, QPointF refpnt, double thresh);
    void _ResizeBox(QGraphicsRectItem *box, QPointF refpnt, RectVerices vertex);
    void makeItemsControllable(bool areControllable);
    Mode sceneMode;
    QPointF origPoint;
    QGraphicsRectItem* itemToDrawRect;
//    QGraphicsLineItem* itemToHorizonDraw;
//    QGraphicsLineItem* itemToVerticalDraw;

};

#endif // SCENEBOX_H
