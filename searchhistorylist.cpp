#include "searchhistorylist.h"
#include <QDialog>
#include <QScrollArea>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include "History.h"

SearchHistoryList::SearchHistoryList(Vegetable* v, Dialog* d, vector<pair<History*, int>> hp)
{
    vegetable = v;
    dialog = d;
    // Vector of Pairs of History and original index
    historyPairs = hp;

    for (unsigned int i = 0; i < historyPairs.size(); i++) {
        this->addItem(QString::fromStdString(historyPairs[i].first->getHistory(vegetable->getUnit())));
    }
}

void SearchHistoryList:: editSearchHistory() {
    QDialog popup(dialog);
    popup.setWindowTitle("Edit");

    //Add the viewport to the scroll area
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    //Create a widget and set its layout as your new layout created above
    QWidget *viewport = new QWidget(&popup);
    scrollArea->setWidget(viewport);
    scrollArea->setWidgetResizable(true);

    QFormLayout* form = new QFormLayout(viewport);
    viewport->setLayout(form);

    QFormLayout *dialog_layout = new QFormLayout(&popup);
    popup.setLayout(dialog_layout);
    popup.layout()->addWidget(scrollArea);

    History* historyTemp = vegetable->getHistoryObject(historyPairs[this->currentRow()].second);
    string label =  "[Date Sold | " + historyTemp->getDateSold() + "] [company | " + historyTemp -> getCompany() + "]";
    form -> addRow(new QLabel(QString::fromStdString(label)));

    // Price
    QLineEdit *priceLineEdit = new QLineEdit(&popup);
    string price = historyTemp -> getPrice();
    priceLineEdit -> setText(QString::fromStdString(price));
    QString label4 = QString("Price: ");
    form->addRow(label4, priceLineEdit);

    // Button Box
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &popup);
    form->addRow(&buttonBox);

    popup.window()->setFixedWidth(popup.window()->sizeHint().width() + 100);

    QObject::connect(&buttonBox, SIGNAL(accepted()), &popup, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &popup, SLOT(reject()));

    if (popup.exec() == QDialog::Accepted ) {
        vegetable->editHistoryPrice(historyPairs[this->currentRow()].second, priceLineEdit->text().toUtf8().constData());
        dialog->needSave = 1;
        dialog->on_vegeList_itemClicked(dialog->ui->vegeList->currentItem());
        // refresh results in search results controller
        //this->searchResultsController->showSearchResults();
        historyTemp->price = priceLineEdit->text().toUtf8().constData();
        this->item(this->currentRow())->setText(QString::fromStdString(historyTemp->getHistory(vegetable->getUnit())));
    }
}

