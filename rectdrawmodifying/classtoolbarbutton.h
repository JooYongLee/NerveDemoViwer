#ifndef CLASSTOOLBARBUTTON_H
#define CLASSTOOLBARBUTTON_H

#include <QAction>
#include <QToolButton>
#include <QDebug>

class ClassToolBarButton : public QToolButton
{
    Q_OBJECT
public:
    explicit ClassToolBarButton(QWidget *parent = 0);
    //~CameraViewButton();
public slots:
    void setNextAction(QAction*);
    void setNextAction();
};

#endif // CLASSTOOLBARBUTTON_H


