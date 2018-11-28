#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <QMainWindow>
#include <QDir>
/**
 * @brief The FileManger class
 */
class FileManger : public QDir
{
public:
    FileManger(QString path ="");
    ~FileManger();
    int     num_files;
    QString     img_path;
    QStringList file_list;
 public:
    void clear();
    void ResearchImgList();
    QStringList GetImgList();
    QString GetImgPath(int n);
    QString GetImgPath(int n,int m);
    QString GetBasePath() const {   return img_path;    }
    void UpdateFileList();
    void WriteBoxImg();
    void WriteImg();
};
#endif // FILEMANAGER_H
