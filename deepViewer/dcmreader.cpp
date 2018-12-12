#include "dcmreader.h"
#include <QDebug>
#define SAVE_DELETE_ARRAY(x)        if(x) {delete []x; x = NULL;}
using namespace std;
//void TestDcmPrint()
//{
//    string root_path;

//    // SOURCE_CODE_LOCATION is set by cmake during compilation.
//    // this contains the path to the source folder.
//    // This is only helper for convenience. Without this
//    // in_file below should contain absolute path.
//    #ifdef SOURCE_CODE_LOCATION
//    root_path = SOURCE_CODE_LOCATION;
//    #endif

//    // where does this example dcm come from, is
//    // is explained at https://github.com/marcinwol/dcmtk-basic-example
//    string in_file{ root_path + "/DCMIMAGES/77654033/20010101/CR1/6154" };

//    DcmFileFormat file_format;
//    OFCondition status = file_format.loadFile(in_file.c_str());

//    if (status.bad()) {
//        cerr << "Problem openning file:" << in_file << endl;
//    }

//    DcmDataset* dataset = file_format.getDataset();

//    OFString patient_name;

//    OFCondition condition;
//    condition = dataset->findAndGetOFStringArray(DCM_PatientName, patient_name);

//    if (condition.good()) {
//        cout << "Patient name is: " << patient_name << endl;
//    }
//    else {
//        cerr << "Could not get patient name" << endl;
//    }
//    cout << "Program finish." << endl;
//}
//void scale_array( unsigned short *buff, unsigned char *buff8, int length, float div)
//{
//    if( buff != NULL && buff8 != NULL)
//    {
//        for( int ind = 0; ind < length; ind++)
//        {
//            int value = (int)( (int)buff[ind] * div);
//            buff8[ind] = value;
//        }
//    }
//}
//void scale_array( short *buff, unsigned char *buff8, int length, double dMin, double dMax)
//{
//    double dScale = 255 / (dMax - dMin);
//    if( dMax == dMin)
//        dScale = 0;

//    if( buff != NULL && buff8 != NULL)
//    {
//        for( int ind = 0; ind < length; ind++)
//        {
//            int value = (int)( (buff[ind] - dMin) * dScale);
//            buff8[ind] = value;
//        }
//    }
//}
//void get_min_max( short *buff, int length, double &dMin, double &dMax)
//{
//    dMin = 100000;
//    dMax = -10000;
//    if( buff != NULL )
//    {
//        for( int ind = 0; ind < length; ind++)
//        {
//            if( buff[ind] < dMin )  dMin = buff[ind];
//            if( buff[ind] > dMax )  dMax = buff[ind];
//        }
//    }
//}

bool dcmFileReader::LoadDicom(QString imgpath, unsigned char *&buff8, int &width, int &height)
{
//    DicomImage *image = new DicomImage(imgpath.c_str());

    DcmFileFormat infile;

    if (!infile.loadFile(imgpath.toStdString().c_str()).good())
    {
        return false;
    }
    //    DVPresentationState pstate; // presentation state handler
    //    DcmDataset *dataSet = infile.getDataset();
//    E_TransferSyntax transferSyntax = infile.getDataset()->getCurrentXfer();
    E_TransferSyntax transferSyntax = infile.getDataset()->getOriginalXfer();

    DicomImage *dcmimage = new DicomImage(&infile,
                                       transferSyntax,
                                       CIF_AcrNemaCompatibility);

//    DcmDataset *dataSet = dcmFormat.getDataset();

//	E_TransferSyntax transferSyntax = dcmFormat.getDataset()->getOriginalXfer();

//	pDcmImg = new DicomImage(
//		&dcmFormat,
//		transferSyntax,
//		CIF_AcrNemaCompatibility,
//		0,
//		1);
    static int cnt = 0;
    if (dcmimage != NULL)
    {
        if (dcmimage->getStatus() == EIS_Normal)
        {
            DcmDataset *dataSet = infile.getDataset();

            double window_width = 0;
            double window_center = 0;

            dataSet->findAndGetFloat64(
                        DCM_WindowCenter,
                        window_center);

            dataSet->findAndGetFloat64(
                        DCM_WindowWidth,
                        window_width);

            double volume_width = 0;
            double volume_height = 0;
            double volume_depth = 0;
            dataSet->findAndGetFloat64(
                        DCM_ImagedVolumeWidth,
                        volume_width);
            dataSet->findAndGetFloat64(
                        DCM_ImagedVolumeHeight,
                        volume_height);
            dataSet->findAndGetFloat64(
                        DCM_ImagedVolumeDepth,
                        volume_depth);


//            dcmimage->getOutputPlane()
            dcmimage->setWindow(window_center, window_width);

            width =  dcmimage->getWidth();
            height = dcmimage->getHeight();
//            dcmimage->getDepth()

            qDebug()<<__FUNCTION__<<volume_width<<volume_height<<volume_depth;


//            qDebug()<<__FUNCTION__<<window_center<<"x"<<window_width;



//            qDebug()<<__FUNCTION__<<height<<width<<size;
//            result.resize(size);
            if( dcmimage->isMonochrome())
            {
                qDebug()<<__FUNCTION__<<"monochrome";
            }
            else
            {
                qDebug()<<__FUNCTION__<<"not monochrome";
            }

//            return statusToCondition(image_->getOutputData(&result[0], size, bits, frame));
//createWindowsDIB

//            if(pDicomDibits)
//                delete pDicomDibits;

            unsigned char *pixelData = (unsigned char *)(dcmimage->getOutputData(8));
//            unsigned short *pixelData = (unsigned short *)(dcmimage->getOutputData(8));
            if (pixelData != NULL && buff8 == NULL)
            {
                buff8 = new unsigned char[ height * width];
//                for(int k = 0;k<width*height;k++)
//                    buff8[k] = pixelData[k]/255;

                memcpy(buff8, pixelData, width*height*sizeof(unsigned char));
            }
//            QImage tmp(width,height,QImage::Format_Grayscale8);
//            unsigned char *pImg = tmp.bits();
//            memcpy(pImg,buff8,sizeof(unsigned char)*width*height);
//            tmp.save(QString("%1.png").arg(cnt++));


//            img.fromData()

        }
        else
        {
            cerr << "Error: cannot load DICOM image (" << DicomImage::getString(dcmimage->getStatus()) << ")" << endl;
            return false;
        }
        delete dcmimage;
    }
    else
    {
        return false;
    }
    return true;
}

//bool get_data_from_dcm(std::string imgpath, unsigned char *&buff8, int &width, int &height)
//{
//    DcmFileFormat file_format;
//    OFCondition status = file_format.loadFile(imgpath.c_str());

//    if (status.bad()) {
//        cerr << "Problem openning file:" << imgpath << endl;
//        return NULL;
//    }

//    return LoadDicom(imgpath,buff8, width,height);
//}

dcmFileReader::dcmFileReader(void)
    : m_nWidth(0),
      m_nHeight(0),
      m_nDepth(0),
      m_cBuffer(nullptr)
{

}

void dcmFileReader::SetDicomPath(QStringList imgList, QString basepath)
{
    m_imglist.clear();
    foreach (QString imgfilename, imgList)
    {
        m_imglist.push_back( basepath + "/" +  imgfilename );
    }
}
bool dcmFileReader::AllSave()
{
    for(int k = 0; k < m_nDepth;k++)
    {
//            #define TEST_DEBUG
//            #ifdef TEST_DEBUG
//                        QImage testsave(m_nWidth,m_nHeight,QImage::Format_Grayscale8);
//                        unsigned char *pImg = testsave.bits();
//                        memcpy(pImg, m_cBuffer + m_nWidth*m_nHeight*k, sizeof(uchar)*m_nWidth*m_nHeight);
//                        testsave.save(QString("%1.png").arg(k++));
//            #endif

    }
    return true;
}

bool dcmFileReader::GetVolumeBuffer()
{
    SAFE_DELETE_ARRAY(m_cBuffer);

    if( m_imglist.count() == 0 )
    {
        qDebug()<<__FUNCTION__<<"image list zeros";
        return false;
    }
    qDebug()<<__FUNCTION__;


    if( dcmFileReader::GetVolumeFromDCM(m_imglist,
                                    m_cBuffer,
                                    m_nWidth,
                                    m_nHeight,
                                    m_nDepth
                                    ))
    {

        return true;
    }
    else
    {
        return false;
    }

}

bool dcmFileReader::GetVolumeFromDCM(QStringList imgpathes,
                          QString      abspath,
                          unsigned char *&buff8,
                          int &width,
                          int &height,
                          int &depth)
{
    QStringList path;
    for( QStringList::iterator filename = imgpathes.begin();
         filename < imgpathes.end();
         filename++)
    {
        path.push_back( abspath + "/" +  *filename );
    }
    return dcmFileReader::GetVolumeFromDCM(
                path,
                buff8,
                width,
                height,
                depth);
}

bool dcmFileReader::GetVolumeFromDCM(QStringList imgpathes,
                                     unsigned char *&buff8,
                                     int &width,
                                     int &height,
                                     int &depth)
{
    DcmFileFormat file_format;
    int depth_vars = imgpathes.size();
    depth = 0;
//    SAFE_DELETE_ARRAY(buff8);

    QProgressDialog progress("Loading files...", "Cancels", 0, depth_vars);
    progress.setWindowTitle("Dicom Volume Loader");
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(1);
    progress.show();


    int move_pitch = 0;
    int pitch = 0;
    for(  QStringList::iterator path = imgpathes.begin(); path < imgpathes.end(); path++)
    {


         progress.setValue(depth);
        OFCondition status = file_format.loadFile( (*path).toStdString().c_str() );

        if (progress.wasCanceled())
        {
            break;
        }
        // invalid dicom files!!
        if( status.bad()  ) continue;

       unsigned char *temp_buff = NULL;
       int temp_width = 0;
       int temp_height = 0;
       if( LoadDicom((*path),temp_buff, temp_width, temp_height) == true)
       {
           //creating only once
           if( buff8  == NULL )
           {
               width = temp_width;
               height = temp_height;
               pitch = width*height;
               buff8 = new unsigned char[width*height*depth_vars];
           }

           if( width == temp_width &&
                   height == temp_height )
           {
               memcpy(buff8 + move_pitch, temp_buff, sizeof(unsigned char)*pitch);
               depth++;
               move_pitch += pitch;
           }
           SAFE_DELETE_ARRAY(temp_buff);

           qDebug()<<__FUNCTION__<<*path<<"loading complte";
       }
    }


    progress.setValue(depth_vars);

    if( depth != depth_vars)
    {
        SAFE_DELETE_ARRAY(buff8);
        return false;
    }
    else
    {
        return true;
    }

}


//bool read_img_from_path(QPixmap &pixmap, QString path)
//{
//    QFileInfo fileinfo(path);
//    QString extension = fileinfo.completeSuffix();

//    if( extension == "dcm") /// dcm reader
//    {
//        unsigned char *buff = NULL;
//        int width = 0;
//        int height = 0;
//        if( get_data_from_dcm(
//                   path.toStdString(),
//                    buff,
//                    width,
//                    height))
//        {
//            if( buff != NULL )
//            {
//                QImage src(width,height, QImage::Format_Grayscale8);
//                memcpy( src.bits(), buff, width*height*sizeof(unsigned char));

//                delete []buff;
//                buff = NULL;

//                return pixmap.convertFromImage(src);
//            }
//            else
//            {
//                return false;
//            }
//        }
//    }
//    else
//    {
//        return pixmap.load(path);
//    }
//    return true;
//}
///////////////////////////////////////////////////////////////////
/// \brief read_sequenceimig_depth_direction
/// \param sequenceImg
/// \param width
/// \param height
/// \param depth
/// \param pos_flag
/// \param pixmap
/// \return
///
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

int dcmFileReader::GetSliceSize(VIEW_FLAG typeflag)
{
    if( typeflag == CORONAL )
    {
        return m_nHeight;
    }
    else if( typeflag == AXIAL )
    {
        return m_nDepth;
    }
    else if( typeflag == SAGITTAL )
    {
        return m_nWidth;
    }
    else
    {
        return 0;
    }
}

/*
 *
 *
*/
bool dcmFileReader::ReadPixmapFromVolume(
                                int     view_flag,
                                int     pos_flag,
                                QPixmap &pixmap
                               )
{
    //            #define TEST_DEBUG
    //            #ifdef TEST_DEBUG
    //                        QImage testsave(m_nWidth,m_nHeight,QImage::Format_Grayscale8);
    //                        unsigned char *pImg = testsave.bits();
    //                        memcpy(pImg, m_cBuffer + m_nWidth*m_nHeight*k, sizeof(uchar)*m_nWidth*m_nHeight);
    //                        testsave.save(QString("%1.png").arg(k++));
    //            #endif
    bool    status = true;
    int     width   =   m_nWidth;
    int     height   =   m_nHeight;
    int     depth   =   m_nDepth;
    if( m_cBuffer != NULL )
    {
        if( view_flag == SAGITTAL )
        {
            read_sequenceimig_width_direction(m_cBuffer, width, height, depth, pos_flag, pixmap);
        }
        else if( view_flag == AXIAL )
        {
            read_sequenceimig_depth_direction(m_cBuffer, width, height, depth, pos_flag, pixmap);
        }
        else if( view_flag == CORONAL )
        {
            read_sequenceimig_height_direction(m_cBuffer, width, height, depth, pos_flag, pixmap);
        }
        else
        {
            status = false;
        }
        qDebug()<<__FUNCTION__<<"loading complete"<<pos_flag;
//        pixmap.save(QString("%1.png").arg(pos_flag));

        return true;
    }
    else        //if( sequenceImg != NULL )
    {
        status = false;
    }
    return status;
}


