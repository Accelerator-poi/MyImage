#pragma once
#ifndef IMAGECONTROLLER_H
#define IMAGECONTROLLER_H

// #include "ImageProcess.h"
// #include "MyImage.h"

class MyImage;
class ImageProcess;
class QThread;
class NoiseParameterDialog;
#include <QObject>

class ImageController : public QObject
{
    Q_OBJECT

public:
    ImageController(MyImage *view, ImageProcess *model, NoiseParameterDialog *noiseDialog);
    ~ImageController();

private:
    MyImage *view;
    ImageProcess *model;
    NoiseParameterDialog *noiseDialog;
    QThread *thread;

};


#endif