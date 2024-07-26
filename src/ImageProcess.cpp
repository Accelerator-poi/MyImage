#include "ImageProcess.h"



void ImageProcess::setImg(QString filename)
{
    if(filename.isEmpty())
    {
        return;
    }

    cv::Mat img = cv::imread(filename.toStdString());
    if(img.empty())
    {
        return;
    }

    this->currentImage = img;
}

QImage ImageProcess::mat2QImage(const cv::Mat &mat)
{
    if (mat.empty())
    {
        return QImage();
    }
    cv::Mat rgb;
    cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
    return QImage((const unsigned char *)rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
}
