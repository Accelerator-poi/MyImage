#include "ImageProcess.h"
#include <QDebug> // Add this line to include the QDebug header

void ImageProcess::setImg(QString filename)
{
    qDebug() << "ImageProcess::setImg() called";
    if (filename.isEmpty())
    {
        return;
    }

    cv::Mat img = cv::imread(filename.toStdString());
    if (img.empty())
    {
        return;
    }

    this->currentImage = img;
    qDebug() << "Image loaded";
}

void ImageProcess::Gray()
{
    if (this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }

    cv::Mat grayImg;
    cv::cvtColor(this->currentImage, grayImg, cv::COLOR_BGR2GRAY);
    emit imageReady(mat2QImage(grayImg));
    qDebug() << "Gray image ready";
}

void ImageProcess::Binary(int threshold)
{
    if (this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }

    cv::Mat grayImg;
    cv::cvtColor(this->currentImage, grayImg, cv::COLOR_BGR2GRAY);
    cv::Mat binaryImg;
    cv::threshold(grayImg, binaryImg, threshold, 255, cv::THRESH_BINARY);
    emit imageReady(mat2QImage(binaryImg));
    qDebug() << "Binary image ready";
}

void ImageProcess::Transform(int size, int angle)
{
    cv::Mat NewImage;
    double scale = size / 10.0;

    // 缩放图像
    cv::resize(this->currentImage, NewImage, cv::Size(), scale, scale, cv::INTER_LINEAR);
    cv::Rect cropRect = cv::Rect(NewImage.cols/2, NewImage.rows/2, 
                                this->currentImage.cols, this->currentImage.rows);
    cropRect.width = std::min(cropRect.width, NewImage.cols);
    cropRect.height = std::min(cropRect.height, NewImage.rows);
    cropRect.x = std::min(std::max(cropRect.x, 0), NewImage.cols - cropRect.width);
    cropRect.y = std::min(std::max(cropRect.y, 0), NewImage.rows - cropRect.height);

    NewImage = NewImage(cropRect);
    // cv::imshow("NewImage", NewImage);
    // 计算缩放后的中心点
    cv::Point2f center(NewImage.cols / 2.0, NewImage.rows / 2.0);

    // 旋转图像
    cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
    cv::warpAffine(NewImage, NewImage, rot, NewImage.size(),
                   cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
    // cv::imshow("NewImage", NewImage);

    emit imageReady(mat2QImage(NewImage));
    qDebug() << "Transformed image ready";
}

void ImageProcess::Mean()
{
    if(this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }
    cv::Mat dst;
    cv::blur(this->currentImage, dst, cv::Size(5, 5));
    emit imageReady(mat2QImage(dst));
}

void ImageProcess::Middle()
{
    if(this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }
    cv::Mat dst;
    cv::medianBlur(this->currentImage, dst, 5);
    emit imageReady(mat2QImage(dst));
}

void ImageProcess::Gaussian()
{
    if(this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }
    cv::Mat dst;
    cv::GaussianBlur(this->currentImage, dst, cv::Size(5, 5), 1);
    emit imageReady(mat2QImage(dst));
}

void ImageProcess::LowPass()
{
    if(this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }
    cv::Mat dst;
    cv::GaussianBlur(this->currentImage, dst, cv::Size(21, 21), 0);
    emit imageReady(mat2QImage(dst));
}

void ImageProcess::HighPass()
{
    if(this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }
    cv::Mat dst;
    cv::Mat lowPass;
    cv::GaussianBlur(this->currentImage, lowPass, cv::Size(21, 21), 0);
    cv::addWeighted(this->currentImage, 1.5, lowPass, -0.5, 0, dst);
    emit imageReady(mat2QImage(dst));
}

QImage ImageProcess::mat2QImage(const cv::Mat &mat)
{
 if (mat.empty())
    {
        qDebug() << "Empty image";
        return QImage();
    }

    // 检查图像类型并进行相应的转换
    if (mat.type() == CV_8UC1) // 灰度图像
    {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
        return image.copy();
    }
    else if (mat.type() == CV_8UC3) // 彩色图像
    {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped().copy(); // OpenCV的BGR格式转换为Qt的RGB格式
    }
    else
    {
        qDebug() << "Unsupported image format";
        return QImage();
    }
}
