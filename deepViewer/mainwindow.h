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
#include <QCloseEvent>

#include "boundingbox.h"
#include "classtoolbarbutton.h"
#include "filemanager.h"
#include "scenebox.h"
#include "filelistwidget.h"

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
//    QListWidget *fileListWidget;
    FileListWidget *fileListWidget;

    QAction *fileListAction;


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
    void triggerChangeActionClass();


    void showPosCursor(QPointF *);

    void clickedtoolbutton();

    //////////////////////
    /// \brief camera view
    ///
    void triggerdSagittalView();
    void triggerAxialView();
    void triggerdCoronalView();

    void triggerPropagate();

    void closeEvent(QCloseEvent *event);

    void setFocusFileList();

private:
    bool m_bPropgateEn;

    void _PropagateBoxes(int beforebox_idx, int nextbox_idx);

    // the number of image to be fixed
    int         m_nCountToBadImg;
    // the number of image which not need to be fixed
    int         m_nCountToGoodImg;

    int         _GetCountOfBadImg() const       {   return m_nCountToBadImg;    }
    int         _GetCountOfGoodImg() const       {   return m_nCountToGoodImg;    }
    void      _ResetCountOfStatusImg()       { m_nCountToBadImg = 0; m_nCountToGoodImg = 0;  }

    ImgType m_imgtype;
    VIEW_FLAG m_viewDcmCamera;

    void    _SetImgType(ImgType type)   {   m_imgtype = type;   }
    // to block drawing image when change row of list number
    bool m_isFileListDeleting;
    bool LoadMultipleJsonFiles(QString filepath);
    void loadMultipleFileBoxToViwer(QStringList filepath, QString basepath);
    /////////////////////////////////
    /// \brief loadBoxToViwer load boxes from filepath of json format
    /// \param filepath
    ///
    ///
    void loadBoxToViwer(QString filepath);
    void UpdateWorkStateOfCurrentFileList(int indCurrentImg = -1);
    void UpdateWorkStateOfAllFileList();

    //
    QLabel *m_CursorTracker;

    QBoxitem::BoxClass     m_nClassId;
    ClassToolBarButton* m_classBarButton;
    QMenu   *ClassViewMenu;
    QAction *ChangeActionClass;
    QAction *ActionNerve;
    QAction *ActionLowerCase;

    ClassToolBarButton  *CameraViewButton;
    QMenu               *CameraViewMenu;

    QAction *ActionCoronal;
    QAction *ActionAxial;
    QAction *ActionSagittal;

    QAction *ActionPropagate;




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

    void UpdateFileListWidget(bool boxrset = true);
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
