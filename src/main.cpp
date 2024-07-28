#include "MyImage.h"
#include "ImageController.h"
#include "ImageProcess.h"
#include "NoiseParameterDialog.h"

#include <QApplication>
// #pragma comment(lib, "user32.lib")

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