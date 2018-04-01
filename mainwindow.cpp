#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QDebug>
#include <QVariant>
#include <QtSql>
#include "stdio.h"
#include <QMessageBox>

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



void MainWindow::on_tabCongCu_themHang_buttonThem_clicked()
{
    // Add new product name into the database
   QSqlQuery query;
   QString name;
   bool success = false;

   name = this->ui->tabCongCu_tenHang_ten->text();

   /* Confirm that user really want to input this field*/
   QMessageBox msgBox;
   msgBox.setText("Bạn có chắc muốn nhập thêm loại hàng: " + name+ " ?");
   msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
   msgBox.setDefaultButton(QMessageBox::No);
   int ret = msgBox.exec();

   if (ret == QMessageBox::No)
   {
        ui->tabCongCu_tenHang_ten->setText("");
        return;
   }

   query.prepare("INSERT INTO tenHang (tenHang) VALUES(?)");
   query.bindValue(0, name);
   if(query.exec())
   {
       success = true;
       msgBox.setText("Bạn đã nhập thành công một mặt hàng ");
       msgBox.setStandardButtons(QMessageBox::Ok);
       msgBox.exec();
       ui->tabCongCu_tenHang_ten->setText("");
   }
   else
   {
        qDebug() << __FUNCTION__ <<"nhap Hang is error: "
                 << query.lastError();
   }
}

void MainWindow::on_tabCongCu_donVi_buttonThem_clicked()
{
    // Add new "don vi" into the database
   QSqlQuery query;
   QString name;
   bool success = false;

   name = this->ui->tabCongCu_donVi_ten->text();

   /* Confirm that user really want to input this field*/
   QMessageBox msgBox;
   msgBox.setText("Bạn có chắc muốn nhập thêm đơn vị: " + name+ " ?");
   msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
   msgBox.setDefaultButton(QMessageBox::No);
   int ret = msgBox.exec();

   if (ret == QMessageBox::No)
   {
        ui->tabCongCu_donVi_ten->setText("");
        return;
   }


   query.prepare("INSERT INTO tenDonVi (tenDonVi) VALUES(?)");
   query.bindValue(0, name);
   if(query.exec())
   {
       success = true;
       msgBox.setText("Bạn đã nhập thành công một đơn vị ");
       msgBox.setStandardButtons(QMessageBox::Ok);
       msgBox.exec();
       ui->tabCongCu_donVi_ten->setText("");
   }
   else
   {
        qDebug() << __FUNCTION__ <<"nhap Don Vi is error: "
                 << query.lastError();
   }
}

void MainWindow::on_tabCongCu_nguonNhap_buttonThem_clicked()
{
    // Add thong tin nguon nhap
   QSqlQuery query;
   QString nguonNhap_ten;
   QString nguonNhap_phoneNumber;
   QString nguonNhap_note;
   bool success = false;

   nguonNhap_ten = this->ui->tabCongCu_nguonNhap_ten->text();
   nguonNhap_phoneNumber = this->ui->tabCongCu_nguonNhap_phoneNumber->text();
   nguonNhap_note = this->ui->tabCongCu_nguonNhap_note->text();

   /* Confirm that user really want to input this field*/
   QMessageBox msgBox;
   QString info = "Bạn có chắc muốn nhập thêm thông tin nguồn nhập như sau: \n\nTên Nguồn Nhập: " + nguonNhap_ten + "\n" + "Số điện thoại: " + nguonNhap_phoneNumber + "\n" + "Ghi chú: " + nguonNhap_note ;
   msgBox.setText(info);
   msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
   msgBox.setDefaultButton(QMessageBox::No);
   int ret = msgBox.exec();

   if (ret == QMessageBox::No)
   {
        ui->tabCongCu_nguonNhap_ten->setText("");
        ui->tabCongCu_nguonNhap_phoneNumber->setText("");
        ui->tabCongCu_nguonNhap_note->setText("");
        return;
   }


   query.prepare("INSERT INTO nguonNhap (tenNguonNhap, phoneNumber, note) VALUES(?,?,?)");
   query.bindValue(0, nguonNhap_ten);
   query.bindValue(1, nguonNhap_phoneNumber);
   query.bindValue(2, nguonNhap_note);
   if(query.exec())
   {
       success = true;
       msgBox.setText("Bạn đã nhập thành công một Nguồn Nhập ");
       msgBox.setStandardButtons(QMessageBox::Ok);
       msgBox.exec();
       ui->tabCongCu_nguonNhap_ten->setText("");
       ui->tabCongCu_nguonNhap_phoneNumber->setText("");
       ui->tabCongCu_nguonNhap_note->setText("");
   }
   else
   {
        qDebug() << __FUNCTION__ <<"nhập Nguồn Nhập is error: "
                 << query.lastError();
   }
}

void MainWindow::on_tabCongCu_khachMua_buttonThem_clicked()
{
    // Add thong tin nguon nhap
   QSqlQuery query;
   QString khachMua_ten;
   QString khachMua_phoneNumber;
   QString khachMua_note;
   bool success = false;

   khachMua_ten = this->ui->tabCongCu_khachMua_ten->text();
   khachMua_phoneNumber = this->ui->tabCongCu_khachMua_phoneNumber->text();
   khachMua_note = this->ui->tabCongCu_khachMua_note->text();

   /* Confirm that user really want to input this field*/
   QMessageBox msgBox;
   QString info = "Bạn có chắc muốn nhập thêm thông tin Khách Mua như sau: \n\nTên Khách Hàng: " + khachMua_ten + "\n" + "Số điện thoại: " + khachMua_phoneNumber + "\n" + "Ghi chú: " + khachMua_note ;
   msgBox.setText(info);
   msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
   msgBox.setDefaultButton(QMessageBox::No);
   int ret = msgBox.exec();

   if (ret == QMessageBox::No)
   {
        ui->tabCongCu_khachMua_ten->setText("");
        ui->tabCongCu_khachMua_phoneNumber->setText("");
        ui->tabCongCu_khachMua_note->setText("");
        return;
   }

   query.prepare("INSERT INTO khachMua (tenkhachMua, phoneNumber, note) VALUES(?,?,?)");
   query.bindValue(0, khachMua_ten);
   query.bindValue(1, khachMua_phoneNumber);
   query.bindValue(2, khachMua_note);
   if(query.exec())
   {
       success = true;
       msgBox.setText("Bạn đã nhập thành công một Khách Mua ");
       msgBox.setStandardButtons(QMessageBox::Ok);
       msgBox.exec();
       ui->tabCongCu_khachMua_ten->setText("");
       ui->tabCongCu_khachMua_phoneNumber->setText("");
       ui->tabCongCu_khachMua_note->setText("");
   }
   else
   {
        qDebug() << __FUNCTION__ <<"nhập Khách Mua is error: "
                 << query.lastError();
   }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
     QSqlQuery query;
     /* enter tab Nhập Hàng */
     if (index == 0)
     {
         /* check the current idx of Ten Hang trong kho */
       query.prepare( "select count(1) from tenHang" );
       if( !query.exec() )
         qDebug() << "errro"<<query.lastError();
       else
       {
         qDebug()<< "Selected!";
         query.seek(0);
         qDebug()<< "value"<<query.value(0).toInt();
       }
         /* check the current idx of Don Vi */

         /* check the current idx of Nguon Nhap */
     }
}
