#include "ImageController.h"

ImageController::ImageController(MyImage *view, ImageProcess *model)
{
    this->view = view;
    this->model = model;
    connect(view, &MyImage::LoadBtnClicked, this, &ImageController::onImportBtnClicked);
}

void ImageController::onImportBtnClicked(QString filename)
{
    if(filename.isEmpty())
    {
        return;
    }
    this->model->setImg(filename);
}