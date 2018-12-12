#ifndef QJSONBOX_H
#define QJSONBOX_H
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>

#include "boundingbox.h"


class BoxFormat
{
public:
    BoxFormat(){}
    BoxFormat(QList<BoxManager> boxes,
              QStringList filenamelist,
              QString cameraView)
    {
        AddBoxes(boxes);
        AddFileList(filenamelist);
        AddView(cameraView);
    }

    ~BoxFormat(){}

    void AddBoxes(QList<BoxManager> boxes);
    void AddFileList(QStringList filenamelist);
    void AddView(QString cameraview);

    void Release();

    QList<BoxManager> GetBoxes(){return BoxesList;}
    QStringList GetFileList(){return filenameList;}
    QString GetCameraVeiw(){return cameraView;}


private:

    QList<BoxManager>   BoxesList;
    QStringList         filenameList;
    QString             cameraView;

};

class JsonBoxSaver
{
public:
    JsonBoxSaver(){}
    ~JsonBoxSaver(){}

    static QString GetBasePath();
    static void InitPath();

    static QJsonArray createJsonObj(BoxFormat boxes);
    static void saveJsons(BoxFormat boxes, QString saveDir);

    static void saveJson(BoxFormat boxes, QString savename);
    static BoxFormat loadJson(QString box_json_path);
    static BoxFormat loadJsonFromFiles(QStringList box_json_path );
    static BoxManager loadJsonFrom(QString box_json_path );
    static bool CheckMultipleJsonBox(QStringList jsonList);

};

#endif // QJSONBOX_H
