#include "NewWindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NewWindow w;
    w.show();
    return a.exec();
}

//MVC模式
/*
#include "MyImage.h"
#include "ImageController.h"
#include "ImageProcess.h"
#include "NoiseParameterDialog.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyImage w;
    ImageProcess model;
    NoiseParameterDialog noiseDialog;
    ImageController controller(&w, &model, &noiseDialog);
    w.show();
    return a.exec();
}
*/