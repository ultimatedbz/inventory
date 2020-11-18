#include "dialog.h"
#include "utils.h"
#include "collapsiblesection.h"
#include <iostream>
using namespace std;

void Dialog::on_Sell_clicked()
{
    if(!currentVege)
        return;

    if(inventory->numberOfNonEmptyVeges()){
        QDialog dialog(this);
        dialog.setWindowTitle(mTranslator->translate("賣").c_str());

        //Add the viewport to the scroll area
        QScrollArea *scrollArea = new QScrollArea;
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        //Create a widget and set its layout as your new layout created above
        QWidget *viewport = new QWidget(&dialog);
        scrollArea->setWidget(viewport);
        scrollArea->setWidgetResizable(true);

        QFormLayout* form = new QFormLayout(viewport);
        viewport->setLayout(form);

        QFormLayout *dialog_layout = new QFormLayout(&dialog);
        dialog.setLayout(dialog_layout);
        dialog.layout()->addWidget(scrollArea);

        /* Generate form items that appear once*/

        /* Customer Drop */
        QComboBox* customerDrop = new QComboBox(&dialog);
        for(int i = 0; i< inventory->getPersonNum(); i++){
            customerDrop->addItem(inventory->getPerson(i).c_str());
        }

        customerDrop->setFont(font);

        QString label2 = QString(mTranslator->translate("賣给谁？").c_str());
        form->addRow(label2, customerDrop);

        /* Date */
        QLineEdit *date = new QLineEdit(&dialog);
        time_t t = time(nullptr);
        struct tm * now = localtime(&t);
        char buffer[128];
        sprintf(buffer, "%d/%d/%d", now->tm_mon+1, now->tm_mday, now->tm_year - 100);
        date -> setText(QString::fromUtf8(buffer));
        QString label4 = QString(mTranslator->translate("那天賣出").c_str());
        form->addRow(label4, date);

        MultiSellController* multiSellController = new MultiSellController(
                    inventory, form, font, scrollArea, &dialog, mTranslator);

        /* Button Box */
        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                   Qt::Horizontal, &dialog);

        dialog_layout->addRow(&buttonBox);
        QToolButton *tb = new QToolButton();
        tb->setText("+");


        QToolButton *tb1 = new QToolButton();
        tb1->setText("-");

        QHBoxLayout* hLay = new QHBoxLayout();
        hLay->addWidget(tb);
        hLay->addWidget(tb1);
        hLay->addWidget(&buttonBox);
        dialog_layout->addRow(hLay);

        dialog.window()->setFixedWidth(dialog.window()->sizeHint().width() + 100);

        QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
        QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
        QObject::connect(tb, SIGNAL(clicked()),multiSellController, SLOT(addElement()));
        QObject::connect(tb1, SIGNAL(clicked()),multiSellController, SLOT(subtractElement()));
        if (dialog.exec() == QDialog::Accepted ) {
            for( int i = 0; i < multiSellController->getElementNum(); i++){
                int vegeIndex = multiSellController->
                        getActualVegeIndex(i);
                string customer = customerDrop->currentText().toUtf8().constData();
                string amountString = multiSellController->
                        getAmounts(i);
                string price = multiSellController->getPrice(i);
                vector<int> remainIndexes = multiSellController->
                        getActualRemainingIndexes(i);

                stringstream ss(amountString);
                vector<string> amounts;

                while (ss.good())
                {
                    string substr;
                    getline(ss, substr, ',');
                    amounts.push_back(substr);
                }

                if(amounts.size() != remainIndexes.size())
                    continue;

                vector<pair<int, int> > indexToPrice = vector<pair<int,int> >();

                /* Link up remainindexes to amounts */
                for(unsigned long j = 0; j < amounts.size(); j++){
                    indexToPrice.push_back(
                                make_pair(remainIndexes[amounts.size() - 1 - j],
                                atoi(amounts[j].c_str())));
                }
                /* Sort the remainIndexes so that they are sold from greatest to least */
                sort(indexToPrice.begin(),indexToPrice.end(),myobject);

                /* Sell the remainIndexes, greatest to least */
                for(vector<pair<int,int> >::iterator it = indexToPrice.begin(); it != indexToPrice.end(); it++){

                    inventory->getVegetableByIndex(vegeIndex)->sellVege(
                                (*it).second, customer, date->text().toStdString(),
                                price, (*it).first);
                }

                /*for(int j = 0; j < amounts.size(); j++){
          inventory->getVegetableByIndex(vegeIndex)->sellVege(
                atoi(amounts[j].c_str()), customer, date->text().toStdString(),
                price, remainIndexes[amounts.size() - 1 - j]);
        }*/
            }
            on_vegeList_itemClicked(ui->vegeList->currentItem());
        }
        /* Deallocate memory*/

    }else{
        QMessageBox messageBox;
        messageBox.critical(nullptr,"警告","No Vegetables to Sell");
        messageBox.setFixedSize(500,200);
    }
}
