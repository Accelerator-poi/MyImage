#pragma once
#ifndef NEWWINDOW_H
#define NEWWINDOW_H

#include <QMainWindow>
#include "ui_MyImage.h"

class ImageProcess;
class NoiseParameterDialog;
class QThread;

class NewWindow : public QMainWindow
{
    Q_OBJECT
public:
    NewWindow(QWidget* parent = nullptr);
    ~NewWindow();

signals:
    void LoadBtnClicked(QString fileName);
    void BinaryBtnClicked(int threshold);
    void TransformBtnClicked(int size, int angle);

private slots:
    void onLoadBtnClicked();
    void onBinaryBtnClicked();
    void onTransformBtnClicked();
    void onThresholdSliderValueChanged(int threshold);
    void onSliderValueChanged();
    void UpdateImage(const QImage &image);
    void UpdateParameter();
    void SaveImg();

private:
    Ui_MyImage *ui;
    NoiseParameterDialog *noiseDialog;
    ImageProcess *model;
    QThread *thread;
    bool IsBinary = false;
    bool IsTransformed = false;
};

#endif