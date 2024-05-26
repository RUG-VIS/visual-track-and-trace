/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include "../Program.h"
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QGroupBox *settingsBox;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *TechniqueBox;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *FirstButton;
    QRadioButton *SecondButton;
    QGroupBox *ChannelBox;
    QGroupBox *ColourBox;
    QVBoxLayout *verticalLayout_12;
    QRadioButton *ComplementaryButton;
    QRadioButton *ContrastingButton;
    QRadioButton *MonochromaticButton;
    QGroupBox *SaturationBox;
    QVBoxLayout *verticalLayout_13;
    QRadioButton *SaturateButton;
    QRadioButton *DesaturateButton;
    QGroupBox *GlyphBox;
    QVBoxLayout *verticalLayout_14;
    QRadioButton *CircleButton;
    QRadioButton *TriangleButton;
    QRadioButton *SquareButton;
    QRadioButton *HexagonButton;
    QGroupBox *GlyphBox_2;
    QVBoxLayout *verticalLayout_16;
    QRadioButton *FullySampledButton;
    QRadioButton *RegularlySubsampledButton;
    QRadioButton *IregularlySubsampledButton;
    Program *program;

    Program* getProgram() {
      return program;
    }

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1048, 772);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        settingsBox = new QGroupBox(centralWidget);
        settingsBox->setObjectName(QString::fromUtf8("settingsBox"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(settingsBox->sizePolicy().hasHeightForWidth());
        settingsBox->setSizePolicy(sizePolicy);
        settingsBox->setMinimumSize(QSize(220, 0));
        settingsBox->setMaximumSize(QSize(280, 16777215));
        verticalLayout_3 = new QVBoxLayout(settingsBox);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        TechniqueBox = new QGroupBox(settingsBox);
        TechniqueBox->setObjectName(QString::fromUtf8("TechniqueBox"));
        sizePolicy.setHeightForWidth(TechniqueBox->sizePolicy().hasHeightForWidth());
        TechniqueBox->setSizePolicy(sizePolicy);
        TechniqueBox->setMinimumSize(QSize(0, 0));
        TechniqueBox->setMaximumSize(QSize(500, 100));
        verticalLayout_2 = new QVBoxLayout(TechniqueBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        FirstButton = new QRadioButton(TechniqueBox);
        FirstButton->setObjectName(QString::fromUtf8("FirstButton"));
        FirstButton->setChecked(true);

        verticalLayout_2->addWidget(FirstButton);

        SecondButton = new QRadioButton(TechniqueBox);
        SecondButton->setObjectName(QString::fromUtf8("SecondButton"));
        SecondButton->setChecked(false);

        verticalLayout_2->addWidget(SecondButton);


        verticalLayout_3->addWidget(TechniqueBox);

        ChannelBox = new QGroupBox(settingsBox);
        ChannelBox->setObjectName(QString::fromUtf8("ChannelBox"));
        ChannelBox->setMinimumSize(QSize(0, 550));
        ColourBox = new QGroupBox(ChannelBox);
        ColourBox->setObjectName(QString::fromUtf8("ColourBox"));
        ColourBox->setGeometry(QRect(0, 30, 300, 120));
        sizePolicy.setHeightForWidth(ColourBox->sizePolicy().hasHeightForWidth());
        ColourBox->setSizePolicy(sizePolicy);
        ColourBox->setMinimumSize(QSize(250, 0));
        ColourBox->setMaximumSize(QSize(500, 120));
        ColourBox->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
        verticalLayout_12 = new QVBoxLayout(ColourBox);
        verticalLayout_12->setSpacing(6);
        verticalLayout_12->setContentsMargins(11, 11, 11, 11);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        ComplementaryButton = new QRadioButton(ColourBox);
        ComplementaryButton->setObjectName(QString::fromUtf8("ComplementaryButton"));

        verticalLayout_12->addWidget(ComplementaryButton);

        ContrastingButton = new QRadioButton(ColourBox);
        ContrastingButton->setObjectName(QString::fromUtf8("ContrastingButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(ContrastingButton->sizePolicy().hasHeightForWidth());
        ContrastingButton->setSizePolicy(sizePolicy1);

        verticalLayout_12->addWidget(ContrastingButton);

        MonochromaticButton = new QRadioButton(ColourBox);
        MonochromaticButton->setObjectName(QString::fromUtf8("MonochromaticButton"));

        verticalLayout_12->addWidget(MonochromaticButton);

        SaturationBox = new QGroupBox(ChannelBox);
        SaturationBox->setObjectName(QString::fromUtf8("SaturationBox"));
        SaturationBox->setGeometry(QRect(0, 160, 269, 100));
        sizePolicy.setHeightForWidth(SaturationBox->sizePolicy().hasHeightForWidth());
        SaturationBox->setSizePolicy(sizePolicy);
        SaturationBox->setMinimumSize(QSize(260, 0));
        SaturationBox->setMaximumSize(QSize(500, 100));
        SaturationBox->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
        verticalLayout_13 = new QVBoxLayout(SaturationBox);
        verticalLayout_13->setSpacing(6);
        verticalLayout_13->setContentsMargins(11, 11, 11, 11);
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        SaturateButton = new QRadioButton(SaturationBox);
        SaturateButton->setObjectName(QString::fromUtf8("SaturateButton"));

        verticalLayout_13->addWidget(SaturateButton);

        DesaturateButton = new QRadioButton(SaturationBox);
        DesaturateButton->setObjectName(QString::fromUtf8("DesaturateButton"));

        verticalLayout_13->addWidget(DesaturateButton);

        GlyphBox = new QGroupBox(ChannelBox);
        GlyphBox->setObjectName(QString::fromUtf8("GlyphBox"));
        GlyphBox->setGeometry(QRect(0, 270, 260, 150));
        sizePolicy.setHeightForWidth(GlyphBox->sizePolicy().hasHeightForWidth());
        GlyphBox->setSizePolicy(sizePolicy);
        GlyphBox->setMinimumSize(QSize(260, 0));
        GlyphBox->setMaximumSize(QSize(500, 150));
        GlyphBox->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
        verticalLayout_14 = new QVBoxLayout(GlyphBox);
        verticalLayout_14->setSpacing(6);
        verticalLayout_14->setContentsMargins(11, 11, 11, 11);
        verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
        CircleButton = new QRadioButton(GlyphBox);
        CircleButton->setObjectName(QString::fromUtf8("CircleButton"));

        verticalLayout_14->addWidget(CircleButton);

        TriangleButton = new QRadioButton(GlyphBox);
        TriangleButton->setObjectName(QString::fromUtf8("TriangleButton"));

        verticalLayout_14->addWidget(TriangleButton);

        SquareButton = new QRadioButton(GlyphBox);
        SquareButton->setObjectName(QString::fromUtf8("SquareButton"));

        verticalLayout_14->addWidget(SquareButton);

        HexagonButton = new QRadioButton(GlyphBox);
        HexagonButton->setObjectName(QString::fromUtf8("HexagonButton"));

        verticalLayout_14->addWidget(HexagonButton);

        GlyphBox_2 = new QGroupBox(ChannelBox);
        GlyphBox_2->setObjectName(QString::fromUtf8("GlyphBox_2"));
        GlyphBox_2->setGeometry(QRect(0, 430, 260, 120));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(GlyphBox_2->sizePolicy().hasHeightForWidth());
        GlyphBox_2->setSizePolicy(sizePolicy2);
        GlyphBox_2->setMinimumSize(QSize(260, 0));
        GlyphBox_2->setMaximumSize(QSize(500, 120));
        GlyphBox_2->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
        verticalLayout_16 = new QVBoxLayout(GlyphBox_2);
        verticalLayout_16->setSpacing(6);
        verticalLayout_16->setContentsMargins(11, 11, 11, 11);
        verticalLayout_16->setObjectName(QString::fromUtf8("verticalLayout_16"));
        FullySampledButton = new QRadioButton(GlyphBox_2);
        FullySampledButton->setObjectName(QString::fromUtf8("FullySampledButton"));

        verticalLayout_16->addWidget(FullySampledButton);

        RegularlySubsampledButton = new QRadioButton(GlyphBox_2);
        RegularlySubsampledButton->setObjectName(QString::fromUtf8("RegularlySampledButton"));

        verticalLayout_16->addWidget(RegularlySubsampledButton);

        IregularlySubsampledButton = new QRadioButton(GlyphBox_2);
        IregularlySubsampledButton->setObjectName(QString::fromUtf8("IregularlySubsampledButton"));

        verticalLayout_16->addWidget(IregularlySubsampledButton);


        verticalLayout_3->addWidget(ChannelBox);


        horizontalLayout->addWidget(settingsBox);

        program = new Program(centralWidget);
        program->setObjectName(QString::fromUtf8("program"));

        horizontalLayout->addWidget(program);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Simulation", nullptr));
        settingsBox->setTitle(QCoreApplication::translate("MainWindow", "Settings", nullptr));
        TechniqueBox->setTitle(QCoreApplication::translate("MainWindow", "Technique", nullptr));
        FirstButton->setText(QCoreApplication::translate("MainWindow", "ECol + LGlyph", nullptr));
        SecondButton->setText(QCoreApplication::translate("MainWindow", "EGlyph + LCol", nullptr));
        ChannelBox->setTitle(QCoreApplication::translate("MainWindow", "Channel Options", nullptr));
        ColourBox->setTitle(QCoreApplication::translate("MainWindow", "Color", nullptr));
        ComplementaryButton->setText(QCoreApplication::translate("MainWindow", "Complementary ", nullptr));
        ContrastingButton->setText(QCoreApplication::translate("MainWindow", "Contrasting", nullptr));
        MonochromaticButton->setText(QCoreApplication::translate("MainWindow", "Monochromatic", nullptr));
        SaturationBox->setTitle(QCoreApplication::translate("MainWindow", "Saturation", nullptr));
        SaturateButton->setText(QCoreApplication::translate("MainWindow", "Fully saturated", nullptr));
        DesaturateButton->setText(QCoreApplication::translate("MainWindow", "Desaturated", nullptr));
        GlyphBox->setTitle(QCoreApplication::translate("MainWindow", "Glyph Shape", nullptr));
        CircleButton->setText(QCoreApplication::translate("MainWindow", "Circle", nullptr));
        TriangleButton->setText(QCoreApplication::translate("MainWindow", "Triangle", nullptr));
        SquareButton->setText(QCoreApplication::translate("MainWindow", "Square", nullptr));
        HexagonButton->setText(QCoreApplication::translate("MainWindow", "Hexagon", nullptr));
        GlyphBox_2->setTitle(QCoreApplication::translate("MainWindow", "Glyph count", nullptr));
        FullySampledButton->setText(QCoreApplication::translate("MainWindow", "Fully sampled", nullptr));
        RegularlySubsampledButton->setText(QCoreApplication::translate("MainWindow", "Regularly subsampled", nullptr));
        IregularlySubsampledButton->setText(QCoreApplication::translate("MainWindow", "Irregularly sampled", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
