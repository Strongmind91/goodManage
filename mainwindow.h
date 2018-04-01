#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSqlDatabase mydb;
    int currentIdx_tenHang;
    int currentIdx_tenDonvi;
    int currentIdx_tenNguonNhap;
    int currentIdx_tenKhachHang;

private slots:

    void on_tabCongCu_themHang_buttonThem_clicked();

    void on_tabCongCu_donVi_buttonThem_clicked();

    void on_tabCongCu_nguonNhap_buttonThem_clicked();

    void on_tabCongCu_khachMua_buttonThem_clicked();

    void on_tabWidget_currentChanged(int index);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
