#include "ImageController.h"
#include "ImageProcess.h"
#include "MyImage.h"
#include "NoiseParameterDialog.h"
#include <QDebug>
#include <QThread>
#include <QElapsedTimer>
#include <QtConcurrent/QtConcurrent>

ImageController::ImageController(MyImage *view, ImageProcess *model, NoiseParameterDialog *noiseDialog)
    : noiseDialog(noiseDialog)
{
    this->view = view;
    this->model = model;

    this->thread = new QThread();
    this->model->moveToThread(thread);
    thread->start();

    connect(view, &MyImage::LoadBtnClicked, model, &ImageProcess::setImg, Qt::QueuedConnection);
    connect(view, &MyImage::GrayBtnClicked, model, &ImageProcess::Gray, Qt::QueuedConnection);
    connect(model,&ImageProcess::imageReady,view,&MyImage::UpdateImage,Qt::QueuedConnection);
    connect(view, &MyImage::BinaryBtnClicked, model, &ImageProcess::Binary, Qt::QueuedConnection);
    connect(view, &MyImage::TransformBtnClicked, model, &ImageProcess::Transform, Qt::QueuedConnection);
    connect(view, &MyImage::MeanBtnClicked, model, &ImageProcess::Mean, Qt::QueuedConnection);
    connect(view, &MyImage::MiddleBtnClicked, model, &ImageProcess::Middle, Qt::QueuedConnection);


    connect(view, &MyImage::GaussianBtnClicked, model, &ImageProcess::Gaussian, Qt::QueuedConnection);
    connect(view, &MyImage::LowPassBtnClicked, model, &ImageProcess::LowPass, Qt::QueuedConnection);
    connect(view, &MyImage::HighPassBtnClicked, model, &ImageProcess::HighPass, Qt::QueuedConnection);
    connect(view, &MyImage::CorrosionBtnClicked, model, &ImageProcess::Corrosion, Qt::QueuedConnection);
    connect(view, &MyImage::ExpansionBtnClicked, model, &ImageProcess::Expansion, Qt::QueuedConnection);
    connect(view, &MyImage::OpenBtnClicked, model, &ImageProcess::Opening, Qt::QueuedConnection);
    connect(view, &MyImage::CloseBtnClicked, model, &ImageProcess::Closing, Qt::QueuedConnection);
    connect(view, &MyImage::RobertBtnClicked, model, &ImageProcess::Roberts, Qt::QueuedConnection);
    connect(view, &MyImage::PrewittBtnClicked, model, &ImageProcess::Prewitt, Qt::QueuedConnection);
    connect(view, &MyImage::SobelBtnClicked, model, &ImageProcess::Sobel, Qt::QueuedConnection);
    connect(view, &MyImage::LoGBtnClicked, model, &ImageProcess::LoG, Qt::QueuedConnection);
    connect(view, &MyImage::ScharrBtnClicked, model, &ImageProcess::Scharr, Qt::QueuedConnection);
    connect(view, &MyImage::CannyBtnClicked, model, &ImageProcess::Canny, Qt::QueuedConnection);
    connect(view, &MyImage::BackBtnClicked, model, &ImageProcess::Back, Qt::QueuedConnection);
    connect(view, &MyImage::NextBtnClicked, model, &ImageProcess::Next, Qt::QueuedConnection);

    connect(view,&MyImage::GaussianNoiseBtnClicked,noiseDialog,&NoiseParameterDialog::GaussianParameterUpdate,Qt::QueuedConnection);
    connect(noiseDialog,&NoiseParameterDialog::GaussianNoiseParameter,model,&ImageProcess::GaussianNoise,Qt::QueuedConnection);
    connect(view,&MyImage::SaltAndPepperNoiseBtnClicked,noiseDialog,&NoiseParameterDialog::SaltParameterUpdate,Qt::QueuedConnection);
    connect(noiseDialog,&NoiseParameterDialog::SaltNoiseParameter,model,&ImageProcess::SaltAndPepperNoise,Qt::QueuedConnection);
    connect(view,&MyImage::UniformNoiseBtnClicked,noiseDialog,&NoiseParameterDialog::UniformParameterUpdate,Qt::QueuedConnection);
    connect(noiseDialog,&NoiseParameterDialog::UniformNoiseParameter,model,&ImageProcess::UniformNoise,Qt::QueuedConnection);
    connect(view, &MyImage::PoissonNoiseBtnClicked, model, &ImageProcess::PoissonNoise, Qt::QueuedConnection);
    connect(view, &MyImage::NoiseParameterBtnClicked, noiseDialog, &NoiseParameterDialog::show, Qt::QueuedConnection);

    connect(thread, &QThread::finished, model, &QObject::deleteLater, Qt::QueuedConnection);

}

ImageController::~ImageController()
{
    thread->quit();
    thread->wait();
    delete thread;
}
