#include "mainwindow.h"
#include "form.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainWindow w;

    //w.setStyleSheet("background:rgba(0, 0, 0;");
    //w.setAutoFillBackground(1);
    //w.setFocusPolicy(Qt::StrongFocus);
    w.setFocusPolicy(Qt::NoFocus);
    w.setWindowOpacity(0.8);
    w.show();
    return a.exec();
    w.show();
}
