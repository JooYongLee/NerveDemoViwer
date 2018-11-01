#ifndef DCMCORE_H
#define DCMCORE_H
#include <iostream>
#include <string>

#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimgle/dcmimage.h"
void TestDcmPrint();

bool get_data_from_dcm(std::string imgpath, unsigned char *&buff8, int &width, int &height);

#endif // DCMCORE_H
