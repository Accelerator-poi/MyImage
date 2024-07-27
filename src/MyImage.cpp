#include "MyImage.h"
#include <QFileDialog>
#include <QDebug>

MyImage::MyImage(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_MyImage)
{
    ui->setupUi(this);
    connect(ui->LoadBtn, &QPushButton::clicked, this, &MyImage::onImportBtnClicked);
    connect(ui->SaveBtn, &QPushButton::clicked, this, &MyImage::Saveimg);
    connect(ui->GrayBtn, &QPushButton::clicked, this, &MyImage::onGrayBtnClicked);
    connect(ui->BinarizationBtn, &QPushButton::clicked, this, &MyImage::onBinaryBtnClicked);
    connect(ui->ThresholeSlider, &QSlider::valueChanged, this, &MyImage::onThresholdSliderValueChanged);
    connect(ui->TransformBtn, &QPushButton::clicked, this, &MyImage::onTransformBtnClicked);
    connect(ui->SizeSlider, &QSlider::valueChanged, this, &MyImage::onSliderValueChanged);
    connect(ui->AngleSlider, &QSlider::valueChanged, this, &MyImage::onSliderValueChanged);
    connect(ui->MeanBtn, &QPushButton::clicked, this, &MyImage::onMeanBtnClicked);
    connect(ui->MiddleBtn, &QPushButton::clicked, this, &MyImage::onMiddleBtnClicked);
    connect(ui->GaussBtn, &QPushButton::clicked, this, &MyImage::onGaussianBtnClicked);
    connect(ui->LowPassBtn, &QPushButton::clicked, this, &MyImage::onLowPassBtnClicked);
    connect(ui->HighPassBtn, &QPushButton::clicked, this, &MyImage::onHighPassBtnClicked);
    connect(ui->CorrosionBtn, &QPushButton::clicked, this, &MyImage::onCorrosionBtnClicked);
    connect(ui->ExpansionBtn, &QPushButton::clicked, this, &MyImage::onExpansionBtnClicked);
    connect(ui->OpenBtn, &QPushButton::clicked, this, &MyImage::onOpenBtnClicked);
    connect(ui->CloseBtn, &QPushButton::clicked, this, &MyImage::onCloseBtnClicked);
    connect(ui->RobertsBtn, &QPushButton::clicked, this, &MyImage::onRobertBtnClicked);
    connect(ui->PrewittBtn, &QPushButton::clicked, this, &MyImage::onPrewittBtnClicked);
    connect(ui->SobelBtn, &QPushButton::clicked, this, &MyImage::onSobelBtnClicked);
    connect(ui->LoGBtn, &QPushButton::clicked, this, &MyImage::onLoGBtnClicked);
    connect(ui->ScharrBtn, &QPushButton::clicked, this, &MyImage::onScharrBtnClicked);
    connect(ui->CannyBtn, &QPushButton::clicked, this, &MyImage::onCannyBtnClicked);
    connect(ui->BackBtn, &QPushButton::clicked, this, &MyImage::onBackBtnClicked);
    connect(ui->NextBtn, &QPushButton::clicked, this, &MyImage::onNextBtnClicked);
    connect(ui->GaussianNoiseBtn, &QPushButton::clicked, this, &MyImage::onGaussianNoiseBtnClicked);
    connect(ui->SaltNoiseBtn, &QPushButton::clicked, this, &MyImage::onSaltAndPepperNoiseBtnClicked);
    connect(ui->PoissonNoiseBtn, &QPushButton::clicked, this, &MyImage::onPoissonNoiseBtnClicked);
    connect(ui->UniformNoiseBtn, &QPushButton::clicked, this, &MyImage::onUniformNoiseBtnClicked);
}

void MyImage::UpdateImage(QImage image)
{
    if (image.isNull())
    {
        qDebug() << "Empty image";
        return;
    }

    QSize labelSize = ui->PicLabelBefore->size();
    QPixmap pixmap = QPixmap::fromImage(image).scaled(labelSize,
                     Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->PicLabelBefore->setPixmap(pixmap);
    
}

void MyImage::Saveimg()
{
    QPixmap pixmap = ui->PicLabelBefore->pixmap();
    QImage image = pixmap.toImage();
    if (image.isNull())
    {
        qDebug() << "Empty image";
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), ".", tr("Image Files (*.png *.jpg *.bmp)"));
    if (fileName.isEmpty())
    {
        qDebug() << "Empty filename";
        return;
    }
    image.save(fileName);
}

MyImage::~MyImage()
{
    delete ui;
}

void MyImage::onImportBtnClicked()
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

    qDebug() << "LoadBtnCLicked signal send";

    emit LoadBtnClicked(fileName);
}

void MyImage::onGrayBtnClicked()
{
    if (IsBinary || IsTransformed)
    {
        IsBinary = false;
        IsTransformed = false;
    }
    emit GrayBtnClicked();
}

void MyImage::onBinaryBtnClicked()
{
    ui->ThresholeSlider->setRange(0, 255);
    int threshold = ui->ThresholeSlider->value();
    emit BinaryBtnClicked(threshold);
    IsBinary = true;
}

void MyImage::onThresholdSliderValueChanged(int threshold)
{
    if (IsBinary)
    {
        emit BinaryBtnClicked(threshold);
    }
}

void MyImage::onTransformBtnClicked()
{
    if (IsBinary)
        IsBinary = false;

    ui->SizeSlider->setRange(10, 100);
    ui->AngleSlider->setRange(0, 360);

    int size = ui->SizeSlider->value();
    int angle = ui->AngleSlider->value();

    emit TransformBtnClicked(size, angle);
    qDebug() << "TransformBtnClicked signal send";
    IsTransformed = true;
}

void MyImage::onSliderValueChanged()
{
    if (IsTransformed)
    {
        int size = ui->SizeSlider->value();
        int angle = ui->AngleSlider->value();
        emit TransformBtnClicked(size, angle);
        qDebug() << "ui->SizeSlider->value() = " << size;
        qDebug() << "ui->AngleSlider->value() = " << angle;
    }
}

void MyImage::onMeanBtnClicked()
{
    if(IsBinary || IsTransformed)
    {
        IsBinary = false;
        IsTransformed = false;
    }
    emit MeanBtnClicked();
}

void MyImage::onMiddleBtnClicked()
{
    if(IsBinary || IsTransformed)
    {
        IsBinary = false;
        IsTransformed = false;
    }
    emit MiddleBtnClicked();
}

void MyImage::onGaussianBtnClicked()
{
    if(IsBinary || IsTransformed)
    {
        IsBinary = false;
        IsTransformed = false;
    }
    emit GaussianBtnClicked();
}

void MyImage::onLowPassBtnClicked()
{
    if(IsBinary || IsTransformed)
    {
        IsBinary = false;
        IsTransformed = false;
    }
    emit LowPassBtnClicked();
}

void MyImage::onHighPassBtnClicked()
{
    if(IsBinary || IsTransformed)
    {
        IsBinary = false;
        IsTransformed = false;
    }
    emit HighPassBtnClicked();
}

void MyImage::onCorrosionBtnClicked()
{
    if(IsBinary || IsTransformed)
    {
        IsBinary = false;
        IsTransformed = false;
    }
    emit CorrosionBtnClicked();
}

void MyImage::onExpansionBtnClicked()
{
    if(IsBinary || IsTransformed)
    {
        IsBinary = false;
        IsTransformed = false;
    }
    emit ExpansionBtnClicked();
}

void MyImage::onOpenBtnClicked()
{
    if(IsBinary || IsTransformed)
    {
        IsBinary = false;
        IsTransformed = false;
    }
    emit OpenBtnClicked();
}

void MyImage::onCloseBtnClicked()
{
    if(IsBinary || IsTransformed)
    {
        IsBinary = false;
        IsTransformed = false;
    }
    emit CloseBtnClicked();
}

void MyImage::onRobertBtnClicked()
{
    if(IsBinary || IsTransformed)
    {
        IsBinary = false;
        IsTransformed = false;
    }
    emit RobertBtnClicked();
}

void MyImage::onPrewittBtnClicked()
{
    if(IsBinary || IsTransformed)
    {
        IsBinary = false;
        IsTransformed = false;
    }
    emit PrewittBtnClicked();
}

void MyImage::onSobelBtnClicked()
{
    if(IsBinary || IsTransformed)
    {
        IsBinary = false;
        IsTransformed = false;
    }
    emit SobelBtnClicked();
}

void MyImage::onLoGBtnClicked()
{
    if(IsBinary || IsTransformed)
    {
        IsBinary = false;
        IsTransformed = false;
    }
    emit LoGBtnClicked();
}

void MyImage::onScharrBtnClicked()
{
    if(IsBinary || IsTransformed)
    {
        IsBinary = false;
        IsTransformed = false;
    }
    emit ScharrBtnClicked();
}

void MyImage::onCannyBtnClicked()
{
    if(IsBinary || IsTransformed)
    {
        IsBinary = false;
        IsTransformed = false;
    }
    emit CannyBtnClicked();
}

void MyImage::onBackBtnClicked()
{
    emit BackBtnClicked();
}

void MyImage::onNextBtnClicked()
{
    emit NextBtnClicked();
}

void MyImage::onGaussianNoiseBtnClicked()
{
    if(IsBinary || IsTransformed)
    {
        IsBinary = false;
        IsTransformed = false;
    }
    emit GaussianNoiseBtnClicked();
}

void MyImage::onSaltAndPepperNoiseBtnClicked()
{
    if(IsBinary || IsTransformed)
    {
        IsBinary = false;
        IsTransformed = false;
    }
    emit SaltAndPepperNoiseBtnClicked();
}

void MyImage::onPoissonNoiseBtnClicked()
{
    if(IsBinary || IsTransformed)
    {
        IsBinary = false;
        IsTransformed = false;
    }
    emit PoissonNoiseBtnClicked();
}

void MyImage::onUniformNoiseBtnClicked()
{
    if(IsBinary || IsTransformed)
    {
        IsBinary = false;
        IsTransformed = false;
    }
    emit UniformNoiseBtnClicked();
}
