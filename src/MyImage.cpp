#include "MyImage.h"
#include <QFileDialog>

MyImage::MyImage(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_MyImage)
{
    ui->setupUi(this);
    connect(ui->LoadBtn, &QPushButton::clicked, this, &MyImage::onImportBtnClicked);
}

MyImage::~MyImage()
{
    delete ui; 
}

void MyImage::onImportBtnClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.bmp)"));
    if (fileName.isEmpty())
    {
        return;
    }
    QImage image(fileName);
    if(image.isNull())
    {
        return;
    }
    QSize labelSize = ui->PicLabelBefore->size();

    QPixmap pixmap = QPixmap::fromImage(image).scaled(labelSize, Qt::KeepAspectRatio);

    ui->PicLabelBefore->setPixmap(pixmap);

    emit LoadBtnClicked(fileName);

}