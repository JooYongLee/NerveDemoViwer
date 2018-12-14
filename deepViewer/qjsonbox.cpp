
#include "qjsonbox.h"
#include "defineconfigure.h"
#include <QDir>
#include <QDateTime>


void JsonBoxSaver::InitPath()
{
    QDir dirCreator;
    if( dirCreator.mkpath(BASE_PATH_BOXES) == true)
    {

    }
    else
    {

    }
}
QString JsonBoxSaver::GetBasePath()
{
    QDate date = QDateTime::currentDateTime().date();
    int year,month,day;
    date.getDate(&year, &month, &day);
    QString basename = QString("%1%2%3")
              .arg(year,4,10)
              .arg(month,2,10,QChar('0'))
              .arg(day,2,10,QChar('0'));
    QString savepath = QDir(BASE_PATH_BOXES).filePath(basename);

    QDir dirCreator;
    dirCreator.mkpath(savepath);

//    qDebug()<<__FUNCTION__<<savepath;
    return savepath;
}

QJsonArray JsonBoxSaver::createJsonObj(BoxFormat boxes)
{
    JsonBoxSaver::InitPath();

    std::map<QBoxitem::BoxClass,QString> map_boxclas;

    QList<BoxManager> boxfilelist=  boxes.GetBoxes();
    QJsonArray fulljson;

    for(int k = 0; k < boxfilelist.size(); k ++)
    {

        QList<QBoxitem> ss = boxfilelist.at(k).boxmap;

        QJsonObject item;
        QJsonArray boxes[3];
        int count[3] = {0,};
        for(int j = 0; j < ss.size(); j++)
        {
            QJsonArray box;

            int boxid = QBoxitem::BoxClass(ss.at(j).id_class);
            count[boxid]++;

            box<<ss.at(j).left;
            box<<ss.at(j).top;
            box<<ss.at(j).right;
            box<<ss.at(j).bottom;
            boxes[boxid]<<box;
        }

        if( count[QBoxitem::NERVE] >0 )
        {
//            qDebug()<<__FUNCTION__<<"------------------"<<map_boxclas[QBoxitem::NERVE];

//            item[map_boxclas[]] = boxes[QBoxitem::NERVE];
            item["nerve"] = boxes[QBoxitem::NERVE];
        }

        if( count[QBoxitem::LOWERCASE] >0)
        {
            item["lowercase"] = boxes[QBoxitem::LOWERCASE];
        }

        if( count[QBoxitem::NERVE] >0 ||
               count[QBoxitem::LOWERCASE] >0  )
        {
            item["filename"] = boxfilelist.at(k).filename;
            fulljson<<item;
        }
    }
    return fulljson;

}
void JsonBoxSaver::saveJsons(BoxFormat boxes, QString saveDir)
{
    QJsonArray fulljson = JsonBoxSaver::createJsonObj(boxes);
//    foreach( QJsonValue box, boxarray)
    foreach (QJsonValue item, fulljson) {
        if( item.isObject())
        {
            QJsonObject itemObj = item.toObject();

            QStringList keylist = itemObj.keys();

            QString filename;
            foreach (QString key, keylist) {
                if( !key.compare("filename"))
                {
                    filename = key;
                    break;
                }
            }
            if( !filename.isEmpty())
            {
                QString filename = QFileInfo(itemObj["filename"].toString())
                                    .completeBaseName();

                QString abs_save_path = saveDir + '/' +  filename + ".json";
                QFile saveFile(abs_save_path);
                if(!saveFile.open(QIODevice::WriteOnly))
                {
                    continue;
                }

                QJsonDocument saveDoc(itemObj);
                saveFile.write(saveDoc.toJson());
            }

        }

    }
}

void JsonBoxSaver::saveJson(BoxFormat boxes, QString savename)
{

    QJsonArray fulljson = JsonBoxSaver::createJsonObj(boxes);

    QString abs_save_path = QDir(GetBasePath()).filePath(savename);
    qDebug()<<__FUNCTION__<<"==============="<<abs_save_path;
    QFile saveFile(abs_save_path);


    if(!saveFile.open(QIODevice::WriteOnly)){
        qDebug("Cound not open json file to save");

        return ;
    }

    QJsonDocument saveDoc(fulljson);
    saveFile.write(saveDoc.toJson());
}
BoxFormat JsonBoxSaver::loadJsonFromFiles(QStringList box_json_path )
{
    BoxFormat boxes;
    QList<BoxManager> boxesinfo;
    for(int cnt = 0; cnt < box_json_path.count(); cnt++)
    {
        QString path = box_json_path.at(cnt);
        path.toInt();
        BoxManager loadbox = JsonBoxSaver::loadJsonFrom(path);
        boxesinfo.append(loadbox);
    }
    boxes.AddBoxes(boxesinfo);
    return boxes;
}
BoxManager JsonBoxSaver::loadJsonFrom(QString box_json_path )
{
    QFile jsonFile(box_json_path);

    BoxManager boxes;
    if( !jsonFile.open(QFile::ReadOnly) )
    {
//        qDebug()<<__FUNCTION__<<"Cound not open json file"<<box_json_path;
        return boxes;
    }

    //
    boxes.filename = QFileInfo(box_json_path).completeBaseName();
    qDebug()<<__FUNCTION__<<QFileInfo(box_json_path).completeBaseName()<<box_json_path;

    QJsonDocument loadDoc = QJsonDocument().fromJson(jsonFile.readAll());
    if( loadDoc.isObject())
    {
//        qDebug()<<__FUNCTION__<<"json array";
        QJsonObject boxes_a_filse = loadDoc.object();

        QStringList keylist = boxes_a_filse.keys();


        foreach (QString key, keylist) {
            QJsonArray boxarray;

            if( !key.compare("nerve"))
            {

                if( boxes_a_filse[key].isArray() )
                {
                    boxarray = boxes_a_filse[key].toArray();
//                    qDebug()<<"nerve"<<boxes_a_filse[key].toArray();
                }
            }
            else if (!key.compare("lowercase"))
            {
                if( boxes_a_filse[key].isArray() )
                {
                    boxarray = boxes_a_filse[key].toArray();
//                    qDebug()<<"lowercase"<<boxes_a_filse[key].toArray();
                }
            }
            else
            {

            }


            if( !boxarray.isEmpty())
            {
                foreach( QJsonValue box, boxarray)
                {
                    QBoxitem boxitem;
                    QJsonArray aBox = box.toArray();
                    if( aBox.count() == 4 )
                    {
                        boxitem.left    = aBox.at(0).toDouble();
                        boxitem.top     = aBox.at(1).toDouble();
                        boxitem.right   = aBox.at(2).toDouble();
                        boxitem.bottom  = aBox.at(3).toDouble();
                        //                        boxes.boxmap.append(boxitem);

                        if( !key.compare("nerve"))       boxitem.setClass(QBoxitem::NERVE);
                        else if( !key.compare("lowercase")) boxitem.setClass(QBoxitem::LOWERCASE);

                        boxes.boxmap.append(boxitem);
                    }
                }
            }
        }


//        qDebug()<<__FUNCTION__<<"loading boxes---------------->"<<boxes.boxmap.count();
    }

    return boxes;
}

BoxFormat JsonBoxSaver::loadJson(QString box_json_path )
{
    QFile jsonFile(box_json_path);

    BoxFormat boxes;
    if( !jsonFile.open(QFile::ReadOnly) )
    {
        qDebug()<<__FUNCTION__<<"Cound not open json file";
        return boxes;
    }


    QList<BoxManager> boxesinfo;    




    QJsonDocument loadDoc = QJsonDocument().fromJson(jsonFile.readAll());
    if( loadDoc.isArray())
    {
        qDebug()<<__FUNCTION__<<"json array";
        QJsonArray boxes_all_filse = loadDoc.array();
        for( int k = 0; k < boxes_all_filse.count(); k++)
        {
            BoxManager loadedBox;

            QString filename = "";
            if( boxes_all_filse.at(k).isObject() )
            {
                QJsonObject obj = boxes_all_filse.at(k).toObject();
                QStringList keylist = obj.keys();
                QJsonArray boxarray;

                int boxescnt = 0;
                foreach (QString key, keylist) {
                    if( !key.compare("filename"))
                    {
//                        qDebug()<<__FUNCTION__<<key<<obj[key].toString();
                        loadedBox.filename = obj[key].toString();
                    }
                    else if( !key.compare("nerve"))
                    {

                        boxarray = obj[key].toArray();
                    }
                    else if( !key.compare("lowercase"))
                    {
                        boxarray = obj[key].toArray();

                    }
                    else
                    {

                    }


                    if( !boxarray.isEmpty())
                    {

                        foreach( QJsonValue box, boxarray)
                        {
                            QBoxitem boxitem;
                            QJsonArray abox = box.toArray();
                            if( abox.count() == 4 )
                            {
                                boxitem.left = abox.at(0).toDouble();
                                boxitem.top = abox.at(1).toDouble();
                                boxitem.right = abox.at(2).toDouble();
                                boxitem.bottom = abox.at(3).toDouble();

                                //boxitem.set(boxescnt++);
                                if( !key.compare("nerve"))       boxitem.setClass(QBoxitem::NERVE);
                                else if( !key.compare("lowercase")) boxitem.setClass(QBoxitem::LOWERCASE);

                                loadedBox.boxmap.append(boxitem);
                            }

                        }
//                        qDebug()<<__FUNCTION__<<"loading--------------------";

                    }
                }

                boxesinfo.append(loadedBox);

            }
            else
            {

            }
        }

        qDebug()<<__FUNCTION__<<"loading boxes---------------->"<<boxesinfo.count();


    }
    else if( loadDoc.isObject() )
    {
        qDebug()<<__FUNCTION__<<"json obejct";
    }
    else
    {

    }

    boxes.AddBoxes(boxesinfo);
    return boxes;
}
bool JsonBoxSaver::CheckMultipleJsonBox(QStringList jsonList)
{
    foreach (QString filename, jsonList)
    {
        QString filebasename = QFileInfo(filename).completeBaseName();
        bool check;
        int file2int = filebasename.toInt(&check);
        if( check == false || file2int < 0 )
            return false;
    }
    return true;
}
/////////////////////////////////////////////////



void BoxFormat::AddBoxes(QList<BoxManager> boxes)
{
    BoxesList = boxes;
}
void BoxFormat::AddFileList(QStringList filenamelist)
{
    filenameList = filenamelist;
}

void BoxFormat::AddView(QString cameraview)
{
    cameraView = cameraview;
}

void BoxFormat::Release()
{
    BoxesList.clear();
    filenameList.clear();
    cameraView.clear();
}



