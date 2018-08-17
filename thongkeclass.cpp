#include "thongkeclass.h"
#include "ui_thongkeclass.h"
#include <QLabel>

thongKeClass::thongKeClass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::thongKeClass)
{
    ui->setupUi(this);

}

thongKeClass::~thongKeClass()
{
    delete ui;
}

thongKeClass::drawUiNameGoodSearch()
{
    if (!isHasNameGoodSearch)
    {
        // it is not suitable to draw as not initaliaze
        return;
    }

    // draw the UI Label for this
    nameGood = new QLabel(this);
    nameGood->setGeometry(60, 50, 47, 13);

    // draw the UI commbobox which contains Name of good
    nameGoodCbb = new QComboBox(this);
    nameGoodCbb->setGeometry(160, 40, 91, 22);

    // draw the UI Label for TimKiemTheo
    timKiemTheo = new QLabel("Tìm Kiếm Theo");
    timKiemTheo->setGeometry(30, 30, 71, 16);

}
