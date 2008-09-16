/********************************************************************************
** Form generated from reading ui file 'CExposureParamsGuiComp.ui'
**
** Created: Tue 16. Sep 12:10:09 2008
**      by: Qt User Interface Compiler version 4.3.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CEXPOSUREPARAMSGUICOMP_H
#define UI_CEXPOSUREPARAMSGUICOMP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

class Ui_CExposureParamsGuiComp
{
public:
    QGridLayout *gridLayout;
    QLabel *ShutterTimeLabel;
    QDoubleSpinBox *ShutterTimeSB;
    QLabel *ShutterTimeUnitLabel;
    QLabel *DelayTimeLabel;
    QDoubleSpinBox *DelayTimeSB;
    QLabel *DelayTimeUnitLabel;
    QLabel *EenDelayTimeLabel;
    QDoubleSpinBox *EenDelayTimeSB;
    QLabel *EenDelayTimeUnitLabel;

    void setupUi(QWidget *CExposureParamsGuiComp)
    {
    if (CExposureParamsGuiComp->objectName().isEmpty())
        CExposureParamsGuiComp->setObjectName(QString::fromUtf8("CExposureParamsGuiComp"));
    CExposureParamsGuiComp->resize(187, 99);
    gridLayout = new QGridLayout(CExposureParamsGuiComp);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    ShutterTimeLabel = new QLabel(CExposureParamsGuiComp);
    ShutterTimeLabel->setObjectName(QString::fromUtf8("ShutterTimeLabel"));

    gridLayout->addWidget(ShutterTimeLabel, 0, 0, 1, 1);

    ShutterTimeSB = new QDoubleSpinBox(CExposureParamsGuiComp);
    ShutterTimeSB->setObjectName(QString::fromUtf8("ShutterTimeSB"));
    ShutterTimeSB->setMaximum(1000);

    gridLayout->addWidget(ShutterTimeSB, 0, 1, 1, 1);

    ShutterTimeUnitLabel = new QLabel(CExposureParamsGuiComp);
    ShutterTimeUnitLabel->setObjectName(QString::fromUtf8("ShutterTimeUnitLabel"));
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(ShutterTimeUnitLabel->sizePolicy().hasHeightForWidth());
    ShutterTimeUnitLabel->setSizePolicy(sizePolicy);

    gridLayout->addWidget(ShutterTimeUnitLabel, 0, 2, 1, 1);

    DelayTimeLabel = new QLabel(CExposureParamsGuiComp);
    DelayTimeLabel->setObjectName(QString::fromUtf8("DelayTimeLabel"));

    gridLayout->addWidget(DelayTimeLabel, 1, 0, 1, 1);

    DelayTimeSB = new QDoubleSpinBox(CExposureParamsGuiComp);
    DelayTimeSB->setObjectName(QString::fromUtf8("DelayTimeSB"));
    DelayTimeSB->setMaximum(1000);

    gridLayout->addWidget(DelayTimeSB, 1, 1, 1, 1);

    DelayTimeUnitLabel = new QLabel(CExposureParamsGuiComp);
    DelayTimeUnitLabel->setObjectName(QString::fromUtf8("DelayTimeUnitLabel"));
    sizePolicy.setHeightForWidth(DelayTimeUnitLabel->sizePolicy().hasHeightForWidth());
    DelayTimeUnitLabel->setSizePolicy(sizePolicy);

    gridLayout->addWidget(DelayTimeUnitLabel, 1, 2, 1, 1);

    EenDelayTimeLabel = new QLabel(CExposureParamsGuiComp);
    EenDelayTimeLabel->setObjectName(QString::fromUtf8("EenDelayTimeLabel"));

    gridLayout->addWidget(EenDelayTimeLabel, 2, 0, 1, 1);

    EenDelayTimeSB = new QDoubleSpinBox(CExposureParamsGuiComp);
    EenDelayTimeSB->setObjectName(QString::fromUtf8("EenDelayTimeSB"));
    EenDelayTimeSB->setMaximum(1000);

    gridLayout->addWidget(EenDelayTimeSB, 2, 1, 1, 1);

    EenDelayTimeUnitLabel = new QLabel(CExposureParamsGuiComp);
    EenDelayTimeUnitLabel->setObjectName(QString::fromUtf8("EenDelayTimeUnitLabel"));
    sizePolicy.setHeightForWidth(EenDelayTimeUnitLabel->sizePolicy().hasHeightForWidth());
    EenDelayTimeUnitLabel->setSizePolicy(sizePolicy);

    gridLayout->addWidget(EenDelayTimeUnitLabel, 2, 2, 1, 1);


    retranslateUi(CExposureParamsGuiComp);

    QMetaObject::connectSlotsByName(CExposureParamsGuiComp);
    } // setupUi

    void retranslateUi(QWidget *CExposureParamsGuiComp)
    {
    CExposureParamsGuiComp->setWindowTitle(QApplication::translate("CExposureParamsGuiComp", "Form", 0, QApplication::UnicodeUTF8));
    ShutterTimeLabel->setText(QApplication::translate("CExposureParamsGuiComp", "Shutter time", 0, QApplication::UnicodeUTF8));
    ShutterTimeUnitLabel->setText(QApplication::translate("CExposureParamsGuiComp", "ms", 0, QApplication::UnicodeUTF8));
    DelayTimeLabel->setText(QApplication::translate("CExposureParamsGuiComp", "Delay Time", 0, QApplication::UnicodeUTF8));
    DelayTimeUnitLabel->setText(QApplication::translate("CExposureParamsGuiComp", "ms", 0, QApplication::UnicodeUTF8));
    EenDelayTimeLabel->setText(QApplication::translate("CExposureParamsGuiComp", "EEN Delay", 0, QApplication::UnicodeUTF8));
    EenDelayTimeUnitLabel->setText(QApplication::translate("CExposureParamsGuiComp", "ms", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(CExposureParamsGuiComp);
    } // retranslateUi

};

namespace Ui {
    class CExposureParamsGuiComp: public Ui_CExposureParamsGuiComp {};
} // namespace Ui

#endif // UI_CEXPOSUREPARAMSGUICOMP_H
