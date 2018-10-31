#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QPushButton>
#include <QDebug>
#include <QFileDialog>
#include <QPainter>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void PushOpenButton();
private:
    QString _dcmpath;

    Ui::MainWindow *ui;
    void DrawImg(QString imgpath);

    void    paintEvent(QPaintEvent *);
    QPixmap _pixmap;

    unsigned char *_imgbuff;
    void _ReleaseBuff()
    {
        if(_imgbuff!=NULL)
        {
            delete []_imgbuff;
            _imgbuff = NULL;
            qDebug()<<"release image buff";
        }
    }
};

#endif // MAINWINDOW_H
