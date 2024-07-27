#include "ImageController.h"
#include <QDebug>

ImageController::ImageController(MyImage *view, ImageProcess *model)
{
    this->view = view;
    this->model = model;
    connect(view, &MyImage::LoadBtnClicked, model, &ImageProcess::setImg);
    // connect(model, &ImageProcess::imageReady, model, &ImageProcess::UpdateImage);
    connect(view, &MyImage::GrayBtnClicked, model, &ImageProcess::Gray);
    connect(model,&ImageProcess::imageReady,view,&MyImage::UpdateImage);
    connect(view, &MyImage::BinaryBtnClicked, model, &ImageProcess::Binary);
    connect(view, &MyImage::TransformBtnClicked, model, &ImageProcess::Transform);
    connect(view, &MyImage::MeanBtnClicked, model, &ImageProcess::Mean);
    connect(view, &MyImage::MiddleBtnClicked, model, &ImageProcess::Middle);
    connect(view, &MyImage::GaussianBtnClicked, model, &ImageProcess::Gaussian);
    connect(view, &MyImage::LowPassBtnClicked, model, &ImageProcess::LowPass);
    connect(view, &MyImage::HighPassBtnClicked, model, &ImageProcess::HighPass);
    connect(view, &MyImage::CorrosionBtnClicked, model, &ImageProcess::Corrosion);
    connect(view, &MyImage::ExpansionBtnClicked, model, &ImageProcess::Expansion);
    connect(view, &MyImage::OpenBtnClicked, model, &ImageProcess::Opening);
    connect(view, &MyImage::CloseBtnClicked, model, &ImageProcess::Closing);
    connect(view, &MyImage::RobertBtnClicked, model, &ImageProcess::Roberts);
    connect(view, &MyImage::PrewittBtnClicked, model, &ImageProcess::Prewitt);
    connect(view, &MyImage::SobelBtnClicked, model, &ImageProcess::Sobel);
    connect(view, &MyImage::LoGBtnClicked, model, &ImageProcess::LoG);
    connect(view, &MyImage::ScharrBtnClicked, model, &ImageProcess::Scharr);
    connect(view, &MyImage::CannyBtnClicked, model, &ImageProcess::Canny);
}

