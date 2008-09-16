/********************************************************************************
** Form generated from reading ui file 'CValueSupplierGuiComp.ui'
**
** Created: Tue 16. Sep 12:10:12 2008
**      by: Qt User Interface Compiler version 4.3.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CVALUESUPPLIERGUICOMP_H
#define UI_CVALUESUPPLIERGUICOMP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

class Ui_CValueSupplierGuiComp
{
public:
    QVBoxLayout *vboxLayout;
    QGroupBox *groupBox;
    QVBoxLayout *vboxLayout1;
    QPushButton *TestButton;
    QFrame *ImageParamsLabel;
    QHBoxLayout *hboxLayout;
    QLabel *label;
    QLabel *PositionLabel;
    QGroupBox *ParamsGB;
    QVBoxLayout *vboxLayout2;
    QFrame *ParamsFrame;
    QVBoxLayout *vboxLayout3;
    QPushButton *LoadParamsButton;
    QPushButton *SaveParamsButton;

    void setupUi(QWidget *CValueSupplierGuiComp)
    {
    if (CValueSupplierGuiComp->objectName().isEmpty())
        CValueSupplierGuiComp->setObjectName(QString::fromUtf8("CValueSupplierGuiComp"));
    CValueSupplierGuiComp->resize(323, 285);
    vboxLayout = new QVBoxLayout(CValueSupplierGuiComp);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    vboxLayout->setContentsMargins(0, 0, 0, 0);
    groupBox = new QGroupBox(CValueSupplierGuiComp);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
    groupBox->setSizePolicy(sizePolicy);
    vboxLayout1 = new QVBoxLayout(groupBox);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    TestButton = new QPushButton(groupBox);
    TestButton->setObjectName(QString::fromUtf8("TestButton"));
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(TestButton->sizePolicy().hasHeightForWidth());
    TestButton->setSizePolicy(sizePolicy1);

    vboxLayout1->addWidget(TestButton);

    ImageParamsLabel = new QFrame(groupBox);
    ImageParamsLabel->setObjectName(QString::fromUtf8("ImageParamsLabel"));
    ImageParamsLabel->setFrameShape(QFrame::NoFrame);
    ImageParamsLabel->setFrameShadow(QFrame::Raised);
    hboxLayout = new QHBoxLayout(ImageParamsLabel);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    hboxLayout->setContentsMargins(0, 0, 0, 0);
    label = new QLabel(ImageParamsLabel);
    label->setObjectName(QString::fromUtf8("label"));

    hboxLayout->addWidget(label);

    PositionLabel = new QLabel(ImageParamsLabel);
    PositionLabel->setObjectName(QString::fromUtf8("PositionLabel"));
    PositionLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    hboxLayout->addWidget(PositionLabel);


    vboxLayout1->addWidget(ImageParamsLabel);


    vboxLayout->addWidget(groupBox);

    ParamsGB = new QGroupBox(CValueSupplierGuiComp);
    ParamsGB->setObjectName(QString::fromUtf8("ParamsGB"));
    vboxLayout2 = new QVBoxLayout(ParamsGB);
    vboxLayout2->setSpacing(6);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    vboxLayout2->setContentsMargins(9, 9, 9, 9);
    ParamsFrame = new QFrame(ParamsGB);
    ParamsFrame->setObjectName(QString::fromUtf8("ParamsFrame"));
    ParamsFrame->setFrameShape(QFrame::NoFrame);
    ParamsFrame->setFrameShadow(QFrame::Raised);
    vboxLayout3 = new QVBoxLayout(ParamsFrame);
    vboxLayout3->setSpacing(6);
    vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
    vboxLayout3->setContentsMargins(0, 0, 0, 0);

    vboxLayout2->addWidget(ParamsFrame);

    LoadParamsButton = new QPushButton(ParamsGB);
    LoadParamsButton->setObjectName(QString::fromUtf8("LoadParamsButton"));
    sizePolicy1.setHeightForWidth(LoadParamsButton->sizePolicy().hasHeightForWidth());
    LoadParamsButton->setSizePolicy(sizePolicy1);

    vboxLayout2->addWidget(LoadParamsButton);

    SaveParamsButton = new QPushButton(ParamsGB);
    SaveParamsButton->setObjectName(QString::fromUtf8("SaveParamsButton"));
    sizePolicy1.setHeightForWidth(SaveParamsButton->sizePolicy().hasHeightForWidth());
    SaveParamsButton->setSizePolicy(sizePolicy1);

    vboxLayout2->addWidget(SaveParamsButton);


    vboxLayout->addWidget(ParamsGB);


    retranslateUi(CValueSupplierGuiComp);

    QMetaObject::connectSlotsByName(CValueSupplierGuiComp);
    } // setupUi

    void retranslateUi(QWidget *CValueSupplierGuiComp)
    {
    CValueSupplierGuiComp->setWindowTitle(QApplication::translate("CValueSupplierGuiComp", "Form", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("CValueSupplierGuiComp", "Point", 0, QApplication::UnicodeUTF8));
    TestButton->setText(QApplication::translate("CValueSupplierGuiComp", "Test", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("CValueSupplierGuiComp", "Found Position", 0, QApplication::UnicodeUTF8));
    PositionLabel->setText(QString());
    ParamsGB->setTitle(QApplication::translate("CValueSupplierGuiComp", "Parameters", 0, QApplication::UnicodeUTF8));
    LoadParamsButton->setText(QApplication::translate("CValueSupplierGuiComp", "Load...", 0, QApplication::UnicodeUTF8));
    SaveParamsButton->setText(QApplication::translate("CValueSupplierGuiComp", "Save...", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(CValueSupplierGuiComp);
    } // retranslateUi

};

namespace Ui {
    class CValueSupplierGuiComp: public Ui_CValueSupplierGuiComp {};
} // namespace Ui

#endif // UI_CVALUESUPPLIERGUICOMP_H
