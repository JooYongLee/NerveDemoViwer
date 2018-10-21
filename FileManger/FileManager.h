#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QWidget>
#include <QSlider>
#include <QPixmap>
#include <QDir>

class FileManger : public QDir
{
public:
    FileManger(QString path);
    ~FileManger();
    int     num_files;
    QString     img_path;
    QStringList file_list;
 public:
    QStringList GetImgList();
    QString GetBasePath() const {   return img_path;    }
    void UpdateFileList();
    void WriteBoxImg();
    void WriteImg();
};





















class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    QWidget *canvas;
    QSlider *slider;
    QPixmap pixmap;

    QString img_path;
    QStringList file_list;

    void paintEvent(QPaintEvent *);

public slots:
    void MoveSlice(int);
    void setZoom(int);

private:
    qreal zoom;
    qreal pixels;
    qreal px_width;
    qreal px_height;
    qreal mm_width;
    qreal mm_height;
};

#endif // WIDGET_H
