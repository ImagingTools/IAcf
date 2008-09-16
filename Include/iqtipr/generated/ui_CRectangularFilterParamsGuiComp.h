/********************************************************************************
** Form generated from reading ui file 'CRectangularFilterParamsGuiComp.ui'
**
** Created: Tue 16. Sep 12:10:12 2008
**      by: Qt User Interface Compiler version 4.3.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CRECTANGULARFILTERPARAMSGUICOMP_H
#define UI_CRECTANGULARFILTERPARAMSGUICOMP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#include <QtGui/QWidget>

class Ui_CRectangularFilterParamsGuiComp
{
public:
    QGridLayout *gridLayout;
    QLabel *FilterWidthLabel;
    QSlider *FilterWidthSlider;
    QLabel *FilterHeightLabel;
    QSlider *FilterHeightSlider;
    QLabel *FilterWidthValueLabel;
    QLabel *FilterHeightValueLabel;

    void setupUi(QWidget *CRectangularFilterParamsGuiComp)
    {
    if (CRectangularFilterParamsGuiComp->objectName().isEmpty())
        CRectangularFilterParamsGuiComp->setObjectName(QString::fromUtf8("CRectangularFilterParamsGuiComp"));
    CRectangularFilterParamsGuiComp->resize(473, 70);
    gridLayout = new QGridLayout(CRectangularFilterParamsGuiComp);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);
    FilterWidthLabel = new QLabel(CRectangularFilterParamsGuiComp);
    FilterWidthLabel->setObjectName(QString::fromUtf8("FilterWidthLabel"));

    gridLayout->addWidget(FilterWidthLabel, 0, 0, 1, 1);

    FilterWidthSlider = new QSlider(CRectangularFilterParamsGuiComp);
    FilterWidthSlider->setObjectName(QString::fromUtf8("FilterWidthSlider"));
    FilterWidthSlider->setMinimum(0);
    FilterWidthSlider->setMaximum(50);
    FilterWidthSlider->setOrientation(Qt::Horizontal);

    gridLayout->addWidget(FilterWidthSlider, 0, 1, 1, 1);

    FilterHeightLabel = new QLabel(CRectangularFilterParamsGuiComp);
    FilterHeightLabel->setObjectName(QString::fromUtf8("FilterHeightLabel"));

    gridLayout->addWidget(FilterHeightLabel, 1, 0, 1, 1);

    FilterHeightSlider = new QSlider(CRectangularFilterParamsGuiComp);
    FilterHeightSlider->setObjectName(QString::fromUtf8("FilterHeightSlider"));
    FilterHeightSlider->setMinimum(0);
    FilterHeightSlider->setMaximum(50);
    FilterHeightSlider->setOrientation(Qt::Horizontal);

    gridLayout->addWidget(FilterHeightSlider, 1, 1, 1, 1);

    FilterWidthValueLabel = new QLabel(CRectangularFilterParamsGuiComp);
    FilterWidthValueLabel->setObjectName(QString::fromUtf8("FilterWidthValueLabel"));

    gridLayout->addWidget(FilterWidthValueLabel, 0, 2, 1, 1);

    FilterHeightValueLabel = new QLabel(CRectangularFilterParamsGuiComp);
    FilterHeightValueLabel->setObjectName(QString::fromUtf8("FilterHeightValueLabel"));

    gridLayout->addWidget(FilterHeightValueLabel, 1, 2, 1, 1);


    retranslateUi(CRectangularFilterParamsGuiComp);

    QMetaObject::connectSlotsByName(CRectangularFilterParamsGuiComp);
    } // setupUi

    void retranslateUi(QWidget *CRectangularFilterParamsGuiComp)
    {
    CRectangularFilterParamsGuiComp->setWindowTitle(QApplication::translate("CRectangularFilterParamsGuiComp", "Form", 0, QApplication::UnicodeUTF8));
    FilterWidthLabel->setText(QApplication::translate("CRectangularFilterParamsGuiComp", "Filter Width", 0, QApplication::UnicodeUTF8));
    FilterHeightLabel->setText(QApplication::translate("CRectangularFilterParamsGuiComp", "Filter Height", 0, QApplication::UnicodeUTF8));
    FilterWidthValueLabel->setText(QApplication::translate("CRectangularFilterParamsGuiComp", "1", 0, QApplication::UnicodeUTF8));
    FilterHeightValueLabel->setText(QApplication::translate("CRectangularFilterParamsGuiComp", "1", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(CRectangularFilterParamsGuiComp);
    } // retranslateUi

};

namespace Ui {
    class CRectangularFilterParamsGuiComp: public Ui_CRectangularFilterParamsGuiComp {};
} // namespace Ui

#endif // UI_CRECTANGULARFILTERPARAMSGUICOMP_H
