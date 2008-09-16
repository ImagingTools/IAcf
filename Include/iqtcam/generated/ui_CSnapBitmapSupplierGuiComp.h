/********************************************************************************
** Form generated from reading ui file 'CSnapBitmapSupplierGuiComp.ui'
**
** Created: Tue 16. Sep 12:10:09 2008
**      by: Qt User Interface Compiler version 4.3.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CSNAPBITMAPSUPPLIERGUICOMP_H
#define UI_CSNAPBITMAPSUPPLIERGUICOMP_H

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

class Ui_CSnapBitmapSupplierGuiComp
{
public:
    QVBoxLayout *vboxLayout;
    QGroupBox *groupBox;
    QVBoxLayout *vboxLayout1;
    QPushButton *SnapImageButton;
    QPushButton *SaveImageButton;
    QFrame *ImageParamsLabel;
    QHBoxLayout *hboxLayout;
    QLabel *label;
    QLabel *SizeLabel;
    QGroupBox *ParamsGB;
    QVBoxLayout *vboxLayout2;
    QFrame *ParamsFrame;
    QVBoxLayout *vboxLayout3;
    QPushButton *LoadParamsButton;
    QPushButton *SaveParamsButton;

    void setupUi(QWidget *CSnapBitmapSupplierGuiComp)
    {
    if (CSnapBitmapSupplierGuiComp->objectName().isEmpty())
        CSnapBitmapSupplierGuiComp->setObjectName(QString::fromUtf8("CSnapBitmapSupplierGuiComp"));
    CSnapBitmapSupplierGuiComp->resize(323, 285);
    vboxLayout = new QVBoxLayout(CSnapBitmapSupplierGuiComp);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    vboxLayout->setContentsMargins(0, 0, 0, 0);
    groupBox = new QGroupBox(CSnapBitmapSupplierGuiComp);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
    groupBox->setSizePolicy(sizePolicy);
    vboxLayout1 = new QVBoxLayout(groupBox);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    SnapImageButton = new QPushButton(groupBox);
    SnapImageButton->setObjectName(QString::fromUtf8("SnapImageButton"));
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(SnapImageButton->sizePolicy().hasHeightForWidth());
    SnapImageButton->setSizePolicy(sizePolicy1);

    vboxLayout1->addWidget(SnapImageButton);

    SaveImageButton = new QPushButton(groupBox);
    SaveImageButton->setObjectName(QString::fromUtf8("SaveImageButton"));
    sizePolicy1.setHeightForWidth(SaveImageButton->sizePolicy().hasHeightForWidth());
    SaveImageButton->setSizePolicy(sizePolicy1);

    vboxLayout1->addWidget(SaveImageButton);

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

    SizeLabel = new QLabel(ImageParamsLabel);
    SizeLabel->setObjectName(QString::fromUtf8("SizeLabel"));
    SizeLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    hboxLayout->addWidget(SizeLabel);


    vboxLayout1->addWidget(ImageParamsLabel);


    vboxLayout->addWidget(groupBox);

    ParamsGB = new QGroupBox(CSnapBitmapSupplierGuiComp);
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


    retranslateUi(CSnapBitmapSupplierGuiComp);

    QMetaObject::connectSlotsByName(CSnapBitmapSupplierGuiComp);
    } // setupUi

    void retranslateUi(QWidget *CSnapBitmapSupplierGuiComp)
    {
    CSnapBitmapSupplierGuiComp->setWindowTitle(QApplication::translate("CSnapBitmapSupplierGuiComp", "Form", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("CSnapBitmapSupplierGuiComp", "Image", 0, QApplication::UnicodeUTF8));
    SnapImageButton->setText(QApplication::translate("CSnapBitmapSupplierGuiComp", "Snap", 0, QApplication::UnicodeUTF8));
    SaveImageButton->setText(QApplication::translate("CSnapBitmapSupplierGuiComp", "Save...", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("CSnapBitmapSupplierGuiComp", "Size", 0, QApplication::UnicodeUTF8));
    SizeLabel->setText(QString());
    ParamsGB->setTitle(QApplication::translate("CSnapBitmapSupplierGuiComp", "Parameters", 0, QApplication::UnicodeUTF8));
    LoadParamsButton->setText(QApplication::translate("CSnapBitmapSupplierGuiComp", "Load...", 0, QApplication::UnicodeUTF8));
    SaveParamsButton->setText(QApplication::translate("CSnapBitmapSupplierGuiComp", "Save...", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(CSnapBitmapSupplierGuiComp);
    } // retranslateUi

};

namespace Ui {
    class CSnapBitmapSupplierGuiComp: public Ui_CSnapBitmapSupplierGuiComp {};
} // namespace Ui

#endif // UI_CSNAPBITMAPSUPPLIERGUICOMP_H
