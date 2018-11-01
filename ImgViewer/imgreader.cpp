#include <QFileInfo>

#include "imgreader.h"
#include "dcmcore.h"
bool read_img_from_path(QPixmap &pixmap, QString path)
{
    QFileInfo fileinfo(path);
    QString extension = fileinfo.completeSuffix();

    if( extension == "dcm") /// dcm reader
    {
        unsigned char *buff = NULL;
        int width = 0;
        int height = 0;
        if( get_data_from_dcm(
                   path.toStdString(),
                    buff,
                    width,
                    height))
        {
            if( buff != NULL )
            {
                QImage src(width,height, QImage::Format_Grayscale8);
                memcpy( src.bits(), buff, width*height*sizeof(unsigned char));

                delete []buff;
                buff = NULL;

                return pixmap.convertFromImage(src);
            }
            else
            {
                return false;
            }
        }
    }
    else
    {
        return pixmap.load(path);
    }
    return true;
}
