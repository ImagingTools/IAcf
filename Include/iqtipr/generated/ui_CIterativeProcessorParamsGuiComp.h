/********************************************************************************
** Form generated from reading ui file 'CIterativeProcessorParamsGuiComp.ui'
**
** Created: Tue 16. Sep 12:10:12 2008
**      by: Qt User Interface Compiler version 4.3.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CITERATIVEPROCESSORPARAMSGUICOMP_H
#define UI_CITERATIVEPROCESSORPARAMSGUICOMP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

class Ui_CIterativeProcessorParamsGuiComp
{
public:
    QGridLayout *gridLayout;
    QLabel *label_2;
    QSlider *IterationsSlider;
    QSpinBox *IterationsSpin;

    void setupUi(QWidget *CIterativeProcessorParamsGuiComp)
    {
    if (CIterativeProcessorParamsGuiComp->objectName().isEmpty())
        CIterativeProcessorParamsGuiComp->setObjectName(QString::fromUtf8("CIterativeProcessorParamsGuiComp"));
    CIterativeProcessorParamsGuiComp->resize(473, 25);
    gridLayout = new QGridLayout(CIterativeProcessorParamsGuiComp);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);
    label_2 = new QLabel(CIterativeProcessorParamsGuiComp);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    gridLayout->addWidget(label_2, 0, 0, 1, 1);

    IterationsSlider = new QSlider(CIterativeProcessorParamsGuiComp);
    IterationsSlider->setObjectName(QString::fromUtf8("IterationsSlider"));
    IterationsSlider->setMinimum(1);
    IterationsSlider->setMaximum(10);
    IterationsSlider->setPageStep(100);
    IterationsSlider->setOrientation(Qt::Horizontal);

    gridLayout->addWidget(IterationsSlider, 0, 1, 1, 1);

    IterationsSpin = new QSpinBox(CIterativeProcessorParamsGuiComp);
    IterationsSpin->setObjectName(QString::fromUtf8("IterationsSpin"));
    IterationsSpin->setMinimum(1);
    IterationsSpin->setMaximum(1000);
    IterationsSpin->setSingleStep(1);

    gridLayout->addWidget(IterationsSpin, 0, 2, 1, 1);


    retranslateUi(CIterativeProcessorParamsGuiComp);
    QObject::connect(IterationsSlider, SIGNAL(valueChanged(int)), IterationsSpin, SLOT(setValue(int)));
    QObject::connect(IterationsSpin, SIGNAL(valueChanged(int)), IterationsSlider, SLOT(setValue(int)));

    QMetaObject::connectSlotsByName(CIterativeProcessorParamsGuiComp);
    } // setupUi

    void retranslateUi(QWidget *CIterativeProcessorParamsGuiComp)
    {
    CIterativeProcessorParamsGuiComp->setWindowTitle(QApplication::translate("CIterativeProcessorParamsGuiComp", "Form", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("CIterativeProcessorParamsGuiComp", "Iterations", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(CIterativeProcessorParamsGuiComp);
    } // retranslateUi

};

namespace Ui {
    class CIterativeProcessorParamsGuiComp: public Ui_CIterativeProcessorParamsGuiComp {};
} // namespace Ui

#endif // UI_CITERATIVEPROCESSORPARAMSGUICOMP_H
