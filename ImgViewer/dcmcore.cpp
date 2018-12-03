#include "dcmcore.h"
#include <stdio.h>
#include <iostream>
#include <QDebug>
#include <QThread>
#include <QProgressDialog>
#define SAVE_DELETE_ARRAY(x)        if(x) {delete []x; x = NULL;}
using namespace std;
void TestDcmPrint()
{
    string root_path;

    // SOURCE_CODE_LOCATION is set by cmake during compilation.
    // this contains the path to the source folder.
    // This is only helper for convenience. Without this
    // in_file below should contain absolute path.
    #ifdef SOURCE_CODE_LOCATION
    root_path = SOURCE_CODE_LOCATION;
    #endif

    // where does this example dcm come from, is
    // is explained at https://github.com/marcinwol/dcmtk-basic-example
    string in_file{ root_path + "/DCMIMAGES/77654033/20010101/CR1/6154" };

    DcmFileFormat file_format;
    OFCondition status = file_format.loadFile(in_file.c_str());

    if (status.bad()) {
        cerr << "Problem openning file:" << in_file << endl;
    }

    DcmDataset* dataset = file_format.getDataset();

    OFString patient_name;

    OFCondition condition;
    condition = dataset->findAndGetOFStringArray(DCM_PatientName, patient_name);

    if (condition.good()) {
        cout << "Patient name is: " << patient_name << endl;
    }
    else {
        cerr << "Could not get patient name" << endl;
    }
    cout << "Program finish." << endl;
}
void scale_array( unsigned short *buff, unsigned char *buff8, int length, float div)
{    
    if( buff != NULL && buff8 != NULL)
    {
        for( int ind = 0; ind < length; ind++)
        {
            int value = (int)( (int)buff[ind] * div);
            buff8[ind] = value;
        }
    }
}

bool LoadDicom(std::string imgpath, unsigned char *&buff8, int &width, int &height)
{
    DcmFileFormat dcmFormat;
    dcmFormat.loadFile( imgpath.c_str() );
    DcmDataset *dataSet = dcmFormat.getDataset();
    E_TransferSyntax transferSyntax = dcmFormat.getDataset()->getOriginalXfer();


    double ww, wc;

//        DicomImage *image = new DicomImage(imgpath.c_str());
//        image->getWindow(wc,ww);


    DicomImage *image = new DicomImage(&dcmFormat,
                    transferSyntax,
                    CIF_AcrNemaCompatibility,
                    0,
                    1);


    dataSet->findAndGetFloat64(
        DCM_WindowCenter,
        wc);

    dataSet->findAndGetFloat64(
        DCM_WindowWidth,
        ww);

    image->setWindow(wc, ww);

    width = image->getWidth();
    height = image->getHeight();

    if (image != NULL)
    {
        if (image->getStatus() == EIS_Normal)
        {
            if (image->isMonochrome())
            {
                qDebug()<<__FUNCTION__<<ww<<wc;

                uchar *pixelData = (uchar *)(image->getOutputData(8));//4/* bits */));
//                image->getOutputData(pixelData,);

                if (pixelData != NULL && buff8 == NULL)
                {
//                    qDebug()<<__FUNCTION__<<window_width<<window_center;
                    buff8 = new unsigned char[ height * width];
                    memcpy(buff8, pixelData, width * height * sizeof(unsigned char));

                }
            }
        }
        else
        {
            cerr << "Error: cannot load DICOM image (" << DicomImage::getString(image->getStatus()) << ")" << endl;
            return false;
        }
        delete image;
    }
    else
    {
        return false;
    }
    return true;
}

bool get_data_from_dcm(std::string imgpath, unsigned char *&buff8, int &width, int &height)
{
    DcmFileFormat file_format;
    OFCondition status = file_format.loadFile(imgpath.c_str());

    if (status.bad()) {
        cerr << "Problem openning file:" << imgpath << endl;
        return NULL;
    }

    return LoadDicom(imgpath,buff8, width,height);
}
bool get_volumen_from_dcm(QStringList imgpathes,
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
    return get_volumen_from_dcm(
                path,
                buff8,
                width,
                height,
                depth);
}

bool get_volumen_from_dcm(QStringList imgpathes, unsigned char *&buff8, int &width, int &height, int &depth)
{
    DcmFileFormat file_format;
    int depth_vars = imgpathes.size();
    depth = 0;
    SAFE_DELETE_ARRAY(buff8);

    QProgressDialog progress("Loading files...", "Cancels", 0, depth_vars);
    progress.setWindowTitle("yosi grando");
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(1);


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
       if( LoadDicom((*path).toStdString(),temp_buff, temp_width, temp_height) == true)
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

