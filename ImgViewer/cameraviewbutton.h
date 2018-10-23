#ifndef CAMERAVIEWBUTTON_H
#define CAMERAVIEWBUTTON_H
#include <QToolButton>
#include <QDebug>

class CameraViewButton : public QToolButton
{
    Q_OBJECT
public:
    explicit CameraViewButton(QWidget *parent = 0);
    //~CameraViewButton();
};
#endif // CAMERAVIEWBUTTON_H
