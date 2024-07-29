#include "ImageProcess.h"
#include <QDebug> // Add this line to include the QDebug header
#include <random>
#include <QElapsedTimer>
#include <QThread>
#include <QThreadPool>
#include <QRunnable>
#include <omp.h>

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
    PicVec.clear();
    UpdatePicVec();
    qDebug() << "Image loaded";
}

ImageProcess::ImageProcess(QObject *parent)
{
    this->threadPool = new QThreadPool(this);
}

ImageProcess::~ImageProcess()
{
    threadPool->waitForDone();
}

void ImageProcess::Gray()
{
    if (this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }
    if (this->currentImage.channels() == 1)
    {
        emit imageReady(mat2QImage(this->currentImage));
        return;
    }

    cv::Mat grayImg;
    cv::cvtColor(this->currentImage, grayImg, cv::COLOR_BGR2GRAY);
    emit imageReady(mat2QImage(grayImg));
    this->currentImage = grayImg;
    UpdatePicVec();
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
    if (this->currentImage.channels() == 3)
        cv::cvtColor(this->currentImage, grayImg, cv::COLOR_BGR2GRAY);
    else
        grayImg = this->currentImage;
    cv::Mat binaryImg;
    cv::threshold(grayImg, binaryImg, threshold, 255, cv::THRESH_BINARY);
    UpdatePicVec();
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
    UpdatePicVec();
    emit imageReady(mat2QImage(NewImage));
    qDebug() << "Transformed image ready";
}

void ImageProcess::Mean()
{
    QElapsedTimer timer;
    timer.start();
    if (this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }
    cv::blur(this->currentImage, this->currentImage, cv::Size(5, 5));
    emit imageReady(mat2QImage(this->currentImage));
    UpdatePicVec();
    qDebug() << "Mean image ready" << timer.elapsed() << "ms";
}

void ImageProcess::Middle()
{
    QElapsedTimer timer;
    timer.start();
    if (this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }

    if (this->currentImage.channels() == 1)
    {
        cv::medianBlur(this->currentImage, this->currentImage, 5);
    }
    else if (this->currentImage.channels() == 3)
    {
        cv::Mat channels[3];
        cv::split(this->currentImage, channels);

        // 使用QThreadPool和QRunnable进行多线程处理
        for (int i = 0; i < 3; ++i)
        {
            auto medianTask = [&, i]()
            {
                cv::medianBlur(channels[i], channels[i], 5);
            };

            QRunnable *task = new MyRunnable([medianTask]()
                                             { medianTask(); });

            threadPool->start(task);
        }

        threadPool->waitForDone();
        cv::merge(channels, 3, this->currentImage);
    }
    else
    {
        qDebug() << "Unsupported image format";
        return;
    }

    emit imageReady(mat2QImage(this->currentImage));
    UpdatePicVec();
    qDebug() << "Middle image ready" << timer.elapsed() << "ms";
}

void ImageProcess::Gaussian()
{
    QElapsedTimer timer;
    timer.start();
    if (this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }
    cv::GaussianBlur(this->currentImage, this->currentImage, cv::Size(5, 5), 1);
    emit imageReady(mat2QImage(this->currentImage));
    // this->currentImage = dst;
    UpdatePicVec();
    qDebug() << "Gaussian image ready" << timer.elapsed() << "ms";
}

void ImageProcess::LowPass()
{
    QElapsedTimer timer;
    timer.start();
    if (this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }
    cv::GaussianBlur(this->currentImage, this->currentImage, cv::Size(21, 21), 0);
    emit imageReady(mat2QImage(this->currentImage));
    UpdatePicVec();
    qDebug() << "LowPass image ready" << timer.elapsed() << "ms";
}

void ImageProcess::HighPass()
{
    QElapsedTimer timer;
    timer.start();
    if (this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }
    cv::Mat lowPass;
    cv::GaussianBlur(this->currentImage, lowPass, cv::Size(21, 21), 0);
    cv::addWeighted(this->currentImage, 1.5, lowPass, -0.5, 0, this->currentImage);
    emit imageReady(mat2QImage(this->currentImage));
    UpdatePicVec();
    qDebug() << "HighPass image ready" << timer.elapsed() << "ms";
}

void ImageProcess::Corrosion()
{
    QElapsedTimer timer;
    timer.start();

    if (this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }

    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5), cv::Point(-1, -1));

    if (this->currentImage.channels() == 1)
    {
        cv::erode(this->currentImage, this->currentImage, element);
    }
    else if (this->currentImage.channels() == 3)
    {
        cv::Mat channels[3];
        cv::split(this->currentImage, channels);

        // 使用QThreadPool和QRunnable进行多线程处理
        for (int i = 0; i < 3; ++i)
        {
            auto erodeTask = [&, i]()
            {
                cv::erode(channels[i], channels[i], element);
            };

            QRunnable *task = new MyRunnable(erodeTask);

            threadPool->start(task);
        }

        threadPool->waitForDone();
        cv::merge(channels, 3, this->currentImage);
    }
    else
    {
        qDebug() << "Unsupported image format";
        return;
    }

    emit imageReady(mat2QImage(this->currentImage));
    UpdatePicVec();
    qDebug() << "Corrosion image ready" << timer.elapsed() << "ms";
}

void ImageProcess::Expansion()
{
    if (this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
                                                cv::Size(5, 5), cv::Point(-1, -1));
    cv::dilate(this->currentImage, this->currentImage, element);
    emit imageReady(mat2QImage(this->currentImage));
    UpdatePicVec();
}

void ImageProcess::Opening()
{
    if (this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }
    // cv::Mat dst;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
                                                cv::Size(5, 5), cv::Point(-1, -1));
    cv::morphologyEx(this->currentImage, this->currentImage, cv::MORPH_OPEN, element);
    emit imageReady(mat2QImage(this->currentImage));
    // this->currentImage = dst;
    UpdatePicVec();
}

void ImageProcess::Closing()
{
    if (this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }
    // cv::Mat dst;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
                                                cv::Size(5, 5), cv::Point(-1, -1));
    cv::morphologyEx(this->currentImage, this->currentImage, cv::MORPH_CLOSE, element);
    emit imageReady(mat2QImage(this->currentImage));
    // this->currentImage = dst;
    UpdatePicVec();
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
    UpdatePicVec();
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
    UpdatePicVec();
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
    UpdatePicVec();
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
    UpdatePicVec();
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
    UpdatePicVec();
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
    UpdatePicVec();
}

void ImageProcess::Back()
{
    if (PicVec.size() == 0)
    {
        return;
    }
    if (PicPoint != &PicVec.front())
    {
        PicPoint--;
        emit imageReady(mat2QImage(*PicPoint));
        this->currentImage = *PicPoint;
    }
}

void ImageProcess::Next()
{
    if (PicVec.size() == 0)
    {
        return;
    }
    if (PicPoint != &PicVec.back())
    {
        PicPoint++;
        emit imageReady(mat2QImage(*PicPoint));
        this->currentImage = *PicPoint;
    }
}

/*
mean: 噪声均值 0-255
stddev: 噪声标准差 0-100
*/
void ImageProcess::GaussianNoise(int mean, int stddev)
{
    QElapsedTimer timer;
    timer.start();
    cv::Mat image = this->currentImage.clone();
    cv::Mat noise(image.size(), image.type());
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(mean, stddev);

    auto addNoise = [&](const cv::Range &range)
    {
        for (int i = range.start; i < range.end; ++i)
        {
            for (int j = 0; j < image.cols; ++j)
            {
                for (int c = 0; c < image.channels(); ++c)
                {
                    double noiseValue = distribution(generator);
                    if (image.channels() == 3)
                        image.at<cv::Vec3b>(i, j)[c] = cv::saturate_cast<uchar>(image.at<cv::Vec3b>(i, j)[c] + noiseValue);
                    else if (image.channels() == 1)
                        image.at<uchar>(i, j) = cv::saturate_cast<uchar>(image.at<uchar>(i, j) + noiseValue);
                }
            }
        }
    };

    cv::parallel_for_(cv::Range(0, image.rows), addNoise);

    emit imageReady(mat2QImage(image));
    this->currentImage = image;
    UpdatePicVec();
    qDebug() << "Gaussian noise image ready" << timer.elapsed() << "ms";
}

// density: 噪声密度 0-1
void ImageProcess::SaltAndPepperNoise(double density)
{
    // double density = 0.1;
    cv::Mat image = this->currentImage.clone();
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    auto addNoise = [&](const cv::Range &range)
    {
        for (int i = range.start; i < range.end; ++i)
        {
            for (int j = 0; j < image.cols; ++j)
            {
                double randomValue = distribution(generator);
                if (randomValue < density)
                {
                    if (image.channels() == 3)
                    {
                        image.at<cv::Vec3b>(i, j) = randomValue < density / 2 ? cv::Vec3b(0, 0, 0) : cv::Vec3b(255, 255, 255);
                    }
                    else if (image.channels() == 1)
                    {
                        image.at<uchar>(i, j) = randomValue < density / 2 ? 0 : 255;
                    }
                }
            }
        }
    };

    cv::parallel_for_(cv::Range(0, image.rows), addNoise);

    emit imageReady(mat2QImage(image));
    this->currentImage = image;
    UpdatePicVec();
}

void ImageProcess::PoissonNoise()
{
    QElapsedTimer timer;
    timer.start();
    cv::Mat image = this->currentImage.clone();
    cv::Mat noise(image.size(), image.type());
    std::default_random_engine generator;
    std::poisson_distribution<int> distribution(128);

    auto addNoise = [&](const cv::Range &range)
    {
        for (int i = range.start; i < range.end; ++i)
        {
            for (int j = 0; j < image.cols; ++j)
            {
                for (int c = 0; c < image.channels(); ++c)
                {
                    int noiseValue = distribution(generator);
                    if (image.channels() == 3)
                        image.at<cv::Vec3b>(i, j)[c] = cv::saturate_cast<uchar>(image.at<cv::Vec3b>(i, j)[c] + noiseValue);
                    else if (image.channels() == 1)
                        image.at<uchar>(i, j) = cv::saturate_cast<uchar>(image.at<uchar>(i, j) + noiseValue);
                }
            }
        }
    };

    cv::parallel_for_(cv::Range(0, image.rows), addNoise);

    emit imageReady(mat2QImage(image));
    this->currentImage = image;
    UpdatePicVec();
    qDebug() << "Poisson noise image ready" << timer.elapsed() << "ms";
}

/*
mean: 噪声均值 0-255
range: 噪声范围 0-255
density: 噪声密度 0-1
*/
void ImageProcess::UniformNoise(int mean, int range, double density)
{
    cv::Mat image = this->currentImage.clone();
    cv::Mat noise(image.size(), image.type());
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(mean - range, mean + range);

    auto addNoise = [&](const cv::Range &range)
    {
        for (int i = range.start; i < range.end; ++i)
        {
            for (int j = 0; j < image.cols; ++j)
            {
                if (static_cast<double>(rand()) / RAND_MAX <= density) // 根据密度决定是否添加噪声
                {
                    for (int c = 0; c < image.channels(); ++c)
                    {
                        int noiseValue = distribution(generator);
                        if (image.channels() == 3)
                            image.at<cv::Vec3b>(i, j)[c] = cv::saturate_cast<uchar>(image.at<cv::Vec3b>(i, j)[c] + noiseValue);
                        else if (image.channels() == 1)
                            image.at<uchar>(i, j) = cv::saturate_cast<uchar>(image.at<uchar>(i, j) + noiseValue);
                    }
                }
            }
        }
    };

    cv::parallel_for_(cv::Range(0, image.rows), addNoise);

    emit imageReady(mat2QImage(image));
    this->currentImage = image;
    UpdatePicVec();
}

void ImageProcess::FFT()
{
    QElapsedTimer timer;
    timer.start();
    if (this->currentImage.empty())
    {
        qDebug() << "Empty image";
        return;
    }

    std::vector<cv::Mat> channels;
    cv::split(this->currentImage, channels);

    std::vector<cv::Mat> spectrumChannels(currentImage.channels());

    for (int i = 0; i < this->currentImage.channels(); ++i)
    {
        auto fft = [&, i]()
        {
            cv::Mat floatImage;
            channels[i].convertTo(floatImage, CV_32F);

            cv::Mat padded;
            int m = cv::getOptimalDFTSize(floatImage.rows);
            int n = cv::getOptimalDFTSize(floatImage.cols);
            cv::copyMakeBorder(floatImage, padded, 0, m - floatImage.rows, 0, n - floatImage.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

            cv::Mat complexImage;
            cv::Mat planes[] = {padded, cv::Mat::zeros(padded.size(), CV_32F)};
            cv::merge(planes, 2, complexImage);

            cv::dft(complexImage, complexImage);

            cv::split(complexImage, planes);
            cv::magnitude(planes[0], planes[1], planes[0]);
            cv::Mat magnitudeImage = planes[0];

            magnitudeImage += cv::Scalar::all(1);
            cv::log(magnitudeImage, magnitudeImage);

            magnitudeImage = magnitudeImage(cv::Rect(0, 0, magnitudeImage.cols & -2, magnitudeImage.rows & -2));

            int cx = magnitudeImage.cols / 2;
            int cy = magnitudeImage.rows / 2;

            cv::Mat q0(magnitudeImage, cv::Rect(0, 0, cx, cy));
            cv::Mat q1(magnitudeImage, cv::Rect(cx, 0, cx, cy));
            cv::Mat q2(magnitudeImage, cv::Rect(0, cy, cx, cy));
            cv::Mat q3(magnitudeImage, cv::Rect(cx, cy, cx, cy));

            cv::Mat tmp;
            q0.copyTo(tmp);
            q3.copyTo(q0);
            tmp.copyTo(q3);

            q1.copyTo(tmp);
            q2.copyTo(q1);
            tmp.copyTo(q2);

            cv::normalize(magnitudeImage, magnitudeImage, 0, 1, cv::NORM_MINMAX);

            spectrumChannels[i] = magnitudeImage;
        };
        QRunnable *task = new MyRunnable(fft);
        threadPool->start(task);
    }

    cv::Mat mergedSpectrum;
    threadPool->waitForDone();
    cv::merge(spectrumChannels, mergedSpectrum);

    emit imageReady(mat2QImage(mergedSpectrum));
    qDebug() << "FFT image ready" << timer.elapsed() << "ms";
}

QImage ImageProcess::mat2QImage(const cv::Mat &mat)
{
    if (mat.type() == CV_8UC1)
    {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
        return image.copy();
    }
    else if (mat.type() == CV_8UC3)
    {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped().copy();
    }
    else if (mat.type() == CV_32FC1)
    {
        cv::Mat temp;
        mat.convertTo(temp, CV_8UC1, 255);
        QImage image(temp.data, temp.cols, temp.rows, temp.step, QImage::Format_Grayscale8);
        return image.copy();
    }
    else if (mat.type() == CV_32FC3)
    {
        cv::Mat temp;
        mat.convertTo(temp, CV_8UC3, 255);
        QImage image(temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
        return image.rgbSwapped().copy();
    }
    else
    {
        return QImage();
    }
}

void ImageProcess::UpdatePicVec()
{
    if (!PicVec.empty() && PicPoint != &PicVec.back())
    {
        auto it = std::find_if(PicVec.begin(), PicVec.end(), [this](const cv::Mat &img)
                               { return cv::norm(img, *PicPoint, cv::NORM_L2) == 0; });
        if (it != PicVec.end())
        {
            PicVec.erase(it + 1, PicVec.end());
        }
    }

    PicVec.push_back(this->currentImage);
    PicPoint = &PicVec.back();
}
