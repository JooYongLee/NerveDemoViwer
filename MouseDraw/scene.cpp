#include "scene.h"

Scene::Scene(QObject* parent): QGraphicsScene(parent)
{
    sceneMode = NoMode;
    itemToDraw = 0;
    itemToHorizonDraw = 0;
    itemToVerticalDraw = 0;
}

void Scene::setMode(Mode mode){
    sceneMode = mode;
    QGraphicsView::DragMode vMode =
            QGraphicsView::NoDrag;
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

void Scene::makeItemsControllable(bool areControllable){
    foreach(QGraphicsItem* item, items()){
        item->setFlag(QGraphicsItem::ItemIsSelectable,
                      areControllable);
        item->setFlag(QGraphicsItem::ItemIsMovable,
                      areControllable);
    }
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if(sceneMode == DrawLine)
        origPoint = event->scenePos();
    QGraphicsScene::mousePressEvent(event);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(sceneMode == DrawLine){
        if(!itemToDraw){
            itemToDraw = new QGraphicsLineItem;
            this->addItem(itemToDraw);
            itemToDraw->setPen(QPen(Qt::black, 3, Qt::SolidLine));
            itemToDraw->setPos(origPoint);
        }
        itemToDraw->setLine(0,0,
                            event->scenePos().x() - origPoint.x(),
                            event->scenePos().y() - origPoint.y());
    }
    else
    {
        if(!itemToVerticalDraw){
            itemToVerticalDraw = new QGraphicsLineItem;
            this->addItem(itemToVerticalDraw);
            itemToVerticalDraw->setPen(QPen(Qt::black, 3, Qt::DashLine));
        }
        if(!itemToHorizonDraw){
            itemToHorizonDraw = new QGraphicsLineItem;
            this->addItem(itemToHorizonDraw);
            itemToHorizonDraw->setPen(QPen(Qt::black, 3, Qt::DashLine));
        }


        int dt = 300;
        itemToHorizonDraw->setLine(event->scenePos().x()-dt,
                            event->scenePos().y(),
                            event->scenePos().x()+dt,
                            event->scenePos().y());

        itemToVerticalDraw->setLine(event->scenePos().x(),
                            event->scenePos().y()-dt,
                            event->scenePos().x(),
                            event->scenePos().y()+dt);
        QGraphicsScene::mouseMoveEvent(event);
    }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    itemToDraw = 0;
    removeItem(itemToHorizonDraw);
    removeItem(itemToVerticalDraw);
    itemToHorizonDraw = 0;
    itemToVerticalDraw = 0;
    QGraphicsScene::mouseReleaseEvent(event);
}

void Scene::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Delete)
        foreach(QGraphicsItem* item, selectedItems()){
            removeItem(item);
            delete item;
        }
    else
        QGraphicsScene::keyPressEvent(event);
}
