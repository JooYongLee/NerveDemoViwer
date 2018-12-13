#ifndef DCMREADER_H
#define DCMREADER_H

#include <QStringList>
#include <stdio.h>
#include <iostream>
#include <QDebug>
#include <QThread>
#include <QProgressDialog>
#include <QPixmap>
#include <QFileInfo>

#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include "defineconfigure.h"


class dcmFileReader
{
public:
    dcmFileReader(void);
    void SetDicomPath(QStringList imgList, QString basepath);
    virtual ~dcmFileReader(){}


    bool GetVolumeBuffer();

    int GetSliceSize(VIEW_FLAG typeflag);
    bool AllSave();

    //////////////////////////////////////////////
    /// \brief read_img_from_seqeunceimg  examples, ) pixel = [5,10,2]-> 5 *(width*height) + 10 * width + 2
    /// \param sequenceImg   epth X height X width ]
    /// \param width sequenceImg dimension
    /// \param height
    /// \param depth
    /// \param view_flag :  coronal,axial,sagittal flag
    /// \param pos_flag : slice number
    /// \param pixmap
    /// \return
    ///
    bool ReadPixmapFromVolume(  int     view_flag,
                                int     pos_flag,
                                QPixmap &pixmap
                                   );
private:
    int     m_nWidth;
    int     m_nHeight;
    int     m_nDepth;
    unsigned\
    char*   m_cBuffer;

    QStringList m_imglist;


public:
    ///////////////////////////////////////////////////////
    /// \brief GetVolumeFromDCM
    /// \param imgpathes dicom image list QStringList
    /// \param abspath base base of the image
    /// \param buff8 dst buffer 8bit
    /// \param width dicom dimension
    /// \param height dicom dimension
    /// \param depth dicom dimension
    /// \return if read successfully, return true otherwise false
    ///
    static bool GetVolumeFromDCM(QStringList imgpathes,
                                 QString      abspath,
                                 unsigned char *&buff8,
                                 int &width,
                                 int &height,
                                 int &depth);
    static bool GetVolumeFromDCM(QStringList imgpathes,
                                 unsigned char *&buff8,
                                 int &width,
                                 int &height,
                                 int &depth);
    ////////////////////////////////////////////////////////
    /// \brief LoadDicom
    /// \param imgpath image path
    /// \param buff8 dst buffer return
    /// \param width
    /// \param height
    /// \return
    ///
    static bool LoadDicom(QString imgpath,
                          unsigned char *&buff8,
                          int &width,
                          int &height);

};

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(_X_)   if(_X_) {   delete []_X_; _X_ = NULL;}
#endif

void TestDcmPrint();

bool get_data_from_dcm(std::string imgpath,
                       unsigned char *&buff8,
                       int &width,
                       int &height);
bool get_volumen_from_dcm(QStringList imgpathes,
                          unsigned char *&buff8,
                          int &width, int &height,
                          int &depth);
bool get_volumen_from_dcm(QStringList imgpathes,
                          QString      abspath,
                          unsigned char *&buff8,
                          int &width,
                          int &height,
                          int &depth);



bool read_img_from_path(QPixmap &pixmap, QString path);

namespace BufferSaver
{
    void Pgm8Save(char filename[], void *buff, int buffsize, int width, int height, int maxValue);
    void Pgm16Save(char filename[], unsigned short *buff, int buffsize, int width, int height, int maxValue);
}

/*
 * sequenceImg : sequenceImg [depth X height X width ]
 * examples, ) pixel = [5,10,2]-> 5 *(width*height) + 10 * width + 2
 * width / height / depth : sequenceImg dimension
 * view_flag : coronal,axial,sagittal flag
 * pos_flag : slice number
*/



//if( read_img_from_seqeunceimg(this->_sequenceImage,
//                          this->_sequenceImageSize.width,
//                          this->_sequenceImageSize.height,
//                          this->_sequenceImageSize.depth,
//                          CORONAL,
//                          pos_flag,
//                          pixmap[ind]
//                          ))

#endif // DCMREADER_H
