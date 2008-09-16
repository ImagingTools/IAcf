/********************************************************************************
** Form generated from reading ui file 'CSamplingParamsGuiComp.ui'
**
** Created: Tue 16. Sep 12:10:09 2008
**      by: Qt User Interface Compiler version 4.3.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CSAMPLINGPARAMSGUICOMP_H
#define UI_CSAMPLINGPARAMSGUICOMP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

class Ui_CSamplingParamsGuiComp
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QDoubleSpinBox *IntervalSB;
    QLabel *label_2;
    QLabel *label_3;
    QComboBox *ModeCB;

    void setupUi(QWidget *CSamplingParamsGuiComp)
    {
    if (CSamplingParamsGuiComp->objectName().isEmpty())
        CSamplingParamsGuiComp->setObjectName(QString::fromUtf8("CSamplingParamsGuiComp"));
    CSamplingParamsGuiComp->resize(220, 70);
    gridLayout = new QGridLayout(CSamplingParamsGuiComp);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);
    label = new QLabel(CSamplingParamsGuiComp);
    label->setObjectName(QString::fromUtf8("label"));

    gridLayout->addWidget(label, 0, 0, 1, 1);

    IntervalSB = new QDoubleSpinBox(CSamplingParamsGuiComp);
    IntervalSB->setObjectName(QString::fromUtf8("IntervalSB"));
    IntervalSB->setDecimals(3);

    gridLayout->addWidget(IntervalSB, 0, 1, 1, 1);

    label_2 = new QLabel(CSamplingParamsGuiComp);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
    label_2->setSizePolicy(sizePolicy);

    gridLayout->addWidget(label_2, 0, 2, 1, 1);

    label_3 = new QLabel(CSamplingParamsGuiComp);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    gridLayout->addWidget(label_3, 1, 0, 1, 1);

    ModeCB = new QComboBox(CSamplingParamsGuiComp);
    ModeCB->setObjectName(QString::fromUtf8("ModeCB"));

    gridLayout->addWidget(ModeCB, 1, 1, 1, 2);


    retranslateUi(CSamplingParamsGuiComp);

    QMetaObject::connectSlotsByName(CSamplingParamsGuiComp);
    } // setupUi

    void retranslateUi(QWidget *CSamplingParamsGuiComp)
    {
    CSamplingParamsGuiComp->setWindowTitle(QApplication::translate("CSamplingParamsGuiComp", "Form", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("CSamplingParamsGuiComp", "Interval", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("CSamplingParamsGuiComp", "ms", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("CSamplingParamsGuiComp", "Mode", 0, QApplication::UnicodeUTF8));
    ModeCB->clear();
    ModeCB->insertItems(0, QStringList()
     << QApplication::translate("CSamplingParamsGuiComp", "Single", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CSamplingParamsGuiComp", "Periodic", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CSamplingParamsGuiComp", "Synchronized", 0, QApplication::UnicodeUTF8)
    );
    Q_UNUSED(CSamplingParamsGuiComp);
    } // retranslateUi

};

namespace Ui {
    class CSamplingParamsGuiComp: public Ui_CSamplingParamsGuiComp {};
} // namespace Ui

#endif // UI_CSAMPLINGPARAMSGUICOMP_H
