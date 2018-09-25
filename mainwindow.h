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
    QSqlTableModel *modelTableTabThongKe_TimHang;
    QSqlTableModel *modelTableTabThongKe_DoanhSo;
    QSqlTableModel *modelTableTabThongKe_CongNo;
    QSqlTableModel *modelTableTabThongKe_CongNo_LichSuThu;
    QSqlTableModel *modelTableTabThongKe_CongNo_LichSuBanHang;
    QSqlTableModel *modelTableTabThongKe_HuHong;
    QSqlRecord record;
    QSqlRecord recordThongKe_TimHang;
    QSqlRecord recordThongKe_DoanhSo;
    QSqlRecord recordThongKe_CongNo;
    QSqlRecord recordThongKe_CongNo_LichSuThu;
    QSqlRecord recordThongKe_CongNo_LichSuBanHang;
    QSqlRecord recordThongKe_HuHong;
    int currentRow_tabXuatHang_tableView;
    QString currentKhachHang;
    int dateInteractive_DoanhSo;
    int dateInteractive_HuHong;
    int thongKeType_DoanhSo;
    int thongKeType_HuHong;

private slots:

    void on_tabCongCu_themHang_buttonThem_clicked();

    void on_tabCongCu_donVi_buttonThem_clicked();

    void on_tabCongCu_nguonNhap_buttonThem_clicked();

    void on_tabCongCu_khachMua_buttonThem_clicked();

    void updateComboBox(int);

    void on_tabNhapHang_btn_nhapVaoKho_clicked();

    void on_tabXuatHang_btn_AddToTableView_clicked();

    void on_tabXuatHang_btn_LuuVao_clicked();

    void on_tabXuatHang_btn_DelToTableView_clicked();

    void on_tabHuHong_btn_nhapVao_clicked();

    void on_tabThuTien_btn_nhapVao_clicked();

    void on_tabThongKe_btn_timTrongKho_clicked();


    void on_tabThongKe_tabWidget_currentChanged(int index);

    void on_tabThongKe_DoanhSo_btn_TuNgay_clicked();

    void on_tabThongKe_DoanhSo_btn_DenNgay_clicked();

    void on_radioButton_KhoangThoiGian_clicked();

    void on_radioButton_khachHang_clicked();

    void on_radioButton_TenHang_clicked();


    void on_tabThongKe_btn_ThongKe_clicked();

    void on_tabThongKe_CongNo_btn_timKiem_clicked();

    void on_tabThongKe_CongNo_btn_LichSuThu_clicked();

    void on_tabThongKe_CongNo_btn_LichSuBanHang_clicked();

    void on_tabThongKe_HuHong_clicked();

    void on_radioButton_KhoangThoiGian_2_clicked();

    void on_radioButton_TenHang_2_clicked();

    void tabThongKe_HuHong_InitialShow();
    void tabThongKe_DoanhSo_InitialShow();

    void on_tabThongKe_HuHong_btn_TuNgay_clicked();

    void on_tabThongKe_HuHong_btn_DenNgay_clicked();


    void on_calendarWidget_DoanhSo_clicked(const QDate &date);

    void on_calendarWidget_HuHong_clicked(const QDate &date);

    void on_tabThongKe_btn_ThongKe_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
