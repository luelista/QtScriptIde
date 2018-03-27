#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qRegisterMetaType<uptr_t>("uptr_t");
    qRegisterMetaType<sptr_t>("sptr_t");


    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
