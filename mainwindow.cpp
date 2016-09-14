#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <scene.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi( this );
    wireMode = false;
    fullscreen = false;
    mouseLock = true;
    this->setMouseTracking( mouseLock );
    ui->centralWidget->setMouseTracking( mouseLock );
    oldPosX = pos().x() + ui->widget->pos().x() + ui->widget->width() / 2.0f;
    oldPosY = pos().y() + ui->widget->pos().y() + ui->widget->height() / 2.0f;
    QCursor::setPos( oldPosX, oldPosY );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   ui->widget->resize(this->width(),this->height());
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    switch ( event->key() ) {
    case Qt::Key_Escape:
        this->close();
    break;
    case Qt::Key_Up:
        ui->widget->rotateUp();      // повернуть сцену вверх
    break;
    case Qt::Key_Down:
        ui->widget->rotateDown();    // повернуть сцену вниз
    break;
    case Qt::Key_A:
        ui->widget->rotateLeft();     // повернуть сцену влево
    break;
    case Qt::Key_D:
        ui->widget->rotateRight();   // повернуть сцену вправо
    break;
    case Qt::Key_W:
        ui->widget->moveForward();  //перемещение камеры вперед
    break;
    case Qt::Key_S:
        ui->widget->moveBack();     //перемещение камеры назад
    break;
    case Qt::Key_V:
        fullscreen = !fullscreen;
        setWindowState( fullscreen ? Qt::WindowFullScreen : Qt::WindowNoState );
    break;
    case Qt::Key_Z:
        mouseLock = !mouseLock;
        this->setMouseTracking( mouseLock );
        ui->centralWidget->setMouseTracking( mouseLock );
    break;
    case Qt::Key_F1:
        //Не функционирует как надо
        /*
        this->setMouseTracking(false);
        ui->centralWidget->setMouseTracking(false);
        helpWindow.setWindowFlags(Qt::WindowStaysOnTopHint);
        helpWindow.show();
        helpWindow.raise();
        this->setMouseTracking(mouseLock);
        ui->centralWidget->setMouseTracking(mouseLock);
        */
    break;
    case Qt::Key_I:
        ui->widget->switchDebug();
    break;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    newPosX = event->globalPos().x();
    newPosY = event->globalPos().y();
    if ( newPosX  == oldPosX && newPosY == oldPosY ) return;
//Смещение старой позиции если курсор заблокирова
    if ( mouseLock ){
        oldPosX = pos().x() + ui->widget->pos().x() + ui->widget->width()/2.0f;
        oldPosY = pos().y() + ui->widget->pos().y() + ui->widget->height()/2.0f;
    }
//Блок проверки смещения
    if ( newPosX > oldPosX ){
        ui->widget->rotateRight();
    }else if ( newPosX < oldPosX ){
        ui->widget->rotateLeft();
    }
    if ( newPosY > oldPosY ){
        ui->widget->rotateDown();
    }else if ( newPosY < oldPosY ){
        ui->widget->rotateUp();
    }
//Возврат курсора если он заблокирован
    if ( mouseLock ){
        QCursor::setPos( oldPosX, oldPosY );
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    if (!mouseLock){
        oldPosX = event->globalPos().x();
        oldPosY = event->globalPos().y();
    }
}



