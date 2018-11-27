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
//void myQView::wheelEvent(QWheelEvent* event)
//{
//    qDebug()<<__FUNCTION__;


////    handleWheelOnGraphicsScene(static_cast<QGraphicsSceneWheelEvent*> (event));
//}
void MainWindow::wheelEvent(QWheelEvent* event)
{
    scene->wheelEvent(event);
        qDebug()<<__FUNCTION__;

}
void MainWindow::CreateFileMenu()
{
    QAction *newAct;
    QAction *openAct;

    newAct = new QAction(tr("&New"), this);
//    newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
//    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

//    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
//    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);

}
void MainWindow::CreateDockWidget()
{

    QDockWidget *dock = new QDockWidget(tr("Bounding Box"), this);
    QDockWidget *dock2 = new QDockWidget(tr("File List"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock2->setAllowedAreas(Qt::RightDockWidgetArea  & Qt::TopDockWidgetArea);

    boundingBoxList = new QListWidget(dock);
    boundingBoxList->addItems(QStringList()
            << "One"
            << "Two"
            << "Three"
            << "Four"
            << "Five");



    QListWidget *customerList2 = new QListWidget(dock);
    QStringList strs;
    for(int i = 0; i<100;i++)
    {
        QString s = QString("file%1").arg(i,3,10, QChar('0'));
        strs.append(s);
    }
    customerList2->addItems(strs);
    //customerList2->item()


    dock->setWidget(boundingBoxList);
    dock2->setWidget(customerList2);
    addDockWidget(Qt::RightDockWidgetArea, dock2);
    addDockWidget(Qt::RightDockWidgetArea, dock);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    CreateFileMenu();
    CreateDockWidget();





    imgslider =    new QSlider(Qt::Vertical,nullptr);

    QSlider *ss =   new QSlider(Qt::Vertical,nullptr);
    ss->setValue(50);
    ss->setRange(0,99);
    ss->setValue(50);
    ss->setGeometry(650,100,10,100);




    button =    new QPushButton("box compute");
    button->setGeometry(800,0,100,30);

//    this->resize(1000,800);
    this->showMaximized();

    scene = new SceneItems(this);
    scene->setSceneRect(0,0,500,500);



    scene->addWidget(button);



    view = new myQView;//(scene);

    view->setScene(scene);    
    setCentralWidget(view);




    connect(button, SIGNAL(clicked(bool)),this,SLOT(buttonclicked()));
    connect(scene,SIGNAL(valuechanged(QBoxitem*)),this,SLOT(boxListUpdate(QBoxitem*)));



    createAction();
    createConnection();
    creatToolBar();


}
void MainWindow::boxListUpdate(QBoxitem* box)
{
    qDebug()<<boundingBoxList->size();
    qDebug()<<boundingBoxList->count();

    for(int i = 0; i<boundingBoxList->count();i++)
    {
        qDebug()<<boundingBoxList->item(i)->text();
    }
    QString boxstr =  QString("ClassID : %1 , Width : %2 : Height :%3").arg(box->id_class)
            .arg((int)(box->left))
            .arg((int)(box->right));
    qDebug()<<__FUNCTION__<<boxstr;
//    boundingBoxList->model()->removeRow(0);
    boundingBoxList->addItem(boxstr);
    qDebug()<<"boxListUpdate\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\";
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
