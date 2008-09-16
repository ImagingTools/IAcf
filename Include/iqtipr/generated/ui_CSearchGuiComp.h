/********************************************************************************
** Form generated from reading ui file 'CSearchGuiComp.ui'
**
** Created: Tue 16. Sep 12:10:12 2008
**      by: Qt User Interface Compiler version 4.3.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CSEARCHGUICOMP_H
#define UI_CSEARCHGUICOMP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

class Ui_CSearchGuiComp
{
public:
    QVBoxLayout *vboxLayout;
    QFrame *ParamsFrame;
    QVBoxLayout *vboxLayout1;
    QHBoxLayout *hboxLayout;
    QHBoxLayout *hboxLayout1;
    QPushButton *FindButton;
    QPushButton *TeachButton;

    void setupUi(QWidget *CSearchGuiComp)
    {
    if (CSearchGuiComp->objectName().isEmpty())
        CSearchGuiComp->setObjectName(QString::fromUtf8("CSearchGuiComp"));
    CSearchGuiComp->resize(377, 263);
    vboxLayout = new QVBoxLayout(CSearchGuiComp);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    vboxLayout->setContentsMargins(0, 0, 0, 0);
    ParamsFrame = new QFrame(CSearchGuiComp);
    ParamsFrame->setObjectName(QString::fromUtf8("ParamsFrame"));
    ParamsFrame->setFrameShape(QFrame::NoFrame);
    ParamsFrame->setFrameShadow(QFrame::Raised);
    vboxLayout1 = new QVBoxLayout(ParamsFrame);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    vboxLayout1->setContentsMargins(0, 0, 0, 0);
    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));

    vboxLayout1->addLayout(hboxLayout);


    vboxLayout->addWidget(ParamsFrame);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    FindButton = new QPushButton(CSearchGuiComp);
    FindButton->setObjectName(QString::fromUtf8("FindButton"));

    hboxLayout1->addWidget(FindButton);

    TeachButton = new QPushButton(CSearchGuiComp);
    TeachButton->setObjectName(QString::fromUtf8("TeachButton"));

    hboxLayout1->addWidget(TeachButton);


    vboxLayout->addLayout(hboxLayout1);


    retranslateUi(CSearchGuiComp);

    QMetaObject::connectSlotsByName(CSearchGuiComp);
    } // setupUi

    void retranslateUi(QWidget *CSearchGuiComp)
    {
    CSearchGuiComp->setWindowTitle(QApplication::translate("CSearchGuiComp", "Form", 0, QApplication::UnicodeUTF8));
    FindButton->setText(QApplication::translate("CSearchGuiComp", "Find Model", 0, QApplication::UnicodeUTF8));
    TeachButton->setText(QApplication::translate("CSearchGuiComp", "Teach Model...", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(CSearchGuiComp);
    } // retranslateUi

};

namespace Ui {
    class CSearchGuiComp: public Ui_CSearchGuiComp {};
} // namespace Ui

#endif // UI_CSEARCHGUICOMP_H
