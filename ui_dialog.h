/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QVBoxLayout *verticalLayout_7;
    QLabel *label_4;
    QSplitter *splitter_4;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_6;
    QLabel *VegieList;
    QPushButton *multiSellButton;
    QGridLayout *gridLayout;
    QListWidget *vegeList;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_5;
    QSplitter *splitter_2;
    QWidget *layoutWidget2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_8;
    QLabel *Inventory;
    QLabel *changeTotal;
    QPushButton *Buy;
    QPushButton *Sell;
    QPushButton *dumpButton;
    QPushButton *CalculateSold;
    QGridLayout *gridLayout_2;
    QListWidget *breakDown;
    QWidget *layoutWidget3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *Returns;
    QLabel *label;
    QLabel *label_2;
    QPushButton *Return;
    QSplitter *splitter;
    QListWidget *returnList;
    QListWidget *returnToFarm;
    QSplitter *splitter_3;
    QWidget *layoutWidget4;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_5;
    QLabel *Total;
    QPushButton *pushButton;
    QGridLayout *gridLayout_3;
    QListWidget *historyList;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout;
    QCheckBox *buyCheck;
    QCheckBox *sellCheck;
    QCheckBox *dumpCheck;
    QCheckBox *returnCheck;
    QCheckBox *tuiCheck;
    QLabel *label_3;
    QPushButton *clearHistoryButton;
    QWidget *layoutWidget5;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *Memo;
    QPushButton *clearReturnButton;
    QTextEdit *Memo_2;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(1197, 612);
        verticalLayout_7 = new QVBoxLayout(Dialog);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        label_4 = new QLabel(Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);

        verticalLayout_7->addWidget(label_4);

        splitter_4 = new QSplitter(Dialog);
        splitter_4->setObjectName(QString::fromUtf8("splitter_4"));
        splitter_4->setOrientation(Qt::Horizontal);
        layoutWidget = new QWidget(splitter_4);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        verticalLayout_6 = new QVBoxLayout(layoutWidget);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        VegieList = new QLabel(layoutWidget);
        VegieList->setObjectName(QString::fromUtf8("VegieList"));

        horizontalLayout_6->addWidget(VegieList);

        multiSellButton = new QPushButton(layoutWidget);
        multiSellButton->setObjectName(QString::fromUtf8("multiSellButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(multiSellButton->sizePolicy().hasHeightForWidth());
        multiSellButton->setSizePolicy(sizePolicy1);

        horizontalLayout_6->addWidget(multiSellButton);


        verticalLayout_6->addLayout(horizontalLayout_6);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        vegeList = new QListWidget(layoutWidget);
        vegeList->setObjectName(QString::fromUtf8("vegeList"));

        gridLayout->addWidget(vegeList, 1, 0, 1, 1);


        verticalLayout_6->addLayout(gridLayout);

        splitter_4->addWidget(layoutWidget);
        layoutWidget1 = new QWidget(splitter_4);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        verticalLayout_5 = new QVBoxLayout(layoutWidget1);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        splitter_2 = new QSplitter(layoutWidget1);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Horizontal);
        layoutWidget2 = new QWidget(splitter_2);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        verticalLayout_2 = new QVBoxLayout(layoutWidget2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setSizeConstraint(QLayout::SetDefaultConstraint);
        Inventory = new QLabel(layoutWidget2);
        Inventory->setObjectName(QString::fromUtf8("Inventory"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(Inventory->sizePolicy().hasHeightForWidth());
        Inventory->setSizePolicy(sizePolicy2);

        horizontalLayout_8->addWidget(Inventory);

        changeTotal = new QLabel(layoutWidget2);
        changeTotal->setObjectName(QString::fromUtf8("changeTotal"));

        horizontalLayout_8->addWidget(changeTotal);

        Buy = new QPushButton(layoutWidget2);
        Buy->setObjectName(QString::fromUtf8("Buy"));
        sizePolicy1.setHeightForWidth(Buy->sizePolicy().hasHeightForWidth());
        Buy->setSizePolicy(sizePolicy1);

        horizontalLayout_8->addWidget(Buy);

        Sell = new QPushButton(layoutWidget2);
        Sell->setObjectName(QString::fromUtf8("Sell"));
        sizePolicy1.setHeightForWidth(Sell->sizePolicy().hasHeightForWidth());
        Sell->setSizePolicy(sizePolicy1);

        horizontalLayout_8->addWidget(Sell);

        dumpButton = new QPushButton(layoutWidget2);
        dumpButton->setObjectName(QString::fromUtf8("dumpButton"));
        sizePolicy1.setHeightForWidth(dumpButton->sizePolicy().hasHeightForWidth());
        dumpButton->setSizePolicy(sizePolicy1);

        horizontalLayout_8->addWidget(dumpButton);

        CalculateSold = new QPushButton(layoutWidget2);
        CalculateSold->setObjectName(QString::fromUtf8("CalculateSold"));
        CalculateSold->setMinimumSize(QSize(99, 0));

        horizontalLayout_8->addWidget(CalculateSold);


        verticalLayout_2->addLayout(horizontalLayout_8);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        breakDown = new QListWidget(layoutWidget2);
        breakDown->setObjectName(QString::fromUtf8("breakDown"));

        gridLayout_2->addWidget(breakDown, 0, 0, 1, 1);


        verticalLayout_2->addLayout(gridLayout_2);

        splitter_2->addWidget(layoutWidget2);
        layoutWidget3 = new QWidget(splitter_2);
        layoutWidget3->setObjectName(QString::fromUtf8("layoutWidget3"));
        verticalLayout = new QVBoxLayout(layoutWidget3);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        Returns = new QLabel(layoutWidget3);
        Returns->setObjectName(QString::fromUtf8("Returns"));

        horizontalLayout_2->addWidget(Returns);

        label = new QLabel(layoutWidget3);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        label_2 = new QLabel(layoutWidget3);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        Return = new QPushButton(layoutWidget3);
        Return->setObjectName(QString::fromUtf8("Return"));
        sizePolicy1.setHeightForWidth(Return->sizePolicy().hasHeightForWidth());
        Return->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(Return);


        verticalLayout->addLayout(horizontalLayout_2);

        splitter = new QSplitter(layoutWidget3);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        returnList = new QListWidget(splitter);
        returnList->setObjectName(QString::fromUtf8("returnList"));
        splitter->addWidget(returnList);
        returnToFarm = new QListWidget(splitter);
        returnToFarm->setObjectName(QString::fromUtf8("returnToFarm"));
        splitter->addWidget(returnToFarm);

        verticalLayout->addWidget(splitter);

        splitter_2->addWidget(layoutWidget3);

        verticalLayout_5->addWidget(splitter_2);

        splitter_3 = new QSplitter(layoutWidget1);
        splitter_3->setObjectName(QString::fromUtf8("splitter_3"));
        splitter_3->setOrientation(Qt::Horizontal);
        layoutWidget4 = new QWidget(splitter_3);
        layoutWidget4->setObjectName(QString::fromUtf8("layoutWidget4"));
        verticalLayout_4 = new QVBoxLayout(layoutWidget4);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        Total = new QLabel(layoutWidget4);
        Total->setObjectName(QString::fromUtf8("Total"));

        horizontalLayout_5->addWidget(Total);

        pushButton = new QPushButton(layoutWidget4);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        sizePolicy1.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy1);

        horizontalLayout_5->addWidget(pushButton);


        verticalLayout_4->addLayout(horizontalLayout_5);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        historyList = new QListWidget(layoutWidget4);
        historyList->setObjectName(QString::fromUtf8("historyList"));

        gridLayout_3->addWidget(historyList, 0, 0, 1, 1);


        verticalLayout_4->addLayout(gridLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        buyCheck = new QCheckBox(layoutWidget4);
        buyCheck->setObjectName(QString::fromUtf8("buyCheck"));
        buyCheck->setEnabled(true);
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(buyCheck->sizePolicy().hasHeightForWidth());
        buyCheck->setSizePolicy(sizePolicy3);
        buyCheck->setChecked(true);

        horizontalLayout->addWidget(buyCheck);

        sellCheck = new QCheckBox(layoutWidget4);
        sellCheck->setObjectName(QString::fromUtf8("sellCheck"));
        sizePolicy3.setHeightForWidth(sellCheck->sizePolicy().hasHeightForWidth());
        sellCheck->setSizePolicy(sizePolicy3);
        sellCheck->setChecked(true);

        horizontalLayout->addWidget(sellCheck);

        dumpCheck = new QCheckBox(layoutWidget4);
        dumpCheck->setObjectName(QString::fromUtf8("dumpCheck"));
        sizePolicy3.setHeightForWidth(dumpCheck->sizePolicy().hasHeightForWidth());
        dumpCheck->setSizePolicy(sizePolicy3);
        dumpCheck->setChecked(true);

        horizontalLayout->addWidget(dumpCheck);

        returnCheck = new QCheckBox(layoutWidget4);
        returnCheck->setObjectName(QString::fromUtf8("returnCheck"));
        sizePolicy3.setHeightForWidth(returnCheck->sizePolicy().hasHeightForWidth());
        returnCheck->setSizePolicy(sizePolicy3);
        returnCheck->setChecked(true);

        horizontalLayout->addWidget(returnCheck);

        tuiCheck = new QCheckBox(layoutWidget4);
        tuiCheck->setObjectName(QString::fromUtf8("tuiCheck"));
        tuiCheck->setChecked(true);

        horizontalLayout->addWidget(tuiCheck);


        horizontalLayout_4->addLayout(horizontalLayout);

        label_3 = new QLabel(layoutWidget4);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_4->addWidget(label_3);

        clearHistoryButton = new QPushButton(layoutWidget4);
        clearHistoryButton->setObjectName(QString::fromUtf8("clearHistoryButton"));
        sizePolicy1.setHeightForWidth(clearHistoryButton->sizePolicy().hasHeightForWidth());
        clearHistoryButton->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(clearHistoryButton);


        verticalLayout_4->addLayout(horizontalLayout_4);

        splitter_3->addWidget(layoutWidget4);
        layoutWidget5 = new QWidget(splitter_3);
        layoutWidget5->setObjectName(QString::fromUtf8("layoutWidget5"));
        verticalLayout_3 = new QVBoxLayout(layoutWidget5);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        Memo = new QLabel(layoutWidget5);
        Memo->setObjectName(QString::fromUtf8("Memo"));

        horizontalLayout_3->addWidget(Memo);

        clearReturnButton = new QPushButton(layoutWidget5);
        clearReturnButton->setObjectName(QString::fromUtf8("clearReturnButton"));
        sizePolicy1.setHeightForWidth(clearReturnButton->sizePolicy().hasHeightForWidth());
        clearReturnButton->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(clearReturnButton);


        verticalLayout_3->addLayout(horizontalLayout_3);

        Memo_2 = new QTextEdit(layoutWidget5);
        Memo_2->setObjectName(QString::fromUtf8("Memo_2"));

        verticalLayout_3->addWidget(Memo_2);

        splitter_3->addWidget(layoutWidget5);

        verticalLayout_5->addWidget(splitter_3);

        splitter_4->addWidget(layoutWidget1);

        verticalLayout_7->addWidget(splitter_4);


        retranslateUi(Dialog);
        QObject::connect(returnCheck, SIGNAL(clicked()), Dialog, SLOT(slot1()));
        QObject::connect(tuiCheck, SIGNAL(clicked()), Dialog, SLOT(slot1()));
        QObject::connect(buyCheck, SIGNAL(toggled(bool)), Dialog, SLOT(slot1()));
        QObject::connect(dumpCheck, SIGNAL(clicked()), Dialog, SLOT(slot1()));
        QObject::connect(sellCheck, SIGNAL(clicked()), Dialog, SLOT(slot1()));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", nullptr));
        label_4->setText(QString());
        VegieList->setText(QApplication::translate("Dialog", "\350\217\234\345\220\215\357\274\232", nullptr));
        multiSellButton->setText(QApplication::translate("Dialog", "Multiple Sell/ \345\277\253\351\200\237\345\215\226", nullptr));
        Inventory->setText(QApplication::translate("Dialog", "\345\272\253\345\255\230\350\241\250\357\274\232", nullptr));
        changeTotal->setText(QString());
        Buy->setText(QApplication::translate("Dialog", "\350\262\267", nullptr));
        Sell->setText(QApplication::translate("Dialog", "\350\263\243", nullptr));
        dumpButton->setText(QApplication::translate("Dialog", "\345\200\222", nullptr));
        CalculateSold->setText(QApplication::translate("Dialog", "Calculate", nullptr));
        Returns->setText(QApplication::translate("Dialog", "\351\200\200\350\262\250\345\226\256\357\274\232", nullptr));
        label->setText(QString());
        label_2->setText(QString());
        Return->setText(QApplication::translate("Dialog", "\351\200\200\350\262\250", nullptr));
        Total->setText(QApplication::translate("Dialog", "\346\255\267\345\217\262:", nullptr));
        pushButton->setText(QApplication::translate("Dialog", "PushButton", nullptr));
        buyCheck->setText(QApplication::translate("Dialog", "\350\262\267", nullptr));
        sellCheck->setText(QApplication::translate("Dialog", "\350\263\243", nullptr));
        dumpCheck->setText(QApplication::translate("Dialog", "\345\200\222", nullptr));
        returnCheck->setText(QApplication::translate("Dialog", "\351\200\200\347\265\246\345\205\254\345\217\270", nullptr));
        tuiCheck->setText(QApplication::translate("Dialog", "\351\200\200\347\265\246\350\276\262\345\240\264", nullptr));
        label_3->setText(QString());
        clearHistoryButton->setText(QApplication::translate("Dialog", "\346\270\205\346\216\211\346\255\267\345\217\262", nullptr));
        Memo->setText(QApplication::translate("Dialog", "Memo:", nullptr));
        clearReturnButton->setText(QApplication::translate("Dialog", "\346\270\205\346\216\211\351\200\200\350\262\250\345\226\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
