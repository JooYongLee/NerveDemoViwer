#ifndef DEFINECONFIGURE_H
#define DEFINECONFIGURE_H

#define IMG_WIDTH 1000
#define IMG_HEIGHT 900

#define BASE_PATH_BOXES     "d:/deepAnnotation"
enum ImgType {DcmImg, NormImg};
typedef enum VIEW_FLAG
{
    AXIAL = 0,
    CORONAL,
    SAGITTAL
}_VIEW_FLAG;
class ViewConfig
{
public:
    ViewConfig( ImgType _imgType = NormImg,
                VIEW_FLAG _viwFlag = CORONAL
                )
        : imgType(_imgType),
          viewFlag(_viwFlag)
    {}
    ~ViewConfig(){}
    ImgType imgType;
    VIEW_FLAG viewFlag;


};

#endif // DEFINECONFIGURE_H
