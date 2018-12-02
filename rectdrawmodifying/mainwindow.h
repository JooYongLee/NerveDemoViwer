#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QDockWidget>
#include <QFileDialog>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QListWidget>
#include <QMainWindow>
#include <QMimeData>
#include <QSlider>
#include <QStatusBar>
#include <QStringList>
#include <QPushButton>
#include <QTextEdit>
#include <QToolBar>
#include <QLabel>

#include "boundingbox.h"
#include "classtoolbarbutton.h"
#include "filemanager.h"
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

    QList<BoxManager> BoxesList;

    void RedrawViwer(int id_item);

    SceneItems *scene;
    myQView *view;

    QListWidget *boundingBoxList;
    QListWidget *fileListWidget;


    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void dragEnterEvent(QDragEnterEvent *);

    void dropEvent(QDropEvent *);

    void boxListDelete(int ind);
public slots:
    void actioniGroupClick(QAction *action);
    void buttonclicked();

    void boxListUpdate(int id_box, QBoxitem* box);
    void DeleteBoxList(QUuid*);
    void updateSelectBox(QBoxitem*);
    void addBoxListToViwer(QBoxitem* box);

    void OpenFileDialog();
    void fileListClicked(QListWidgetItem*);
    void fileListChanged(int);
    void actionBoxSave();
    void actionloadBox();

    void triggeredNerve();
    void triggeredLowercase();
    void triggeredLoadBoxes();


    void showPosCursor(QPointF *);

    void clickedtoolbutton();




private:
    // to block drawing image when change row of list number
    bool m_isFileListDeleting;

    //
    QLabel *m_CursorTracker;

    int     m_nClassId;
    ClassToolBarButton* m_classBarButton;
    QMenu   *ClassViewMenu;

    QAction *ActionNerve;
    QAction *ActionLowerCase;


    void createToolBarActions();
    void createToolBarMenu();
    void createToolBarButtons();
    void createClassToolBars();




    QString _GetBoxStringFormat(QBoxitem *box);

    int m_NumImgInViwer;
    void _SetStatusImg(int n){  m_NumImgInViwer = n;}
    int _GetStatusImg(){     return m_NumImgInViwer;}
    FileManger fileManager;
    Ui::MainWindow *ui;

    void creatToolBar();
    void createToolBarAction();
    void createConnection();



    void CreateFileMenu();    
    void CreateDockWidget();
    void CreateFileMenuConnection();

    void UpdateFileListWidget();
    int ResetFileMangerAndUpdateFileList(QFileInfo fileinfo);



    QSlider *imgslider;
    QPushButton *button;

    QAction *newAct;
    QAction *openAct;
    QMenu *fileMenu;

    void wheelEvent(QWheelEvent *event);



    QAction *selectAction;
    QAction *drawAction;
    QAction *saveAction;
    QAction *loadBoxAction;
    QAction *dnnAction;
    QActionGroup *actionGroup;


    QToolBar *drawingToolBar;
};

#endif // MAINWINDOW_H
