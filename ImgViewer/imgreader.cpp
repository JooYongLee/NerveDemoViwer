#include <QFileInfo>
#include <QDebug>
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
bool read_sequenceimig_depth_direction( unsigned char *sequenceImg,
                                     int     width,
                                     int     height,
                                     int     depth,
                                     int     pos_flag,
                                     QPixmap &pixmap
                                    )
{
    if( pos_flag < 0 || pos_flag >= depth )            return false;
    if( sequenceImg != NULL )
    {
        //[5,10,2] ------->5 *(width*height) + 10 * width + 2
        // [pos_flag,0,0]  ----> pos_flag * width
        unsigned char *pAddr = sequenceImg + pos_flag * width * height ;
        const int     step_01 = width;
        const int     step_00 = 1;
        //const int     step    =   step_01 - width;
        QImage heightimg(width,height,QImage::Format_Grayscale8);
        unsigned char *pImg = heightimg.bits();
        qDebug()<<"depth"<<depth<<"width"<<width;

        for(int h = 0; h < height; h++)
        {
            for(int w = 0; w < width; w++)
            {

                (*pImg++) = (*pAddr);
                pAddr += step_00;
            }
           // pAddr += step;
        }
        return pixmap.convertFromImage(heightimg);
    }
    else
    {
        return false;
    }
    return true;
}
bool read_sequenceimig_width_direction( unsigned char *sequenceImg,
                                     int     width,
                                     int     height,
                                     int     depth,
                                     int     pos_flag,
                                     QPixmap &pixmap
                                    )
{
    if( pos_flag < 0 || pos_flag >= width )            return false;
    if( sequenceImg != NULL )
    {
        //[5,10,2] ------->5 *(width*height) + 10 * width + 2
        unsigned char *pAddr = sequenceImg + pos_flag + (height-1)* width ;
        //const int     step_01 = width*height;
        const int     step_00 = width;
        const int     step    =   2 * width * height;
        QImage heightimg(width,depth,QImage::Format_Grayscale8);
        unsigned char *pImg = heightimg.bits();
        qDebug()<<"depth"<<depth<<"width"<<width;

        for(int d = 0; d < depth; d++)
        {
            for(int h = 0; h < height; h++)
            {

                (*pImg++) = (*pAddr);
                pAddr -= step_00;
            }
            pAddr += step;
        }
        return pixmap.convertFromImage(heightimg);
    }
    else
    {
        return false;
    }
    return true;
}

bool read_sequenceimig_height_direction( unsigned char *sequenceImg,
                                     int     width,
                                     int     height,
                                     int     depth,
                                     int     pos_flag,
                                     QPixmap &pixmap
                                    )
{
    if( pos_flag < 0 || pos_flag >= height )            return false;
    if( sequenceImg != NULL )
    {
        //[5,10,2] ------->5 *(width*height) + 10 * width + 2
        // [0,pos_flag,0]  ----> pos_flag * width
        unsigned char *pAddr = sequenceImg + pos_flag * width ;
        const int     step_01 = width*height;
        const int     step_00 = 1;
        const int     step    =   step_01 - width;
        QImage heightimg(width,depth,QImage::Format_Grayscale8);
        unsigned char *pImg = heightimg.bits();
        qDebug()<<"depth"<<depth<<"width"<<width;

        for(int d = 0; d < depth; d++)
        {
            for(int w = 0; w < width; w++)
            {

                (*pImg++) = (*pAddr);
                pAddr += step_00;
            }
            pAddr += step;
        }
        return pixmap.convertFromImage(heightimg);
    }
    else
    {
        return false;
    }
}

/*
 *
 *
*/
bool read_img_from_seqeunceimg( unsigned char *sequenceImg,
                                int     width,
                                int     height,
                                int     depth,
                                int     view_flag,
                                int     pos_flag,
                                QPixmap &pixmap
                               )
{
    bool    status = true;
    if( sequenceImg != NULL )
    {
        if( view_flag == SAGITTAL )
        {
            read_sequenceimig_width_direction(sequenceImg, width, height, depth, pos_flag, pixmap);
        }
        else if( view_flag == AXIAL )
        {
            read_sequenceimig_depth_direction(sequenceImg, width, height, depth, pos_flag, pixmap);
        }
        else if( view_flag == CORONAL )
        {
            read_sequenceimig_height_direction(sequenceImg, width, height, depth, pos_flag, pixmap);
        }
        else
        {
            status = false;
        }
        return true;
    }
    else        //if( sequenceImg != NULL )
    {
        status = false;
    }
    return status;
}
