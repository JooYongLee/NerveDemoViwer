
#include "qjsonbox.h"


void JsonBoxSaver::saveJson(BoxFormat boxes)
{
    qDebug()<<__FUNCTION__;
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
            qDebug()<<__FUNCTION__<<"------------------"<<map_boxclas[QBoxitem::NERVE];

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
    qDebug()<<__FUNCTION__<<"===============";

    QFile saveFile(QStringLiteral("deviceInfo.json"));

    if(!saveFile.open(QIODevice::WriteOnly)){
        qDebug("Cound not open json file to save");

        return ;
    }

    QJsonDocument saveDoc(fulljson);
    saveFile.write(saveDoc.toJson());
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
                        qDebug()<<__FUNCTION__<<key<<obj[key].toString();
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
                        qDebug()<<__FUNCTION__<<"loading--------------------";

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
