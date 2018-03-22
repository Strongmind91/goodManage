#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QDebug>
#include <QVariant>
#include <QtSql>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set up for display time in status bar
    QDateTime local(QDateTime::currentDateTime());
    qDebug() << "Local time is:" << local.currentDateTime();
    ui->statusBar->showMessage(local.toString("Giờ hh:mm Ngày d MMMM yyyy"));

    // Set up for open connection to database file
    mydb = QSqlDatabase::addDatabase("QSQLITE");
    QString dbName = "../goodManage/db.db3";
    mydb.setDatabaseName(dbName);
    if (!mydb.open())
    {
        qDebug() << "can not create connection to database ok";
    }
    else
    {
        qDebug() << "connection to database ok";
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tabCongCu_themHang_clicked()
{
    // Add new product name into the database
   QSqlQuery query;
   QString name;
   bool success = false;

   name = this->ui->tabCongCu_tenhang->text();

   if(query.exec("INSERT INTO tableName (tenHang)" "VALUES ('Chen')"))
   {
       success = true;
   }
   else
   {
        qDebug() << "addPerson error:  "
                 << query.lastError();
   }
}
