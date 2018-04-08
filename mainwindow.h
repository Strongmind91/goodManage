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
    QSqlTableModel *modelTable;
    QSqlRecord record;
    int currentRow_tabXuatHang_tableView;

private slots:

    void on_tabCongCu_themHang_buttonThem_clicked();

    void on_tabCongCu_donVi_buttonThem_clicked();

    void on_tabCongCu_nguonNhap_buttonThem_clicked();

    void on_tabCongCu_khachMua_buttonThem_clicked();

    void updateComboBox(int);

    void on_tabNhapHang_btn_nhapVaoKho_clicked();

    void on_tabXuatHang_btn_AddToTableView_clicked();

    void on_tabXuatHang_btn_LuuVao_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
