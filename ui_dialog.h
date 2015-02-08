/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QGridLayout *gridLayout_5;
    QHBoxLayout *horizontalLayout;
    QCheckBox *buyCheck;
    QCheckBox *sellCheck;
    QCheckBox *dumpCheck;
    QCheckBox *returnCheck;
    QCheckBox *tuiCheck;
    QLabel *Total;
    QGridLayout *gridLayout_4;
    QListWidget *returnList;
    QListWidget *returnToFarm;
    QTextEdit *Memo_2;
    QGridLayout *gridLayout_3;
    QListWidget *historyList;
    QPushButton *clearHistoryButton;
    QHBoxLayout *horizontalLayout_9;
    QLabel *Returns;
    QLabel *label;
    QLabel *label_2;
    QPushButton *Return;
    QLabel *VegieList;
    QGridLayout *gridLayout;
    QListWidget *vegeList;
    QGridLayout *gridLayout_2;
    QListWidget *breakDown;
    QLabel *Memo;
    QPushButton *clearReturnButton;
    QHBoxLayout *horizontalLayout_8;
    QLabel *Inventory;
    QLabel *changeTotal;
    QPushButton *Buy;
    QPushButton *Sell;
    QPushButton *dumpButton;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(1197, 612);
        gridLayout_5 = new QGridLayout(Dialog);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        buyCheck = new QCheckBox(Dialog);
        buyCheck->setObjectName(QStringLiteral("buyCheck"));
        buyCheck->setEnabled(true);
        buyCheck->setChecked(true);

        horizontalLayout->addWidget(buyCheck);

        sellCheck = new QCheckBox(Dialog);
        sellCheck->setObjectName(QStringLiteral("sellCheck"));
        sellCheck->setChecked(true);

        horizontalLayout->addWidget(sellCheck);

        dumpCheck = new QCheckBox(Dialog);
        dumpCheck->setObjectName(QStringLiteral("dumpCheck"));
        dumpCheck->setChecked(true);

        horizontalLayout->addWidget(dumpCheck);

        returnCheck = new QCheckBox(Dialog);
        returnCheck->setObjectName(QStringLiteral("returnCheck"));
        returnCheck->setChecked(true);

        horizontalLayout->addWidget(returnCheck);

        tuiCheck = new QCheckBox(Dialog);
        tuiCheck->setObjectName(QStringLiteral("tuiCheck"));
        tuiCheck->setChecked(true);

        horizontalLayout->addWidget(tuiCheck);


        gridLayout_5->addLayout(horizontalLayout, 4, 1, 1, 1);

        Total = new QLabel(Dialog);
        Total->setObjectName(QStringLiteral("Total"));

        gridLayout_5->addWidget(Total, 2, 1, 1, 1);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        returnList = new QListWidget(Dialog);
        returnList->setObjectName(QStringLiteral("returnList"));

        gridLayout_4->addWidget(returnList, 1, 0, 1, 1);

        returnToFarm = new QListWidget(Dialog);
        returnToFarm->setObjectName(QStringLiteral("returnToFarm"));

        gridLayout_4->addWidget(returnToFarm, 2, 0, 1, 1);


        gridLayout_5->addLayout(gridLayout_4, 1, 2, 1, 3);

        Memo_2 = new QTextEdit(Dialog);
        Memo_2->setObjectName(QStringLiteral("Memo_2"));

        gridLayout_5->addWidget(Memo_2, 3, 3, 1, 2);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        historyList = new QListWidget(Dialog);
        historyList->setObjectName(QStringLiteral("historyList"));

        gridLayout_3->addWidget(historyList, 0, 0, 1, 1);


        gridLayout_5->addLayout(gridLayout_3, 3, 1, 1, 2);

        clearHistoryButton = new QPushButton(Dialog);
        clearHistoryButton->setObjectName(QStringLiteral("clearHistoryButton"));

        gridLayout_5->addWidget(clearHistoryButton, 4, 2, 1, 1);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        Returns = new QLabel(Dialog);
        Returns->setObjectName(QStringLiteral("Returns"));

        horizontalLayout_9->addWidget(Returns);

        label = new QLabel(Dialog);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_9->addWidget(label);

        label_2 = new QLabel(Dialog);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_9->addWidget(label_2);

        Return = new QPushButton(Dialog);
        Return->setObjectName(QStringLiteral("Return"));

        horizontalLayout_9->addWidget(Return);


        gridLayout_5->addLayout(horizontalLayout_9, 0, 2, 1, 3);

        VegieList = new QLabel(Dialog);
        VegieList->setObjectName(QStringLiteral("VegieList"));

        gridLayout_5->addWidget(VegieList, 0, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        vegeList = new QListWidget(Dialog);
        vegeList->setObjectName(QStringLiteral("vegeList"));

        gridLayout->addWidget(vegeList, 1, 0, 1, 1);


        gridLayout_5->addLayout(gridLayout, 1, 0, 3, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        breakDown = new QListWidget(Dialog);
        breakDown->setObjectName(QStringLiteral("breakDown"));

        gridLayout_2->addWidget(breakDown, 0, 0, 1, 1);


        gridLayout_5->addLayout(gridLayout_2, 1, 1, 1, 1);

        Memo = new QLabel(Dialog);
        Memo->setObjectName(QStringLiteral("Memo"));

        gridLayout_5->addWidget(Memo, 2, 3, 1, 1);

        clearReturnButton = new QPushButton(Dialog);
        clearReturnButton->setObjectName(QStringLiteral("clearReturnButton"));

        gridLayout_5->addWidget(clearReturnButton, 2, 4, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        Inventory = new QLabel(Dialog);
        Inventory->setObjectName(QStringLiteral("Inventory"));

        horizontalLayout_8->addWidget(Inventory);

        changeTotal = new QLabel(Dialog);
        changeTotal->setObjectName(QStringLiteral("changeTotal"));

        horizontalLayout_8->addWidget(changeTotal);

        Buy = new QPushButton(Dialog);
        Buy->setObjectName(QStringLiteral("Buy"));

        horizontalLayout_8->addWidget(Buy);

        Sell = new QPushButton(Dialog);
        Sell->setObjectName(QStringLiteral("Sell"));

        horizontalLayout_8->addWidget(Sell);

        dumpButton = new QPushButton(Dialog);
        dumpButton->setObjectName(QStringLiteral("dumpButton"));

        horizontalLayout_8->addWidget(dumpButton);


        gridLayout_5->addLayout(horizontalLayout_8, 0, 1, 1, 1);


        retranslateUi(Dialog);
        QObject::connect(buyCheck, SIGNAL(toggled(bool)), Dialog, SLOT(slot1()));
        QObject::connect(sellCheck, SIGNAL(clicked()), Dialog, SLOT(slot1()));
        QObject::connect(dumpCheck, SIGNAL(clicked()), Dialog, SLOT(slot1()));
        QObject::connect(returnCheck, SIGNAL(clicked()), Dialog, SLOT(slot1()));
        QObject::connect(tuiCheck, SIGNAL(clicked()), Dialog, SLOT(slot1()));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0));
        buyCheck->setText(QApplication::translate("Dialog", "\350\262\267", 0));
        sellCheck->setText(QApplication::translate("Dialog", "\350\263\243", 0));
        dumpCheck->setText(QApplication::translate("Dialog", "\345\200\222", 0));
        returnCheck->setText(QApplication::translate("Dialog", "\351\200\200\347\265\246\345\205\254\345\217\270", 0));
        tuiCheck->setText(QApplication::translate("Dialog", "\351\200\200\347\265\246\350\276\262\345\240\264", 0));
        Total->setText(QApplication::translate("Dialog", "\346\255\267\345\217\262:", 0));
        clearHistoryButton->setText(QApplication::translate("Dialog", "\346\270\205\346\216\211\346\255\267\345\217\262", 0));
        Returns->setText(QApplication::translate("Dialog", "\351\200\200\350\262\250\345\226\256\357\274\232", 0));
        label->setText(QString());
        label_2->setText(QString());
        Return->setText(QApplication::translate("Dialog", "\351\200\200\350\262\250", 0));
        VegieList->setText(QApplication::translate("Dialog", "\350\217\234\345\220\215\357\274\232", 0));
        Memo->setText(QApplication::translate("Dialog", "Memo:", 0));
        clearReturnButton->setText(QApplication::translate("Dialog", "\346\270\205\346\216\211\351\200\200\350\262\250\345\226\256", 0));
        Inventory->setText(QApplication::translate("Dialog", "\345\272\253\345\255\230\350\241\250\357\274\232", 0));
        changeTotal->setText(QString());
        Buy->setText(QApplication::translate("Dialog", "\350\262\267", 0));
        Sell->setText(QApplication::translate("Dialog", "\350\263\243", 0));
        dumpButton->setText(QApplication::translate("Dialog", "\345\200\222", 0));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
