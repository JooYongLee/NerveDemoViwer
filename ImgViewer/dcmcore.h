#ifndef DCMCORE_H
#define DCMCORE_H
#include <iostream>
#include <string>
#include <QStringList>
#include <QString>

#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimgle/dcmimage.h"

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

#endif // DCMCORE_H
