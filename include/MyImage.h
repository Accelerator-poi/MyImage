#pragma once
#include "ui_MyImage.h"
#include <QMainWindow>

class MyImage : public QMainWindow {
    Q_OBJECT

signals:
    void LoadBtnClicked(QString fileName);
    
public:
    MyImage(QWidget* parent = nullptr);
    ~MyImage();

private:
    Ui_MyImage* ui;

private slots:
    void onImportBtnClicked();
};