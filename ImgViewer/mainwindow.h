#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <Qslider>
#include <QPixmap>
#include <QDir>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QPushButton>

#include "filemanager.h"
#include "cameraviewbutton.h"
/***********************/
#ifndef         IMG_SLOT
#define         IMG_SLOT           6
#endif
/***********************/


/**
 * @brief The BoundingBox class
 *
 */
class BoundingBox : public QRect
{
public:
    BoundingBox()
        :QRect()
    {}
    ~BoundingBox()
    {}
};


/**
 * @brief The ImgView class
 */
class ImgView : public QWidget
{
    Q_OBJECT

public:
    explicit ImgView(QWidget *parent = 0);
    ~ImgView();
public:
    int     GetSlice(int num_slot);
    void    SetSlice(int n);
    void    AddView();
    void    DeleteView();

    void    paintEvent(QPaintEvent *);
    QPixmap pixmap[IMG_SLOT];
    FileManger mng;
public slots:


    void    MoveSlice1(int);
    void    MoveSlice2(int);
    void    MoveSlice3(int);
    void    MoveSlice4(int);
    void    MoveSlice5(int);
    void    MoveSlice6(int);

    void    FileOpen();
    void    FileSave();

// cameara view button
    void    ViewCoronal();
    void    ViewAxial();
    void    ViewSagittal();



private:

    int     _i_slice_num;
    int     _num_viwer;
    QSize   _img_default_size;

    QWidget *canvas;
    QSlider *slider[IMG_SLOT];
    void    _CreateSliders();
    void     _CreateSlider( int num_slot );
    void     _AddSliderInBox(QHBoxLayout *layout);
    void     _LoadImgSlots(QString img_path);
    QPoint  _GetImgPos( int num_slot);
    void    _SetImgSize(QPixmap *pixmap);



    QMenu   *CameraViewMenu;
    QToolBar *editToolBar;
    CameraViewButton* cameraviewToolButton;

    QAction *ActionCoronal;
    QAction *ActionSagittal;
    QAction *ActionAxial;

    void    createCameraViewMenu();
    void    createActions();
    void    createToolButtons();
    void    createToolBars();
};

#endif // MAINWINDOW_H
