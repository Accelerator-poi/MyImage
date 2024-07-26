#pragma once
#ifndef IMAGECONTROLLER_H
#define IMAGECONTROLLER_H

#include "ImageProcess.h"
#include "MyImage.h"

class ImageController : public QObject
{
    Q_OBJECT

public:
    ImageController(MyImage *view, ImageProcess *model);

private:
    MyImage *view;
    ImageProcess *model;

private slots:
    void onImportBtnClicked(QString filename);

};


#endif