#pragma once
#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <QObject>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <vector>

class ImageProcess : public QObject
{    
    Q_OBJECT

public slots:
    void setImg(QString filename);
    // void UpdateImage(QImage image);
    void Gray();
    void Binary(int threshold);
    void Transform(int size, int angle);
    void Mean();
    void Middle();
    void Gaussian();
    void LowPass();
    void HighPass();
    void Corrosion();
    void Expansion();
    void Opening();
    void Closing();
    void Roberts();
    void Prewitt();
    void Sobel();
    void LoG();
    void Scharr();
    void Canny();
    void Back();
    void Next();
    void GaussianNoise(int mean, int dev);
    void SaltAndPepperNoise(double density);
    void PoissonNoise();
    void UniformNoise(int mean, int dev, double density);

signals:
    void imageReady(const QImage &image);

private:
    cv::Mat currentImage;
    QImage mat2QImage(const cv::Mat &mat);
    std::vector<cv::Mat> PicVec;
    cv::Mat* PicPoint;
    void UpdatePicVec();
};

#endif