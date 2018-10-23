#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QDebug>
#include <QSlider>
#include <QPixmap>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QFileInfo>


ImgView::ImgView(QWidget *parent) :
    QWidget(parent),
    _img_default_size(QSize(150,230))
{
    this->resize(1000, 600);
    QHBoxLayout *layout = new QHBoxLayout;

    //("C:/workspace/pictures");
    mng.setPath("C:/workspace/pictures");
    mng.ResearchImgList();
    //---------------------------------------------------------------------------------------------------//
    //---------------------------------------------------------------------------------------------------//
    //---------------------------------------------------------------------------------------------------//
    QPushButton *button_save = \
            new QPushButton(QString("Save"),this);
    QPushButton *button_open = \
            new QPushButton(QString("Open"),this);
    QPushButton *button_merge_points = \
            new QPushButton(QString("Merge Points"),this);
    QPushButton *button_load_points = \
            new QPushButton(QString("Load Points"),this);

    int button_width = 100;
    int button_height = 20;
    int dx = button_width *1.2;
    int ix = 10;

    // arrange horizontal dirrection for buttons
    int x0 = dx*0 + ix;
    int x1 = dx*1 + ix;
    int x2 = dx*2 + ix;
    int x3 = dx*3 + ix;

    int iy = 10;

    button_open->setGeometry(x0,iy,button_width,button_height);
    button_save->setGeometry(x1,iy,button_width,button_height);
    button_merge_points->setGeometry(x2,iy,button_width,button_height);
    button_load_points->setGeometry(x3,iy,button_width,button_height);
    //---------------------------------------------------------------------------------------------------//
    //---------------------------------------------------------------------------------------------------//
    //---------------------------------------------------------------------------------------------------//
    connect(button_open, SIGNAL(clicked(bool)),this, SLOT(FileOpen()));
    connect(button_save, SIGNAL(clicked(bool)),this, SLOT(FileSave()));


    //---------------------------------------------------------------------------------------------------//
    //---------------------------------------------------------------------------------------------------//
    //---------------------------------------------------------------------------------------------------//
    canvas = new QWidget;

    _CreateSliders();
    _AddSliderInBox(layout);
    layout->addWidget(canvas);

    _LoadImgSlots("C:/workspace/pictures/siba.png");

    //---------------------------------------------------------------------------------------------------//
    //---------------------------------------------------------------------------------------------------//
    //---------------------------------------------------------------------------------------------------//
    connect(slider[0], SIGNAL(valueChanged(int)),this, SLOT(MoveSlice1(int)));
    connect(slider[0], SIGNAL(valueChanged(int)),this, SLOT(MoveSlice4(int)));

    connect(slider[1], SIGNAL(valueChanged(int)),this, SLOT(MoveSlice2(int)));
    connect(slider[1], SIGNAL(valueChanged(int)),this, SLOT(MoveSlice5(int)));

    connect(slider[2], SIGNAL(valueChanged(int)),this, SLOT(MoveSlice3(int)));
    connect(slider[2], SIGNAL(valueChanged(int)),this, SLOT(MoveSlice6(int)));

    connect(slider[3], SIGNAL(valueChanged(int)),this, SLOT(MoveSlice1(int)));
    connect(slider[3], SIGNAL(valueChanged(int)),this, SLOT(MoveSlice4(int)));

    connect(slider[4], SIGNAL(valueChanged(int)),this, SLOT(MoveSlice2(int)));
    connect(slider[4], SIGNAL(valueChanged(int)),this, SLOT(MoveSlice5(int)));

    connect(slider[5], SIGNAL(valueChanged(int)),this, SLOT(MoveSlice3(int)));
    connect(slider[5], SIGNAL(valueChanged(int)),this, SLOT(MoveSlice6(int)));

    createActions();
    createCameraViewMenu();
    createToolButtons();
    createToolBars();
//    repaint();
}
QRect  _GetSliderPos( int num_slot)
{
    int     i_row   =   num_slot % 3;
    int     i_column   =   num_slot / 3;
    int     dx  =   280;
    int     dy  =   280;
    int     init_x  =   220;
    int     init_y  =   50;
    int     x_pos = dx * i_row + init_x;
    int     y_pos = dy * i_column + init_y;
    return QRect( x_pos, y_pos, 30, 230);
}
QPoint  ImgView::_GetImgPos( int num_slot)
{

    int     i_row   =   num_slot % 3;
    int     i_column   =   num_slot / 3;
    int     dx  =   280;
    int     dy  =   300;
    int     init_x  =   50;
    int     init_y  =   50;
    int     x_pos = dx * i_row + init_x;
    int     y_pos = dy * i_column + init_y;
    return QPoint(x_pos, y_pos);
}
void ImgView::_CreateSliders()
{
    for( int i = 0; i<IMG_SLOT; i++)
        _CreateSlider(i);
}

void ImgView::_AddSliderInBox(QHBoxLayout *layout)
{
    for( int i = 0; i<IMG_SLOT; i++)
        layout->addWidget(slider[i]);
}
void  ImgView::_CreateSlider( int num_slot )
{
    slider[num_slot]    =   new QSlider(Qt::Vertical, this);
    slider[num_slot]->setGeometry(_GetSliderPos(num_slot));
    slider[num_slot]->setRange(0,99);
    slider[num_slot]->setValue(50);
}

int ImgView::GetSlice(int num_slot)
{
    Q_ASSERT(num_slot>=0 && num_slot<IMG_SLOT);
    return slider[num_slot]->value();
}

ImgView::~ImgView()
{


}
void ImgView::MoveSlice(int num_slice)
{
    QString strdebug;
    for(int i = 0 ; i < IMG_SLOT;i++)
    {
        QString str = "0";
        str.sprintf("[%d]:%d  ",i,GetSlice(i));
        strdebug += str;
    }



    qDebug()<<strdebug;
}
void     ImgView::_LoadImgSlots(QString img_path)
{    
    for(int i = 0; i <IMG_SLOT; i++)
    {
        qDebug()<<"image loading"<<i;
        QString load_img_path = mng.GetImgPath(
                    slider[i]->value(),
                    100
                    );
        pixmap[i].load(load_img_path);
        pixmap[i] = pixmap[i].scaled(_img_default_size);
    }

}

void ImgView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter paint;
    paint.begin(this);
    //    paint.scale(1.0, 1.0);
    for( int i = 0; i <IMG_SLOT;i++)
    {
        QPoint imgpos = _GetImgPos(i);
        paint.drawPixmap(imgpos.x(), imgpos.y(), pixmap[i]);
    }
    paint.end();
}

void ImgView::MoveSlice1(int num_slice)
{
    QString path = mng.GetImgPath(num_slice, 100);
    if( pixmap[0].load(path))
    {

        pixmap[0] = pixmap[0].scaled(_img_default_size);
    }
    repaint();
}
void ImgView::MoveSlice2(int num_slice)
{
    QString path = mng.GetImgPath(num_slice, 100);
    qDebug()<<mng.absolutePath();
    qDebug()<<path;
    if( pixmap[1].load(path))
    {

        pixmap[1] = pixmap[1].scaled(_img_default_size);
    }
    repaint();
}
void ImgView::MoveSlice3(int num_slice)
{
    QString path = mng.GetImgPath(num_slice, 100);
    if( pixmap[2].load(path))
    {

        pixmap[2] = pixmap[2].scaled(_img_default_size);
    }
    repaint();
}
void ImgView::MoveSlice4(int num_slice)
{
    QString path = mng.GetImgPath(num_slice, 100);
    if( pixmap[3].load(path))
    {
        pixmap[3] = pixmap[3].scaled(_img_default_size);
    }
    repaint();
}
void ImgView::MoveSlice5(int num_slice)
{
    QString path = mng.GetImgPath(num_slice, 100);
    if( pixmap[4].load(path))
    {
        pixmap[4] = pixmap[4].scaled(_img_default_size);
    }
    repaint();
}
void ImgView::MoveSlice6(int num_slice)
{
    QString path = mng.GetImgPath(num_slice, 100);
    if( pixmap[5].load(path))
    {

        pixmap[5] = pixmap[5].scaled(_img_default_size);
    }
    repaint();
}

void    ImgView::FileOpen()
{
    QString filepath;
    filepath = QFileDialog::getOpenFileName(this,tr("Open Image"),tr("c:/"),tr("Image Files (*.png *.jpg *.bmp)"));
    QFileInfo fileinfo(filepath);

    qDebug()<<"filepath_path"<<fileinfo.path();
    qDebug()<<"filepath_fileName"<<fileinfo.fileName().split(".");
    qDebug()<<"filepath_filePath"<<fileinfo.filePath();


//    qDebug()<<"filepath_fileName"<<fileinfo.extension();

    if( filepath.length() > 0 )
    {

        mng.setPath(fileinfo.path());
        mng.ResearchImgList();
        _LoadImgSlots(fileinfo.path());
        repaint();
    }
    qDebug()<<filepath;

}
void    ImgView::FileSave()
{
    QString filepath;
    filepath = QFileDialog::getSaveFileName(this,tr("Save File"),tr("c:/"),tr("Image Files (*.png *.jpg *.bmp)"));
    qDebug()<<filepath;
}


void    ImgView::ViewCoronal()
{
    qDebug()<<"ViewCoronal";
}
void    ImgView::ViewAxial()
{
    qDebug()<<"ViewAxial";
}
void    ImgView::ViewSagittal()
{
    qDebug()<<"ViewSagittal";
}
void ImgView::createActions()
{
    ActionCoronal   =   new QAction("Coronal",this);
    ActionSagittal  =   new QAction("Sagittal",this);
    ActionAxial     =   new QAction("Axial",this);

    connect(ActionCoronal   , SIGNAL(triggered()), this, SLOT(ViewCoronal()));
    connect(ActionSagittal  , SIGNAL(triggered()), this, SLOT(ViewSagittal()));
    connect(ActionAxial     , SIGNAL(triggered()), this, SLOT(ViewAxial()));
}
void ImgView::createCameraViewMenu()
{
    CameraViewMenu = new QMenu;
    CameraViewMenu->addAction(ActionCoronal);
    CameraViewMenu->addAction(ActionSagittal);
    CameraViewMenu->addAction(ActionAxial);
}

void ImgView::createToolButtons(){
    cameraviewToolButton = new CameraViewButton();
    cameraviewToolButton->setMenu(CameraViewMenu);
    cameraviewToolButton->setDefaultAction(ActionCoronal);
}

void ImgView::createToolBars(){
    editToolBar = new QToolBar(this);
    editToolBar->setGeometry(700,0,100,50);
    editToolBar->addWidget(cameraviewToolButton);
}
