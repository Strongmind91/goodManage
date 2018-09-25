/********************************************************************************
** Form generated from reading UI file 'thongkeclass.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_THONGKECLASS_H
#define UI_THONGKECLASS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_thongKeClass
{
public:

    void setupUi(QWidget *thongKeClass)
    {
        if (thongKeClass->objectName().isEmpty())
            thongKeClass->setObjectName(QStringLiteral("thongKeClass"));
        thongKeClass->resize(400, 300);

        retranslateUi(thongKeClass);

        QMetaObject::connectSlotsByName(thongKeClass);
    } // setupUi

    void retranslateUi(QWidget *thongKeClass)
    {
        thongKeClass->setWindowTitle(QApplication::translate("thongKeClass", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class thongKeClass: public Ui_thongKeClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_THONGKECLASS_H
