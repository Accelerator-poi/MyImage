#include "NoiseParameterDialog.h"
#include <QPushButton> 

NoiseParameterDialog::NoiseParameterDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui_NoiseParameterDialog)
{
    ui->setupUi(this);
    SetValue();

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &NoiseParameterDialog::UpdataValue);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &NoiseParameterDialog::close);

}

NoiseParameterDialog::~NoiseParameterDialog()
{
    delete ui;
}


void NoiseParameterDialog::GaussianParameterUpdate()
{
    int mean = ui->GaussMeanspinBox->value();
    int dev = ui->GaussdevspinBox->value();
    emit GaussianNoiseParameter(mean, dev);
}

void NoiseParameterDialog::SaltParameterUpdate()
{
    double density = ui->SaltdoubleSpinBox->value();
    emit SaltNoiseParameter(density);
}

void NoiseParameterDialog::UniformParameterUpdate()
{
    int mean = ui->UniformMeanspinBox->value();
    int range = ui->UniformRangespinBox->value();
    double density = ui->UniformdoubleSpinBox->value();
    emit UniformNoiseParameter(mean, range, density);
}

void NoiseParameterDialog::SetValue()
{
    ui->GaussMeanspinBox->setValue(0);
    ui->GaussMeanspinBox->setRange(0, 255);
    ui->GaussMeanspinBox->setSingleStep(1);

    ui->GaussdevspinBox->setValue(50);
    ui->GaussdevspinBox->setRange(0, 100);
    ui->GaussdevspinBox->setSingleStep(1);

    ui->SaltdoubleSpinBox->setValue(0.05);
    ui->SaltdoubleSpinBox->setRange(0, 1);
    ui->SaltdoubleSpinBox->setSingleStep(0.01);

    ui->UniformMeanspinBox->setValue(0);
    ui->UniformMeanspinBox->setRange(0, 255);
    ui->UniformMeanspinBox->setSingleStep(1);

    ui->UniformRangespinBox->setValue(50);
    ui->UniformRangespinBox->setRange(0, 255);
    ui->UniformRangespinBox->setSingleStep(1);

    ui->UniformdoubleSpinBox->setValue(0.05);
    ui->UniformdoubleSpinBox->setRange(0, 1);
    ui->UniformdoubleSpinBox->setSingleStep(0.01);
}

void NoiseParameterDialog::UpdataValue()
{
    int GaussMean = ui->GaussMeanspinBox->value();
    int GaussDev = ui->GaussdevspinBox->value();
    double Salt = ui->SaltdoubleSpinBox->value();
    int UniformMean = ui->UniformMeanspinBox->value();
    int UniformRange = ui->UniformRangespinBox->value();
    double Uniform = ui->UniformdoubleSpinBox->value();

    ui->GaussMeanspinBox->setValue(GaussMean);
    ui->GaussdevspinBox->setValue(GaussDev);
    ui->SaltdoubleSpinBox->setValue(Salt);
    ui->UniformMeanspinBox->setValue(UniformMean);
    ui->UniformRangespinBox->setValue(UniformRange);
    ui->UniformdoubleSpinBox->setValue(Uniform);
}
