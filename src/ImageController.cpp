#include "ImageController.h"
#include <QDebug>

ImageController::ImageController(MyImage *view, ImageProcess *model)
{
    this->view = view;
    this->model = model;
    connect(view, &MyImage::LoadBtnClicked, model, &ImageProcess::setImg);
    connect(view, &MyImage::GrayBtnClicked, model, &ImageProcess::Gray);
    connect(model,&ImageProcess::imageReady,view,&MyImage::UpdateImage);
    connect(view, &MyImage::BinaryBtnClicked, model, &ImageProcess::Binary);
    connect(view, &MyImage::TransformBtnClicked, model, &ImageProcess::Transform);
    connect(view, &MyImage::MeanBtnClicked, model, &ImageProcess::Mean);
    connect(view, &MyImage::MiddleBtnClicked, model, &ImageProcess::Middle);
    connect(view, &MyImage::GaussianBtnClicked, model, &ImageProcess::Gaussian);
    connect(view, &MyImage::LowPassBtnClicked, model, &ImageProcess::LowPass);
    connect(view, &MyImage::HighPassBtnClicked, model, &ImageProcess::HighPass);
}

