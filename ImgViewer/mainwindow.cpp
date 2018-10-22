#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QDebug>
#include <QSlider>
#include <QPixmap>
#include <QHBoxLayout>


ImgView::ImgView(QWidget *parent) :
    QWidget(parent),
    _img_default_size(QSize(150,230))
{
    this->resize(1000, 600);
    QHBoxLayout *layout = new QHBoxLayout;

    //("C:/workspace/pictures");
    mng.setPath("C:/workspace/pictures");
    mng.GetImgList();



    canvas = new QWidget;

    _CreateSliders();
    _AddSliderInBox(layout);
    layout->addWidget(canvas);
    _LoadImgSlots();
//    for(int n = 0; n < IMG_SLOT;n++)
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

    repaint();


}
QRect  _GetSliderPos( int num_slot)
{
    int     i_row   =   num_slot % 3;
    int     i_column   =   num_slot / 3;
    int     dx  =   300;
    int     dy  =   300;
    int     init_x  =   200;
    int     init_y  =   30;
    int     x_pos = dx * i_row + init_x;
    int     y_pos = dy * i_column + init_y;
    return QRect( x_pos, y_pos, 30, 230);
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
    qDebug()<<this;

//    if( _pixmap->load("C:/workspace/pictures/siba.png"))
//    {
//        qDebug()<<"loading complete";
//
//        *_pixmap = _pixmap->scaled(_img_default_size);
//    }

    qDebug()<<strdebug;
}
void     ImgView::_LoadImgSlots()
{
    QString test_img_path = "C:/workspace/pictures/yuna.jpg";
    for(int i = 0; i <IMG_SLOT; i++)
    {
        pixmap[i].load(test_img_path);
        //pixmap[i] = pixmap[i].scaled(_img_default_size);
        pixmap[i] = pixmap[i].scaled(_img_default_size);
    }

}
QPoint  ImgView::_GetImgPos( int num_slot)
{

    int     i_row   =   num_slot % 3;
    int     i_column   =   num_slot / 3;
    int     dx  =   280;
    int     dy  =   300;
    int     init_x  =   50;
    int     init_y  =   30;
    int     x_pos = dx * i_row + init_x;
    int     y_pos = dy * i_column + init_y;
    return QPoint(x_pos, y_pos);
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
     //   qDebug()<< i<< imgpos.x() <<"," << imgpos.y();
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


FileManger::FileManger(QString path)
    :QDir(path)
{
}
FileManger::~FileManger()
{
}
QStringList FileManger::GetImgList()
{
    this->setNameFilters(QStringList()<<"*.jpg");

    file_list = this->entryList();
    UpdateFileList();
    return file_list;
}
QString FileManger::GetImgPath(int n)
{
    QString path;

    if( n < file_list.size())
    {
        path = img_path + '/' + file_list[n];
    }

    return path;
}
QString FileManger::GetImgPath(int n,int m)
{
    Q_ASSERT(m>0);
    double  norm_pos = (double)n/m;
    int  ind = (int)(norm_pos * file_list.size());
    //qDebug()<<"int"<<ind<<"file_lisze_size"<<file_list.size();
    return GetImgPath(ind);
}

void FileManger::UpdateFileList()
{
    num_files = file_list.size();
    img_path = this->absolutePath();
}
