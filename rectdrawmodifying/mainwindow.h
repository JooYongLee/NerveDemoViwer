#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QToolBar>
#include <QAction>
#include "scenebox.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    enum Mode {NoMode, SelectObject, DrawLine};

    SceneItems *scene;
    QGraphicsView *view;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void actioniGroupClick(QAction *action);

private:
    Ui::MainWindow *ui;

    void creatToolBar();
    void createAction();
    void createConnection();



    QAction *selectAction;
    QAction *drawAction;
    QActionGroup *actionGroup;

    QToolBar *drawingToolBar;
};

#endif // MAINWINDOW_H
