/********************************************************************************
** Form generated from reading ui file 'CSearchParamsGuiComp.ui'
**
** Created: Tue 16. Sep 12:10:12 2008
**      by: Qt User Interface Compiler version 4.3.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CSEARCHPARAMSGUICOMP_H
#define UI_CSEARCHPARAMSGUICOMP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

class Ui_CSearchParamsGuiComp
{
public:
    QVBoxLayout *vboxLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout;
    QLabel *label;
    QSpinBox *m_spinMinimumScore;
    QLabel *label_2;
    QSpinBox *m_spinModelNumber;
    QGridLayout *gridLayout1;
    QLabel *label_3;
    QSpinBox *m_spinMinimumAngle;
    QLabel *label_6;
    QDoubleSpinBox *m_spinMinimumScale;
    QGridLayout *gridLayout2;
    QLabel *label_4;
    QSpinBox *m_spinMaximumAngle;
    QLabel *label_5;
    QDoubleSpinBox *m_spinMaximumScale;
    QVBoxLayout *vboxLayout1;
    QCheckBox *m_checkEnableRotation;
    QCheckBox *m_checkEnableScale;
    QWidget *tab_2;
    QVBoxLayout *vboxLayout2;
    QFrame *ModelFrame;

    void setupUi(QWidget *CSearchParamsGuiComp)
    {
    if (CSearchParamsGuiComp->objectName().isEmpty())
        CSearchParamsGuiComp->setObjectName(QString::fromUtf8("CSearchParamsGuiComp"));
    CSearchParamsGuiComp->resize(418, 118);
    vboxLayout = new QVBoxLayout(CSearchParamsGuiComp);
    vboxLayout->setSpacing(6);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    vboxLayout->setContentsMargins(0, 0, 0, 0);
    tabWidget = new QTabWidget(CSearchParamsGuiComp);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    tabWidget->setEnabled(true);
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
    tabWidget->setSizePolicy(sizePolicy);
    tabWidget->setFocusPolicy(Qt::TabFocus);
    tabWidget->setTabPosition(QTabWidget::South);
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    gridLayout = new QGridLayout(tab);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    label = new QLabel(tab);
    label->setObjectName(QString::fromUtf8("label"));
    QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
    label->setSizePolicy(sizePolicy1);

    gridLayout->addWidget(label, 0, 0, 1, 1);

    m_spinMinimumScore = new QSpinBox(tab);
    m_spinMinimumScore->setObjectName(QString::fromUtf8("m_spinMinimumScore"));
    QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Fixed);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(m_spinMinimumScore->sizePolicy().hasHeightForWidth());
    m_spinMinimumScore->setSizePolicy(sizePolicy2);
    m_spinMinimumScore->setMaximum(100);
    m_spinMinimumScore->setSingleStep(5);
    m_spinMinimumScore->setValue(70);

    gridLayout->addWidget(m_spinMinimumScore, 0, 1, 1, 1);

    label_2 = new QLabel(tab);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    gridLayout->addWidget(label_2, 0, 2, 1, 1);

    m_spinModelNumber = new QSpinBox(tab);
    m_spinModelNumber->setObjectName(QString::fromUtf8("m_spinModelNumber"));
    m_spinModelNumber->setMinimum(-1);
    m_spinModelNumber->setMaximum(1000);
    m_spinModelNumber->setSingleStep(1);
    m_spinModelNumber->setValue(1);

    gridLayout->addWidget(m_spinModelNumber, 0, 3, 1, 2);

    gridLayout1 = new QGridLayout();
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    label_3 = new QLabel(tab);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    gridLayout1->addWidget(label_3, 0, 0, 1, 1);

    m_spinMinimumAngle = new QSpinBox(tab);
    m_spinMinimumAngle->setObjectName(QString::fromUtf8("m_spinMinimumAngle"));
    m_spinMinimumAngle->setMinimum(-180);
    m_spinMinimumAngle->setMaximum(0);

    gridLayout1->addWidget(m_spinMinimumAngle, 0, 1, 1, 1);

    label_6 = new QLabel(tab);
    label_6->setObjectName(QString::fromUtf8("label_6"));
    sizePolicy1.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
    label_6->setSizePolicy(sizePolicy1);

    gridLayout1->addWidget(label_6, 1, 0, 1, 1);

    m_spinMinimumScale = new QDoubleSpinBox(tab);
    m_spinMinimumScale->setObjectName(QString::fromUtf8("m_spinMinimumScale"));
    m_spinMinimumScale->setMinimum(0.01);
    m_spinMinimumScale->setMaximum(10);
    m_spinMinimumScale->setSingleStep(0.1);
    m_spinMinimumScale->setValue(0.5);

    gridLayout1->addWidget(m_spinMinimumScale, 1, 1, 1, 1);


    gridLayout->addLayout(gridLayout1, 1, 0, 1, 2);

    gridLayout2 = new QGridLayout();
    gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
    label_4 = new QLabel(tab);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    gridLayout2->addWidget(label_4, 0, 0, 1, 1);

    m_spinMaximumAngle = new QSpinBox(tab);
    m_spinMaximumAngle->setObjectName(QString::fromUtf8("m_spinMaximumAngle"));
    m_spinMaximumAngle->setMinimum(0);
    m_spinMaximumAngle->setMaximum(180);
    m_spinMaximumAngle->setValue(180);

    gridLayout2->addWidget(m_spinMaximumAngle, 0, 1, 1, 1);

    label_5 = new QLabel(tab);
    label_5->setObjectName(QString::fromUtf8("label_5"));

    gridLayout2->addWidget(label_5, 1, 0, 1, 1);

    m_spinMaximumScale = new QDoubleSpinBox(tab);
    m_spinMaximumScale->setObjectName(QString::fromUtf8("m_spinMaximumScale"));
    m_spinMaximumScale->setDecimals(2);
    m_spinMaximumScale->setMinimum(0);
    m_spinMaximumScale->setMaximum(10);
    m_spinMaximumScale->setSingleStep(0.1);
    m_spinMaximumScale->setValue(0);

    gridLayout2->addWidget(m_spinMaximumScale, 1, 1, 1, 1);


    gridLayout->addLayout(gridLayout2, 1, 2, 1, 2);

    vboxLayout1 = new QVBoxLayout();
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    m_checkEnableRotation = new QCheckBox(tab);
    m_checkEnableRotation->setObjectName(QString::fromUtf8("m_checkEnableRotation"));
    m_checkEnableRotation->setEnabled(true);
    m_checkEnableRotation->setChecked(true);

    vboxLayout1->addWidget(m_checkEnableRotation);

    m_checkEnableScale = new QCheckBox(tab);
    m_checkEnableScale->setObjectName(QString::fromUtf8("m_checkEnableScale"));
    m_checkEnableScale->setEnabled(true);
    m_checkEnableScale->setChecked(true);

    vboxLayout1->addWidget(m_checkEnableScale);


    gridLayout->addLayout(vboxLayout1, 1, 4, 1, 1);

    tabWidget->addTab(tab, QString());
    tab_2 = new QWidget();
    tab_2->setObjectName(QString::fromUtf8("tab_2"));
    vboxLayout2 = new QVBoxLayout(tab_2);
    vboxLayout2->setSpacing(6);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    vboxLayout2->setContentsMargins(9, 9, 9, 9);
    ModelFrame = new QFrame(tab_2);
    ModelFrame->setObjectName(QString::fromUtf8("ModelFrame"));
    ModelFrame->setFrameShape(QFrame::NoFrame);
    ModelFrame->setFrameShadow(QFrame::Raised);

    vboxLayout2->addWidget(ModelFrame);

    tabWidget->addTab(tab_2, QString());

    vboxLayout->addWidget(tabWidget);


    retranslateUi(CSearchParamsGuiComp);
    QObject::connect(m_checkEnableRotation, SIGNAL(toggled(bool)), m_spinMinimumAngle, SLOT(setEnabled(bool)));
    QObject::connect(m_checkEnableRotation, SIGNAL(toggled(bool)), m_spinMaximumAngle, SLOT(setEnabled(bool)));
    QObject::connect(m_checkEnableScale, SIGNAL(toggled(bool)), m_spinMinimumScale, SLOT(setEnabled(bool)));
    QObject::connect(m_checkEnableScale, SIGNAL(toggled(bool)), m_spinMaximumScale, SLOT(setEnabled(bool)));

    tabWidget->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(CSearchParamsGuiComp);
    } // setupUi

    void retranslateUi(QWidget *CSearchParamsGuiComp)
    {
    CSearchParamsGuiComp->setWindowTitle(QApplication::translate("CSearchParamsGuiComp", "Form", 0, QApplication::UnicodeUTF8));
    label->setToolTip(QApplication::translate("CSearchParamsGuiComp", "Sets the acceptance level for the score. \n"
"An occurrence will be returned only if the match score \n"
"between the target and the model is greater than or equal to this level.\n"
"The score is a measure, as a percentage, \n"
"of the presence and fit of the model's active edges in the occurrence. \n"
"Using Advanced/Fit Error Weight, you can specify how much the fit influences the score.", 0, QApplication::UnicodeUTF8));
    label->setWhatsThis(QApplication::translate("CSearchParamsGuiComp", "Sets the acceptance level for the score. \n"
"An occurrence will be returned only if the match score \n"
"between the target and the model is greater than or equal to this level.\n"
"The score is a measure, as a percentage, \n"
"of the presence and fit of the model's active edges in the occurrence. \n"
"Using Advanced/Fit Error Weight, you can specify how much the fit influences the score.", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("CSearchParamsGuiComp", "Minimum Score", 0, QApplication::UnicodeUTF8));
    m_spinMinimumScore->setToolTip(QApplication::translate("CSearchParamsGuiComp", "Sets the acceptance level for the score. \n"
"An occurrence will be returned only if the match score \n"
"between the target and the model is greater than or equal to this level.\n"
"The score is a measure, as a percentage, \n"
"of the presence and fit of the model's active edges in the occurrence. \n"
"Using Advanced/Fit Error Weight, you can specify how much the fit influences the score.", 0, QApplication::UnicodeUTF8));
    m_spinMinimumScore->setWhatsThis(QApplication::translate("CSearchParamsGuiComp", "Sets the acceptance level for the score. \n"
"An occurrence will be returned only if the match score \n"
"between the target and the model is greater than or equal to this level.\n"
"The score is a measure, as a percentage, \n"
"of the presence and fit of the model's active edges in the occurrence. \n"
"Using Advanced/Fit Error Weight, you can specify how much the fit influences the score.", 0, QApplication::UnicodeUTF8));
    m_spinMinimumScore->setSuffix(QApplication::translate("CSearchParamsGuiComp", " %", 0, QApplication::UnicodeUTF8));
    m_spinMinimumScore->setPrefix(QString());
    label_2->setToolTip(QApplication::translate("CSearchParamsGuiComp", "Sets the number of occurrences for which to search.", 0, QApplication::UnicodeUTF8));
    label_2->setWhatsThis(QApplication::translate("CSearchParamsGuiComp", "Sets the number of occurrences for which to search.", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("CSearchParamsGuiComp", "Model Occurrence", 0, QApplication::UnicodeUTF8));
    m_spinModelNumber->setToolTip(QApplication::translate("CSearchParamsGuiComp", "Sets the number of occurrences for which to search.", 0, QApplication::UnicodeUTF8));
    m_spinModelNumber->setWhatsThis(QApplication::translate("CSearchParamsGuiComp", "Sets the number of occurrences for which to search.", 0, QApplication::UnicodeUTF8));
    m_spinModelNumber->setSpecialValueText(QApplication::translate("CSearchParamsGuiComp", "All", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("CSearchParamsGuiComp", "Minimum Angle", 0, QApplication::UnicodeUTF8));
    m_spinMinimumAngle->setToolTip(QApplication::translate("CSearchParamsGuiComp", "Defines the start rotation angle of the angle range.", 0, QApplication::UnicodeUTF8));
    m_spinMinimumAngle->setStatusTip(QString());
    m_spinMinimumAngle->setWhatsThis(QApplication::translate("CSearchParamsGuiComp", "Defines the start rotation angle of the angle range.", 0, QApplication::UnicodeUTF8));
    m_spinMinimumAngle->setSuffix(QApplication::translate("CSearchParamsGuiComp", " \302\260", 0, QApplication::UnicodeUTF8));
    label_6->setToolTip(QApplication::translate("CSearchParamsGuiComp", "Sets the minimum scale for the search.", 0, QApplication::UnicodeUTF8));
    label_6->setWhatsThis(QApplication::translate("CSearchParamsGuiComp", "Sets the minimum scale for the search.", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("CSearchParamsGuiComp", "Minimum Scale", 0, QApplication::UnicodeUTF8));
    m_spinMinimumScale->setToolTip(QApplication::translate("CSearchParamsGuiComp", "Sets the minimum scale for the search.", 0, QApplication::UnicodeUTF8));
    m_spinMinimumScale->setWhatsThis(QApplication::translate("CSearchParamsGuiComp", "Sets the minimum scale for the search.", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("CSearchParamsGuiComp", "Maximum Angle", 0, QApplication::UnicodeUTF8));
    m_spinMaximumAngle->setToolTip(QApplication::translate("CSearchParamsGuiComp", "Defines the stop rotation angle of the angle range.", 0, QApplication::UnicodeUTF8));
    m_spinMaximumAngle->setWhatsThis(QApplication::translate("CSearchParamsGuiComp", "Defines the stop rotation angle of the angle range.", 0, QApplication::UnicodeUTF8));
    m_spinMaximumAngle->setSuffix(QApplication::translate("CSearchParamsGuiComp", " \302\260", 0, QApplication::UnicodeUTF8));
    label_5->setToolTip(QApplication::translate("CSearchParamsGuiComp", "Sets the maximum scale for the search.", 0, QApplication::UnicodeUTF8));
    label_5->setWhatsThis(QApplication::translate("CSearchParamsGuiComp", "Sets the maximum scale for the search.", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("CSearchParamsGuiComp", "Maximum Scale", 0, QApplication::UnicodeUTF8));
    m_spinMaximumScale->setToolTip(QApplication::translate("CSearchParamsGuiComp", "Sets the maximum scale for the search.", 0, QApplication::UnicodeUTF8));
    m_spinMaximumScale->setWhatsThis(QApplication::translate("CSearchParamsGuiComp", "Sets the maximum scale for the search.", 0, QApplication::UnicodeUTF8));
    m_checkEnableRotation->setToolTip(QApplication::translate("CSearchParamsGuiComp", "Sets whether to perform calculations specific to angular-range search strategies. \n"
"Typically, to search for models within their specified angular range [Minimum Angle, Maximum Angle] in the target, \n"
"calculations specific to angular-range search strategies should be enabled for the context. \n"
"Disabling these calculations might speed up the search depending on the model and the target. \n"
"Note that rotation must be enabled to search for rotation-invariant non-synthetic models \n"
"(for example, an image-type model of a circle). \n"
"Also note that candidates can only be returned as occurrences \n"
"if found within the angular range specified for their model. \n"
"Therefore, whether you enable or disable the calculation, \n"
"you can restrict which candidates are returned as occurrences by narrowing the angular range.", 0, QApplication::UnicodeUTF8));
    m_checkEnableRotation->setWhatsThis(QApplication::translate("CSearchParamsGuiComp", "Sets whether to perform calculations specific to angular-range search strategies. \n"
"Typically, to search for models within their specified angular range [Minimum Angle, Maximum Angle] in the target, \n"
"calculations specific to angular-range search strategies should be enabled for the context. \n"
"Disabling these calculations might speed up the search depending on the model and the target. \n"
"Note that rotation must be enabled to search for rotation-invariant non-synthetic models \n"
"(for example, an image-type model of a circle). \n"
"Also note that candidates can only be returned as occurrences \n"
"if found within the angular range specified for their model. \n"
"Therefore, whether you enable or disable the calculation, \n"
"you can restrict which candidates are returned as occurrences by narrowing the angular range.", 0, QApplication::UnicodeUTF8));
    m_checkEnableRotation->setText(QApplication::translate("CSearchParamsGuiComp", "Enable rotation", 0, QApplication::UnicodeUTF8));
    m_checkEnableScale->setToolTip(QApplication::translate("CSearchParamsGuiComp", "Sets whether to perform calculations specific to scale-range search strategies.\n"
"Typically, to search for models within their specified scale range (Adavanced/Nominal Scale x Minimum Scale) to (Adavanced/Nominal Scale x Maximum Scale) in the target, \n"
"calculations specific to scale-range search strategies should be enabled for the context. \n"
"These calculations are not required to search for models at their nominal scale. \n"
"In addition, if you expect that the occurrences sought are close to their model's nominal scale, \n"
"you can try disabling these calculations to see if Model Finder can still find the required occurrences. Disabling these calculations might speed up the search depending on the model and the target. \n"
"Note that candidates can only be returned as occurrences if found within the scale range specified for their model. \n"
"Therefore, whether you enable or disable the calculation, you can restrict which candidates are returned as occurrences by narrowing the scale range.", 0, QApplication::UnicodeUTF8));
    m_checkEnableScale->setWhatsThis(QApplication::translate("CSearchParamsGuiComp", "Sets whether to perform calculations specific to scale-range search strategies.\n"
"Typically, to search for models within their specified scale range (Adavanced/Nominal Scale x Minimum Scale) to (Adavanced/Nominal Scale x Maximum Scale) in the target, \n"
"calculations specific to scale-range search strategies should be enabled for the context. \n"
"These calculations are not required to search for models at their nominal scale. \n"
"In addition, if you expect that the occurrences sought are close to their model's nominal scale, \n"
"you can try disabling these calculations to see if Model Finder can still find the required occurrences. Disabling these calculations might speed up the search depending on the model and the target. \n"
"Note that candidates can only be returned as occurrences if found within the scale range specified for their model. \n"
"Therefore, whether you enable or disable the calculation, you can restrict which candidates are returned as occurrences by narrowing the scale range.", 0, QApplication::UnicodeUTF8));
    m_checkEnableScale->setText(QApplication::translate("CSearchParamsGuiComp", "Enable scale", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("CSearchParamsGuiComp", "General", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("CSearchParamsGuiComp", "Model", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(CSearchParamsGuiComp);
    } // retranslateUi

};

namespace Ui {
    class CSearchParamsGuiComp: public Ui_CSearchParamsGuiComp {};
} // namespace Ui

#endif // UI_CSEARCHPARAMSGUICOMP_H
