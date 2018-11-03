#ifndef IMGREADER_H
#define IMGREADER_H
#include <QPixmap>
#include <QImage>


typedef enum VIEW_FLAG
{
    AXIAL = 0,
    CORONAL,
    SAGITTAL
}_VIEW_FLAG;
bool read_img_from_path(QPixmap &pixmap, QString path);
/*
 * sequenceImg : sequenceImg [depth X height X width ]
 * examples, ) pixel = [5,10,2]-> 5 *(width*height) + 10 * width + 2
 * width / height / depth : sequenceImg dimension
 * view_flag : coronal,axial,sagittal flag
 * pos_flag : slice number
*/
bool read_img_from_seqeunceimg( unsigned char *sequenceImg,
                                int     width,
                                int     height,
                                int     depth,
                                int     view_flag,
                                int     pos_flag,
                                QPixmap &pixmap
                               );
#endif // IMGREADER_H
