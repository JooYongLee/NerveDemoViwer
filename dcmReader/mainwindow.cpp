#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dcmcore.h"
#include <QImage>
#include <QFileInfo>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _imgbuff(NULL)
{
    ui->setupUi(this);

    QPushButton *button_open = \
            new QPushButton(QString("Open"),this);
    button_open->setGeometry(10,20,100,20);

    connect(button_open,SIGNAL(clicked(bool)),this,SLOT(PushOpenButton()));
}
void MainWindow::PushOpenButton()
{
    qDebug()<<"PushOpenButton";
    QString filepath;
    filepath = QFileDialog::getOpenFileName(
                this,
                tr("Open Image"),
                QString(),
                tr("Image Files (*.png *.jpg *.bmp *.dcm )"));

    if( filepath.length()>0)
    {
        _dcmpath.clear();
        _dcmpath = filepath;
        filepath.clear();
        qDebug()<<_dcmpath;
        DrawImg(_dcmpath);
        //TestDcmPrint();
    }
    else
    {
        qDebug()<<"canceled";
    }
}
void MainWindow::DrawImg(QString imgpath)
{
    _ReleaseBuff();
    QFileInfo filinfo(imgpath);
    QStringList filename = filinfo.fileName().split(".");

    QString file_extension = filename.at(1);


    QString lower_extension = file_extension.toLower();
    QString upper_extension = file_extension.toUpper();

    if(     !QString::compare(lower_extension,"dcm") ||
            !QString::compare(upper_extension,"DCM")
            )
    {

        int width = 0;
        int height = 0;
        get_data_from_dcm(
                    imgpath.toStdString(),
                    _imgbuff,
                    width,
                    height);

        if( _imgbuff != NULL )
        {
            QImage src( _imgbuff, width, height, QImage::Format_Grayscale8);

            if( _pixmap.convertFromImage(src))
            {
                qDebug()<<"loading";
            }
        }
    }
    else        // normal bmp, jpg, png
    {
        if( _pixmap.load(imgpath))
        {
            // load complete
        }
    }
    _pixmap=_pixmap.scaled(600,600);

    repaint();

}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter paint;
    paint.begin(this);
//    _pixmap.scaled(200,200);
    paint.drawPixmap(20,50,_pixmap);
    paint.end();
}
MainWindow::~MainWindow()
{
    _ReleaseBuff();
    delete ui;
}
