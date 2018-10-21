#include <QCoreApplication>

//#include "opencv2/opencv.hpp"
#include <opencv2/opencv.hpp>
#include <QDir>
#include <QDebug>

int main(int argc, char *argv[])
{
    cv::Mat image = cv::imread("F://Image//yuna.jpg", 1);
    if (!image.empty())
    {
        // create image window named "My Image"
        cv::namedWindow("My Image");
        // show the image on window
        cv::imshow("My Image", image);
        cv::waitKey(0);
    }
    QCoreApplication a(argc, argv);
    return a.exec();
}
