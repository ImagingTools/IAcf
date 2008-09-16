/********************************************************************************
** Form generated from reading ui file 'CCaliperParamsGuiComp.ui'
**
** Created: Tue 16. Sep 12:10:12 2008
**      by: Qt User Interface Compiler version 4.3.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CCALIPERPARAMSGUICOMP_H
#define UI_CCALIPERPARAMSGUICOMP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

class Ui_CCaliperParamsGuiComp
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QSpinBox *ThresholdSB;
    QLabel *label_2;
    QLabel *label_3;
    QComboBox *EdgePolarityCB;
    QLabel *label_4;
    QComboBox *DirectionCB;

    void setupUi(QWidget *CCaliperParamsGuiComp)
    {
    if (CCaliperParamsGuiComp->objectName().isEmpty())
        CCaliperParamsGuiComp->setObjectName(QString::fromUtf8("CCaliperParamsGuiComp"));
    CCaliperParamsGuiComp->resize(186, 99);
    gridLayout = new QGridLayout(CCaliperParamsGuiComp);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);
    label = new QLabel(CCaliperParamsGuiComp);
    label->setObjectName(QString::fromUtf8("label"));

    gridLayout->addWidget(label, 0, 0, 1, 1);

    ThresholdSB = new QSpinBox(CCaliperParamsGuiComp);
    ThresholdSB->setObjectName(QString::fromUtf8("ThresholdSB"));

    gridLayout->addWidget(ThresholdSB, 0, 1, 1, 1);

    label_2 = new QLabel(CCaliperParamsGuiComp);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
    label_2->setSizePolicy(sizePolicy);

    gridLayout->addWidget(label_2, 0, 2, 1, 1);

    label_3 = new QLabel(CCaliperParamsGuiComp);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    gridLayout->addWidget(label_3, 1, 0, 1, 1);

    EdgePolarityCB = new QComboBox(CCaliperParamsGuiComp);
    EdgePolarityCB->setObjectName(QString::fromUtf8("EdgePolarityCB"));

    gridLayout->addWidget(EdgePolarityCB, 1, 1, 1, 2);

    label_4 = new QLabel(CCaliperParamsGuiComp);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    gridLayout->addWidget(label_4, 2, 0, 1, 1);

    DirectionCB = new QComboBox(CCaliperParamsGuiComp);
    DirectionCB->setObjectName(QString::fromUtf8("DirectionCB"));

    gridLayout->addWidget(DirectionCB, 2, 1, 1, 2);


    retranslateUi(CCaliperParamsGuiComp);

    QMetaObject::connectSlotsByName(CCaliperParamsGuiComp);
    } // setupUi

    void retranslateUi(QWidget *CCaliperParamsGuiComp)
    {
    CCaliperParamsGuiComp->setWindowTitle(QApplication::translate("CCaliperParamsGuiComp", "Form", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("CCaliperParamsGuiComp", "Threshold", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("CCaliperParamsGuiComp", "%", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("CCaliperParamsGuiComp", "Edge polarity", 0, QApplication::UnicodeUTF8));
    EdgePolarityCB->clear();
    EdgePolarityCB->insertItems(0, QStringList()
     << QApplication::translate("CCaliperParamsGuiComp", "Rise", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CCaliperParamsGuiComp", "Drop", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CCaliperParamsGuiComp", "Any", 0, QApplication::UnicodeUTF8)
    );
    label_4->setText(QApplication::translate("CCaliperParamsGuiComp", "Direction", 0, QApplication::UnicodeUTF8));
    DirectionCB->clear();
    DirectionCB->insertItems(0, QStringList()
     << QApplication::translate("CCaliperParamsGuiComp", "Forwards", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CCaliperParamsGuiComp", "Backwards", 0, QApplication::UnicodeUTF8)
    );
    Q_UNUSED(CCaliperParamsGuiComp);
    } // retranslateUi

};

namespace Ui {
    class CCaliperParamsGuiComp: public Ui_CCaliperParamsGuiComp {};
} // namespace Ui

#endif // UI_CCALIPERPARAMSGUICOMP_H
