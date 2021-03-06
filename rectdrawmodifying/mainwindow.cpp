#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QDebug>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsProxyWidget>
#include <QFormLayout>
#include <QGraphicsLayout>
#include <QDir>
#include <QStatusBar>
#include <QLabel>
#include "filemanager.h"
#include "qjsonbox.h"
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

    fileMenu = menuBar()->addMenu(tr("&File"));
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
    boundingBoxList->addItems(QStringList());



    fileListWidget = new QListWidget(dock);
    QStringList strs;
    fileListWidget->addItems(strs);
    //customerList2->item()


    dock->setWidget(boundingBoxList);
    dock2->setWidget(fileListWidget);
    addDockWidget(Qt::RightDockWidgetArea, dock2);
    addDockWidget(Qt::RightDockWidgetArea, dock);
}
void MainWindow::OpenFileDialog()
{
    QString filepath;
    filepath = QFileDialog::getOpenFileName(
                this,
                tr("Open Image"),
                QString(),
                tr("Image Files (*.png *.jpg *.bmp *.dcm)"));
    QFileInfo fileinfo(filepath);


    if( filepath.length() > 0 )
    {
        int filenum = ResetFileMangerAndUpdateFileList(fileinfo);
        fileListChanged(filenum);
    }

}
int MainWindow::ResetFileMangerAndUpdateFileList(QFileInfo fileinfo)
{

    fileManager.setPath(fileinfo.path());
//    qDebug()<<fileManager.path();
    fileManager.ResearchImgList();

    UpdateFileListWidget();

    // check the filenum;
    QStringList imgList = fileManager.GetImgList();

    int     fileNum = 0;
    for( int cnt = 0; cnt < imgList.size(); cnt++)
    {
        QString name = imgList.at(cnt);
        if( name.contains(fileinfo.fileName()))
        {
            fileNum = cnt;
            break;
        }
    }

    scene->changeBoxClass(QBoxitem::BoxClass(m_nClassId));

    return fileNum;
}

void MainWindow::CreateFileMenuConnection()
{
    connect(openAct,SIGNAL(triggered(bool)),this,SLOT(OpenFileDialog()));

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_NumImgInViwer(0),
    m_nClassId(0),
    m_isFileListDeleting(false)
{    
    QBoxitem::init_map_box();
    ui->setupUi(this);
    this->setAcceptDrops(true);

    CreateFileMenu();
    CreateFileMenuConnection();
    CreateDockWidget();

    imgslider =    new QSlider(Qt::Vertical,nullptr);

    BoxesList.append(BoxManager());

//    BoxesList.insert()

    this->showMaximized();
//    this->resize(700,800);

    scene = new SceneItems(this);
    scene->setSceneRect(-100,-50,900,800);


    view = new myQView;//(scene);
    view->setMouseTracking(true);

    view->setScene(scene);    
    setCentralWidget(view);
//    fileListWidget->mouse
    m_CursorTracker = new QLabel(this);

    ui->statusBar->addPermanentWidget(m_CursorTracker);


//    QLabel statusLabel(this);
//    statusLabel.setText("Status Label");
//    ui->statusBar->addPermanentWidget(&statusLabel);
//    layout

//    QStatusBar *bar = new QStatusBar(this);
//    ui->->addWidget(bar);
//    statusbar->insertPermanentWidget(0,this);
//    statusbar->addPermanentWidget(this);
//    addDockWidget(Qt::BottomDockWidgetArea, statusbar);

//    statusbar->setGeometry(0,0,100,100);
//    ui->statusBar->addPermanentWidget(statusbar);

    connect(fileListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(fileListClicked(QListWidgetItem*)));
    connect(fileListWidget,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(fileListClicked(QListWidgetItem*)));
    connect(fileListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(fileListClicked(QListWidgetItem*)));
    connect(fileListWidget,SIGNAL(itemEntered(QListWidgetItem*)),this,SLOT(fileListClicked(QListWidgetItem*)));
    connect(fileListWidget,SIGNAL(itemPressed(QListWidgetItem*)),this,SLOT(fileListClicked(QListWidgetItem*)));
    connect(fileListWidget,SIGNAL(currentRowChanged(int)),this,SLOT(fileListChanged(int)));

    //connect(fileListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(fileListClicked(QListWidgetItem*)));



    connect(scene,SIGNAL(valuechanged(QBoxitem*)),this,SLOT(addBoxListToViwer(QBoxitem*)));
    connect(scene,SIGNAL(deletedItems(QUuid*)),this,SLOT(DeleteBoxList(QUuid*)));
    connect(scene,SIGNAL(updateItems(QBoxitem*)),this,SLOT(updateSelectBox(QBoxitem*)));
    connect(scene,SIGNAL(cursorPos(QPointF*)),this,SLOT(showPosCursor(QPointF*)));



    createToolBarAction();
    createConnection();
    creatToolBar();

    createToolBarActions();
    createToolBarMenu();
    createToolBarButtons();
    createClassToolBars();



}
void MainWindow::showPosCursor(QPointF *curpnt)
{
//    QLabel *curosrprnt = new QLabel(this);
    m_CursorTracker->setText(QString("%1,%2")
                   .arg((int)curpnt->x())
                   .arg((int)curpnt->y()));
    qDebug()<<__FUNCTION__;

}

void MainWindow::fileListChanged(int file_num)
{
    if( m_isFileListDeleting == false )
    {
        qDebug()<<file_num;
    //    fileListWidget->row
        QString selectFilename = fileListWidget->item(file_num)->text();
        qDebug()<<__FUNCTION__<<file_num<<selectFilename;


        QString full_abs_path = fileManager.GetBasePath() + '/' +   selectFilename;
        scene->Redraw(full_abs_path, BoxesList.at(file_num).boxmap );
        _SetStatusImg(file_num);
        RedrawViwer(file_num);
    }




}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{

    QStringList filelist;
    foreach (const QUrl &url, event->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
//        qDebug() << "Dropped file:" << fileName;
        filelist.append(fileName);
    }

    QFileInfo fileinfo(filelist.first());
    qDebug()<<__FUNCTION__;
    int fileNum = this->ResetFileMangerAndUpdateFileList(fileinfo);
    fileListChanged(fileNum);
//    fileListWidget->item(fileNum)->setSelected(true);
    fileListWidget->setCurrentRow(fileNum);

}
void MainWindow::RedrawViwer(int id_item)
{
    QList<QBoxitem> boxitems = BoxesList.at(id_item).boxmap;

    qDebug()<<__FUNCTION__<<id_item<<BoxesList.at(id_item).filename;
    qDebug()<<__FUNCTION__<<boxitems.count();

//    QMapIterator<int,QBoxitem> iter(boxitems);
     boundingBoxList->clear();     
     for( int ind_key = 0; ind_key < boxitems.count(); ind_key++)
     {
        boxListUpdate(ind_key,(QBoxitem*)&boxitems.at(ind_key));

     }
//    while (iter.hasNext()) {
//        iter.next();
//        qDebug()<< iter.key() << ": " << _GetBoxStringFormat((QBoxitem*)&iter.value());
//        iter.value()
//        boxListUpdate(iter.key(),(QBoxitem*)&iter.value());
//    }
    //if( boxitems.size() == 0)
}
void MainWindow::fileListClicked(QListWidgetItem *items)
{
    int item_ind = items->listWidget()->row(items);


    QString full_abs_path = fileManager.GetBasePath() + '/' +   items->text();

    scene->Redraw(full_abs_path, BoxesList.at(item_ind).boxmap );


    _SetStatusImg(item_ind);
    RedrawViwer(item_ind);

   // boxListUpdate();
    qDebug()<<__FUNCTION__;
}

void MainWindow::UpdateFileListWidget()
{
    QStringList imglist = fileManager.GetImgList();
    imglist.sort();
    qDebug()<<__FUNCTION__;
    qDebug()<<__FUNCTION__;
    qDebug()<<imglist;

    qDebug()<<__FUNCTION__<<"before"<<fileListWidget->count();
    m_isFileListDeleting = true;
    while(fileListWidget->count()>0)
    {
        qDebug()<<"count"<<fileListWidget->count();
      fileListWidget->takeItem(0);//handle the item if you don't
                              //have a pointer to it elsewhere

    }

    qDebug()<<__FUNCTION__;
    qDebug()<<__FUNCTION__;
    qDebug()<<__FUNCTION__;
    qDebug()<<__FUNCTION__<<"clear"<<fileListWidget->count();

    fileListWidget->addItems(imglist);
    qDebug()<<__FUNCTION__<<"after"<<fileListWidget->count();
    m_isFileListDeleting = false;
    BoxesList.clear();
//    BoxesList.reserve(imglist.size());
//    qDebug()<<"+++++++++"<<BoxesList.size()<<imglist.size();
    for( int ind = 0; ind < imglist.size(); ind++)
    {
        BoxManager imgbox;
        imgbox.filename = imglist.at(ind);
        imgbox.id_filename = ind;
        BoxesList.append(imgbox);
//        qDebug()<<BoxesList.at(ind).filename<<BoxesList.at(ind).id_filename;
    }
}
QString MainWindow::_GetBoxStringFormat(QBoxitem *box)
{
    QString boxstr =  QString("ClassID : %1,(X,Y,W,H)<%2,%3,%4,%5>").arg(box->id_class)
            .arg((int)(box->left),4)
            .arg((int)(box->top),4)
            .arg((int)(box->right - box->left+1),4)
            .arg((int)(box->bottom - box->top+1),4);
    return boxstr;
}
void MainWindow::updateSelectBox(QBoxitem *selectBox)
{
    qDebug()<<__FUNCTION__<<selectBox->getID();
    if( selectBox )
    {
        // get box info from current image from viwer
        int file_num = _GetStatusImg();
        QList<QBoxitem> boxitems = BoxesList.at(file_num).boxmap;
        QUuid selected_id = selectBox->getID();
        // check uuid with given selected box among from list-widet
        for( int ind_key = 0; ind_key < boxitems.count(); ind_key++)
        {
            QBoxitem box = boxitems.at(ind_key);
            if( selected_id == box.getID())
            {
                int ind_update_key = ind_key;
                BoxesList[file_num].boxmap[ind_update_key] = *selectBox;

                boundingBoxList->item(ind_update_key)->setText(_GetBoxStringFormat(selectBox));

                break;
            }
        }
        qDebug()<<__FUNCTION__<<"after"<<BoxesList[file_num].boxmap.count();

    }

}

void MainWindow::DeleteBoxList(QUuid *tobeDeletedItem)
{
    if( tobeDeletedItem != nullptr &&
            ( _GetStatusImg() < BoxesList.size()))
    {
        int file_num = _GetStatusImg();


        qDebug()<<__FUNCTION__<<"before"<<BoxesList[file_num].boxmap.count();
        QList<QBoxitem> boxitems = BoxesList.at(file_num).boxmap;

        for( int ind_key = 0; ind_key < boxitems.count(); ind_key++)
        {
            QBoxitem box = boxitems.at(ind_key);
            if( *tobeDeletedItem == box.getID())
            {
                int ind_delete_key = ind_key;
//                qDebug()<<__FUNCTION__<<"delete items"<<ind_delete_key<<*tobeDeletedItem;
                boxListDelete(ind_delete_key);

//                boundingBoxList->item(ind_key)->setText(_GetBoxStringFormat(*tobeDeletedItem));
                BoxesList[file_num].boxmap.removeAt(ind_delete_key);
                break;
            }
        }
    }

}
void MainWindow::boxListDelete(int ind)
{
    qDebug()<<__FUNCTION__<<ind<<boundingBoxList->item(ind)->text();
    boundingBoxList->takeItem(ind);
//    boundingBoxList->removeItemWidget(boundingBoxList->item(ind));
}

void MainWindow::boxListUpdate(int ind, QBoxitem* box)
{
    boundingBoxList->insertItem(ind, _GetBoxStringFormat(box));
}

void MainWindow::addBoxListToViwer(QBoxitem* box)
{
    qDebug()<<__FUNCTION__;
    int    row_id = boundingBoxList->count();

    QString boxstr = _GetBoxStringFormat(box);


    boundingBoxList->insertItem(row_id,boxstr);
    qDebug()<<__FUNCTION__<<"_GetBoxStringFormat"<<boundingBoxList->count()<<row_id;
    qDebug()<<__FUNCTION__<<_GetStatusImg()<<BoxesList.size();//->count()<<row_id;
    //qDebug()<<__FUNCTION__<<boxstr<<box->getID();
//    BoxesList.at(_GetStatusImg()).boxmap.insert(row_id, QBoxitem());
    if( _GetStatusImg() < BoxesList.size())
    {
        qDebug()<<__FUNCTION__<<"add box to viewer"<<box->getID();
        BoxesList[_GetStatusImg()].boxmap.insert(row_id, *box);
    }
    fileListWidget->setFocus(Qt::NoFocusReason);
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
void MainWindow::createToolBarAction()
{

    this->dnnAction = new QAction("Save Boxes",this);
    this->dnnAction->setIcon(QIcon(":/icons/dnnicon.png"));
    this->dnnAction->setCheckable(false);

    this->saveAction = new QAction("Save Boxes",this);
    this->saveAction->setData(SceneItems::NoMode);//QIcon(":/icons/saveicon.png"));
    this->saveAction->setIcon(QIcon(":/icons/saveicon.png"));
    this->saveAction->setCheckable(false);

    // graphics item actions
    this->selectAction = new QAction("Select Item(S)",this);
    this->selectAction->setData(SceneItems::SelectObject);
    this->selectAction->setIcon(QIcon(":/icons/select.png"));
    this->selectAction->setCheckable(true);
    this->selectAction->setShortcut(QKeySequence(Qt::Key_S));

//    this->selectAction->setSho


    this->drawAction = new QAction("draw item(D)",this);
    this->drawAction->setData(SceneItems::DrawLine);
    this->drawAction->setIcon(QIcon(":/icons/drawbox.png"));
    this->drawAction->setCheckable(true);
    this->drawAction->setShortcut(QKeySequence(Qt::Key_D));

    loadBoxAction       =   new QAction("Load Boxes", this);
    loadBoxAction->setIcon(QIcon(":/icons/loadBox.png"));
    loadBoxAction->setCheckable(false);


    actionGroup = new QActionGroup(this);
    actionGroup->addAction(selectAction);
    actionGroup->addAction(drawAction);
//    actionGroup->addAction(saveAction);
//    actionGroup->
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

    connect(saveAction, SIGNAL(triggered(bool)), this, SLOT(actionBoxSave()));

    connect(loadBoxAction, SIGNAL(triggered(bool)), this, SLOT(actionloadBox()));
}
void MainWindow::actionloadBox()
{

    QString filepath;
    filepath = QFileDialog::getOpenFileName(
                this,
                tr("Open Image"),
                QString(),
                tr("Image Files (*.json)"));
    QFile checkfile(filepath);


    if( !checkfile.exists())
    {
        qDebug()<<__FUNCTION__<<"not exist---------------------------";
        return;
    }


    BoxFormat loadboxFormat = JsonBoxSaver::loadJson(filepath);
  //  qDebug()<<__FUNCTION__<<filepath;

    QList<BoxManager> boxFromJson = loadboxFormat.GetBoxes();

//    qDebug()<<__FUNCTION__<<boxFromJson.count()<<BoxesList.count();


    // loading boxes from json if only the counts of box is less than the number of list image
    if( boxFromJson.count() > 0 && boxFromJson.count() <= BoxesList.count())
    {
        for( int k = 0 ; k < boxFromJson.count(); k++)
        {
            // check loaded filename, with file list widget
            // if the filename is same with file list
            QString item_filename = boxFromJson.at(k).filename;
            for( int cnt = 0; cnt < fileListWidget->count(); cnt++)
            {
                QString check_filename = fileListWidget->item(cnt)->text();
                if( !item_filename.compare(check_filename))
                {
                    // when file name from json is in accordance with the filename of the list widget
                    // load box info
                    BoxesList[cnt].boxmap.swap(boxFromJson[k].boxmap);
                    qDebug()<<__FUNCTION__<<"loading complte"<<item_filename;
                    break;
                }
            }
        }
        fileListChanged(fileListWidget->currentRow());
    }
    //qDebug()<<__FUNCTION__<<"currentitems"<<fileListWidget->currentRow();

}

void MainWindow::actionBoxSave()
{
    qDebug()<<__FUNCTION__;
    BoxFormat savebox(BoxesList, fileManager.GetImgList(), "coronal");

    JsonBoxSaver::saveJson(savebox);

}

void MainWindow::creatToolBar()
{
    drawingToolBar  =   new QToolBar;
//    drawingToolBar->setFixedWidth(200);
    addToolBar(Qt::LeftToolBarArea, drawingToolBar);

    drawingToolBar->setIconSize(QSize(80,80));


    drawingToolBar->addAction(selectAction);
    drawingToolBar->addAction(drawAction);
    drawingToolBar->addAction(saveAction);
    drawingToolBar->addAction(dnnAction);
    drawingToolBar->addAction(loadBoxAction);
}



void MainWindow::triggeredNerve()
{
    scene->changeBoxClass(QBoxitem::BoxClass::NERVE);
    m_nClassId = QBoxitem::NERVE;
    qDebug()<<__FUNCTION__<<m_nClassId;
}

void MainWindow::triggeredLowercase()
{
    scene->changeBoxClass(QBoxitem::BoxClass::LOWERCASE);
    m_nClassId = QBoxitem::LOWERCASE;
    qDebug()<<__FUNCTION__<<m_nClassId;
}
void MainWindow::triggeredLoadBoxes()
{
    qDebug()<<__FUNCTION__;
}

void MainWindow::createToolBarActions()
{
    ActionNerve   =   new QAction("Nerve",this);
    ActionNerve->setData(QBoxitem::NERVE);
//    ActionNerve->setShortcut(Qt::Key_C);
//    ActionCoronal->setIcon(QIcon(":/icon/skull_coronal.png"));
    ActionLowerCase  =   new QAction("Lowercase",this);
    ActionLowerCase->setData(QBoxitem::LOWERCASE);

    ActionNerve->setIcon(QIcon(":/icons/nerve.png"));
    ActionLowerCase->setIcon(QIcon(":/icons/toothbone.png"));    


    connect(ActionNerve   , SIGNAL(triggered()), this, SLOT(triggeredNerve()));
    connect(ActionLowerCase  , SIGNAL(triggered()), this, SLOT(triggeredLowercase()));


    connect(m_classBarButton, SIGNAL(pressed()), this, SLOT(clickedtoolbutton()));
//    connect(ActionNerve   , SIGNAL(changed()), this, SLOT(actionBoxSave()));
//    connect(m_classBarButton, SIGNAL(toggled(bool)), this, SLOT(actionBoxSave()));
}
void MainWindow::clickedtoolbutton()
{
    qDebug()<<__FUNCTION__;
    qDebug()<<__FUNCTION__;

}

void MainWindow::createToolBarMenu()
{
    ClassViewMenu = new QMenu;
    ClassViewMenu->addAction(ActionNerve);
    ClassViewMenu->addAction(ActionLowerCase);
}

void MainWindow::createToolBarButtons()
{
    m_classBarButton = new ClassToolBarButton();
    m_classBarButton->setMenu(ClassViewMenu);
    m_classBarButton->setDefaultAction(ActionNerve);
    m_classBarButton->setCheckable(true);
//    m_classBarButton->setAutoRepeat(true);
//    m_classBarButton->setAutoRepeatDelay(true);
//    m_classBarButton->setAutoRepeatInterval(1);
//    m_classBarButton->setAutoRepeat(true);
//    m_classBarButton->setAutoRaise(true);

//    m_classBarButton->setShortcut(Qt::Key_C);
//    m_classBarButton->autoRaise();
//    m_classBarButton->autoRepeat();
// set default class
    m_nClassId      =   QBoxitem::NERVE;
}

void MainWindow::createClassToolBars()
{
//    editToolBar = new QToolBar(this);
//    editToolBar->setGeometry(700,0,100,50);
//    editToolBar->addWidget(cameraviewToolButton);
    drawingToolBar->addWidget(m_classBarButton);
}
