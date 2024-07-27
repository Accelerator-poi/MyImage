#pragma once
#include "ui_MyImage.h"
#include <QMainWindow>

class MyImage : public QMainWindow {
    Q_OBJECT

signals:
    void LoadBtnClicked(QString fileName);
    void GrayBtnClicked();
    void BinaryBtnClicked(int threshold);
    void TransformBtnClicked(int size, int angle);
    void MeanBtnClicked();
    void MiddleBtnClicked();
    void GaussianBtnClicked();
    void LowPassBtnClicked();
    void HighPassBtnClicked();
    
public:
    MyImage(QWidget* parent = nullptr);
    void Saveimg();
    ~MyImage();

private:
    Ui_MyImage* ui;
    bool IsBinary = false;
    bool IsTransformed = false;

public slots:
    void UpdateImage(QImage image);

private slots:
    void onImportBtnClicked();
    void onGrayBtnClicked();
    void onBinaryBtnClicked();
    void onThresholdSliderValueChanged(int threshold);
    void onTransformBtnClicked();
    void onSliderValueChanged();
    void onMeanBtnClicked();
    void onMiddleBtnClicked();
    void onGaussianBtnClicked();
    void onLowPassBtnClicked();
    void onHighPassBtnClicked();
};