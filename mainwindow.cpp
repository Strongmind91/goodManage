#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QDebug>
#include <QVariant>
#include <QtSql>
#include "stdio.h"
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractTableModel>

#define CBB_TEN_HANG         0
#define CBB_DON_VI           1
#define CBB_NGUON_NHAP       2
#define CBB_KHACH_MUA        3


#define COL_TEN_HANG         1
#define COL_TEN_DONVI        1
#define COL_TEN_NGUON_NHAP   1
#define COL_TEN_KHACH_MUA    1

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

    /*updare comboboxs Ten Hang*/
    updateComboBox(CBB_TEN_HANG);

    /*updare comboboxs Don Vi */
    updateComboBox(CBB_DON_VI);

    /*updare comboboxs Ten Nguon Nhap */
    updateComboBox(CBB_NGUON_NHAP);

    /*update Comboboxs Khach Mua*/
    updateComboBox(CBB_KHACH_MUA);

    /* create modelTable for tab Xuat Hang */
    currentRow_tabXuatHang_tableView = 1;
    modelTable = new QSqlTableModel(this, mydb);
    modelTable->setTable("tempXuatHang");
    modelTable->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelTable->select();

    ui->tabXuatHang_tableView->setModel(modelTable);
    ui->tabXuatHang_tableView->show();

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

       /*update comboboxs Ten Hang*/
       updateComboBox(CBB_TEN_HANG);
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

       /*updare comboboxs Don Vi */
       updateComboBox(CBB_DON_VI);
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

       /*updare comboboxs Ten Nguon Nhap */
       updateComboBox(CBB_NGUON_NHAP);
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

       /*update Comboboxs Khach Mua*/
       updateComboBox(CBB_KHACH_MUA);
   }
   else
   {
        qDebug() << __FUNCTION__ <<"nhập Khách Mua is error: "
                 << query.lastError();
   }
}

void MainWindow::updateComboBox(int index)
{
     QSqlQuery query;
     QSqlRecord rec;
     int idx_temp;
     int idx_current;

     switch(index)
     {
     case CBB_TEN_HANG:
         /* check the current idx of Ten Hang trong kho */
         query.prepare( "select count(*) from tenHang" );
         if(!query.exec())
         {
             qDebug() << "can not check number of elements in tenHang table"<<query.lastError();
             return;
         }
         query.seek(0);
         idx_temp = query.value(0).toInt();
         idx_current = ui->tabNhapHang_cb_tenHang->count();
         if (idx_current < idx_temp)
         {
             /*start to get and add ten hang to the current list */
             query.prepare( "select id, tenHang from tenHang");
             if( !query.exec() )
             {
                 qDebug() << "errro"<<query.lastError();
                 return;
             }
             else
             {
                 do
                 {
                     query.seek(idx_current);
                     QString tempString;
                     tempString = query.value(COL_TEN_HANG).toString();
                     ui->tabNhapHang_cb_tenHang->addItem(tempString);
                     ui->tabXuatHang_cb_tenHang->addItem(tempString);
                     ui->tabHuHong_cb_tenHang->addItem(tempString);
                     idx_current++;
                 }while(idx_current <idx_temp);
             }
         }
         break;
     case CBB_DON_VI:
         /* check the current idx of Don Vi */
         query.prepare( "select count(*) from tenDonVi" );
         if(!query.exec())
         {
           qDebug() << "can not check number of elements in tenDonVi table"<<query.lastError();
           return;
         }
         query.seek(0);
         idx_temp = query.value(0).toInt();
         idx_current = ui->tabNhapHang_cb_tenDonVi->count();
         if (idx_current < idx_temp)
         {
             /*start to get and add ten hang to the current list */
             query.prepare( "select id, tenDonVi from tenDonVi");
             if( !query.exec() )
             {
                 qDebug() << "errro"<<query.lastError();
                 return;
             }
             else
             {
                 do
                 {
                     query.seek(idx_current);
                     ui->tabNhapHang_cb_tenDonVi->addItem(query.value(COL_TEN_DONVI).toString());
                     idx_current++;
                 }while(idx_current <idx_temp);
             }
         }
         break;
     case CBB_NGUON_NHAP:
         /* check the current idx of Nguon Nhap */
         query.prepare( "select count(*) from nguonNhap" );
         if(!query.exec())
         {
           qDebug() << "can not check number of elements in nguonNhap table"<<query.lastError();
           return;
         }
         query.seek(0);
         idx_temp = query.value(0).toInt();
         idx_current = ui->tabNhapHang_cb_tenNguonNhap->count();
         if (idx_current < idx_temp)
         {
             /*start to get and add ten hang to the current list */
             query.prepare( "select id, tenNguonNhap from nguonNhap");
             if( !query.exec() )
             {
                 qDebug() << "errro"<<query.lastError();
                 return;
             }
             else
             {
                 do
                 {
                     query.seek(idx_current);
                     ui->tabNhapHang_cb_tenNguonNhap->addItem(query.value(COL_TEN_NGUON_NHAP).toString());
                     idx_current++;
                 }while(idx_current <idx_temp);
             }
         }
         break;
     case CBB_KHACH_MUA:
         /* check the current idx of Khach Mua */
         query.prepare( "select count(*) from khachMua" );
         if(!query.exec())
         {
           qDebug() << "can not check number of elements in khachMua table"<<query.lastError();
           return;
         }
         query.seek(0);
         idx_temp = query.value(0).toInt();
         idx_current = ui->tabXuatHang_cb_tenKhachHang->count();
         if (idx_current < idx_temp)
         {
             /*start to get and add ten Khach Mua to the current list */
             query.prepare( "select id, tenKhachMua from khachMua");
             if( !query.exec() )
             {
                 qDebug() << "errro"<<query.lastError();
                 return;
             }
             else
             {
                 do
                 {
                     query.seek(idx_current);
                     QString tempString;
                     tempString = query.value(COL_TEN_KHACH_MUA).toString();
                     ui->tabXuatHang_cb_tenKhachHang->addItem(tempString);
                     ui->tabThuTien_cb_tenKhachHang->addItem(tempString);
                     idx_current++;
                 }while(idx_current <idx_temp);
             }
         }
         break;
     default:
         break;
     }
 }

void MainWindow::on_tabNhapHang_btn_nhapVaoKho_clicked()
{
    QMessageBox msgBox;
    QSqlQuery query;

    QString tenHang;
    QString donVi;
    QString note;
    QString nguonNhap;
    QString nguoiNhap;
    qint64 giaNhap;
    qint64 thoigianNhap;
    qint64 soLuong;
    int ret;

    /* Check valid input fields */
    if ((ui->tabNhapHang_inputSoLuong->text() == "") ||
        (ui->tabNhapHang_inputGiaNhap->text() == "") ||
        (ui->tabNhapHang_inputGhiChu->text() == ""))
    {
        msgBox.setWindowTitle("quản lý tốt để thành công");
        msgBox.setText("Làm ơn nhập đầy đủ thông tin: Số Lượng, Giá Nhập, Ghi Chú");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }

    /* Let user confirm about their inputs */
    do
    {
        /*Consolidate data input from users*/
        QDateTime local(QDateTime::currentDateTime());
        tenHang      = ui->tabNhapHang_cb_tenHang->currentText();
        donVi        = ui->tabNhapHang_cb_tenDonVi->currentText();
        note         = ui->tabNhapHang_inputGhiChu->text();
        nguonNhap    = ui->tabNhapHang_cb_tenNguonNhap->currentText();
        nguoiNhap    = ui->tabNhapHang_cb_nguoiNhap->currentText();
        giaNhap      = ui->tabNhapHang_inputGiaNhap->text().toInt();
        thoigianNhap = local.currentSecsSinceEpoch();
        soLuong      = ui->tabNhapHang_inputSoLuong->text().toInt();

        QString tempStr = "Bạn hãy kiểm tra thông tin hàng hóa một lần nữa:\n\nTên Hàng: "+ tenHang+ "\nSố Lượng: "+ QString::number(soLuong) +"\nGiá Nhập: "+ QString::number(giaNhap)+ "\nNguồn Nhập: " + nguonNhap + "\nĐơn Vị: "+ donVi +"\nNgười Nhập: "+ nguoiNhap+ "\nGhi Chú: "+ note;
        msgBox.setText(tempStr);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);

        ret = msgBox.exec();
        if (ret == QMessageBox::Yes)
        {
            break;
        }
        else if (ret == QMessageBox::No)
        {
            ui->tabNhapHang_inputGhiChu->setText("");
            ui->tabNhapHang_inputGiaNhap->setText("");
            ui->tabNhapHang_inputSoLuong->setText("");
        }
    }while(1);

    /* Insert infomartion to table khoHang*/
     query.prepare( "select count(*) from khoHang" );
     if(!query.exec())
     {
       qDebug() << "can not select khoHang table"<<query.lastError();
       return;
     }
     query.last();
     query.prepare("INSERT INTO khoHang (tenHang, donVi, note, nguonNhap, nguoiNhap, giaNhap, thoigianNhap, soLuong) VALUES(:tenHang, :donVi, :note, :nguonNhap, :nguoiNhap, :giaNhap, :thoigianNhap, :soLuong)");
     query.bindValue(":tenHang", tenHang);
     query.bindValue(":donVi", donVi);
     query.bindValue(":note", note);
     query.bindValue(":nguonNhap", nguonNhap);
     query.bindValue(":nguoiNhap", nguoiNhap);
     query.bindValue(":giaNhap", giaNhap);
     query.bindValue(":thoigianNhap", thoigianNhap);
     query.bindValue(":soLuong", soLuong);
     if(!query.exec())
     {
       qDebug() << "can not insert info to khoHang table"<<query.lastError();
       return;
     }
     else
     {
         msgBox.setText("Bạn đã nhập kho thành công một mặt hàng ");
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.exec();
         ui->tabNhapHang_inputGhiChu->setText("");
         ui->tabNhapHang_inputGiaNhap->setText("");
         ui->tabNhapHang_inputSoLuong->setText("");
     }
}

void MainWindow::on_tabXuatHang_btn_AddToTableView_clicked()
{
    QString tenKhachHang;
    QString currentKhachHang;
    QString tenHang;
    qint64 soLuong;
    qint64 giaXuat;
    QString content;

    tenKhachHang = ui->tabXuatHang_cb_tenKhachHang->currentText();
    tenHang      = ui->tabXuatHang_cb_tenHang->currentText();
    soLuong      = ui->tabXuatHang_inputSoLuong->text().toInt();
    giaXuat      = ui->tabXuatHang_inputGiaXuat->text().toInt();

    content =  "Tên Khách Hàng: " + tenKhachHang;
    ui->tabXuatHang_TextBrowser->setText(content);


    record = modelTable->record();
    record.setValue("STT", currentRow_tabXuatHang_tableView);
    record.setValue("tenHang", tenHang);
    record.setValue("soLuong", soLuong);
    record.setValue("giaXuat", giaXuat);

    if(modelTable->insertRecord(-1, record))
    {
        qDebug()<<"successful insertion";
    }
    else
    {
        qDebug()<<"failure insertion";
    }
    currentRow_tabXuatHang_tableView++;
}



void MainWindow::on_tabXuatHang_btn_LuuVao_clicked()
{
    qDebug()<<"record row "<<record.count();
//    for(int i=0; i< record.count(); i++)
//    {
        qDebug()<<"row "<<modelTable->record(0).value("STT");
        qDebug()<<"row "<<modelTable->record(0).value("tenHang");
        qDebug()<<"row "<<modelTable->record(0).value("soLuong");
        qDebug()<<"row "<<modelTable->record(0).value("giaXuat");
 //   }
}
