#ifndef THONGKECLASS_H
#define THONGKECLASS_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

namespace Ui {
class thongKeClass;
}

class thongKeClass : public QWidget
{
    Q_OBJECT

public:
    explicit thongKeClass(QWidget *parent = 0);
    ~thongKeClass();
    void drawUiNameGoodSearch();
    void drawUiDateSearch();
    void drawUiNameCustomerSearch();
    QLabel *nameGood;
    QLabel *timKiemTheo;
    QComboBox *nameGoodCbb;
    QPushButton *buttonSearch;

private:
    Ui::thongKeClass *ui;
    bool isHasNameGoodSearch;
    bool isHasDateSearch;
    bool isHasNameCustomerSearch;
};

#endif // THONGKECLASS_H
