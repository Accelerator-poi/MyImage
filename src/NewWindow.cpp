#include "NewWindow.h"
#include "NoiseParameterDialog.h"
#include "ImageProcess.h"

#include <QThread>
#include <QFileDialog>

NewWindow::NewWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui_MyImage)
{
    ui->setupUi(this);
    this->noiseDialog = new NoiseParameterDialog;
    this->model = new ImageProcess;
    this->thread = new QThread;
    this->model->moveToThread(this->thread);
    thread->start();

    connect(model, &ImageProcess::imageReady, this, &NewWindow::UpdateImage, Qt::QueuedConnection);

    connect(ui->LoadBtn, &QPushButton::clicked, this, &NewWindow::onLoadBtnClicked, Qt::QueuedConnection);
    connect(this, &NewWindow::LoadBtnClicked, model, &ImageProcess::setImg, Qt::QueuedConnection);
    connect(ui->SaveBtn, &QPushButton::clicked, this, &NewWindow::SaveImg, Qt::QueuedConnection);
    connect(ui->GrayBtn, &QPushButton::clicked, model, &ImageProcess::Gray, Qt::QueuedConnection);
    connect(ui->BinarizationBtn, &QPushButton::clicked, this, &NewWindow::onBinaryBtnClicked, Qt::QueuedConnection);
    connect(this, &NewWindow::BinaryBtnClicked, model, &ImageProcess::Binary, Qt::QueuedConnection);
    connect(ui->ThresholeSlider, &QSlider::valueChanged, this, &NewWindow::onThresholdSliderValueChanged, Qt::QueuedConnection);
    connect(ui->TransformBtn, &QPushButton::clicked, this, &NewWindow::onTransformBtnClicked, Qt::QueuedConnection);
    connect(ui->SizeSlider, &QSlider::valueChanged, this, &NewWindow::onSliderValueChanged, Qt::QueuedConnection);
    connect(ui->AngleSlider, &QSlider::valueChanged, this, &NewWindow::onSliderValueChanged, Qt::QueuedConnection);
    connect(this, &NewWindow::TransformBtnClicked, model, &ImageProcess::Transform, Qt::QueuedConnection);
    connect(ui->FFTBtn, &QPushButton::clicked, model, &ImageProcess::FFT, Qt::QueuedConnection);

    connect(ui->MeanBtn, &QPushButton::clicked, model, &ImageProcess::Mean, Qt::QueuedConnection);
    connect(ui->MiddleBtn, &QPushButton::clicked, model, &ImageProcess::Middle, Qt::QueuedConnection);
    connect(ui->GaussBtn, &QPushButton::clicked, model, &ImageProcess::Gaussian, Qt::QueuedConnection);
    connect(ui->LowPassBtn, &QPushButton::clicked, model, &ImageProcess::LowPass, Qt::QueuedConnection);
    connect(ui->HighPassBtn, &QPushButton::clicked, model, &ImageProcess::HighPass, Qt::QueuedConnection);

    connect(ui->CorrosionBtn, &QPushButton::clicked, model, &ImageProcess::Corrosion, Qt::QueuedConnection);
    connect(ui->ExpansionBtn, &QPushButton::clicked, model, &ImageProcess::Expansion, Qt::QueuedConnection);
    connect(ui->OpenBtn, &QPushButton::clicked, model, &ImageProcess::Opening, Qt::QueuedConnection);
    connect(ui->CloseBtn, &QPushButton::clicked, model, &ImageProcess::Closing, Qt::QueuedConnection);

    connect(ui->RobertsBtn, &QPushButton::clicked, model, &ImageProcess::Roberts, Qt::QueuedConnection);
    connect(ui->PrewittBtn, &QPushButton::clicked, model, &ImageProcess::Prewitt, Qt::QueuedConnection);
    connect(ui->SobelBtn, &QPushButton::clicked, model, &ImageProcess::Sobel, Qt::QueuedConnection);
    connect(ui->LoGBtn, &QPushButton::clicked, model, &ImageProcess::LoG, Qt::QueuedConnection);
    connect(ui->ScharrBtn, &QPushButton::clicked, model, &ImageProcess::Scharr, Qt::QueuedConnection);
    connect(ui->CannyBtn, &QPushButton::clicked, model, &ImageProcess::Canny, Qt::QueuedConnection);

    connect(ui->BackBtn, &QPushButton::clicked, model, &ImageProcess::Back, Qt::QueuedConnection);
    connect(ui->NextBtn, &QPushButton::clicked, model, &ImageProcess::Next, Qt::QueuedConnection);

    connect(ui->NoiseUpdateBtn, &QPushButton::clicked, noiseDialog, &NoiseParameterDialog::show, Qt::QueuedConnection);
    connect(ui->GaussianNoiseBtn, &QPushButton::clicked, noiseDialog, &NoiseParameterDialog::GaussianParameterUpdate, Qt::QueuedConnection);
    connect(noiseDialog, &NoiseParameterDialog::GaussianNoiseParameter, model, &ImageProcess::GaussianNoise, Qt::QueuedConnection);
    connect(ui->SaltNoiseBtn, &QPushButton::clicked, noiseDialog, &NoiseParameterDialog::SaltParameterUpdate, Qt::QueuedConnection);
    connect(noiseDialog, &NoiseParameterDialog::SaltNoiseParameter, model, &ImageProcess::SaltAndPepperNoise, Qt::QueuedConnection);
    connect(ui->UniformNoiseBtn, &QPushButton::clicked, noiseDialog, &NoiseParameterDialog::UniformParameterUpdate, Qt::QueuedConnection);
    connect(noiseDialog, &NoiseParameterDialog::UniformNoiseParameter, model, &ImageProcess::UniformNoise, Qt::QueuedConnection);
    connect(ui->PoissonNoiseBtn, &QPushButton::clicked, model, &ImageProcess::PoissonNoise,Qt::QueuedConnection);

    connect(thread, &QThread::finished, model, &QObject::deleteLater, Qt::QueuedConnection);
}

NewWindow::~NewWindow()
{
    thread->quit();
    thread->wait();
    delete thread;
    delete noiseDialog;
    delete model;
    delete ui;
}

void NewWindow::onLoadBtnClicked()
{
    if (IsTransformed || IsBinary)
    {
        IsBinary = false;
        IsTransformed = false;
    }

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.bmp)"));
    if (fileName.isEmpty())
    {
        return;
    }
    QImage image(fileName);
    if (image.isNull())
    {
        return;
    }
    QSize labelSize = ui->PicLabelBefore->size();

    QPixmap pixmap = QPixmap::fromImage(image).scaled(labelSize, Qt::KeepAspectRatio);

    ui->PicLabelBefore->setPixmap(pixmap);

    emit LoadBtnClicked(fileName);
}

void NewWindow::onBinaryBtnClicked()
{
    ui->ThresholeSlider->setRange(0, 255);
    int threshold = ui->ThresholeSlider->value();
    emit BinaryBtnClicked(threshold);
    IsBinary = true;
}

void NewWindow::onTransformBtnClicked()
{
    if (IsBinary)
        IsBinary = false;

    ui->SizeSlider->setRange(10, 100);
    ui->AngleSlider->setRange(0, 360);

    int size = ui->SizeSlider->value();
    int angle = ui->AngleSlider->value();

    emit TransformBtnClicked(size, angle);
    IsTransformed = true;
}

void NewWindow::onThresholdSliderValueChanged(int threshold)
{
    if (IsBinary)
    {
        emit BinaryBtnClicked(threshold);
    }
}

void NewWindow::onSliderValueChanged()
{
    if (IsTransformed)
    {
        int size = ui->SizeSlider->value();
        int angle = ui->AngleSlider->value();
        emit TransformBtnClicked(size, angle);
    }
}

void NewWindow::UpdateImage(const QImage &image)
{
    if (image.isNull())
    {
        qDebug() << "Empty image";
        return;
    }

    // 获取标签的大小
    const QSize labelSize = ui->PicLabelBefore->size();

    // 仅在图像大小与标签大小不同时进行缩放
    if (image.size() != labelSize)
    {
        QPixmap pixmap = QPixmap::fromImage(image).scaled(labelSize,
                                                          Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->PicLabelBefore->setPixmap(pixmap);
    }
    else
    {
        ui->PicLabelBefore->setPixmap(QPixmap::fromImage(image));
    }
}

void NewWindow::UpdateParameter()
{
    if (IsBinary || IsTransformed)
    {
        IsBinary = false;
        IsTransformed = false;
    }
}

void NewWindow::SaveImg()
{
    QPixmap pixmap = ui->PicLabelBefore->pixmap();
    QImage image = pixmap.toImage();
    if (image.isNull())
    {
        qDebug() << "Empty image";
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"),
                                                    ".", tr("Image Files (*.png *.jpg *.bmp)"));
    if (fileName.isEmpty())
    {
        qDebug() << "Empty filename";
        return;
    }
    image.save(fileName);
}
