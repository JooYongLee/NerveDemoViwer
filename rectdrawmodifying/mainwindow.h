#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QToolBar>
#include <QAction>
#include <QSlider>
#include <QPushButton>
#include <QDockWidget>
#include <QListWidget>
#include <QTextEdit>
#include <QStatusBar>

#include "scenebox.h"
namespace Ui {
class MainWindow;
}
class myQView : public QGraphicsView
{

public:
    myQView(QWidget *parent = 0)
        :QGraphicsView(parent)
    {




    }





  //  void wheelEvent(QWheelEvent* event);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:



    enum Mode {NoMode, SelectObject, DrawLine};

    SceneItems *scene;
    myQView *view;

    QListWidget *boundingBoxList;


    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void actioniGroupClick(QAction *action);
    void buttonclicked();
    void imgslider_changed(int);
    void boxListUpdate();

private:
    Ui::MainWindow *ui;

    void creatToolBar();
    void createAction();
    void createConnection();

    void CreateFileMenu();
    void CreateDockWidget();

    QSlider *imgslider;
    QPushButton *button;



    void wheelEvent(QWheelEvent *event);



    QAction *selectAction;
    QAction *drawAction;
    QActionGroup *actionGroup;

    QToolBar *drawingToolBar;
};

#endif // MAINWINDOW_H
