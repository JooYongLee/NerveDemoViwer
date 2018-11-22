#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rectdragitem.h"
#include "draggablerectitem.h"
#include <QDebug>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsProxyWidget>
#include <QFormLayout>
#include <QGraphicsLayout>
#include <QDir>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    imgslider =    new QSlider(Qt::Vertical,nullptr);
//QSlider(, this)
    imgslider->setRange(0,99);
    imgslider->setValue(50);
    imgslider->setGeometry(500,100,10,100);
    button =    new QPushButton("box compute");
    button->setGeometry(400,0,100,20);


    this->resize(1000,800);





    scene = new SceneItems(this);
    scene->setSceneRect(0,0,500,500);


    scene->addWidget(button);
    scene->addWidget(imgslider);
    //proxy->setFlag(QGraphicsItem::ItemIsSelectable,false);



    view = new QGraphicsView(scene);
    setCentralWidget(view);

    connect(imgslider,SIGNAL(valueChanged(int)),this,SLOT(imgslider_changed(int)));
    connect(button, SIGNAL(clicked(bool)),this,SLOT(buttonclicked()));

    createAction();
    createConnection();
    creatToolBar();
}
void MainWindow::imgslider_changed(int num)
{
    QString img_path = "d:/";
    QDir imgExplore(img_path);
    imgExplore.setNameFilters(QStringList()<<\
                         "*.jpg"<<\
                         "*.png"<<\
                         "*.bmp"<<
                         "*.dcm");

    QStringList file_list = imgExplore.entryList();
    if( file_list.size() > 0 )
    {
        QString path = file_list.at((int)(file_list.size()*num/100));
        path = img_path +  path;
        scene->Redraw(path);
    }
}

void MainWindow::buttonclicked()
{
    scene->ComputeBoxInImg();
    qDebug()<<"buttonclicked";
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
