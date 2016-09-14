#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <scene.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QSurfaceFormat format;
    format.setSamples(8); //2, 4, 8, 16
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);
    w.setWindowFlags(Qt::WindowStaysOnTopHint);
    //w.setWindowState(Qt::WindowNoState);
    w.setFocusPolicy(Qt::StrongFocus);
    w.show();
    return a.exec();
}
