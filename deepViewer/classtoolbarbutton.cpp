#include "classtoolbarbutton.h"
#include <QDebug>
ClassToolBarButton::ClassToolBarButton(QWidget *parent)
    :
    QToolButton(parent)
{
//    this->autoRepeat();
//    this->autoRepeatInterval();
//    this->autoRaise();
    setPopupMode(QToolButton::MenuButtonPopup);
    QObject::connect(this, SIGNAL(triggered(QAction*)),
                     this, SLOT(setDefaultAction(QAction*)));
    //QObject::connect(this, SIGNAL(toggled(bool)), this ,SLOT((setNextAction(bool))));
//    this->setCheckable(true);

//    QObject::connect(this, SIGNAL(clicked(bool)), this, SLOT(setNextAction()));

}
void ClassToolBarButton::setNextAction()
{
    qDebug()<<__FUNCTION__;
}

void ClassToolBarButton::setNextAction(QAction *action)
{
    qDebug()<<__FUNCTION__;
}

