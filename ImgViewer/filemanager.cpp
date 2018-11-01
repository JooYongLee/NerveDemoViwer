#include "filemanager.h"
FileManger::FileManger(QString path)
    :QDir(path)
{

}

FileManger::~FileManger()
{
}
void FileManger::ResearchImgList()
{
    this->setNameFilters(QStringList()<<\
                         "*.jpg"<<\
                         "*.png"<<\
                         "*.bmp"<<
                         "*.dcm");

    file_list = this->entryList();
    UpdateFileList();
}

QStringList FileManger::GetImgList()
{
    return file_list;
}
QString FileManger::GetImgPath(int n)
{
    QString path;

    if( n < file_list.size())
    {
        path = img_path + '/' + file_list[n];
    }

    return path;
}
QString FileManger::GetImgPath(int n,int m)
{
    Q_ASSERT(m>0);
    double  norm_pos = (double)n/m;
    int  ind = (int)(norm_pos * file_list.size());
    return GetImgPath(ind);
}

void FileManger::UpdateFileList()
{
    num_files = file_list.size();
    img_path = this->absolutePath();
}
