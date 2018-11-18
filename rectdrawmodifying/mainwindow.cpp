#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rectdragitem.h"
#include "draggablerectitem.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(1000,800);
    scene = new SceneItems(this);
    scene->setSceneRect(0,0,900,700);

    view = new QGraphicsView(scene);
//    view->setScene(scene_extra);
//    view->setScene();

    setCentralWidget(view);


    createAction();
    createConnection();
    creatToolBar();
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::createAction()
{
    this->selectAction = new QAction("Select Item",this);
    this->selectAction->setData(SceneItems::SelectObject);
    this->selectAction->setIcon(QIcon(":/icons/select.png"));
    this->selectAction->setCheckable(true);


    this->drawAction = new QAction("draw item",this);
    this->drawAction->setData(SceneItems::DrawLine);
    this->drawAction->setIcon(QIcon(":/icons/line.png"));
    this->drawAction->setCheckable(true);

    actionGroup = new QActionGroup(this);
    actionGroup->addAction(selectAction);
    actionGroup->addAction(drawAction);
    qDebug()<<"====================";
}
void MainWindow::actioniGroupClick(QAction *action)
{
    scene->setMode(SceneItems::Mode(action->data().toInt()));
}

void MainWindow::createConnection()
{
    connect(actionGroup, SIGNAL(triggered(QAction*)),
                                this, SLOT(actioniGroupClick(QAction*) ));
}
void MainWindow::creatToolBar()
{
    drawingToolBar  =   new QToolBar;
    addToolBar(Qt::TopToolBarArea, drawingToolBar);

    drawingToolBar->addAction(selectAction);
    drawingToolBar->addAction(drawAction);

}
