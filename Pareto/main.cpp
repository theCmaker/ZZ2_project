#include "mainwindow.h"
#include "debug_stream.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    debug_stream _cout(std::cout, w.Sortie_application());

    w.show();

    return a.exec();
}
