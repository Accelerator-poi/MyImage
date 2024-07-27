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
    void CorrosionBtnClicked();
    void ExpansionBtnClicked();
    void OpenBtnClicked();
    void CloseBtnClicked();
    void RobertBtnClicked();
    void PrewittBtnClicked();
    void SobelBtnClicked();
    void LoGBtnClicked();
    void ScharrBtnClicked();
    void CannyBtnClicked();
    void BackBtnClicked();
    void NextBtnClicked();
    void GaussianNoiseBtnClicked();
    void SaltAndPepperNoiseBtnClicked();
    void PoissonNoiseBtnClicked();
    void UniformNoiseBtnClicked();
    
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
    void onCorrosionBtnClicked();
    void onExpansionBtnClicked();
    void onOpenBtnClicked();
    void onCloseBtnClicked();
    void onRobertBtnClicked();
    void onPrewittBtnClicked();
    void onSobelBtnClicked();
    void onLoGBtnClicked();
    void onScharrBtnClicked();
    void onCannyBtnClicked();
    void onBackBtnClicked();
    void onNextBtnClicked();
    void onGaussianNoiseBtnClicked();
    void onSaltAndPepperNoiseBtnClicked();
    void onPoissonNoiseBtnClicked();
    void onUniformNoiseBtnClicked();
};