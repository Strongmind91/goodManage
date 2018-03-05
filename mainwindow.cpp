#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QDateTime local(QDateTime::currentDateTime());
    qDebug() << "Local time is:" << local.currentDateTime();
    ui->statusBar->showMessage(local.toString("Giờ hh:mm Ngày d MMMM yyyy"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
