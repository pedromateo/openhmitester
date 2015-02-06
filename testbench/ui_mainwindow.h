/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDial>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QAction *action_Reset_all;
    QWidget *centralWidget;
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout;
    QLabel *label;
    QSpinBox *spinBox;
    QSpinBox *spinBox_2;
    QSpinBox *spinBox_3;
    QSpinBox *spinBox_4;
    QSpinBox *spinBox_5;
    QSpinBox *spinBox_6;
    QLabel *label_2;
    QSpinBox *spinBox_7;
    QSpinBox *spinBox_8;
    QComboBox *comboBoxBuoyType;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QSpacerItem *verticalSpacer;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QPushButton *pushButton_2;
    QPushButton *pushButton;
    QListWidget *listWidget;
    QPushButton *pushButton_3;
    QLabel *label_14;
    QGridLayout *gridLayout_5;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QDial *di_nrLevel;
    QDial *di_volumeLevel;
    QLCDNumber *lcdNumber;
    QLCDNumber *lcdNumber_2;
    QLabel *label_18;
    QSlider *hs_balanceLevel;
    QLabel *label_19;
    QLabel *label_20;
    QDial *di_ampliLevel;
    QDial *di_signalEcho;
    QLCDNumber *lc_ampliLevel;
    QLCDNumber *lc_signalEcho;
    QLabel *label_21;
    QSpinBox *sb_bufferSize;
    QLabel *label_22;
    QSpinBox *sb_sampleRate;
    QProgressBar *pb_left;
    QProgressBar *pb_right;
    QSpacerItem *verticalSpacer_2;
    QPushButton *reset;
    QPushButton *apply;
    QGridLayout *gridLayout_2;
    QDial *dial_5;
    QDial *dial_6;
    QDial *dial_7;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QPushButton *pushButton_6;
    QLabel *label_23;
    QListWidget *listWidget_2;
    QPushButton *pushButton_7;
    QSpacerItem *verticalSpacer_4;
    QLabel *label_26;
    QLabel *label_25;
    QLabel *label_24;
    QProgressBar *progressBar_5;
    QProgressBar *progressBar_4;
    QProgressBar *progressBar_3;
    QFrame *line;
    QFrame *line_2;
    QFrame *line_5;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(967, 712);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        action_Reset_all = new QAction(MainWindow);
        action_Reset_all->setObjectName(QString::fromUtf8("action_Reset_all"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_4 = new QGridLayout(centralWidget);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setSizeConstraint(QLayout::SetMaximumSize);
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        gridLayout->addWidget(label, 2, 0, 1, 1);

        spinBox = new QSpinBox(centralWidget);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));

        gridLayout->addWidget(spinBox, 2, 3, 1, 1);

        spinBox_2 = new QSpinBox(centralWidget);
        spinBox_2->setObjectName(QString::fromUtf8("spinBox_2"));

        gridLayout->addWidget(spinBox_2, 2, 1, 1, 1);

        spinBox_3 = new QSpinBox(centralWidget);
        spinBox_3->setObjectName(QString::fromUtf8("spinBox_3"));

        gridLayout->addWidget(spinBox_3, 2, 5, 1, 1);

        spinBox_4 = new QSpinBox(centralWidget);
        spinBox_4->setObjectName(QString::fromUtf8("spinBox_4"));

        gridLayout->addWidget(spinBox_4, 3, 1, 1, 1);

        spinBox_5 = new QSpinBox(centralWidget);
        spinBox_5->setObjectName(QString::fromUtf8("spinBox_5"));

        gridLayout->addWidget(spinBox_5, 3, 3, 1, 1);

        spinBox_6 = new QSpinBox(centralWidget);
        spinBox_6->setObjectName(QString::fromUtf8("spinBox_6"));

        gridLayout->addWidget(spinBox_6, 3, 5, 1, 1);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);

        gridLayout->addWidget(label_2, 3, 0, 1, 1);

        spinBox_7 = new QSpinBox(centralWidget);
        spinBox_7->setObjectName(QString::fromUtf8("spinBox_7"));

        gridLayout->addWidget(spinBox_7, 4, 1, 1, 2);

        spinBox_8 = new QSpinBox(centralWidget);
        spinBox_8->setObjectName(QString::fromUtf8("spinBox_8"));

        gridLayout->addWidget(spinBox_8, 5, 1, 1, 2);

        comboBoxBuoyType = new QComboBox(centralWidget);
        comboBoxBuoyType->setObjectName(QString::fromUtf8("comboBoxBuoyType"));

        gridLayout->addWidget(comboBoxBuoyType, 1, 1, 1, 2);

        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font);

        gridLayout->addWidget(label_4, 4, 0, 1, 1);

        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font);

        gridLayout->addWidget(label_5, 5, 0, 1, 1);

        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        QFont font1;
        font1.setItalic(true);
        label_6->setFont(font1);

        gridLayout->addWidget(label_6, 4, 3, 1, 1);

        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setFont(font1);

        gridLayout->addWidget(label_7, 5, 3, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 6, 1, 1, 1);

        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 2, 2, 1, 1);

        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 2, 4, 1, 1);

        label_10 = new QLabel(centralWidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout->addWidget(label_10, 2, 6, 1, 1);

        label_11 = new QLabel(centralWidget);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout->addWidget(label_11, 3, 2, 1, 1);

        label_12 = new QLabel(centralWidget);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout->addWidget(label_12, 3, 4, 1, 1);

        label_13 = new QLabel(centralWidget);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout->addWidget(label_13, 3, 6, 1, 1);

        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 7, 5, 1, 2);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setEnabled(false);

        gridLayout->addWidget(pushButton, 7, 3, 1, 2);

        listWidget = new QListWidget(centralWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setAlternatingRowColors(true);

        gridLayout->addWidget(listWidget, 1, 7, 6, 1);

        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        gridLayout->addWidget(pushButton_3, 7, 7, 1, 1);

        label_14 = new QLabel(centralWidget);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout->addWidget(label_14, 0, 0, 1, 6);


        gridLayout_3->addLayout(gridLayout, 0, 0, 1, 1);

        gridLayout_5 = new QGridLayout();
        gridLayout_5->setSpacing(6);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        label_15 = new QLabel(centralWidget);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_5->addWidget(label_15, 0, 0, 1, 2);

        label_16 = new QLabel(centralWidget);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setFont(font);
        label_16->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(label_16, 1, 0, 1, 1);

        label_17 = new QLabel(centralWidget);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setFont(font);
        label_17->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(label_17, 1, 1, 1, 1);

        di_nrLevel = new QDial(centralWidget);
        di_nrLevel->setObjectName(QString::fromUtf8("di_nrLevel"));

        gridLayout_5->addWidget(di_nrLevel, 2, 0, 1, 1);

        di_volumeLevel = new QDial(centralWidget);
        di_volumeLevel->setObjectName(QString::fromUtf8("di_volumeLevel"));

        gridLayout_5->addWidget(di_volumeLevel, 2, 1, 1, 1);

        lcdNumber = new QLCDNumber(centralWidget);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));

        gridLayout_5->addWidget(lcdNumber, 3, 0, 1, 1);

        lcdNumber_2 = new QLCDNumber(centralWidget);
        lcdNumber_2->setObjectName(QString::fromUtf8("lcdNumber_2"));

        gridLayout_5->addWidget(lcdNumber_2, 3, 1, 1, 1);

        label_18 = new QLabel(centralWidget);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setFont(font);
        label_18->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(label_18, 4, 0, 1, 2);

        hs_balanceLevel = new QSlider(centralWidget);
        hs_balanceLevel->setObjectName(QString::fromUtf8("hs_balanceLevel"));
        hs_balanceLevel->setValue(50);
        hs_balanceLevel->setOrientation(Qt::Horizontal);

        gridLayout_5->addWidget(hs_balanceLevel, 5, 0, 1, 2);

        label_19 = new QLabel(centralWidget);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setFont(font);
        label_19->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(label_19, 6, 0, 1, 1);

        label_20 = new QLabel(centralWidget);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setFont(font);
        label_20->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(label_20, 6, 1, 1, 1);

        di_ampliLevel = new QDial(centralWidget);
        di_ampliLevel->setObjectName(QString::fromUtf8("di_ampliLevel"));

        gridLayout_5->addWidget(di_ampliLevel, 7, 0, 1, 1);

        di_signalEcho = new QDial(centralWidget);
        di_signalEcho->setObjectName(QString::fromUtf8("di_signalEcho"));

        gridLayout_5->addWidget(di_signalEcho, 7, 1, 1, 1);

        lc_ampliLevel = new QLCDNumber(centralWidget);
        lc_ampliLevel->setObjectName(QString::fromUtf8("lc_ampliLevel"));

        gridLayout_5->addWidget(lc_ampliLevel, 8, 0, 1, 1);

        lc_signalEcho = new QLCDNumber(centralWidget);
        lc_signalEcho->setObjectName(QString::fromUtf8("lc_signalEcho"));

        gridLayout_5->addWidget(lc_signalEcho, 8, 1, 1, 1);

        label_21 = new QLabel(centralWidget);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setFont(font);

        gridLayout_5->addWidget(label_21, 9, 0, 1, 1);

        sb_bufferSize = new QSpinBox(centralWidget);
        sb_bufferSize->setObjectName(QString::fromUtf8("sb_bufferSize"));

        gridLayout_5->addWidget(sb_bufferSize, 9, 1, 1, 1);

        label_22 = new QLabel(centralWidget);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setFont(font);

        gridLayout_5->addWidget(label_22, 10, 0, 1, 1);

        sb_sampleRate = new QSpinBox(centralWidget);
        sb_sampleRate->setObjectName(QString::fromUtf8("sb_sampleRate"));

        gridLayout_5->addWidget(sb_sampleRate, 10, 1, 1, 1);

        pb_left = new QProgressBar(centralWidget);
        pb_left->setObjectName(QString::fromUtf8("pb_left"));
        pb_left->setValue(0);
        pb_left->setTextVisible(true);
        pb_left->setInvertedAppearance(false);

        gridLayout_5->addWidget(pb_left, 11, 0, 1, 2);

        pb_right = new QProgressBar(centralWidget);
        pb_right->setObjectName(QString::fromUtf8("pb_right"));
        pb_right->setValue(0);
        pb_right->setTextVisible(true);
        pb_right->setInvertedAppearance(false);

        gridLayout_5->addWidget(pb_right, 12, 0, 1, 2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer_2, 13, 0, 1, 2);

        reset = new QPushButton(centralWidget);
        reset->setObjectName(QString::fromUtf8("reset"));

        gridLayout_5->addWidget(reset, 14, 0, 1, 2);

        apply = new QPushButton(centralWidget);
        apply->setObjectName(QString::fromUtf8("apply"));

        gridLayout_5->addWidget(apply, 15, 0, 1, 2);


        gridLayout_3->addLayout(gridLayout_5, 0, 2, 3, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        dial_5 = new QDial(centralWidget);
        dial_5->setObjectName(QString::fromUtf8("dial_5"));

        gridLayout_2->addWidget(dial_5, 5, 2, 4, 1);

        dial_6 = new QDial(centralWidget);
        dial_6->setObjectName(QString::fromUtf8("dial_6"));

        gridLayout_2->addWidget(dial_6, 5, 1, 4, 1);

        dial_7 = new QDial(centralWidget);
        dial_7->setObjectName(QString::fromUtf8("dial_7"));

        gridLayout_2->addWidget(dial_7, 5, 0, 4, 1);

        checkBox = new QCheckBox(centralWidget);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        gridLayout_2->addWidget(checkBox, 7, 4, 1, 1);

        checkBox_2 = new QCheckBox(centralWidget);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));

        gridLayout_2->addWidget(checkBox_2, 8, 4, 1, 1);

        checkBox_3 = new QCheckBox(centralWidget);
        checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));

        gridLayout_2->addWidget(checkBox_3, 5, 4, 1, 1);

        checkBox_4 = new QCheckBox(centralWidget);
        checkBox_4->setObjectName(QString::fromUtf8("checkBox_4"));

        gridLayout_2->addWidget(checkBox_4, 6, 4, 1, 1);

        pushButton_6 = new QPushButton(centralWidget);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));

        gridLayout_2->addWidget(pushButton_6, 10, 2, 1, 3);

        label_23 = new QLabel(centralWidget);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        gridLayout_2->addWidget(label_23, 1, 0, 1, 2);

        listWidget_2 = new QListWidget(centralWidget);
        new QListWidgetItem(listWidget_2);
        new QListWidgetItem(listWidget_2);
        new QListWidgetItem(listWidget_2);
        new QListWidgetItem(listWidget_2);
        listWidget_2->setObjectName(QString::fromUtf8("listWidget_2"));
        listWidget_2->setAlternatingRowColors(true);
        listWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
        listWidget_2->setSortingEnabled(false);

        gridLayout_2->addWidget(listWidget_2, 2, 0, 2, 3);

        pushButton_7 = new QPushButton(centralWidget);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));

        gridLayout_2->addWidget(pushButton_7, 2, 4, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_4, 3, 4, 1, 1);

        label_26 = new QLabel(centralWidget);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setFont(font);
        label_26->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_26, 4, 2, 1, 1);

        label_25 = new QLabel(centralWidget);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setFont(font);
        label_25->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_25, 4, 1, 1, 1);

        label_24 = new QLabel(centralWidget);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setFont(font);
        label_24->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_24, 4, 0, 1, 1);

        progressBar_5 = new QProgressBar(centralWidget);
        progressBar_5->setObjectName(QString::fromUtf8("progressBar_5"));
        progressBar_5->setValue(0);

        gridLayout_2->addWidget(progressBar_5, 9, 2, 1, 1);

        progressBar_4 = new QProgressBar(centralWidget);
        progressBar_4->setObjectName(QString::fromUtf8("progressBar_4"));
        progressBar_4->setValue(0);

        gridLayout_2->addWidget(progressBar_4, 9, 1, 1, 1);

        progressBar_3 = new QProgressBar(centralWidget);
        progressBar_3->setObjectName(QString::fromUtf8("progressBar_3"));
        progressBar_3->setValue(0);

        gridLayout_2->addWidget(progressBar_3, 9, 0, 1, 1);

        line = new QFrame(centralWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(line, 4, 3, 6, 1);


        gridLayout_3->addLayout(gridLayout_2, 2, 0, 1, 1);

        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShadow(QFrame::Sunken);
        line_2->setLineWidth(30);
        line_2->setMidLineWidth(4);
        line_2->setFrameShape(QFrame::VLine);

        gridLayout_3->addWidget(line_2, 0, 1, 3, 1);

        line_5 = new QFrame(centralWidget);
        line_5->setObjectName(QString::fromUtf8("line_5"));
        line_5->setFrameShadow(QFrame::Sunken);
        line_5->setLineWidth(30);
        line_5->setMidLineWidth(4);
        line_5->setFrameShape(QFrame::HLine);

        gridLayout_3->addWidget(line_5, 1, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout_3, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        listWidget_2->setCurrentRow(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindow", "&Exit", 0, QApplication::UnicodeUTF8));
        action_Reset_all->setText(QApplication::translate("MainWindow", "&Reset all!!!", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Latitude:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Longitude:", 0, QApplication::UnicodeUTF8));
        comboBoxBuoyType->clear();
        comboBoxBuoyType->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "A", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "B", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "C", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "D", 0, QApplication::UnicodeUTF8)
        );
        label_3->setText(QApplication::translate("MainWindow", "Buoy type:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Speed:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Direction:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "knots", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "degrees", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "\302\272", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MainWindow", "'", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("MainWindow", "''", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("MainWindow", "\302\272", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("MainWindow", "'", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("MainWindow", "''", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("MainWindow", "Reset values", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MainWindow", "Add buoy", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("MainWindow", "Delete Buoy", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt; font-weight:600;\">Sonobuoy simuform</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt; font-weight:600;\">Calibration simuform</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("MainWindow", "N/R level", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("MainWindow", "Volume level", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("MainWindow", "Balance level", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("MainWindow", "Ampli level", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("MainWindow", "Signal echo", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("MainWindow", "Buffer size:", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("MainWindow", "Sample rate:", 0, QApplication::UnicodeUTF8));
        pb_left->setFormat(QApplication::translate("MainWindow", "Left channel %p%", 0, QApplication::UnicodeUTF8));
        pb_right->setFormat(QApplication::translate("MainWindow", "Right channel %p%", 0, QApplication::UnicodeUTF8));
        reset->setText(QApplication::translate("MainWindow", "Reset", 0, QApplication::UnicodeUTF8));
        apply->setText(QApplication::translate("MainWindow", "Apply", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("MainWindow", "Limiter", 0, QApplication::UnicodeUTF8));
        checkBox_2->setText(QApplication::translate("MainWindow", "Phantom", 0, QApplication::UnicodeUTF8));
        checkBox_3->setText(QApplication::translate("MainWindow", "Mono", 0, QApplication::UnicodeUTF8));
        checkBox_4->setText(QApplication::translate("MainWindow", "Mon SW", 0, QApplication::UnicodeUTF8));
        pushButton_6->setText(QApplication::translate("MainWindow", "Save configuration", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt; font-weight:600;\">Sonar simuform</span></p></body></html>", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = listWidget_2->isSortingEnabled();
        listWidget_2->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = listWidget_2->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("MainWindow", "Configuration alfa", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem1 = listWidget_2->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("MainWindow", "Configuration beta", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem2 = listWidget_2->item(2);
        ___qlistwidgetitem2->setText(QApplication::translate("MainWindow", "Configuration gamma", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem3 = listWidget_2->item(3);
        ___qlistwidgetitem3->setText(QApplication::translate("MainWindow", "Configuration delta", 0, QApplication::UnicodeUTF8));
        listWidget_2->setSortingEnabled(__sortingEnabled);

        pushButton_7->setText(QApplication::translate("MainWindow", "Apply", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("MainWindow", "Output", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("MainWindow", "Volume", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("MainWindow", "Sens", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
