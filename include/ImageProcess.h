#pragma once
#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <QObject>
#include <QImage>
#include <opencv2/opencv.hpp>

class ImageProcess : public QObject
{    
    Q_OBJECT

public slots:
    void setImg(QString filename);
    void Gray();
    void Binary(int threshold);
    void Transform(int size, int angle);
    void Mean();
    void Middle();
    void Gaussian();
    void LowPass();
    void HighPass();

signals:
    void imageReady(const QImage &image);

private:
    cv::Mat currentImage;
    QImage mat2QImage(const cv::Mat &mat);

};

#endif