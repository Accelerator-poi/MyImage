#pragma once 
#ifndef NOISEPARAMETERDIALOG_H
#define NOISEPARAMETERDIALOG_H

#include "ui_NoiseParameterDialog.h"
#include <QDialog>

class NoiseParameterDialog : public QDialog
{
    Q_OBJECT

    public:
        NoiseParameterDialog(QWidget* parent = nullptr);
        ~NoiseParameterDialog();

    signals:
        void GaussianNoiseParameter(int mean,int dev);
        void SaltNoiseParameter(double density);
        void UniformNoiseParameter(int mean,int range, double density);

    private slots:
        void UpdataValue();

    public slots:
        void GaussianParameterUpdate();
        void SaltParameterUpdate();
        void UniformParameterUpdate();

    private:
        Ui_NoiseParameterDialog* ui;
        void SetValue();
};


#endif