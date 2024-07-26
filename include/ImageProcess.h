#pragma once
#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <QObject>
#include <QImage>
#include <opencv2/opencv.hpp>

class ImageProcess : public QObject
{    
    Q_OBJECT

public:
    void setImg(QString filename);

signals:
    void imageReady(const QImage &image);

private:
    cv::Mat currentImage;
    QImage mat2QImage(const cv::Mat &mat);

};

#endif