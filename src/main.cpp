#include "MyImage.h"
#include "ImageController.h"
#include "ImageProcess.h"

#include <QApplication>
// #pragma comment(lib, "user32.lib")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyImage w;
    ImageProcess model;
    ImageController controller(&w, &model);
    w.show();
    return a.exec();
}