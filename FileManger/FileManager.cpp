#include "FileManager.h"

#include <QHBoxLayout>
#include <QDesktopWidget>
#include <QPainter>
#include <QDebug>
#include <QDir>

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
void FileManger::UpdateFileList()
{
    num_files = file_list.size();
    img_path = this->absolutePath();
}






Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->resize(621, 375);

    QHBoxLayout *layout = new QHBoxLayout;

    canvas = new QWidget;
    slider = new QSlider;
    slider->setMinimum(0);
    slider->setMaximum(100);
    slider->setValue(50);

    layout->addWidget(canvas);
    layout->addWidget(slider);

    setLayout(layout);
    QString test_img_path = "C:/workspace/pictures/yuna.jpg";

    if(!pixmap.load(test_img_path))
    {
        qDebug() << "Fatal error: Unable to load image";
        exit(-1);
    }

    FileManger mng("C:/workspace/pictures");
    qDebug()<<"file manager"<<mng.absolutePath();
    qDebug()<<"-----------------------\n"<< mng.GetImgList();
    mng.setPath("C:/workspace/pictures/test");
    qDebug()<<"file manager"<<mng.absolutePath();
    qDebug()<<"-----------------------\n"<< mng.GetImgList();


    QDir dirs("C:/workspace/pictures");
    dirs.setNameFilters(QStringList()<<"*.jpg");


    file_list = mng.GetImgList();
    img_path = mng.GetBasePath();
    qDebug()<<img_path;
    qDebug()<<file_list;

    connect(slider, SIGNAL(valueChanged(int)),
            SLOT(MoveSlice(int)));

}

void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter paint;
    paint.begin(this);
    paint.scale(zoom, zoom);
    paint.drawPixmap(0,0, pixmap);
    paint.end();
}

void Widget::MoveSlice(int num_slice)
{
    int n = (int)(floor(num_slice / 101.0*file_list.size()));
    //qDebug()<<"MoveSlice Call" << file_list.size() <<"n"<<n;
    qDebug()<<"MoveSlice";
  //  QString test_img_path = "C:/workspace/pictures/yuna.jpg";

    if( n < file_list.size())
    {
        QString loadimg_path =  img_path + "/" + file_list[n];
        qDebug() <<loadimg_path;
        //pixmap.load(loadimg_path);
        if( pixmap.load(loadimg_path))
        {
            qDebug()<<"loading complete";
            zoom = 1.5;
            pixels = pixmap.width() * zoom;
            repaint();
        }
    }
}


void Widget::setZoom(int new_zoom)
{

    zoom = (qreal)(50+new_zoom) / 50;
    pixels = pixmap.width() * zoom;

    QDesktopWidget desk;
    px_width = desk.width() / pixels;
    px_height = desk.height() / pixels;
    mm_width = px_width * 17.9;
    mm_height = px_height * 17.9;

    qDebug() << "Zoom: " << zoom;
    qDebug() << "desk.widthMM:" << desk.widthMM();
    qDebug() << "px_width: " << px_width;
    qDebug() << "px_height: " << px_height;
    qDebug() << "mm_width: " << mm_width;
    qDebug() << "mm_height: " << mm_height;

    repaint();
}

Widget::~Widget()
{

}
















