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

// void ImageProcess::UpdateImage(QImage image)
// {
//     if (image.isNull())
//     {
//         qDebug() << "Empty image";
//         return;
//     }

//     cv::Mat img = QImage2mat(image);
//     if (img.empty())
//     {
//         qDebug() << "Empty image";
//         return;
//     }

//     this->currentImage = img;
//     qDebug() << "Image updated";
// }

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
    this->currentImage = grayImg;
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
    if(this->currentImage.channels() == 3)
        cv::cvtColor(this->currentImage, grayImg, cv::COLOR_BGR2GRAY);
    else
        grayImg = this->currentImage;
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
    cv::Rect cropRect = cv::Rect(NewImage.cols / 2, NewImage.rows / 2,
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
    if (this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }
    cv::Mat dst;
    cv::blur(this->currentImage, dst, cv::Size(5, 5));
    emit imageReady(mat2QImage(dst));
    this->currentImage = dst;
}

void ImageProcess::Middle()
{
    if (this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }
    cv::Mat dst;
    cv::medianBlur(this->currentImage, dst, 5);
    emit imageReady(mat2QImage(dst));
    this->currentImage = dst;
}

void ImageProcess::Gaussian()
{
    if (this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }
    cv::Mat dst;
    cv::GaussianBlur(this->currentImage, dst, cv::Size(5, 5), 1);
    emit imageReady(mat2QImage(dst));
    this->currentImage = dst;
}

void ImageProcess::LowPass()
{
    if (this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }
    cv::Mat dst;
    cv::GaussianBlur(this->currentImage, dst, cv::Size(21, 21), 0);
    emit imageReady(mat2QImage(dst));
    this->currentImage = dst;
}

void ImageProcess::HighPass()
{
    if (this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }
    cv::Mat dst;
    cv::Mat lowPass;
    cv::GaussianBlur(this->currentImage, lowPass, cv::Size(21, 21), 0);
    cv::addWeighted(this->currentImage, 1.5, lowPass, -0.5, 0, dst);
    emit imageReady(mat2QImage(dst));
    this->currentImage = dst;
}

void ImageProcess::Corrosion()
{
    if (this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }
    cv::Mat dst;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
                                                cv::Size(5, 5), cv::Point(-1, -1));
    cv::erode(this->currentImage, dst, element);
    emit imageReady(mat2QImage(dst));
    this->currentImage = dst;
}

void ImageProcess::Expansion()
{
    if (this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }
    cv::Mat dst;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
                                                cv::Size(5, 5), cv::Point(-1, -1));
    cv::dilate(this->currentImage, dst, element);
    emit imageReady(mat2QImage(dst));
    this->currentImage = dst;
}

void ImageProcess::Opening()
{
    if (this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }
    cv::Mat dst;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
                                                cv::Size(5, 5), cv::Point(-1, -1));
    cv::morphologyEx(this->currentImage, dst, cv::MORPH_OPEN, element);
    emit imageReady(mat2QImage(dst));
    this->currentImage = dst;
}

void ImageProcess::Closing()
{
    if (this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }
    cv::Mat dst;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
                                                cv::Size(5, 5), cv::Point(-1, -1));
    cv::morphologyEx(this->currentImage, dst, cv::MORPH_CLOSE, element);
    emit imageReady(mat2QImage(dst));
    this->currentImage = dst;
}

void ImageProcess::Roberts()
{
    cv::Mat gray, roberts;
    if (this->currentImage.channels() == 3)
        cv::cvtColor(this->currentImage, gray, cv::COLOR_BGR2GRAY);
    else
        gray = this->currentImage;

    cv::Mat robertsX = (cv::Mat_<int>(2, 2) << 1, 0, 0, -1);
    // cv::Mat robertsY = (cv::Mat_<int>(2, 2) << 0, 1, -1, 0);
    cv::filter2D(gray, roberts, CV_16S, robertsX);
    cv::convertScaleAbs(roberts, roberts);
    emit imageReady(mat2QImage(roberts));
    this->currentImage = roberts;
}

void ImageProcess::Prewitt()
{
    cv::Mat gray, prewitt;
    if (this->currentImage.channels() == 3)
        cv::cvtColor(this->currentImage, gray, cv::COLOR_BGR2GRAY);
    else
        gray = this->currentImage;

    cv::Mat prewittX = (cv::Mat_<int>(3, 3) << -1, 0, 1, -1, 0, 1, -1, 0, 1);
    cv::Mat prewittY = (cv::Mat_<int>(3, 3) << 1, 1, 1, 0, 0, 0, -1, -1, -1);
    cv::Mat prewittXImg, prewittYImg;
    cv::filter2D(gray, prewittXImg, CV_16S, prewittX);
    cv::filter2D(gray, prewittYImg, CV_16S, prewittY);
    cv::convertScaleAbs(prewittXImg, prewittXImg);
    cv::convertScaleAbs(prewittYImg, prewittYImg);
    cv::addWeighted(prewittXImg, 0.5, prewittYImg, 0.5, 0, prewitt);
    emit imageReady(mat2QImage(prewitt));
    this->currentImage = prewitt;
}

void ImageProcess::Sobel()
{
    cv::Mat gray, sobel;
    if (this->currentImage.channels() == 3)
        cv::cvtColor(this->currentImage, gray, cv::COLOR_BGR2GRAY);
    else
        gray = this->currentImage;

    cv::Mat sobelX, sobelY;
    cv::Sobel(gray, sobelX, CV_16S, 1, 0);
    cv::Sobel(gray, sobelY, CV_16S, 0, 1);
    cv::convertScaleAbs(sobelX, sobelX);
    cv::convertScaleAbs(sobelY, sobelY);
    cv::addWeighted(sobelX, 0.5, sobelY, 0.5, 0, sobel);
    emit imageReady(mat2QImage(sobel));
    this->currentImage = sobel;
}

void ImageProcess::LoG()
{
    cv::Mat gray, log;
    if (this->currentImage.channels() == 3)
        cv::cvtColor(this->currentImage, gray, cv::COLOR_BGR2GRAY);
    else 
        gray = this->currentImage;

    cv::Mat dst;
    cv::GaussianBlur(gray, dst, cv::Size(3, 3), 0);
    cv::Laplacian(dst, log, CV_16S, 3);
    cv::convertScaleAbs(log, log);
    emit imageReady(mat2QImage(log));
    this->currentImage = log;
}

void ImageProcess::Scharr()
{
    cv::Mat gray, scharr;
    if (this->currentImage.channels() == 3)
        cv::cvtColor(this->currentImage, gray, cv::COLOR_BGR2GRAY);
    else
        gray = this->currentImage;

    cv::Mat scharrX = (cv::Mat_<int>(3, 3) << -3, 0, 3, -10, 0, 10, -3, 0, 3);
    cv::Mat scharrY = (cv::Mat_<int>(3, 3) << -3, -10, -3, 0, 0, 0, 3, 10, 3);
    cv::Mat scharrXImg, scharrYImg;
    cv::filter2D(gray, scharrXImg, CV_16S, scharrX);
    cv::filter2D(gray, scharrYImg, CV_16S, scharrY);
    cv::convertScaleAbs(scharrXImg, scharrXImg);
    cv::convertScaleAbs(scharrYImg, scharrYImg);
    cv::addWeighted(scharrXImg, 0.5, scharrYImg, 0.5, 0, scharr);
    emit imageReady(mat2QImage(scharr));
    this->currentImage = scharr;
}

void ImageProcess::Canny()
{
    cv::Mat gray, canny;
    if (this->currentImage.channels() == 3)
        cv::cvtColor(this->currentImage, gray, cv::COLOR_BGR2GRAY);
    else
        gray = this->currentImage;

    cv::Mat dst;
    cv::GaussianBlur(gray, dst, cv::Size(3, 3), 0);
    cv::Canny(dst, canny, 50, 150);
    emit imageReady(mat2QImage(canny));
    this->currentImage = canny;
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

// cv::Mat ImageProcess::QImage2mat(const QImage &image)
// {
//     cv::Mat mat;
//     switch (image.format())
//     {
//     case QImage::Format_RGB32:
//     {
//         mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void *)image.constBits(), image.bytesPerLine());
//         cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR);
//         break;
//     }
//     case QImage::Format_RGB888:
//     {
//         mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void *)image.constBits(), image.bytesPerLine());
//         cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
//         break;
//     }
//     case QImage::Format_Grayscale8:
//     case QImage::Format_Indexed8:
//     {
//         mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void *)image.constBits(), image.bytesPerLine());
//         break;
//     }
//     default:
//         throw std::runtime_error("Unsupported QImage format");
//     }
//     return mat;
// }
