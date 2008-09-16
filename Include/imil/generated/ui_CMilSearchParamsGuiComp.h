/********************************************************************************
** Form generated from reading ui file 'CMilSearchParamsGuiComp.ui'
**
** Created: Tue 16. Sep 12:10:13 2008
**      by: Qt User Interface Compiler version 4.3.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CMILSEARCHPARAMSGUICOMP_H
#define UI_CMILSEARCHPARAMSGUICOMP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

class Ui_CMilSearchParamsGuiComp
{
public:
    QGridLayout *gridLayout;
    QGridLayout *gridLayout1;
    QLabel *label_9;
    QSpinBox *m_spinFirstLevel;
    QLabel *label_10;
    QSpinBox *m_spinSecondLevel;
    QGridLayout *gridLayout2;
    QLabel *label_7;
    QComboBox *m_comboSpeed;
    QLabel *label_8;
    QComboBox *m_comboAccuracy;
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QVBoxLayout *vboxLayout1;
    QComboBox *m_comboPolarity;
    QSpinBox *m_spinAcceptanceTarget;
    QSpinBox *m_spinCertainty;
    QSpinBox *m_spinCertaintyTarget;
    QSpinBox *m_spinFitErrorWeight;
    QVBoxLayout *vboxLayout2;
    QLabel *label_17;
    QLabel *label_24;
    QLabel *label_18;
    QLabel *label_19;
    QLabel *label_20;
    QLabel *label_21;
    QVBoxLayout *vboxLayout3;
    QDoubleSpinBox *m_spinNominalScale;
    QSpinBox *m_spinNominalAngle;
    QComboBox *m_comboDetailLevel;
    QSpinBox *m_spinSmoothness;
    QComboBox *m_comboSharedEdges;
    QComboBox *m_comboTargetCaching;
    QHBoxLayout *hboxLayout1;
    QLabel *label_22;
    QComboBox *m_comboFilterType;
    QLabel *m_labelKernelSize;
    QSpinBox *m_spinKernelSize;
    QSpacerItem *spacerItem;
    QLabel *label_11;
    QSpinBox *m_spinTimeout;

    void setupUi(QWidget *CMilSearchParamsGuiComp)
    {
    if (CMilSearchParamsGuiComp->objectName().isEmpty())
        CMilSearchParamsGuiComp->setObjectName(QString::fromUtf8("CMilSearchParamsGuiComp"));
    CMilSearchParamsGuiComp->resize(355, 236);
    gridLayout = new QGridLayout(CMilSearchParamsGuiComp);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout1 = new QGridLayout();
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    label_9 = new QLabel(CMilSearchParamsGuiComp);
    label_9->setObjectName(QString::fromUtf8("label_9"));

    gridLayout1->addWidget(label_9, 0, 0, 1, 1);

    m_spinFirstLevel = new QSpinBox(CMilSearchParamsGuiComp);
    m_spinFirstLevel->setObjectName(QString::fromUtf8("m_spinFirstLevel"));
    m_spinFirstLevel->setMaximum(7);

    gridLayout1->addWidget(m_spinFirstLevel, 0, 1, 1, 1);

    label_10 = new QLabel(CMilSearchParamsGuiComp);
    label_10->setObjectName(QString::fromUtf8("label_10"));

    gridLayout1->addWidget(label_10, 1, 0, 1, 1);

    m_spinSecondLevel = new QSpinBox(CMilSearchParamsGuiComp);
    m_spinSecondLevel->setObjectName(QString::fromUtf8("m_spinSecondLevel"));
    m_spinSecondLevel->setMaximum(7);

    gridLayout1->addWidget(m_spinSecondLevel, 1, 1, 1, 1);


    gridLayout->addLayout(gridLayout1, 0, 0, 1, 1);

    gridLayout2 = new QGridLayout();
    gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
    label_7 = new QLabel(CMilSearchParamsGuiComp);
    label_7->setObjectName(QString::fromUtf8("label_7"));

    gridLayout2->addWidget(label_7, 0, 0, 1, 1);

    m_comboSpeed = new QComboBox(CMilSearchParamsGuiComp);
    m_comboSpeed->setObjectName(QString::fromUtf8("m_comboSpeed"));
    m_comboSpeed->setEnabled(true);

    gridLayout2->addWidget(m_comboSpeed, 0, 1, 1, 1);

    label_8 = new QLabel(CMilSearchParamsGuiComp);
    label_8->setObjectName(QString::fromUtf8("label_8"));

    gridLayout2->addWidget(label_8, 1, 0, 1, 1);

    m_comboAccuracy = new QComboBox(CMilSearchParamsGuiComp);
    m_comboAccuracy->setObjectName(QString::fromUtf8("m_comboAccuracy"));
    m_comboAccuracy->setEnabled(true);

    gridLayout2->addWidget(m_comboAccuracy, 1, 1, 1, 1);


    gridLayout->addLayout(gridLayout2, 0, 1, 1, 1);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    hboxLayout->setContentsMargins(0, 0, 0, 0);
    vboxLayout = new QVBoxLayout();
    vboxLayout->setSpacing(6);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    vboxLayout->setContentsMargins(0, 0, 0, 0);
    label_12 = new QLabel(CMilSearchParamsGuiComp);
    label_12->setObjectName(QString::fromUtf8("label_12"));

    vboxLayout->addWidget(label_12);

    label_13 = new QLabel(CMilSearchParamsGuiComp);
    label_13->setObjectName(QString::fromUtf8("label_13"));

    vboxLayout->addWidget(label_13);

    label_14 = new QLabel(CMilSearchParamsGuiComp);
    label_14->setObjectName(QString::fromUtf8("label_14"));

    vboxLayout->addWidget(label_14);

    label_15 = new QLabel(CMilSearchParamsGuiComp);
    label_15->setObjectName(QString::fromUtf8("label_15"));

    vboxLayout->addWidget(label_15);

    label_16 = new QLabel(CMilSearchParamsGuiComp);
    label_16->setObjectName(QString::fromUtf8("label_16"));

    vboxLayout->addWidget(label_16);


    hboxLayout->addLayout(vboxLayout);

    vboxLayout1 = new QVBoxLayout();
    vboxLayout1->setSpacing(6);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    vboxLayout1->setContentsMargins(0, 0, 0, 0);
    m_comboPolarity = new QComboBox(CMilSearchParamsGuiComp);
    m_comboPolarity->setObjectName(QString::fromUtf8("m_comboPolarity"));

    vboxLayout1->addWidget(m_comboPolarity);

    m_spinAcceptanceTarget = new QSpinBox(CMilSearchParamsGuiComp);
    m_spinAcceptanceTarget->setObjectName(QString::fromUtf8("m_spinAcceptanceTarget"));
    m_spinAcceptanceTarget->setMaximum(100);
    m_spinAcceptanceTarget->setSingleStep(10);
    m_spinAcceptanceTarget->setValue(0);

    vboxLayout1->addWidget(m_spinAcceptanceTarget);

    m_spinCertainty = new QSpinBox(CMilSearchParamsGuiComp);
    m_spinCertainty->setObjectName(QString::fromUtf8("m_spinCertainty"));
    m_spinCertainty->setMaximum(100);
    m_spinCertainty->setValue(90);

    vboxLayout1->addWidget(m_spinCertainty);

    m_spinCertaintyTarget = new QSpinBox(CMilSearchParamsGuiComp);
    m_spinCertaintyTarget->setObjectName(QString::fromUtf8("m_spinCertaintyTarget"));
    m_spinCertaintyTarget->setMaximum(100);
    m_spinCertaintyTarget->setValue(0);

    vboxLayout1->addWidget(m_spinCertaintyTarget);

    m_spinFitErrorWeight = new QSpinBox(CMilSearchParamsGuiComp);
    m_spinFitErrorWeight->setObjectName(QString::fromUtf8("m_spinFitErrorWeight"));
    m_spinFitErrorWeight->setMaximum(100);
    m_spinFitErrorWeight->setSingleStep(5);
    m_spinFitErrorWeight->setValue(25);

    vboxLayout1->addWidget(m_spinFitErrorWeight);


    hboxLayout->addLayout(vboxLayout1);

    vboxLayout2 = new QVBoxLayout();
    vboxLayout2->setSpacing(6);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    vboxLayout2->setContentsMargins(0, 0, 0, 0);
    label_17 = new QLabel(CMilSearchParamsGuiComp);
    label_17->setObjectName(QString::fromUtf8("label_17"));

    vboxLayout2->addWidget(label_17);

    label_24 = new QLabel(CMilSearchParamsGuiComp);
    label_24->setObjectName(QString::fromUtf8("label_24"));

    vboxLayout2->addWidget(label_24);

    label_18 = new QLabel(CMilSearchParamsGuiComp);
    label_18->setObjectName(QString::fromUtf8("label_18"));

    vboxLayout2->addWidget(label_18);

    label_19 = new QLabel(CMilSearchParamsGuiComp);
    label_19->setObjectName(QString::fromUtf8("label_19"));

    vboxLayout2->addWidget(label_19);

    label_20 = new QLabel(CMilSearchParamsGuiComp);
    label_20->setObjectName(QString::fromUtf8("label_20"));

    vboxLayout2->addWidget(label_20);

    label_21 = new QLabel(CMilSearchParamsGuiComp);
    label_21->setObjectName(QString::fromUtf8("label_21"));

    vboxLayout2->addWidget(label_21);


    hboxLayout->addLayout(vboxLayout2);

    vboxLayout3 = new QVBoxLayout();
    vboxLayout3->setSpacing(6);
    vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
    vboxLayout3->setContentsMargins(0, 0, 0, 0);
    m_spinNominalScale = new QDoubleSpinBox(CMilSearchParamsGuiComp);
    m_spinNominalScale->setObjectName(QString::fromUtf8("m_spinNominalScale"));
    m_spinNominalScale->setMinimum(1);
    m_spinNominalScale->setMaximum(2);
    m_spinNominalScale->setSingleStep(0.1);
    m_spinNominalScale->setValue(1);

    vboxLayout3->addWidget(m_spinNominalScale);

    m_spinNominalAngle = new QSpinBox(CMilSearchParamsGuiComp);
    m_spinNominalAngle->setObjectName(QString::fromUtf8("m_spinNominalAngle"));
    m_spinNominalAngle->setMinimum(0);
    m_spinNominalAngle->setMaximum(360);

    vboxLayout3->addWidget(m_spinNominalAngle);

    m_comboDetailLevel = new QComboBox(CMilSearchParamsGuiComp);
    m_comboDetailLevel->setObjectName(QString::fromUtf8("m_comboDetailLevel"));

    vboxLayout3->addWidget(m_comboDetailLevel);

    m_spinSmoothness = new QSpinBox(CMilSearchParamsGuiComp);
    m_spinSmoothness->setObjectName(QString::fromUtf8("m_spinSmoothness"));
    m_spinSmoothness->setMaximum(100);
    m_spinSmoothness->setSingleStep(1);
    m_spinSmoothness->setValue(50);

    vboxLayout3->addWidget(m_spinSmoothness);

    m_comboSharedEdges = new QComboBox(CMilSearchParamsGuiComp);
    m_comboSharedEdges->setObjectName(QString::fromUtf8("m_comboSharedEdges"));
    m_comboSharedEdges->setEnabled(true);

    vboxLayout3->addWidget(m_comboSharedEdges);

    m_comboTargetCaching = new QComboBox(CMilSearchParamsGuiComp);
    m_comboTargetCaching->setObjectName(QString::fromUtf8("m_comboTargetCaching"));

    vboxLayout3->addWidget(m_comboTargetCaching);


    hboxLayout->addLayout(vboxLayout3);


    gridLayout->addLayout(hboxLayout, 1, 0, 1, 2);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    label_22 = new QLabel(CMilSearchParamsGuiComp);
    label_22->setObjectName(QString::fromUtf8("label_22"));

    hboxLayout1->addWidget(label_22);

    m_comboFilterType = new QComboBox(CMilSearchParamsGuiComp);
    m_comboFilterType->setObjectName(QString::fromUtf8("m_comboFilterType"));

    hboxLayout1->addWidget(m_comboFilterType);

    m_labelKernelSize = new QLabel(CMilSearchParamsGuiComp);
    m_labelKernelSize->setObjectName(QString::fromUtf8("m_labelKernelSize"));

    hboxLayout1->addWidget(m_labelKernelSize);

    m_spinKernelSize = new QSpinBox(CMilSearchParamsGuiComp);
    m_spinKernelSize->setObjectName(QString::fromUtf8("m_spinKernelSize"));
    m_spinKernelSize->setMinimum(3);
    m_spinKernelSize->setSingleStep(2);
    m_spinKernelSize->setValue(3);

    hboxLayout1->addWidget(m_spinKernelSize);

    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem);

    label_11 = new QLabel(CMilSearchParamsGuiComp);
    label_11->setObjectName(QString::fromUtf8("label_11"));

    hboxLayout1->addWidget(label_11);

    m_spinTimeout = new QSpinBox(CMilSearchParamsGuiComp);
    m_spinTimeout->setObjectName(QString::fromUtf8("m_spinTimeout"));
    m_spinTimeout->setMinimum(0);
    m_spinTimeout->setMaximum(3000);
    m_spinTimeout->setSingleStep(10);
    m_spinTimeout->setValue(2000);

    hboxLayout1->addWidget(m_spinTimeout);


    gridLayout->addLayout(hboxLayout1, 2, 0, 1, 2);


    retranslateUi(CMilSearchParamsGuiComp);

    QMetaObject::connectSlotsByName(CMilSearchParamsGuiComp);
    } // setupUi

    void retranslateUi(QWidget *CMilSearchParamsGuiComp)
    {
    CMilSearchParamsGuiComp->setWindowTitle(QApplication::translate("CMilSearchParamsGuiComp", "Form", 0, QApplication::UnicodeUTF8));
    label_9->setToolTip(QApplication::translate("CMilSearchParamsGuiComp", "Sets the resolution level for the initial stage (lowest resolution) of the search.\n"
"Level 0 is the original target and each higher level is half the size (and resolution) of the previous one. If the specified level is not supported by the search algorithm, the highest possible level will be used. \n"
"A higher first level speeds up the initial search but might make it less reliable because the model might not retain enough distinctive features at a lower resolution.\n"
"", 0, QApplication::UnicodeUTF8));
    label_9->setWhatsThis(QApplication::translate("CMilSearchParamsGuiComp", "Sets the resolution level for the initial stage (lowest resolution) of the search.\n"
"Level 0 is the original target and each higher level is half the size (and resolution) of the previous one. If the specified level is not supported by the search algorithm, the highest possible level will be used. \n"
"A higher first level speeds up the initial search but might make it less reliable because the model might not retain enough distinctive features at a lower resolution.\n"
"", 0, QApplication::UnicodeUTF8));
    label_9->setText(QApplication::translate("CMilSearchParamsGuiComp", "First Level", 0, QApplication::UnicodeUTF8));
    m_spinFirstLevel->setToolTip(QApplication::translate("CMilSearchParamsGuiComp", "Sets the resolution level for the initial stage (lowest resolution) of the search.\n"
"Level 0 is the original target and each higher level is half the size (and resolution) of the previous one. If the specified level is not supported by the search algorithm, the highest possible level will be used. \n"
"A higher first level speeds up the initial search but might make it less reliable because the model might not retain enough distinctive features at a lower resolution.\n"
"", 0, QApplication::UnicodeUTF8));
    m_spinFirstLevel->setWhatsThis(QApplication::translate("CMilSearchParamsGuiComp", "Sets the resolution level for the initial stage (lowest resolution) of the search.\n"
"Level 0 is the original target and each higher level is half the size (and resolution) of the previous one. If the specified level is not supported by the search algorithm, the highest possible level will be used. \n"
"A higher first level speeds up the initial search but might make it less reliable because the model might not retain enough distinctive features at a lower resolution.\n"
"", 0, QApplication::UnicodeUTF8));
    label_10->setToolTip(QApplication::translate("CMilSearchParamsGuiComp", "Sets the resolution level for the final stage (the highest resolution) of the search.\n"
"Level 0 is the original target and each higher level is half the size (and resolution) of the previous one. If the specified level is not supported by the search algorithm, the highest possible level will be used. \n"
"A higher last level speeds up the initial search but might make it less reliable and less accurate because the model might not retain enough distinctive features at a low resolution. \n"
"If the specified level is not supported by the search algorithm, the highest acceptable level will be used. \n"
"", 0, QApplication::UnicodeUTF8));
    label_10->setWhatsThis(QApplication::translate("CMilSearchParamsGuiComp", "Sets the resolution level for the final stage (the highest resolution) of the search.\n"
"Level 0 is the original target and each higher level is half the size (and resolution) of the previous one. If the specified level is not supported by the search algorithm, the highest possible level will be used. \n"
"A higher last level speeds up the initial search but might make it less reliable and less accurate because the model might not retain enough distinctive features at a low resolution. \n"
"If the specified level is not supported by the search algorithm, the highest acceptable level will be used. \n"
"", 0, QApplication::UnicodeUTF8));
    label_10->setText(QApplication::translate("CMilSearchParamsGuiComp", "Last Level", 0, QApplication::UnicodeUTF8));
    m_spinSecondLevel->setToolTip(QApplication::translate("CMilSearchParamsGuiComp", "Sets the resolution level for the final stage (the highest resolution) of the search.\n"
"Level 0 is the original target and each higher level is half the size (and resolution) of the previous one. If the specified level is not supported by the search algorithm, the highest possible level will be used. \n"
"A higher last level speeds up the initial search but might make it less reliable and less accurate because the model might not retain enough distinctive features at a low resolution. \n"
"If the specified level is not supported by the search algorithm, the highest acceptable level will be used. \n"
"", 0, QApplication::UnicodeUTF8));
    m_spinSecondLevel->setWhatsThis(QApplication::translate("CMilSearchParamsGuiComp", "Sets the resolution level for the final stage (the highest resolution) of the search.\n"
"Level 0 is the original target and each higher level is half the size (and resolution) of the previous one. If the specified level is not supported by the search algorithm, the highest possible level will be used. \n"
"A higher last level speeds up the initial search but might make it less reliable and less accurate because the model might not retain enough distinctive features at a low resolution. \n"
"If the specified level is not supported by the search algorithm, the highest acceptable level will be used. \n"
"", 0, QApplication::UnicodeUTF8));
    label_7->setToolTip(QApplication::translate("CMilSearchParamsGuiComp", "Sets the algorithm's search speed. \n"
"Note that increasing the search speed can decrease the robustness \n"
"and subpixel accuracy of the search operation.\n"
"", 0, QApplication::UnicodeUTF8));
    label_7->setWhatsThis(QApplication::translate("CMilSearchParamsGuiComp", "Sets the algorithm's search speed. \n"
"Note that increasing the search speed can decrease the robustness \n"
"and subpixel accuracy of the search operation.\n"
"", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("CMilSearchParamsGuiComp", "Speed", 0, QApplication::UnicodeUTF8));
    m_comboSpeed->clear();
    m_comboSpeed->insertItems(0, QStringList()
     << QApplication::translate("CMilSearchParamsGuiComp", "Very High", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CMilSearchParamsGuiComp", "High", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CMilSearchParamsGuiComp", "Medium", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CMilSearchParamsGuiComp", "Low", 0, QApplication::UnicodeUTF8)
    );
    m_comboSpeed->setToolTip(QApplication::translate("CMilSearchParamsGuiComp", "Sets the algorithm's search speed. \n"
"Note that increasing the search speed can decrease the robustness \n"
"and subpixel accuracy of the search operation.\n"
"", 0, QApplication::UnicodeUTF8));
    m_comboSpeed->setWhatsThis(QApplication::translate("CMilSearchParamsGuiComp", "Sets the algorithm's search speed. \n"
"Note that increasing the search speed can decrease the robustness \n"
"and subpixel accuracy of the search operation.\n"
"", 0, QApplication::UnicodeUTF8));
    label_8->setToolTip(QApplication::translate("CMilSearchParamsGuiComp", "Sets the accuracy with which to find occurrences. \n"
"The precision achieved is dependent on the quality of the model and the target. \n"
"In addition, positional accuracy is slightly affected by the setting of the Speed parameter.", 0, QApplication::UnicodeUTF8));
    label_8->setWhatsThis(QApplication::translate("CMilSearchParamsGuiComp", "Sets the accuracy with which to find occurrences. \n"
"The precision achieved is dependent on the quality of the model and the target. \n"
"In addition, positional accuracy is slightly affected by the setting of the Speed parameter.", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("CMilSearchParamsGuiComp", "Accuracy", 0, QApplication::UnicodeUTF8));
    m_comboAccuracy->clear();
    m_comboAccuracy->insertItems(0, QStringList()
     << QApplication::translate("CMilSearchParamsGuiComp", "Medium", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CMilSearchParamsGuiComp", "High", 0, QApplication::UnicodeUTF8)
    );
    m_comboAccuracy->setToolTip(QApplication::translate("CMilSearchParamsGuiComp", "Sets the accuracy with which to find occurrences. \n"
"The precision achieved is dependent on the quality of the model and the target. \n"
"In addition, positional accuracy is slightly affected by the setting of the Speed parameter.", 0, QApplication::UnicodeUTF8));
    m_comboAccuracy->setWhatsThis(QApplication::translate("CMilSearchParamsGuiComp", "Sets the accuracy with which to find occurrences. \n"
"The precision achieved is dependent on the quality of the model and the target. \n"
"In addition, positional accuracy is slightly affected by the setting of the Speed parameter.", 0, QApplication::UnicodeUTF8));
    label_12->setText(QApplication::translate("CMilSearchParamsGuiComp", "Polarity", 0, QApplication::UnicodeUTF8));
    label_13->setToolTip(QString());
    label_13->setText(QApplication::translate("CMilSearchParamsGuiComp", "Acceptance Target", 0, QApplication::UnicodeUTF8));
    label_14->setText(QApplication::translate("CMilSearchParamsGuiComp", "Certainty", 0, QApplication::UnicodeUTF8));
    label_15->setText(QApplication::translate("CMilSearchParamsGuiComp", "Certainty Target", 0, QApplication::UnicodeUTF8));
    label_16->setText(QApplication::translate("CMilSearchParamsGuiComp", "Fit Error Weight", 0, QApplication::UnicodeUTF8));
    m_comboPolarity->clear();
    m_comboPolarity->insertItems(0, QStringList()
     << QApplication::translate("CMilSearchParamsGuiComp", "Same", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CMilSearchParamsGuiComp", "Reverse", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CMilSearchParamsGuiComp", "Both", 0, QApplication::UnicodeUTF8)
    );
    m_spinAcceptanceTarget->setToolTip(QApplication::translate("CMilSearchParamsGuiComp", "Sets the acceptance level for the target score. An occurrence will be returned only if the target score between the target and the model is greater than or equal to this level. \n"
"The target score is a measure, as a percentage, of edges found in the occurrence that are not found in the original model, weighted by the deviation in position of the common edges.\n"
"Using M_FIT_ERROR_WEIGHTING_FACTOR, you can specify how much the fit influences the target score.", 0, QApplication::UnicodeUTF8));
    m_spinAcceptanceTarget->setWhatsThis(QApplication::translate("CMilSearchParamsGuiComp", "Sets the acceptance level for the target score. An occurrence will be returned only if the target score between the target and the model is greater than or equal to this level. \n"
"The target score is a measure, as a percentage, of edges found in the occurrence that are not found in the original model, weighted by the deviation in position of the common edges.\n"
"Using M_FIT_ERROR_WEIGHTING_FACTOR, you can specify how much the fit influences the target score.", 0, QApplication::UnicodeUTF8));
    m_spinAcceptanceTarget->setSuffix(QApplication::translate("CMilSearchParamsGuiComp", " %", 0, QApplication::UnicodeUTF8));
    m_spinCertainty->setSuffix(QApplication::translate("CMilSearchParamsGuiComp", " %", 0, QApplication::UnicodeUTF8));
    m_spinCertaintyTarget->setSuffix(QApplication::translate("CMilSearchParamsGuiComp", " %", 0, QApplication::UnicodeUTF8));
    m_spinFitErrorWeight->setSuffix(QApplication::translate("CMilSearchParamsGuiComp", " %", 0, QApplication::UnicodeUTF8));
    label_17->setText(QApplication::translate("CMilSearchParamsGuiComp", "Nominal Scale", 0, QApplication::UnicodeUTF8));
    label_24->setToolTip(QApplication::translate("CMilSearchParamsGuiComp", "Sets the nominal search angle, relative to the model reference axis angle.", 0, QApplication::UnicodeUTF8));
    label_24->setWhatsThis(QApplication::translate("CMilSearchParamsGuiComp", "Sets the nominal search angle, relative to the model reference axis angle.", 0, QApplication::UnicodeUTF8));
    label_24->setText(QApplication::translate("CMilSearchParamsGuiComp", "Nominal Angle", 0, QApplication::UnicodeUTF8));
    label_18->setText(QApplication::translate("CMilSearchParamsGuiComp", "Detail Level", 0, QApplication::UnicodeUTF8));
    label_19->setText(QApplication::translate("CMilSearchParamsGuiComp", "Smoothness", 0, QApplication::UnicodeUTF8));
    label_20->setText(QApplication::translate("CMilSearchParamsGuiComp", "Shared Edges", 0, QApplication::UnicodeUTF8));
    label_21->setText(QApplication::translate("CMilSearchParamsGuiComp", "Target Caching", 0, QApplication::UnicodeUTF8));
    m_spinNominalAngle->setToolTip(QApplication::translate("CMilSearchParamsGuiComp", "Sets the nominal search angle, relative to the model reference axis angle.", 0, QApplication::UnicodeUTF8));
    m_spinNominalAngle->setWhatsThis(QApplication::translate("CMilSearchParamsGuiComp", "Sets the nominal search angle, relative to the model reference axis angle.", 0, QApplication::UnicodeUTF8));
    m_spinNominalAngle->setSuffix(QApplication::translate("CMilSearchParamsGuiComp", " \302\260", 0, QApplication::UnicodeUTF8));
    m_comboDetailLevel->clear();
    m_comboDetailLevel->insertItems(0, QStringList()
     << QApplication::translate("CMilSearchParamsGuiComp", "Medium", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CMilSearchParamsGuiComp", "High", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CMilSearchParamsGuiComp", "Very High", 0, QApplication::UnicodeUTF8)
    );
    m_spinSmoothness->setSuffix(QApplication::translate("CMilSearchParamsGuiComp", " %", 0, QApplication::UnicodeUTF8));
    m_comboSharedEdges->clear();
    m_comboSharedEdges->insertItems(0, QStringList()
     << QApplication::translate("CMilSearchParamsGuiComp", "Disabled", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CMilSearchParamsGuiComp", "Enabled", 0, QApplication::UnicodeUTF8)
     << QString()
    );
    m_comboTargetCaching->clear();
    m_comboTargetCaching->insertItems(0, QStringList()
     << QApplication::translate("CMilSearchParamsGuiComp", "Disabled", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CMilSearchParamsGuiComp", "Enabled", 0, QApplication::UnicodeUTF8)
    );
    label_22->setText(QApplication::translate("CMilSearchParamsGuiComp", "Filter", 0, QApplication::UnicodeUTF8));
    m_comboFilterType->clear();
    m_comboFilterType->insertItems(0, QStringList()
     << QApplication::translate("CMilSearchParamsGuiComp", "Kernel", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CMilSearchParamsGuiComp", "Recursive", 0, QApplication::UnicodeUTF8)
    );
    m_labelKernelSize->setText(QApplication::translate("CMilSearchParamsGuiComp", "Kernel Size", 0, QApplication::UnicodeUTF8));
    label_11->setToolTip(QApplication::translate("CMilSearchParamsGuiComp", "Sets the maximum search time, in msec.\n"
"If a search times out, the number of occurrences found at that point will be returned.", 0, QApplication::UnicodeUTF8));
    label_11->setWhatsThis(QApplication::translate("CMilSearchParamsGuiComp", "Sets the maximum search time, in msec.\n"
"If a search times out, the number of occurrences found at that point will be returned.", 0, QApplication::UnicodeUTF8));
    label_11->setText(QApplication::translate("CMilSearchParamsGuiComp", "Timeout", 0, QApplication::UnicodeUTF8));
    m_spinTimeout->setToolTip(QApplication::translate("CMilSearchParamsGuiComp", "Sets the maximum search time, in msec.\n"
"If a search times out, the number of occurrences found at that point will be returned.", 0, QApplication::UnicodeUTF8));
    m_spinTimeout->setWhatsThis(QApplication::translate("CMilSearchParamsGuiComp", "Sets the maximum search time, in msec.\n"
"If a search times out, the number of occurrences found at that point will be returned.", 0, QApplication::UnicodeUTF8));
    m_spinTimeout->setSuffix(QApplication::translate("CMilSearchParamsGuiComp", " ms", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(CMilSearchParamsGuiComp);
    } // retranslateUi

};

namespace Ui {
    class CMilSearchParamsGuiComp: public Ui_CMilSearchParamsGuiComp {};
} // namespace Ui

#endif // UI_CMILSEARCHPARAMSGUICOMP_H
