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
    emit MeanBtnClicked();
}

void MyImage::onMiddleBtnClicked()
{
    emit MiddleBtnClicked();
}

void MyImage::onGaussianBtnClicked()
{
    emit GaussianBtnClicked();
}

void MyImage::onLowPassBtnClicked()
{
    emit LowPassBtnClicked();
}

void MyImage::onHighPassBtnClicked()
{
    emit HighPassBtnClicked();
}


