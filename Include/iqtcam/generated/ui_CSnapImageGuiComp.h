/********************************************************************************
** Form generated from reading ui file 'CSnapImageGuiComp.ui'
**
** Created: Tue 16. Sep 12:10:09 2008
**      by: Qt User Interface Compiler version 4.3.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CSNAPIMAGEGUICOMP_H
#define UI_CSNAPIMAGEGUICOMP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

class Ui_CSnapImageGuiComp
{
public:
    QVBoxLayout *vboxLayout;
    QFrame *frame;
    QVBoxLayout *vboxLayout1;
    QGroupBox *groupBox;
    QVBoxLayout *vboxLayout2;
    QHBoxLayout *hboxLayout;
    QPushButton *SnapImageButton;
    QToolButton *LiveImageButton;
    QPushButton *SaveImageButton;
    QGroupBox *ParamsGB;
    QVBoxLayout *vboxLayout3;
    QFrame *ParamsFrame;
    QVBoxLayout *vboxLayout4;
    QPushButton *LoadParamsButton;
    QPushButton *SaveParamsButton;

    void setupUi(QWidget *CSnapImageGuiComp)
    {
    if (CSnapImageGuiComp->objectName().isEmpty())
        CSnapImageGuiComp->setObjectName(QString::fromUtf8("CSnapImageGuiComp"));
    CSnapImageGuiComp->resize(323, 239);
    vboxLayout = new QVBoxLayout(CSnapImageGuiComp);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    frame = new QFrame(CSnapImageGuiComp);
    frame->setObjectName(QString::fromUtf8("frame"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(1);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
    frame->setSizePolicy(sizePolicy);
    frame->setFrameShape(QFrame::NoFrame);
    frame->setFrameShadow(QFrame::Raised);
    vboxLayout1 = new QVBoxLayout(frame);
    vboxLayout1->setSpacing(6);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    vboxLayout1->setContentsMargins(0, 0, 0, 0);
    groupBox = new QGroupBox(frame);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Maximum);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
    groupBox->setSizePolicy(sizePolicy1);
    vboxLayout2 = new QVBoxLayout(groupBox);
    vboxLayout2->setSpacing(6);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    vboxLayout2->setContentsMargins(9, 9, 9, 9);
    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    hboxLayout->setContentsMargins(0, 0, 0, 0);
    SnapImageButton = new QPushButton(groupBox);
    SnapImageButton->setObjectName(QString::fromUtf8("SnapImageButton"));
    QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(SnapImageButton->sizePolicy().hasHeightForWidth());
    SnapImageButton->setSizePolicy(sizePolicy2);

    hboxLayout->addWidget(SnapImageButton);

    LiveImageButton = new QToolButton(groupBox);
    LiveImageButton->setObjectName(QString::fromUtf8("LiveImageButton"));
    QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Preferred);
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(LiveImageButton->sizePolicy().hasHeightForWidth());
    LiveImageButton->setSizePolicy(sizePolicy3);
    LiveImageButton->setIcon(QIcon());
    LiveImageButton->setCheckable(true);

    hboxLayout->addWidget(LiveImageButton);


    vboxLayout2->addLayout(hboxLayout);

    SaveImageButton = new QPushButton(groupBox);
    SaveImageButton->setObjectName(QString::fromUtf8("SaveImageButton"));
    sizePolicy2.setHeightForWidth(SaveImageButton->sizePolicy().hasHeightForWidth());
    SaveImageButton->setSizePolicy(sizePolicy2);

    vboxLayout2->addWidget(SaveImageButton);


    vboxLayout1->addWidget(groupBox);

    ParamsGB = new QGroupBox(frame);
    ParamsGB->setObjectName(QString::fromUtf8("ParamsGB"));
    vboxLayout3 = new QVBoxLayout(ParamsGB);
    vboxLayout3->setSpacing(6);
    vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
    vboxLayout3->setContentsMargins(9, 9, 9, 9);
    ParamsFrame = new QFrame(ParamsGB);
    ParamsFrame->setObjectName(QString::fromUtf8("ParamsFrame"));
    ParamsFrame->setFrameShape(QFrame::NoFrame);
    ParamsFrame->setFrameShadow(QFrame::Raised);
    vboxLayout4 = new QVBoxLayout(ParamsFrame);
    vboxLayout4->setSpacing(6);
    vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
    vboxLayout4->setContentsMargins(0, 0, 0, 0);

    vboxLayout3->addWidget(ParamsFrame);

    LoadParamsButton = new QPushButton(ParamsGB);
    LoadParamsButton->setObjectName(QString::fromUtf8("LoadParamsButton"));
    sizePolicy2.setHeightForWidth(LoadParamsButton->sizePolicy().hasHeightForWidth());
    LoadParamsButton->setSizePolicy(sizePolicy2);

    vboxLayout3->addWidget(LoadParamsButton);

    SaveParamsButton = new QPushButton(ParamsGB);
    SaveParamsButton->setObjectName(QString::fromUtf8("SaveParamsButton"));
    sizePolicy2.setHeightForWidth(SaveParamsButton->sizePolicy().hasHeightForWidth());
    SaveParamsButton->setSizePolicy(sizePolicy2);

    vboxLayout3->addWidget(SaveParamsButton);


    vboxLayout1->addWidget(ParamsGB);


    vboxLayout->addWidget(frame);


    retranslateUi(CSnapImageGuiComp);

    QMetaObject::connectSlotsByName(CSnapImageGuiComp);
    } // setupUi

    void retranslateUi(QWidget *CSnapImageGuiComp)
    {
    CSnapImageGuiComp->setWindowTitle(QApplication::translate("CSnapImageGuiComp", "Form", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("CSnapImageGuiComp", "Image", 0, QApplication::UnicodeUTF8));
    SnapImageButton->setText(QApplication::translate("CSnapImageGuiComp", "Snap", 0, QApplication::UnicodeUTF8));
    LiveImageButton->setText(QApplication::translate("CSnapImageGuiComp", "...", 0, QApplication::UnicodeUTF8));
    SaveImageButton->setText(QApplication::translate("CSnapImageGuiComp", "Save...", 0, QApplication::UnicodeUTF8));
    ParamsGB->setTitle(QApplication::translate("CSnapImageGuiComp", "Parameters", 0, QApplication::UnicodeUTF8));
    LoadParamsButton->setText(QApplication::translate("CSnapImageGuiComp", "Load...", 0, QApplication::UnicodeUTF8));
    SaveParamsButton->setText(QApplication::translate("CSnapImageGuiComp", "Save...", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(CSnapImageGuiComp);
    } // retranslateUi

};

namespace Ui {
    class CSnapImageGuiComp: public Ui_CSnapImageGuiComp {};
} // namespace Ui

#endif // UI_CSNAPIMAGEGUICOMP_H
