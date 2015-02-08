/********************************************************************************
** Form generated from reading UI file 'returns.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RETURNS_H
#define UI_RETURNS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_10;
    QLabel *Returns_2;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *Return_2;
    QGridLayout *gridLayout_5;
    QListWidget *returnToFarm_2;
    QListWidget *returnList_2;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->resize(295, 466);
        verticalLayout = new QVBoxLayout(Form);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        Returns_2 = new QLabel(Form);
        Returns_2->setObjectName(QStringLiteral("Returns_2"));

        horizontalLayout_10->addWidget(Returns_2);

        label_3 = new QLabel(Form);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_10->addWidget(label_3);

        label_4 = new QLabel(Form);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_10->addWidget(label_4);

        Return_2 = new QPushButton(Form);
        Return_2->setObjectName(QStringLiteral("Return_2"));

        horizontalLayout_10->addWidget(Return_2);


        verticalLayout->addLayout(horizontalLayout_10);

        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        returnToFarm_2 = new QListWidget(Form);
        returnToFarm_2->setObjectName(QStringLiteral("returnToFarm_2"));

        gridLayout_5->addWidget(returnToFarm_2, 2, 0, 1, 1);

        returnList_2 = new QListWidget(Form);
        returnList_2->setObjectName(QStringLiteral("returnList_2"));

        gridLayout_5->addWidget(returnList_2, 1, 0, 1, 1);


        verticalLayout->addLayout(gridLayout_5);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0));
        Returns_2->setText(QApplication::translate("Form", "\351\200\200\350\262\250\345\226\256\357\274\232", 0));
        label_3->setText(QString());
        label_4->setText(QString());
        Return_2->setText(QApplication::translate("Form", "\351\200\200\350\262\250", 0));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RETURNS_H
