#include "dcmcore.h"
#include <stdio.h>
#include <iostream>
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
    double dScale = 255.0 / (3072.0 + 2304.0);
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
    DicomImage *image = new DicomImage(imgpath.c_str());
    if (image != NULL)
    {
        if (image->getStatus() == EIS_Normal)
        {
            if (image->isMonochrome())
            {
                image->setMinMaxWindow();
                double dmin;
                double dmax;

                image->getMinMaxValues(dmin,dmax);

                cout<<"min"<<dmin<<"max"<<dmax<<"size"<<image->getOutputDataSize();


                height =  image->getHeight();
                width = image->getWidth();

                Uint16 *pixelData = (Uint16 *)(image->getOutputData(14/* bits */));
//                image->getOutputData(pixelData,);

                if (pixelData != NULL && buff8 == NULL)
                {
                    buff8 = new unsigned char[ height * width];
                    memset(buff8, 0, width * height * sizeof(unsigned char));

                    cout << "loading complete "<<endl;
                    cout << "img width" << width << endl;
                    cout << "img height" << height << endl;
                    scale_array(pixelData, buff8, height * width, 255.0/16383.0 );
                    /* do something useful with the pixel data */
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

