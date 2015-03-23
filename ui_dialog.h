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
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(1197, 612);
        verticalLayout_7 = new QVBoxLayout(Dialog);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        label_4 = new QLabel(Dialog);
        label_4->setObjectName(QStringLiteral("label_4"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);

        verticalLayout_7->addWidget(label_4);

        splitter_4 = new QSplitter(Dialog);
        splitter_4->setObjectName(QStringLiteral("splitter_4"));
        splitter_4->setOrientation(Qt::Horizontal);
        layoutWidget = new QWidget(splitter_4);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        verticalLayout_6 = new QVBoxLayout(layoutWidget);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        VegieList = new QLabel(layoutWidget);
        VegieList->setObjectName(QStringLiteral("VegieList"));

        horizontalLayout_6->addWidget(VegieList);

        multiSellButton = new QPushButton(layoutWidget);
        multiSellButton->setObjectName(QStringLiteral("multiSellButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(multiSellButton->sizePolicy().hasHeightForWidth());
        multiSellButton->setSizePolicy(sizePolicy1);

        horizontalLayout_6->addWidget(multiSellButton);


        verticalLayout_6->addLayout(horizontalLayout_6);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        vegeList = new QListWidget(layoutWidget);
        vegeList->setObjectName(QStringLiteral("vegeList"));

        gridLayout->addWidget(vegeList, 1, 0, 1, 1);


        verticalLayout_6->addLayout(gridLayout);

        splitter_4->addWidget(layoutWidget);
        layoutWidget1 = new QWidget(splitter_4);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        verticalLayout_5 = new QVBoxLayout(layoutWidget1);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        splitter_2 = new QSplitter(layoutWidget1);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setOrientation(Qt::Horizontal);
        layoutWidget2 = new QWidget(splitter_2);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        verticalLayout_2 = new QVBoxLayout(layoutWidget2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalLayout_8->setSizeConstraint(QLayout::SetDefaultConstraint);
        Inventory = new QLabel(layoutWidget2);
        Inventory->setObjectName(QStringLiteral("Inventory"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(Inventory->sizePolicy().hasHeightForWidth());
        Inventory->setSizePolicy(sizePolicy2);

        horizontalLayout_8->addWidget(Inventory);

        changeTotal = new QLabel(layoutWidget2);
        changeTotal->setObjectName(QStringLiteral("changeTotal"));

        horizontalLayout_8->addWidget(changeTotal);

        Buy = new QPushButton(layoutWidget2);
        Buy->setObjectName(QStringLiteral("Buy"));
        sizePolicy1.setHeightForWidth(Buy->sizePolicy().hasHeightForWidth());
        Buy->setSizePolicy(sizePolicy1);

        horizontalLayout_8->addWidget(Buy);

        Sell = new QPushButton(layoutWidget2);
        Sell->setObjectName(QStringLiteral("Sell"));
        sizePolicy1.setHeightForWidth(Sell->sizePolicy().hasHeightForWidth());
        Sell->setSizePolicy(sizePolicy1);

        horizontalLayout_8->addWidget(Sell);

        dumpButton = new QPushButton(layoutWidget2);
        dumpButton->setObjectName(QStringLiteral("dumpButton"));
        sizePolicy1.setHeightForWidth(dumpButton->sizePolicy().hasHeightForWidth());
        dumpButton->setSizePolicy(sizePolicy1);

        horizontalLayout_8->addWidget(dumpButton);


        verticalLayout_2->addLayout(horizontalLayout_8);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        breakDown = new QListWidget(layoutWidget2);
        breakDown->setObjectName(QStringLiteral("breakDown"));

        gridLayout_2->addWidget(breakDown, 0, 0, 1, 1);


        verticalLayout_2->addLayout(gridLayout_2);

        splitter_2->addWidget(layoutWidget2);
        layoutWidget3 = new QWidget(splitter_2);
        layoutWidget3->setObjectName(QStringLiteral("layoutWidget3"));
        verticalLayout = new QVBoxLayout(layoutWidget3);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        Returns = new QLabel(layoutWidget3);
        Returns->setObjectName(QStringLiteral("Returns"));

        horizontalLayout_2->addWidget(Returns);

        label = new QLabel(layoutWidget3);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        label_2 = new QLabel(layoutWidget3);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        Return = new QPushButton(layoutWidget3);
        Return->setObjectName(QStringLiteral("Return"));
        sizePolicy1.setHeightForWidth(Return->sizePolicy().hasHeightForWidth());
        Return->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(Return);


        verticalLayout->addLayout(horizontalLayout_2);

        splitter = new QSplitter(layoutWidget3);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Vertical);
        returnList = new QListWidget(splitter);
        returnList->setObjectName(QStringLiteral("returnList"));
        splitter->addWidget(returnList);
        returnToFarm = new QListWidget(splitter);
        returnToFarm->setObjectName(QStringLiteral("returnToFarm"));
        splitter->addWidget(returnToFarm);

        verticalLayout->addWidget(splitter);

        splitter_2->addWidget(layoutWidget3);

        verticalLayout_5->addWidget(splitter_2);

        splitter_3 = new QSplitter(layoutWidget1);
        splitter_3->setObjectName(QStringLiteral("splitter_3"));
        splitter_3->setOrientation(Qt::Horizontal);
        layoutWidget4 = new QWidget(splitter_3);
        layoutWidget4->setObjectName(QStringLiteral("layoutWidget4"));
        verticalLayout_4 = new QVBoxLayout(layoutWidget4);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        Total = new QLabel(layoutWidget4);
        Total->setObjectName(QStringLiteral("Total"));

        horizontalLayout_5->addWidget(Total);

        pushButton = new QPushButton(layoutWidget4);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        sizePolicy1.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy1);

        horizontalLayout_5->addWidget(pushButton);


        verticalLayout_4->addLayout(horizontalLayout_5);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        historyList = new QListWidget(layoutWidget4);
        historyList->setObjectName(QStringLiteral("historyList"));

        gridLayout_3->addWidget(historyList, 0, 0, 1, 1);


        verticalLayout_4->addLayout(gridLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        buyCheck = new QCheckBox(layoutWidget4);
        buyCheck->setObjectName(QStringLiteral("buyCheck"));
        buyCheck->setEnabled(true);
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(buyCheck->sizePolicy().hasHeightForWidth());
        buyCheck->setSizePolicy(sizePolicy3);
        buyCheck->setChecked(true);

        horizontalLayout->addWidget(buyCheck);

        sellCheck = new QCheckBox(layoutWidget4);
        sellCheck->setObjectName(QStringLiteral("sellCheck"));
        sizePolicy3.setHeightForWidth(sellCheck->sizePolicy().hasHeightForWidth());
        sellCheck->setSizePolicy(sizePolicy3);
        sellCheck->setChecked(true);

        horizontalLayout->addWidget(sellCheck);

        dumpCheck = new QCheckBox(layoutWidget4);
        dumpCheck->setObjectName(QStringLiteral("dumpCheck"));
        sizePolicy3.setHeightForWidth(dumpCheck->sizePolicy().hasHeightForWidth());
        dumpCheck->setSizePolicy(sizePolicy3);
        dumpCheck->setChecked(true);

        horizontalLayout->addWidget(dumpCheck);

        returnCheck = new QCheckBox(layoutWidget4);
        returnCheck->setObjectName(QStringLiteral("returnCheck"));
        sizePolicy3.setHeightForWidth(returnCheck->sizePolicy().hasHeightForWidth());
        returnCheck->setSizePolicy(sizePolicy3);
        returnCheck->setChecked(true);

        horizontalLayout->addWidget(returnCheck);

        tuiCheck = new QCheckBox(layoutWidget4);
        tuiCheck->setObjectName(QStringLiteral("tuiCheck"));
        tuiCheck->setChecked(true);

        horizontalLayout->addWidget(tuiCheck);


        horizontalLayout_4->addLayout(horizontalLayout);

        label_3 = new QLabel(layoutWidget4);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_4->addWidget(label_3);

        clearHistoryButton = new QPushButton(layoutWidget4);
        clearHistoryButton->setObjectName(QStringLiteral("clearHistoryButton"));
        sizePolicy1.setHeightForWidth(clearHistoryButton->sizePolicy().hasHeightForWidth());
        clearHistoryButton->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(clearHistoryButton);


        verticalLayout_4->addLayout(horizontalLayout_4);

        splitter_3->addWidget(layoutWidget4);
        layoutWidget5 = new QWidget(splitter_3);
        layoutWidget5->setObjectName(QStringLiteral("layoutWidget5"));
        verticalLayout_3 = new QVBoxLayout(layoutWidget5);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        Memo = new QLabel(layoutWidget5);
        Memo->setObjectName(QStringLiteral("Memo"));

        horizontalLayout_3->addWidget(Memo);

        clearReturnButton = new QPushButton(layoutWidget5);
        clearReturnButton->setObjectName(QStringLiteral("clearReturnButton"));
        sizePolicy1.setHeightForWidth(clearReturnButton->sizePolicy().hasHeightForWidth());
        clearReturnButton->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(clearReturnButton);


        verticalLayout_3->addLayout(horizontalLayout_3);

        Memo_2 = new QTextEdit(layoutWidget5);
        Memo_2->setObjectName(QStringLiteral("Memo_2"));

        verticalLayout_3->addWidget(Memo_2);

        splitter_3->addWidget(layoutWidget5);

        verticalLayout_5->addWidget(splitter_3);

        splitter_4->addWidget(layoutWidget1);

        verticalLayout_7->addWidget(splitter_4);


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
        label_4->setText(QString());
        VegieList->setText(QApplication::translate("Dialog", "\350\217\234\345\220\215\357\274\232", 0));
        multiSellButton->setText(QApplication::translate("Dialog", "Multiple Sell/ \345\277\253\351\200\237\345\215\226", 0));
        Inventory->setText(QApplication::translate("Dialog", "\345\272\253\345\255\230\350\241\250\357\274\232", 0));
        changeTotal->setText(QString());
        Buy->setText(QApplication::translate("Dialog", "\350\262\267", 0));
        Sell->setText(QApplication::translate("Dialog", "\350\263\243", 0));
        dumpButton->setText(QApplication::translate("Dialog", "\345\200\222", 0));
        Returns->setText(QApplication::translate("Dialog", "\351\200\200\350\262\250\345\226\256\357\274\232", 0));
        label->setText(QString());
        label_2->setText(QString());
        Return->setText(QApplication::translate("Dialog", "\351\200\200\350\262\250", 0));
        Total->setText(QApplication::translate("Dialog", "\346\255\267\345\217\262:", 0));
        pushButton->setText(QApplication::translate("Dialog", "PushButton", 0));
        buyCheck->setText(QApplication::translate("Dialog", "\350\262\267", 0));
        sellCheck->setText(QApplication::translate("Dialog", "\350\263\243", 0));
        dumpCheck->setText(QApplication::translate("Dialog", "\345\200\222", 0));
        returnCheck->setText(QApplication::translate("Dialog", "\351\200\200\347\265\246\345\205\254\345\217\270", 0));
        tuiCheck->setText(QApplication::translate("Dialog", "\351\200\200\347\265\246\350\276\262\345\240\264", 0));
        label_3->setText(QString());
        clearHistoryButton->setText(QApplication::translate("Dialog", "\346\270\205\346\216\211\346\255\267\345\217\262", 0));
        Memo->setText(QApplication::translate("Dialog", "Memo:", 0));
        clearReturnButton->setText(QApplication::translate("Dialog", "\346\270\205\346\216\211\351\200\200\350\262\250\345\226\256", 0));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
