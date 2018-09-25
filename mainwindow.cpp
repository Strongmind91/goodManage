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
#define COL_SO_LUONG_KHOHANG 2

#define TAB_TIM_HANG         0
#define TAB_DOANH_SO         1
#define TAB_CONG_NO          2
#define TAB_HU_HONG          3

#define DATE_INTERACTIVE_FROM 1
#define DATE_INTERACTIVE_TO   2

#define THONG_KE_KHOANG_THOIGIAN    0
#define THONG_KE_KHACHHANG          1
#define THONG_KE_TENHANG            2

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
    QString dbName = "db.db3";
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
    ui->tabXuatHang_tableView->setColumnWidth(0, 30);

    /* create modelTable for tab Thong Ke */
    modelTableTabThongKe_TimHang = new QSqlTableModel(this, mydb);
    modelTableTabThongKe_DoanhSo = new QSqlTableModel(this, mydb);
    modelTableTabThongKe_CongNo  = new QSqlTableModel(this, mydb);
    modelTableTabThongKe_CongNo_LichSuThu     = new QSqlTableModel(this, mydb);
    modelTableTabThongKe_CongNo_LichSuBanHang = new QSqlTableModel(this, mydb);
    modelTableTabThongKe_HuHong =  new QSqlTableModel(this, mydb);

    modelTableTabThongKe_TimHang->setTable("thongKeTimHang");
    modelTableTabThongKe_DoanhSo->setTable("thongKeDoanhSo");
    modelTableTabThongKe_CongNo->setTable("thongKeCongNo");
    modelTableTabThongKe_CongNo_LichSuThu->setTable("lichSuThu");
    modelTableTabThongKe_CongNo_LichSuBanHang->setTable("lichSuBanHang");
    modelTableTabThongKe_HuHong->setTable("thongKeHuHong");

    modelTableTabThongKe_TimHang->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelTableTabThongKe_TimHang->select();

    modelTableTabThongKe_DoanhSo->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelTableTabThongKe_DoanhSo->select();

    modelTableTabThongKe_CongNo->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelTableTabThongKe_CongNo->select();

    modelTableTabThongKe_CongNo_LichSuThu->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelTableTabThongKe_CongNo_LichSuThu->select();

    modelTableTabThongKe_CongNo_LichSuBanHang->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelTableTabThongKe_CongNo_LichSuBanHang->select();

    modelTableTabThongKe_HuHong->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelTableTabThongKe_HuHong->select();


    recordThongKe_DoanhSo = modelTableTabThongKe_DoanhSo->record();
    recordThongKe_TimHang = modelTableTabThongKe_TimHang->record();
    recordThongKe_CongNo  = modelTableTabThongKe_CongNo->record();
    recordThongKe_CongNo_LichSuBanHang = modelTableTabThongKe_CongNo_LichSuBanHang->record();
    recordThongKe_CongNo_LichSuThu     = modelTableTabThongKe_CongNo_LichSuThu->record();
    recordThongKe_HuHong  = modelTableTabThongKe_HuHong->record();

    ui->tabThongKe_CongNo_btn_LichSuBanHang->hide();
    ui->tabThongKe_CongNo_btn_LichSuThu->hide();

    /* Initial show for tabThongKe_DoanhSo*/
    tabThongKe_DoanhSo_InitialShow();

    /* Initial show for tabThongKe_HuHong*/
    tabThongKe_HuHong_InitialShow();
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
                     ui->tabThongKe_cb_tenHang->addItem(tempString);
                     ui->tabThongKe_DoanhSo_cbb_tenHang->addItem(tempString);
                     ui->tabThongKe_HuHong_cbb_tenHang->addItem(tempString);
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
                     ui->tabThongKe_DoanhSo_cbb_tenKhachHang->addItem(tempString);
                     ui->tabThongKe_CongNo_cb_tenKhachHang->addItem(tempString);
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

        ui->tabNhapHang_inputSoLuong->setText("");
        ui->tabNhapHang_inputGiaNhap->setText("");
        ui->tabNhapHang_inputGhiChu->setText("");
        return;
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
            return;
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
    QString tenHang;
    qint64 soLuong;
    qint64 giaXuat;
    QMessageBox msgBox;

    /* check if "so Luong" or "gia xuat" is null--> should notify to the user */
    if (!(ui->tabXuatHang_inputSoLuong->text().compare("")) ||
        !(ui->tabXuatHang_inputGiaXuat->text().compare("")))
    {
        /* This changes to another customer, a notice should be showed up*/
        msgBox.setWindowTitle("quản lý tốt để thành công");
        msgBox.setText("Làm ơn nhập đầy đủ thông tin \"Số lượng\" và \"Giá xuất\" ");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return;
    }

    tenKhachHang = ui->tabXuatHang_cb_tenKhachHang->currentText();
    tenHang      = ui->tabXuatHang_cb_tenHang->currentText();
    soLuong      = ui->tabXuatHang_inputSoLuong->text().toInt();
    giaXuat      = ui->tabXuatHang_inputGiaXuat->text().toInt();

    if ((currentKhachHang != "")&&
        (QString::compare(currentKhachHang, tenKhachHang) != 0))
    {
        /* This changes to another customer, a notice should be showed up*/
        msgBox.setWindowTitle("quản lý tốt để thành công");
        msgBox.setText("Làm ơn xuất hàng cho lần lượt từng khách ");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return;
    }

    currentKhachHang = tenKhachHang;

    record = modelTable->record();
    record.setValue("STT", modelTable->rowCount());
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
   // currentRow_tabXuatHang_tableView++;
}

void MainWindow::on_tabXuatHang_btn_LuuVao_clicked()
{
     QSqlQuery query;
     QSqlQuery queryInside;
     QMessageBox msgBox;
     QString tenHang;
     QString nguoiXuat;
     int giaXuat;
     int thoiGianXuat;
     int soLuong;
     QString khachHang;
     int tmp_count;
     int soLuongTmp = 0;
     int soLuongRequest = 0;

     tmp_count = modelTable->rowCount();

     /* check valid of modelTable before try to insert it to the database */
     if (tmp_count > 0)
     {
         for(int idx=0; idx< tmp_count; idx++)
         {
              qDebug() << "idx "<<idx;

              /* Select current tenHang trong kho */
              tenHang   = modelTable->record(idx).value("tenHang").toString();
              QString tempStr = "select tenHang, soLuong, id from khoHang where tenHang = '" + tenHang+"'";
              query.prepare( tempStr);

              if(!query.exec())
              {
                  qDebug() << "can not select current tenHang in khoHang table"<<query.lastError();
                  return;
              }

              soLuong   = modelTable->record(idx).value("soLuong").toInt();
              tenHang   = modelTable->record(idx).value("tenHang").toString();
              soLuongRequest = soLuong;
              /* Calculate all count of this tenHang in khoHang */
              while(query.next())
              {
                  soLuongTmp +=  query.value(1).toInt();
              }

              if (soLuongTmp < modelTable->record(idx).value("soLuong").toInt())
              {
                  /* can not export this soLuong -- need to notice user*/
                  msgBox.setText("Số lượng của mặt hàng này trong kho không có đủ: "+ tenHang);
                  msgBox.setStandardButtons(QMessageBox::Ok);
                  msgBox.setIcon(QMessageBox::Critical);
                  msgBox.exec();
                  modelTable->removeRow(idx);
                  return;
              }

              /* Reset the pointer of query*/
              query.first();
              while(soLuong > 0)
              {
                  soLuongTmp = query.value(1).toInt();

                  /* Start to minus soLuong cua loai hang nay trong kho */
                  // if remain value is zero, we should remove this tenHang
                  QString currentIdx =  query.value(2).toString();
                  if ((soLuong - soLuongTmp ) >=0)
                  {
                      tempStr = "DELETE FROM khoHang WHERE id = '" + currentIdx + "'";
                  }
                  else
                  {
                      tempStr = "UPDATE khoHang SET soLuong = '" + QString::number(soLuongTmp - soLuong)+ "' WHERE id = '" + currentIdx + "'";
                  }

                  queryInside.prepare( "select count(*) from khoHang" );
                  if(!queryInside.exec())
                      qDebug() << queryInside.lastError();

                  queryInside.prepare(tempStr);
                  if(!queryInside.exec())
                      qDebug() << queryInside.lastError();
                  /*update so luong can phai xuat*/
                  soLuong -= soLuongTmp;

                  query.next();
              }

             /* Insert infomartion to table xuathang */
             query.prepare( "select count(*) from xuatHang" );
             if(!query.exec())
             {
                 qDebug() << "can not select xuatHang table"<<query.lastError();
                 return;
             }

             /* get input from users */
             nguoiXuat = ui->tabXuatHang_cb_nguoiXuat->currentText();
             giaXuat   = modelTable->record(idx).value("giaXuat").toInt();
             QDateTime local(QDateTime::currentDateTime());
             thoiGianXuat = local.currentSecsSinceEpoch();
             khachHang = currentKhachHang;
             query.last();
             query.prepare("INSERT INTO xuatHang (tenHang, nguoiXuat, giaXuat, thoiGian, soLuong, khachHang) VALUES(:tenHang, :nguoiXuat, :giaXuat, :thoiGianXuat, :soLuong, :khachHang)");
             query.bindValue(":tenHang", tenHang);
             query.bindValue(":nguoiXuat", nguoiXuat);
             query.bindValue(":giaXuat", giaXuat);
             query.bindValue(":thoiGianXuat", thoiGianXuat);
             query.bindValue(":soLuong", soLuongRequest);
             query.bindValue(":khachHang", khachHang);
             if(!query.exec())
             {
                 qDebug() << "can not insert info to xuatHang table"<<query.lastError();
                 return;
             }
         }
         msgBox.setText("Bạn đã xuất kho thành công một khách hàng ");
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.exec();
         currentKhachHang = "";
         ui->tabXuatHang_inputGiaXuat->setText("");
         ui->tabXuatHang_inputSoLuong->setText("");

         for(int idx = 0; idx < tmp_count; idx++)
         {
             modelTable->removeRow(idx);
         }
    }
}

void MainWindow::on_tabXuatHang_btn_DelToTableView_clicked()
{
    int tmp_rowCount;

    modelTable->removeRow(ui->tabXuatHang_tableView->currentIndex().row());
    tmp_rowCount = modelTable->rowCount();

    for(int idx =0 ; idx < tmp_rowCount; idx++)
    {
         modelTable->setData(modelTable->index(idx,0), idx);
    }
}

void MainWindow::on_tabHuHong_btn_nhapVao_clicked()
{
    QSqlQuery query;
    QSqlQuery queryInSide;
    QString tenHang;
    int giaTien;
    int thoiGian;
    int soLuong;
    QMessageBox msgBox;
    int ret;
    int soLuongTmp = 0;
    int col_Id = 0;
    int col_soLuong = 2;
    int numUpdateHuHongDb = 0;
     QString currentIdx;

    /* Check invalid of input, they should be fully information */
    if (!(ui->tabHuHong_giaTien->text().compare("")) ||
        !(ui->tabHuHong_soLuong->text().compare("")))
    {
        /* This changes to another customer, a notice should be showed up*/
        msgBox.setWindowTitle("quản lý tốt để thành công");
        msgBox.setText("Làm ơn nhập đầy đủ thông tin \"Số lượng\" và \"Giá tiền\" ");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return;
    }

    /* Let user confirm about their inputs */
    /*Consolidate data input from users*/
    QDateTime local(QDateTime::currentDateTime());
    tenHang      = ui->tabHuHong_cb_tenHang->currentText();
    giaTien      = ui->tabHuHong_giaTien->text().toInt();
    thoiGian     = local.currentSecsSinceEpoch();
    soLuong      = ui->tabHuHong_soLuong->text().toInt();
    numUpdateHuHongDb = soLuong;

    QString tempStr = "Bạn hãy kiểm tra thông tin hàng hóa hư hỏng một lần nữa:\n\nTên Hàng: "+ tenHang+ "\nSố Lượng: "+ QString::number(soLuong) +"\nGiá tiền: "+ QString::number(giaTien);
    msgBox.setText(tempStr);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    ret = msgBox.exec();
    if (ret == QMessageBox::No)
    {
        ui->tabHuHong_giaTien->setText("");
        ui->tabHuHong_soLuong->setText("");
        return;
    }

     query.prepare( "select count(*) from khoHang" );
    /* Check whether the current good exists in stock and ensure counts is valid */
    if(!query.exec())
    {
        qDebug() << "can not select khoHang table"<<query.lastError();
        return;
    }
    query.last();
    tempStr = "select id, tenHang, soLuong from khoHang where tenHang = '" + tenHang+"'";
    query.prepare(tempStr);
    if(!query.exec())
    {
        qDebug() << "can not select item nay trong kho hang "<<query.lastError();
        return;
    }

        // Count sum of this good in the stock
    soLuong   = ui->tabHuHong_soLuong->text().toInt();
    tenHang   = ui->tabHuHong_cb_tenHang->currentText();
    /* Calculate all count of this tenHang in khoHang */
    while(query.next())
    {
        soLuongTmp +=  query.value(col_soLuong).toInt();
        qDebug()<<" id "<<  query.value(0).toInt();
        qDebug()<<" so luong "<<  query.value(2).toInt();
    }

    if (soLuongTmp < soLuong)
    {
        /* can not input this soLuong -- need to notice user*/
        msgBox.setText("Số lượng của mặt hàng này trong kho không có đủ: "+ tenHang);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        ui->tabHuHong_giaTien->setText("");
        ui->tabHuHong_soLuong->setText("");
        return;
    }

    /* Reset the pointer of query*/
    query.first();
    while(soLuong > 0)
    {
        soLuongTmp = query.value(col_soLuong).toInt();

        /* Start to minus soLuong cua loai hang nay trong kho */
        // if remain value is zero, we should remove this tenHang
        currentIdx =  QString::number(query.value(col_Id).toInt());
        if ((soLuong - soLuongTmp ) >=0)
        {
            tempStr = "DELETE FROM khoHang WHERE id = '" + currentIdx + "'";
        }
        else
        {
            tempStr = "UPDATE khoHang SET soLuong = '" + QString::number(soLuongTmp - soLuong)+ "' WHERE id = '" + currentIdx + "'";
        }

        queryInSide.prepare( "select count(*) from khoHang" );
       /* Check whether the current good exists in stock and ensure counts is valid */
       if(!queryInSide.exec())
       {
           qDebug() << "can not select khoHang table"<<query.lastError();
           return;
       }
        queryInSide.prepare(tempStr);
        if(!queryInSide.exec())
            qDebug() << queryInSide.lastError();
        /*update so luong can phai xuat*/
        soLuong -= soLuongTmp;

        if (!query.next())
        {
            break;
        }
    }

    /* Update the information into huHong database */
    query.prepare( "select count(*) from huHong" );
    if(!query.exec())
    {
        qDebug() << "can not select huHong table"<<query.lastError();
        return;
    }
    query.last();
    query.prepare("INSERT INTO huHong (tenHang, giaTien, thoiGian, soLuong) VALUES(:tenHang, :giaTien, :thoiGian, :soLuong)");
    query.bindValue(":tenHang", tenHang);
    query.bindValue(":giaTien", giaTien);
    query.bindValue(":thoiGian", thoiGian);
    query.bindValue(":soLuong", numUpdateHuHongDb);

    if(!query.exec())
    {
        qDebug() << "can not insert info to huHong table"<<query.lastError();
        ui->tabHuHong_giaTien->setText("");
        ui->tabHuHong_soLuong->setText("");
        return;
    }
    ui->tabHuHong_giaTien->setText("");
    ui->tabHuHong_soLuong->setText("");
}

void MainWindow::on_tabThuTien_btn_nhapVao_clicked()
{
    QSqlQuery query;
    QString nguoiThu;
    int soTien;
    int thoiGian;
    QString khachHang;
    QMessageBox msgBox;
    int ret;

    query.prepare( "select count(*) from thuTien" );
    if(!query.exec())
    {
        qDebug() << "can not select thuTien table"<<query.lastError();
        return;
    }

    /* Let user confirm about their inputs */
    do
    {
        /*Consolidate data input from users*/
        QDateTime local(QDateTime::currentDateTime());
        nguoiThu      = ui->tabThuTien_cbb_nguoiNhap->currentText();
        soTien        = ui->tabThuTien_tbx_tienThu->text().toInt();
        thoiGian      = local.currentSecsSinceEpoch();
        khachHang     = ui->tabThuTien_cb_tenKhachHang->currentText();

        QString tempStr = "Bạn hãy kiểm tra thông tin tiền thu một lần nữa:\n\nNgười Thu: "+ nguoiThu+ "\nSố Tiền: "+ QString::number(soTien)+"\nKhách Hàng: "+ khachHang;
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
            ui->tabHuHong_giaTien->setText("");
            ui->tabHuHong_soLuong->setText("");
            return;
        }
    }while(1);

    query.last();
    query.prepare("INSERT INTO thuTien (nguoiThu, soTien, thoiGian, khachHang) VALUES(:nguoiThu, :soTien, :thoiGian, :khachHang)");
    query.bindValue(":nguoiThu", nguoiThu);
    query.bindValue(":soTien", soTien);
    query.bindValue(":thoiGian", thoiGian);
    query.bindValue(":khachHang", khachHang);

    if(!query.exec())
    {
        qDebug() << "can not insert info to thuTien table"<<query.lastError();
        return;
    }
}

void MainWindow::on_tabThongKe_btn_timTrongKho_clicked()
{
    QString tenHangSearch;
    QSqlQuery query;
    QString tenHang;
    QString note;
    int soLuong;
    int giaNhap;
    int id;
    int ngayNhap;

    /* Try to remove all current values of model */
    int tmpCount = modelTableTabThongKe_TimHang->rowCount();
    for(int idx =0 ; idx < tmpCount; idx++)
    {
        modelTableTabThongKe_TimHang->removeRow(0);
    }

    tenHangSearch = ui->tabThongKe_cb_tenHang->currentText();


    /* search tenHang trong kho */
    query.prepare( "select count(*) from khoHang" );
    if(!query.exec())
    {
        qDebug() << "can not select khoHang table"<<query.lastError();
        return;
    }
    QString tempStr = "select id, tenHang, soLuong, note, giaNhap, thoiGianNhap from khoHang where tenHang = '" + tenHangSearch+"'";
    query.prepare( tempStr);

    if(!query.exec())
    {
        qDebug() << "can not select current tenHang in khoHang table"<<query.lastError();
        return;
    }
    if (query.first())
    {
        do
        {
            /* Read data from database*/
            id = query.value(0).toInt();
            tenHang = query.value(1).toString();
            soLuong = query.value(2).toInt();
            note    = query.value(3).toString();
            giaNhap = query.value(4).toInt();
            ngayNhap = query.value(5).toInt();
            QString ngayNhapStr = QDateTime::fromSecsSinceEpoch(ngayNhap).toString("hh:mm:ss dd/MM/yyyy");

            /* Update the data to the model for showing */
            recordThongKe_TimHang.setValue("id", id);
            recordThongKe_TimHang.setValue("tenHang", tenHang);
            recordThongKe_TimHang.setValue("note", note);
            recordThongKe_TimHang.setValue("soLuong", soLuong);
            recordThongKe_TimHang.setValue("giaNhap", giaNhap);
            recordThongKe_TimHang.setValue("ngayNhap", ngayNhapStr);

            /* Insert to record */
            modelTableTabThongKe_TimHang->insertRecord(-1, recordThongKe_TimHang);
        }while(query.next());
    }
}

void MainWindow::on_tabThongKe_tabWidget_currentChanged(int index)
{
    switch(index)
    {
    case TAB_TIM_HANG:
        ui->tabThongKe_tableView->setModel(modelTableTabThongKe_TimHang);
        ui->tabThongKe_tableView->setColumnWidth(0, 30);
        ui->tabThongKe_tableView->setColumnWidth(5, 120);
        ui->tabThongKe_tableView->show();
        break;
    case TAB_DOANH_SO:
        ui->tabThongKe_tableView->setModel(modelTableTabThongKe_DoanhSo);
        ui->tabThongKe_tableView->setColumnWidth(0, 30);
        ui->tabThongKe_tableView->setColumnWidth(5, 120);
        ui->tabThongKe_tableView->show();
        break;
    case TAB_CONG_NO:
        ui->tabThongKe_tableView->setModel(modelTableTabThongKe_CongNo);
        ui->tabThongKe_tableView->setColumnWidth(0, 30);
        ui->tabThongKe_tableView->show();
        break;
    case TAB_HU_HONG:
        ui->tabThongKe_tableView->setModel(modelTableTabThongKe_HuHong);
        ui->tabThongKe_tableView->setColumnWidth(0, 30);
        ui->tabThongKe_tableView->show();
        break;
    }
}



void MainWindow::on_tabThongKe_DoanhSo_btn_TuNgay_clicked()
{
   ui->calendarWidget_DoanhSo->show();
   ui->tabThongKe_DoanhSo_btn_TuNgay->hide();
   dateInteractive_DoanhSo = DATE_INTERACTIVE_FROM;
}

void MainWindow::on_tabThongKe_DoanhSo_btn_DenNgay_clicked()
{
    ui->calendarWidget_DoanhSo->show();
    ui->tabThongKe_DoanhSo_btn_DenNgay->hide();
    dateInteractive_DoanhSo = DATE_INTERACTIVE_TO;
}

void MainWindow::on_radioButton_KhoangThoiGian_clicked()
{
    int tmp_count = modelTableTabThongKe_DoanhSo->rowCount();
    for(int idx = 0; idx < tmp_count; idx++)
    {
        modelTableTabThongKe_DoanhSo->removeRow(0);
    }

    /* Hide all thing related to tim theo KhachHang */
    ui->tabThongKe_DoanhSo_lbl_TenKhachHang->hide();
    ui->tabThongKe_DoanhSo_cbb_tenKhachHang->hide();

    /* Hide all thing related to tim theo TenHang*/
    ui->tabThongKe_DoanhSo_cbb_tenHang->hide();
    ui->tabThongKe_DoanhSo_lbl_TenHang->hide();

    /* show related things */
    ui->tabThongKe_DoanhSo_lbl_DenNgay->show();
    ui->tabThongKe_DoanhSo_lbl_TuNgay->show();
    ui->tabThongKe_DoanhSo_btn_DenNgay->show();
    ui->tabThongKe_DoanhSo_btn_TuNgay->show();

    thongKeType_DoanhSo = THONG_KE_KHOANG_THOIGIAN;
}

void MainWindow::on_radioButton_khachHang_clicked()
{
    int tmp_count = modelTableTabThongKe_DoanhSo->rowCount();
    for(int idx = 0; idx < tmp_count; idx++)
    {
        modelTableTabThongKe_DoanhSo->removeRow(0);
    }

    /* Hide all thing related to tim theo khoangThoiGian*/
    ui->calendarWidget_DoanhSo->hide();
    ui->tabThongKe_DoanhSo_btn_DenNgay->hide();
    ui->tabThongKe_DoanhSo_btn_TuNgay->hide();
    ui->tabThongKe_DoanhSo_lbl_DenNgay->hide();
    ui->tabThongKe_DoanhSo_lbl_TuNgay->hide();
    ui->tabThongKe_DoanhSo_LEdit_DenNgay->hide();
    ui->tabThongKe_DoanhSo_LEdit_TuNgay->hide();

    /* Hide all thing related to tim theo TenHang*/
    ui->tabThongKe_DoanhSo_cbb_tenHang->hide();
    ui->tabThongKe_DoanhSo_lbl_TenHang->hide();

    /* show related things */
    ui->tabThongKe_DoanhSo_lbl_TenKhachHang->show();
    ui->tabThongKe_DoanhSo_cbb_tenKhachHang->show();

    thongKeType_DoanhSo = THONG_KE_KHACHHANG;
}

void MainWindow::on_radioButton_TenHang_clicked()
{
    int tmp_count = modelTableTabThongKe_DoanhSo->rowCount();
    for(int idx = 0; idx < tmp_count; idx++)
    {
        modelTableTabThongKe_DoanhSo->removeRow(0);
    }

    /* Hide all thing related to tim theo khoangThoiGian*/
    ui->calendarWidget_DoanhSo->hide();
    ui->tabThongKe_DoanhSo_btn_DenNgay->hide();
    ui->tabThongKe_DoanhSo_btn_TuNgay->hide();
    ui->tabThongKe_DoanhSo_lbl_DenNgay->hide();
    ui->tabThongKe_DoanhSo_lbl_TuNgay->hide();
    ui->tabThongKe_DoanhSo_LEdit_DenNgay->hide();
    ui->tabThongKe_DoanhSo_LEdit_TuNgay->hide();

    /* Hide all thing related to tim theo KhachHang*/
    ui->tabThongKe_DoanhSo_lbl_TenKhachHang->hide();
    ui->tabThongKe_DoanhSo_cbb_tenKhachHang->hide();

    /* show related things */
    ui->tabThongKe_DoanhSo_cbb_tenHang->show();
    ui->tabThongKe_DoanhSo_lbl_TenHang->show();

    thongKeType_DoanhSo = THONG_KE_TENHANG;
}

void MainWindow::on_tabThongKe_btn_ThongKe_clicked()
{
    QString fromDateStr;
    QString toDateStr;
    QDateTime fromDate;
    QDateTime toDate;
    QSqlQuery query;
    qint64 fromDateInt;
    qint64 toDateInt;
    int id;
    QString tenHang;
    QString khachHang;
    int soLuong;
    int giaXuat;
    int thoiGian;
    QString thoiGianStr;
    QString tenKhachHang;
    QString tempStr;
    static int idxTest = 0;

    /*remove model data */
    int tmp_count;
    tmp_count = modelTableTabThongKe_DoanhSo->rowCount();
    for(int idx = 0; idx < tmp_count; idx++)
    {
        modelTableTabThongKe_DoanhSo->removeRow(0);
    }

    switch (thongKeType_DoanhSo)
    {
    case THONG_KE_KHOANG_THOIGIAN:
        fromDateStr = ui->tabThongKe_DoanhSo_LEdit_TuNgay->text();
        toDateStr   = ui->tabThongKe_DoanhSo_LEdit_DenNgay->text();

        fromDate = QDateTime::fromString(fromDateStr + "00:00:00", "dd/MM/yyyyhh:mm:ss");
        toDate   = QDateTime::fromString(toDateStr +"23:59:59", "dd/MM/yyyyhh:mm:ss");

        fromDateInt = fromDate.toSecsSinceEpoch();
        toDateInt   = toDate.toSecsSinceEpoch();

        tempStr = "select id, tenHang, khachHang, soLuong, giaXuat, thoiGian from xuatHang where thoiGian >= '" + QString::number(fromDateInt) +"' and thoiGian <= '" + QString::number(toDateInt) + "'";
        query.prepare( tempStr);

        if(!query.exec())
        {
            qDebug() << query.lastError();
            return;
        }

        if (query.first())
        {
            do
            {
                id = query.value(0).toInt();
                tenHang = query.value(1).toString();
                khachHang = query.value(2).toString();
                soLuong  = query.value(3).toInt();
                giaXuat  = query.value(4).toInt();
                thoiGian = query.value(5).toInt();
                thoiGianStr = QDateTime::fromSecsSinceEpoch(thoiGian).toString("hh:mm:ss \n dd/MM/yyyy");

                recordThongKe_DoanhSo.setValue("id", id);
                recordThongKe_DoanhSo.setValue("tenHang", tenHang);
                recordThongKe_DoanhSo.setValue("khachHang", khachHang);
                recordThongKe_DoanhSo.setValue("soLuong", soLuong);
                recordThongKe_DoanhSo.setValue("giaXuat", giaXuat);
                recordThongKe_DoanhSo.setValue("ngayXuat", thoiGianStr);
                /* Insert to record */
                modelTableTabThongKe_DoanhSo->insertRecord(-1, recordThongKe_DoanhSo);
            }while(query.next());
        }

        /* show again the instruction to */
        ui->tabThongKe_DoanhSo_lbl_DenNgay->show();
        ui->tabThongKe_DoanhSo_lbl_TuNgay->show();
        ui->tabThongKe_DoanhSo_btn_DenNgay->show();
        ui->tabThongKe_DoanhSo_btn_TuNgay->show();

        ui->tabThongKe_DoanhSo_LEdit_DenNgay->hide();
        ui->tabThongKe_DoanhSo_LEdit_TuNgay->hide();

    break;
    case THONG_KE_KHACHHANG:
        tenKhachHang = ui->tabThongKe_DoanhSo_cbb_tenKhachHang->currentText();
        tempStr = "select id, tenHang, khachHang, soLuong, giaXuat, thoiGian from xuatHang where khachHang = '" + tenKhachHang + "'";
        query.prepare( tempStr);

        if(!query.exec())
        {
            qDebug() << query.lastError();
            return;
        }

        if (query.first())
        {
            do
            {
                id = query.value(0).toInt();
                tenHang = query.value(1).toString();
                khachHang = query.value(2).toString();
                soLuong  = query.value(3).toInt();
                giaXuat  = query.value(4).toInt();
                thoiGian = query.value(5).toInt();
                thoiGianStr = QDateTime::fromSecsSinceEpoch(thoiGian).toString("hh:mm:ss \n dd/MM/yyyy");

                recordThongKe_DoanhSo.setValue("id", id);
                recordThongKe_DoanhSo.setValue("tenHang", tenHang);
                recordThongKe_DoanhSo.setValue("khachHang", khachHang);
                recordThongKe_DoanhSo.setValue("soLuong", soLuong);
                recordThongKe_DoanhSo.setValue("giaXuat", giaXuat);
                recordThongKe_DoanhSo.setValue("ngayXuat", thoiGianStr);
                /* Insert to record */
                modelTableTabThongKe_DoanhSo->insertRecord(-1, recordThongKe_DoanhSo);
            }while(query.next());
        }
        break;
    case THONG_KE_TENHANG:
        tenHang = ui->tabThongKe_DoanhSo_cbb_tenHang->currentText();
        tempStr = "select id, tenHang, khachHang, soLuong, giaXuat, thoiGian from xuatHang where tenHang = '" + tenHang + "'";
        query.prepare( tempStr);

        if(!query.exec())
        {
            qDebug() << query.lastError();
            return;
        }

        if (query.first())
        {
            do
            {
                id = query.value(0).toInt();
                tenHang = query.value(1).toString();
                khachHang = query.value(2).toString();
                soLuong  = query.value(3).toInt();
                giaXuat  = query.value(4).toInt();
                thoiGian = query.value(5).toInt();
                thoiGianStr = QDateTime::fromSecsSinceEpoch(thoiGian).toString("hh:mm:ss \n dd/MM/yyyy");

                recordThongKe_DoanhSo.setValue("id", id);
                recordThongKe_DoanhSo.setValue("tenHang", tenHang);
                recordThongKe_DoanhSo.setValue("khachHang", khachHang);
                recordThongKe_DoanhSo.setValue("soLuong", soLuong);
                recordThongKe_DoanhSo.setValue("giaXuat", giaXuat);
                recordThongKe_DoanhSo.setValue("ngayXuat", thoiGianStr);
                /* Insert to record */
                modelTableTabThongKe_DoanhSo->insertRecord(-1, recordThongKe_DoanhSo);
            }while(query.next());
        }
        break;
    }
}

void MainWindow::on_tabThongKe_CongNo_btn_timKiem_clicked()
{
    QSqlQuery query;
    int soTienNo = 0;
    int soTienThu = 0;
    int soTienTongHang = 0;

    /* set model to display */
     ui->tabThongKe_tableView->setModel(modelTableTabThongKe_CongNo);
     ui->tabThongKe_tableView->setColumnWidth(0, 30);
     ui->tabThongKe_tableView->show();

    /*get tenKhachHang*/
    QString tenKhachHang = ui->tabThongKe_CongNo_cb_tenKhachHang->currentText();

    /*Remove if any thong tin model ton tai*/
    int tmp_count = modelTableTabThongKe_CongNo->rowCount();
    for(int idx = 0; idx < tmp_count; idx++)
    {
        modelTableTabThongKe_CongNo->removeRow(0);
    }

    /*Search tong so tien da thu */
        // search tat ca va luu vao query
    query.prepare( "select count(*) from thuTien" );
    if(!query.exec())
    {
        qDebug() << "can not select thuTien table"<<query.lastError();
        return;
    }
    QString tempStr = "select soTien from thuTien where khachHang = '" + tenKhachHang+"'";
    query.prepare( tempStr);

    if(!query.exec())
    {
        qDebug() << "can not select current tenKhachHang in thuTien table"<<query.lastError();
        return;
    }
        // bat dau xu ly de lay data
    if (query.first())
    {
        do
        {
            soTienThu += query.value(0).toInt();

        }while(query.next());
    }
    else
    {
        soTienThu = 0;
    }

    /*Search tong so tien da xuat kho*/
        // search tat ca va luu vao query
    query.prepare( "select count(*) from xuatHang" );
    if(!query.exec())
    {
        qDebug() << "can not select xuatHang table"<<query.lastError();
        return;
    }
    tempStr = "select giaXuat, soLuong from xuatHang where khachHang = '" + tenKhachHang+"'";
    query.prepare( tempStr);

    if(!query.exec())
    {
        qDebug() << "can not select current tenKhachHang in xuatHang table"<<query.lastError();
        return;
    }
        // bat dau xu ly de lay data
    if (query.first())
    {
        do
        {
            soTienTongHang += query.value(0).toInt() * query.value(1).toInt() ;

        }while(query.next());

        soTienNo = soTienTongHang - soTienThu;
        recordThongKe_CongNo.setValue("id", 0);
        recordThongKe_CongNo.setValue("khachHang", tenKhachHang);
        recordThongKe_CongNo.setValue("soNo", soTienNo);
        modelTableTabThongKe_CongNo->insertRecord(-1, recordThongKe_CongNo);

        ui->tabThongKe_CongNo_btn_LichSuBanHang->show();
        ui->tabThongKe_CongNo_btn_LichSuThu->show();
    }
    else
    {
        ui->tabThongKe_CongNo_btn_LichSuBanHang->hide();
        ui->tabThongKe_CongNo_btn_LichSuThu->hide();
    }
}

void MainWindow::on_tabThongKe_CongNo_btn_LichSuThu_clicked()
{
    QSqlQuery query;
    int id;
    QString nguoiThu;
    int soTien;
    int thoiGian;
    QString thoiGianStr;

    /*get tenKhachHang*/
    QString tenKhachHang = ui->tabThongKe_CongNo_cb_tenKhachHang->currentText();

    /* set model to display */
     ui->tabThongKe_tableView->setModel(modelTableTabThongKe_CongNo_LichSuThu);

     /*set current columnWeight of thoiGian*/
     ui->tabThongKe_tableView->setColumnWidth(0, 30);
     ui->tabThongKe_tableView->setColumnWidth(3, 120);
     ui->tabThongKe_tableView->show();

    /* Remove if any thong tin model ton tai */
    int tmp_count = modelTableTabThongKe_CongNo_LichSuThu->rowCount();
    for(int idx = 0; idx < tmp_count; idx++)
    {
        modelTableTabThongKe_CongNo_LichSuThu->removeRow(0);
    }

    /* Search thong tin and show for users */
        // search tat ca va luu vao query
    query.prepare( "select count(*) from thuTien" );
    if(!query.exec())
    {
        qDebug() << "can not select thuTien table"<<query.lastError();
        return;
    }
    QString tempStr = "select id, nguoiThu, soTien, thoiGian from thuTien where khachHang = '" + tenKhachHang+"'";
    query.prepare( tempStr);

    if(!query.exec())
    {
        qDebug() << "can not select current tenKhachHang in xuatHang table"<<query.lastError();
        return;
    }
        // bat dau xu ly de lay data
    if (query.first())
    {
        do
        {
            id = query.value(0).toInt();
            nguoiThu = query.value(1).toString();
            soTien = query.value(2).toInt();
            thoiGian = query.value(3).toInt();
            thoiGianStr = QDateTime::fromSecsSinceEpoch(thoiGian).toString("hh:mm:ss \n dd/MM/yyyy");

            recordThongKe_CongNo_LichSuThu.setValue("id", id);
            recordThongKe_CongNo_LichSuThu.setValue("nguoiThu", nguoiThu);
            recordThongKe_CongNo_LichSuThu.setValue("tienThu", soTien);
            recordThongKe_CongNo_LichSuThu.setValue("thoiGian", thoiGianStr);
            /* Insert to record */
            modelTableTabThongKe_CongNo_LichSuThu->insertRecord(-1, recordThongKe_CongNo_LichSuThu);
        }while(query.next());
    }
}

void MainWindow::on_tabThongKe_CongNo_btn_LichSuBanHang_clicked()
{
    QSqlQuery query;
    int id;
    QString nguoiXuat;
    int giaXuat;
    int thoiGian;
    QString thoiGianStr;
    QString tenHang;
    int soLuong;

    /*get tenKhachHang*/
    QString tenKhachHang = ui->tabThongKe_CongNo_cb_tenKhachHang->currentText();

    /* set model to display */
     ui->tabThongKe_tableView->setModel(modelTableTabThongKe_CongNo_LichSuBanHang);

     /*set current columnWeight of thoiGian*/
     ui->tabThongKe_tableView->setColumnWidth(0, 30);
     ui->tabThongKe_tableView->setColumnWidth(5, 120);
     ui->tabThongKe_tableView->show();

    /* Remove if any thong tin model ton tai */
    int tmp_count = modelTableTabThongKe_CongNo_LichSuBanHang->rowCount();
    for(int idx = 0; idx < tmp_count; idx++)
    {
        modelTableTabThongKe_CongNo_LichSuBanHang->removeRow(0);
    }

    /* Search thong tin and show for users */
        // search tat ca va luu vao query
    query.prepare( "select count(*) from xuatHang" );
    if(!query.exec())
    {
        qDebug() << "can not select xuatHang table"<<query.lastError();
        return;
    }
    QString tempStr = "select id, tenHang, soLuong, giaXuat, nguoiXuat, thoiGian from xuatHang where khachHang = '" + tenKhachHang+"'";
    query.prepare( tempStr);

    if(!query.exec())
    {
        qDebug() << "can not select current tenKhachHang in xuatHang table"<<query.lastError();
        return;
    }
        // bat dau xu ly de lay data
    if (query.first())
    {
        do
        {
            id = query.value(0).toInt();
            tenHang = query.value(1).toString();
            soLuong = query.value(2).toInt();
            giaXuat = query.value(3).toInt();
            nguoiXuat = query.value(4).toString();
            thoiGian  = query.value(5).toInt();
            thoiGianStr = QDateTime::fromSecsSinceEpoch(thoiGian).toString("hh:mm:ss \n dd/MM/yyyy");

            recordThongKe_CongNo_LichSuBanHang.setValue("id", id);
            recordThongKe_CongNo_LichSuBanHang.setValue("tenHang", tenHang);
            recordThongKe_CongNo_LichSuBanHang.setValue("soLuong", soLuong);
            recordThongKe_CongNo_LichSuBanHang.setValue("donGia", giaXuat);
            recordThongKe_CongNo_LichSuBanHang.setValue("nguoiXuat", nguoiXuat);
            recordThongKe_CongNo_LichSuBanHang.setValue("thoiGian", thoiGianStr);

            /* Insert to record */
            modelTableTabThongKe_CongNo_LichSuBanHang->insertRecord(-1, recordThongKe_CongNo_LichSuBanHang);
        }while(query.next());
    }
}

void MainWindow::on_tabThongKe_HuHong_clicked()
{
    QString tenHangSearch;
    QSqlQuery query;
    QString tenHang;
    QString note;
    int soLuong;
    int giaNhap;
    int id;
    int ngayNhap;

//    /* Try to remove all current values of model */
//    int tmpCount = modelTableTabThongKe_HuHong->rowCount();
//    for(int idx =0 ; idx < tmpCount; idx++)
//    {
//        modelTableTabThongKe_HuHong->removeRow(0);
//    }

//    switch (thongKeType_DoanhSo)
//    {
//    case THONG_KE_KHOANG_THOIGIAN:
//        fromDateStr = ui->tabThongKe_DoanhSo_LEdit_TuNgay->text();
//        toDateStr   = ui->tabThongKe_DoanhSo_LEdit_DenNgay->text();

//        fromDate = QDateTime::fromString(fromDateStr + "00:00:00", "dd/MM/yyyyhh:mm:ss");
//        toDate   = QDateTime::fromString(toDateStr +"23:59:59", "dd/MM/yyyyhh:mm:ss");

//        fromDateInt = fromDate.toSecsSinceEpoch();
//        toDateInt   = toDate.toSecsSinceEpoch();

//        tempStr = "select id, tenHang, soLuong, giaXuat, thoiGian from huHong where thoiGian >= '" + QString::number(fromDateInt) +"' and thoiGian <= '" + QString::number(toDateInt) + "'";
//        query.prepare( tempStr);

//        if(!query.exec())
//        {
//            qDebug() << query.lastError();
//            return;
//        }

//        if (query.first())
//        {
//            do
//            {
//                id = query.value(0).toInt();
//                tenHang = query.value(1).toString();
//                soLuong  = query.value(2).toInt();
//                giaXuat  = query.value(3).toInt();
//                thoiGian = query.value(4).toInt();
//                thoiGianStr = QDateTime::fromSecsSinceEpoch(thoiGian).toString("hh:mm:ss \n dd/MM/yyyy");

//                recordThongKe_DoanhSo.setValue("id", id);
//                recordThongKe_DoanhSo.setValue("tenHang", tenHang);
//                recordThongKe_DoanhSo.setValue("khachHang", khachHang);
//                recordThongKe_DoanhSo.setValue("soLuong", soLuong);
//                recordThongKe_DoanhSo.setValue("giaXuat", giaXuat);
//                recordThongKe_DoanhSo.setValue("ngayXuat", thoiGianStr);
//                /* Insert to record */
//                modelTableTabThongKe_DoanhSo->insertRecord(-1, recordThongKe_DoanhSo);
//            }while(query.next());
//        }

//        /* show again the instruction to */
//        ui->tabThongKe_DoanhSo_lbl_DenNgay->show();
//        ui->tabThongKe_DoanhSo_lbl_TuNgay->show();
//        ui->tabThongKe_DoanhSo_btn_DenNgay->show();
//        ui->tabThongKe_DoanhSo_btn_TuNgay->show();

//        ui->tabThongKe_DoanhSo_LEdit_DenNgay->hide();
//        ui->tabThongKe_DoanhSo_LEdit_TuNgay->hide();

//    break;
//    case THONG_KE_TENHANG:
//        tenHang = ui->tabThongKe_DoanhSo_cbb_tenHang->currentText();
//        tempStr = "select id, tenHang, khachHang, soLuong, giaXuat, thoiGian from xuatHang where tenHang = '" + tenHang + "'";
//        query.prepare( tempStr);

//        if(!query.exec())
//        {
//            qDebug() << query.lastError();
//            return;
//        }

//        if (query.first())
//        {
//            do
//            {
//                id = query.value(0).toInt();
//                tenHang = query.value(1).toString();
//                khachHang = query.value(2).toString();
//                soLuong  = query.value(3).toInt();
//                giaXuat  = query.value(4).toInt();
//                thoiGian = query.value(5).toInt();
//                thoiGianStr = QDateTime::fromSecsSinceEpoch(thoiGian).toString("hh:mm:ss \n dd/MM/yyyy");

//                recordThongKe_DoanhSo.setValue("id", id);
//                recordThongKe_DoanhSo.setValue("tenHang", tenHang);
//                recordThongKe_DoanhSo.setValue("khachHang", khachHang);
//                recordThongKe_DoanhSo.setValue("soLuong", soLuong);
//                recordThongKe_DoanhSo.setValue("giaXuat", giaXuat);
//                recordThongKe_DoanhSo.setValue("ngayXuat", thoiGianStr);
//                /* Insert to record */
//                modelTableTabThongKe_DoanhSo->insertRecord(-1, recordThongKe_DoanhSo);
//            }while(query.next());
//        }
//        break;
//    }




//   // tenHangSearch = ui->tabThongKe_cb_tenHang->currentText();


//    /* search tenHang trong kho */
//    query.prepare( "select count(*) from huHong" );
//    if(!query.exec())
//    {
//        qDebug() << "can not select khoHang table"<<query.lastError();
//        return;
//    }
//    QString tempStr = "select id, tenHang, soLuong, giaTien, thoiGian from huHong";
//    query.prepare( tempStr);

//    if(!query.exec())
//    {
//        qDebug() << "can not select in huHong table"<<query.lastError();
//        return;
//    }
//    if (query.first())
//    {
//        do
//        {
//            /* Read data from database*/
//            id = query.value(0).toInt();
//            tenHang = query.value(1).toString();
//            soLuong = query.value(2).toInt();
//            giaNhap = query.value(3).toInt();
//            ngayNhap = query.value(4).toInt();
//            QString ngayNhapStr = QDateTime::fromSecsSinceEpoch(ngayNhap).toString("hh:mm:ss dd/MM/yyyy");

//            /* Update the data to the model for showing */
//            recordThongKe_HuHong.setValue("id", id);
//            recordThongKe_HuHong.setValue("tenHang", tenHang);
//            recordThongKe_HuHong.setValue("soLuong", soLuong);
//            recordThongKe_HuHong.setValue("giaTien", giaNhap);
//            recordThongKe_HuHong.setValue("thoiGian", ngayNhapStr);

//            /* Insert to record */
//            modelTableTabThongKe_HuHong->insertRecord(-1, recordThongKe_HuHong);
//        }while(query.next());
//    }
}



void MainWindow::on_radioButton_TenHang_2_clicked()
{
    int tmp_count = modelTableTabThongKe_HuHong->rowCount();
    for(int idx = 0; idx < tmp_count; idx++)
    {
        modelTableTabThongKe_HuHong->removeRow(0);
    }

    /* Hide all thing related to tim theo khoangThoiGian*/
    ui->calendarWidget_HuHong->hide();
    ui->tabThongKe_HuHong_btn_DenNgay->hide();
    ui->tabThongKe_HuHong_btn_TuNgay->hide();
    ui->tabThongKe_HuHong_lbl_DenNgay->hide();
    ui->tabThongKe_HuHong_lbl_TuNgay->hide();
    ui->tabThongKe_HuHong_LEdit_DenNgay->hide();
    ui->tabThongKe_HuHong_LEdit_TuNgay->hide();

    /* show related things */
    ui->tabThongKe_HuHong_cbb_tenHang->show();
    ui->tabThongKe_HuHong_lbl_TenHang->show();

    thongKeType_HuHong = THONG_KE_TENHANG;
}

void MainWindow::on_radioButton_KhoangThoiGian_2_clicked()
{
    int tmp_count = modelTableTabThongKe_HuHong->rowCount();
    for(int idx = 0; idx < tmp_count; idx++)
    {
        modelTableTabThongKe_HuHong->removeRow(0);
    }

    /* Hide all thing related to tim theo TenHang*/
    ui->tabThongKe_HuHong_cbb_tenHang->hide();
    ui->tabThongKe_HuHong_lbl_TenHang->hide();

    /* show related things */
    ui->tabThongKe_HuHong_lbl_DenNgay->show();
    ui->tabThongKe_HuHong_lbl_TuNgay->show();
    ui->tabThongKe_HuHong_btn_DenNgay->show();
    ui->tabThongKe_HuHong_btn_TuNgay->show();

    thongKeType_HuHong = THONG_KE_KHOANG_THOIGIAN;
}
void MainWindow:: tabThongKe_HuHong_InitialShow()
{
    /* Hide calendar component */
    ui->calendarWidget_HuHong->hide();
    dateInteractive_HuHong = -1;

    /* Hide Ten Hang search*/
    ui->tabThongKe_HuHong_lbl_TenHang->hide();
    ui->tabThongKe_HuHong_cbb_tenHang->hide();

    /* Hide Data results*/
    ui->tabThongKe_HuHong_LEdit_DenNgay->hide();
    ui->tabThongKe_HuHong_LEdit_TuNgay->hide();

    /* Set search through "Khoang thoi gian" as default */
     ui->radioButton_KhoangThoiGian_2->setChecked(1);
}

void MainWindow:: tabThongKe_DoanhSo_InitialShow()
{
    /* Hide calendar component */
    ui->calendarWidget_DoanhSo->hide();
    dateInteractive_DoanhSo = -1;

     /* Hide Data results*/
    ui->tabThongKe_DoanhSo_LEdit_DenNgay->hide();
    ui->tabThongKe_DoanhSo_LEdit_TuNgay->hide();

     /* Hide Ten Hang search*/
    ui->tabThongKe_DoanhSo_cbb_tenHang->hide();
    ui->tabThongKe_DoanhSo_lbl_TenHang->hide();

     /* Hide Ten Khach Hang search*/
    ui->tabThongKe_DoanhSo_lbl_TenKhachHang->hide();
    ui->tabThongKe_DoanhSo_cbb_tenKhachHang->hide();

    /* Set search through "Khoang thoi gian" as default */
    ui->radioButton_KhoangThoiGian->setChecked(1);

}

void MainWindow::on_tabThongKe_HuHong_btn_TuNgay_clicked()
{
    ui->calendarWidget_HuHong->show();
    ui->tabThongKe_HuHong_btn_TuNgay->hide();
    dateInteractive_HuHong = DATE_INTERACTIVE_FROM;
}

void MainWindow::on_tabThongKe_HuHong_btn_DenNgay_clicked()
{
    ui->calendarWidget_HuHong->show();
    ui->tabThongKe_HuHong_btn_DenNgay->hide();
    dateInteractive_HuHong = DATE_INTERACTIVE_TO;
}




void MainWindow::on_calendarWidget_DoanhSo_clicked(const QDate &date)
{
    ui->calendarWidget_DoanhSo->hide();
    if(dateInteractive_DoanhSo == DATE_INTERACTIVE_FROM)
    {
        ui->tabThongKe_DoanhSo_btn_TuNgay->hide();
        ui->tabThongKe_DoanhSo_LEdit_TuNgay->show();
        ui->tabThongKe_DoanhSo_LEdit_TuNgay->setText(date.toString("dd/MM/yyyy"));
    }
    else if(dateInteractive_DoanhSo == DATE_INTERACTIVE_TO)
    {
        ui->tabThongKe_DoanhSo_btn_DenNgay->hide();
        ui->tabThongKe_DoanhSo_LEdit_DenNgay->show();
        ui->tabThongKe_DoanhSo_LEdit_DenNgay->setText(date.toString("dd/MM/yyyy"));
    }
}
void MainWindow::on_calendarWidget_HuHong_clicked(const QDate &date)
{
    ui->calendarWidget_HuHong->hide();
    if(dateInteractive_HuHong == DATE_INTERACTIVE_FROM)
    {
        ui->tabThongKe_HuHong_btn_TuNgay->hide();
        ui->tabThongKe_HuHong_LEdit_TuNgay->show();
        ui->tabThongKe_HuHong_LEdit_TuNgay->setText(date.toString("dd/MM/yyyy"));
    }
    else if(dateInteractive_HuHong == DATE_INTERACTIVE_TO)
    {
        ui->tabThongKe_HuHong_btn_DenNgay->hide();
        ui->tabThongKe_HuHong_LEdit_DenNgay->show();
        ui->tabThongKe_HuHong_LEdit_DenNgay->setText(date.toString("dd/MM/yyyy"));
    }
}

void MainWindow::on_tabThongKe_btn_ThongKe_2_clicked()
{
    QString fromDateStr;
    QString toDateStr;
    QDateTime fromDate;
    QDateTime toDate;
    QSqlQuery query;
    qint64 fromDateInt;
    qint64 toDateInt;
    int id;
    QString tenHang;
    int soLuong;
    int giaXuat;
    int giaNhap;
    int thoiGian;
    QString thoiGianStr;
    QString tempStr;
    int ngayNhap;

    /* Try to remove all current values of model */
    int tmpCount = modelTableTabThongKe_HuHong->rowCount();
    for(int idx =0 ; idx < tmpCount; idx++)
    {
        modelTableTabThongKe_HuHong->removeRow(0);
    }

    switch (thongKeType_HuHong)
    {
    case THONG_KE_KHOANG_THOIGIAN:
        fromDateStr = ui->tabThongKe_HuHong_LEdit_TuNgay->text();
        toDateStr   = ui->tabThongKe_HuHong_LEdit_DenNgay->text();

        fromDate = QDateTime::fromString(fromDateStr + "00:00:00", "dd/MM/yyyyhh:mm:ss");
        toDate   = QDateTime::fromString(toDateStr +"23:59:59", "dd/MM/yyyyhh:mm:ss");

        fromDateInt = fromDate.toSecsSinceEpoch();
        toDateInt   = toDate.toSecsSinceEpoch();

        tempStr = "select id, tenHang, soLuong, giaTien, thoiGian from huHong where thoiGian >= '" + QString::number(fromDateInt) +"' and thoiGian <= '" + QString::number(toDateInt) + "'";
        query.prepare( tempStr);

        if(!query.exec())
        {
            qDebug() << query.lastError();
            return;
        }

        if (query.first())
        {
            do
            {
                id = query.value(0).toInt();
                tenHang = query.value(1).toString();
                soLuong  = query.value(2).toInt();
                giaXuat  = query.value(3).toInt();
                thoiGian = query.value(4).toInt();
                thoiGianStr = QDateTime::fromSecsSinceEpoch(thoiGian).toString("hh:mm:ss \n dd/MM/yyyy");

                recordThongKe_HuHong.setValue("id", id);
                recordThongKe_HuHong.setValue("tenHang", tenHang);
                recordThongKe_HuHong.setValue("soLuong", soLuong);
                recordThongKe_HuHong.setValue("giaTien", giaXuat);
                recordThongKe_HuHong.setValue("thoiGian", thoiGianStr);
                /* Insert to record */
                modelTableTabThongKe_HuHong->insertRecord(-1, recordThongKe_HuHong);
            }while(query.next());
        }

        /* show again the instruction to */
        ui->tabThongKe_HuHong_lbl_DenNgay->show();
        ui->tabThongKe_HuHong_lbl_TuNgay->show();
        ui->tabThongKe_HuHong_btn_DenNgay->show();
        ui->tabThongKe_HuHong_btn_TuNgay->show();

        ui->tabThongKe_HuHong_LEdit_DenNgay->hide();
        ui->tabThongKe_HuHong_LEdit_TuNgay->hide();

    break;
    case THONG_KE_TENHANG:
        tenHang = ui->tabThongKe_HuHong_cbb_tenHang->currentText();
        tempStr = "select id, tenHang, soLuong, giaTien, thoiGian from huHong where tenHang = '" + tenHang + "'";
        query.prepare( tempStr);

        if(!query.exec())
        {
            qDebug() << query.lastError();
            return;
        }

        if (query.first())
        {
            do
            {
                id = query.value(0).toInt();
                tenHang = query.value(1).toString();
                soLuong  = query.value(2).toInt();
                giaXuat  = query.value(3).toInt();
                thoiGian = query.value(4).toInt();
                thoiGianStr = QDateTime::fromSecsSinceEpoch(thoiGian).toString("hh:mm:ss \n dd/MM/yyyy");

                recordThongKe_HuHong.setValue("id", id);
                recordThongKe_HuHong.setValue("tenHang", tenHang);
                recordThongKe_HuHong.setValue("soLuong", soLuong);
                recordThongKe_HuHong.setValue("giaTien", giaXuat);
                recordThongKe_HuHong.setValue("thoiGian", thoiGianStr);
                /* Insert to record */
                modelTableTabThongKe_HuHong->insertRecord(-1, recordThongKe_HuHong);
            }while(query.next());
        }
        break;
    }
}
