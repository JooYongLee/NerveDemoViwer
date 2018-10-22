#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <Qslider>
#include <QPixmap>
#include <QDir>
#include <QMainWindow>
#include <QHBoxLayout>

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
 * @brief The FileManger class
 */
class FileManger : public QDir
{
public:
    FileManger(QString path ="");
    ~FileManger();
    int     num_files;
    QString     img_path;
    QStringList file_list;
 public:
    QStringList GetImgList();
    QString GetImgPath(int n);
    QString GetImgPath(int n,int m);
    QString GetBasePath() const {   return img_path;    }
    void UpdateFileList();
    void WriteBoxImg();
    void WriteImg();
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
public slots:
    void    MoveSlice(int);

    void    MoveSlice1(int);
    void    MoveSlice2(int);
    void    MoveSlice3(int);
    void    MoveSlice4(int);
    void    MoveSlice5(int);
    void    MoveSlice6(int);



private:
    FileManger mng;
    int     _i_slice_num;
    int     _num_viwer;
    QSize   _img_default_size;

    QWidget *canvas;
    QSlider *slider[IMG_SLOT];
    void    _CreateSliders();
    void     _CreateSlider( int num_slot );
    void     _AddSliderInBox(QHBoxLayout *layout);
    void     _LoadImgSlots();
    QPoint  _GetImgPos( int num_slot);
    void    _SetImgSize(QPixmap *pixmap);
};

#endif // MAINWINDOW_H
