#include "dialog.h"
#include "utils.h"
#include "collapsiblesection.h"
#include "SearchResultsController.h"
#include <iostream>
using namespace std;

void Dialog::on_searchButton_clicked() {
    if (!currentVege) {
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Search");

    //Add the viewport to the scroll area
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    //Create a widget and set its layout as your new layout created above
    QWidget *viewport = new QWidget(&dialog);
    //viewport->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy ::Expanding);
    scrollArea->setWidget(viewport);
    scrollArea->setWidgetResizable(true);

    QFormLayout* form = new QFormLayout(viewport);
    viewport->setLayout(form);

    QFormLayout *dialog_layout = new QFormLayout(&dialog);
    dialog.setLayout(dialog_layout);
    dialog.layout()->addWidget(scrollArea);

    /* Generate form items that appear once*/

    /* Company Drop */
    QComboBox* companyDrop = new QComboBox(&dialog);
    for(int i = 0; i< inventory->getCompanyNum(); i++){
        companyDrop->addItem(inventory->getCompany(i).c_str());
    }

    companyDrop->setFont(font);

    QString label2 = QString("Company");
    form->addRow(label2, companyDrop);

    /* Customer Drop */
    QComboBox* customerDrop = new QComboBox(&dialog);
    for(int i = 0; i< inventory->getPersonNum(); i++){
        customerDrop->addItem(inventory->getPerson(i).c_str());
    }

    customerDrop->setFont(font);

    QString label3 = QString("Customer");
    form->addRow(label3, customerDrop);

    /* Date */
    QLineEdit *dateTextField = new QLineEdit(&dialog);
    time_t t = time(nullptr);
    struct tm * now = localtime(&t);
    char buffer[128];
    sprintf(buffer, "%d/%d/%d", now->tm_mon+1, now->tm_mday, now->tm_year - 100);
    dateTextField -> setText(QString::fromUtf8(buffer));
    QString label4 = QString("Date");
    form->addRow(label4, dateTextField);

    /* Button Box
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok, Qt::Horizontal, &dialog);

    dialog_layout->addRow(&buttonBox);

    QHBoxLayout* hLay = new QHBoxLayout();
    hLay->addWidget(&buttonBox);
    dialog_layout->addRow(hLay);
*/
    /* Button Box */
    QToolButton *searchButton = new QToolButton();
    searchButton->setText("Search");

    QHBoxLayout* hLay = new QHBoxLayout();
    hLay->addWidget(searchButton);
    dialog_layout->addRow(hLay);

    SearchResultsController* searchResults = new SearchResultsController(inventory, form, font, scrollArea, &dialog, dateTextField, companyDrop, customerDrop);

    dialog.window()->setFixedWidth(dialog.window()->sizeHint().width() + 100);

    QObject::connect(searchButton, SIGNAL(clicked()),searchResults, SLOT(showSearchResults()));

    dialog.exec();
}

// Not needed for now. will try to show this all in the same dialog.
void Dialog:: showSearchResults(string date, string company, string customer) {
    if (date == "" || (company == "" && customer == "")) {
        QMessageBox messageBox;
        messageBox.critical(nullptr,"Error","You need to set date, and company / customer");
        messageBox.setFixedSize(500,200);
        return;
    }

    QDialog* dialog = new QDialog;
    dialog->setWindowTitle("Search results");

    //Add the viewport to the scroll area
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    //Create a widget and set its layout as your new layout created above
    QWidget *viewport = new QWidget(dialog);
    scrollArea->setWidget(viewport);
    scrollArea->setWidgetResizable(true);

    QFormLayout* form = new QFormLayout(viewport);
    viewport->setLayout(form);

    QFormLayout *dialog_layout = new QFormLayout(dialog);
    dialog->setLayout(dialog_layout);
    dialog->layout()->addWidget(scrollArea);

    bool hasResults = false;
    for (int i = 0; i < inventory->getVegNum(); i++) {
        Vegetable* vegetable = inventory->getVegetableByIndex(i);

        vector<History*> matchingDates = vector<History*>();
        for (int j = 0; j < vegetable->getHistoryNum(); j++) {
            History* history = vegetable->getHistoryObject(j);
            if (history->getDateToCompare() == date) {
                matchingDates.push_back(history);
            }
        }

        vector<History*> matchingCompany = vector<History*>();
        if (company != "") {
            for (unsigned int j = 0; j < matchingDates.size(); j++) {
                History* history = matchingDates[j];
                if (company != "" && company == history->getCompany()) {
                    matchingCompany.push_back(history);
                }
            }
        } else {
            matchingCompany = matchingDates;
        }

        vector<History*> matchingCustomer = vector<History*>();
        if (customer != "") {
            for (unsigned int j = 0; j < matchingCompany.size(); j++) {
                History* history = matchingCompany[j];
                if (customer != "" && customer == history->getCustomer()) {
                    matchingCustomer.push_back(history);
                }
            }
        } else {
            matchingCustomer = matchingCompany;
        }

        if (matchingCustomer.size() == 0) {
            continue;
        } else {
            hasResults = true;
        }

        // Adds the history item here
        auto * anyLayout = new QVBoxLayout();
        QListWidget* historyList = new QListWidget();
        historyList->setFixedWidth(1000);
        anyLayout->addWidget(historyList);
        for (unsigned int i = 0; i < matchingCustomer.size(); i++) {
            historyList->addItem(QString::fromStdString(matchingCustomer[i]->getHistory(vegetable->getUnit())));
        }

        CollapsibleSection* collapsibleSection = new CollapsibleSection(QString(vegetable->getVegetablename().c_str()), 300, nullptr);
        collapsibleSection->setContentLayout(*anyLayout);
        form->addRow(collapsibleSection);
        dialog->adjustSize();

        historyList->setContextMenuPolicy(Qt::ActionsContextMenu);

        editHistoryAction = new QAction(tr("&Edit"), this);
        connect(editHistoryAction, SIGNAL(triggered()), this, SLOT(editHistory()));
        ui->historyList->addAction(editHistoryAction);
    }

    if (!hasResults) {
        QMessageBox messageBox;
        messageBox.critical(nullptr,"Error","No results!");
        messageBox.setFixedSize(500,200);
        return;
    }

    dialog->show();
}

//void Dialog:: editSearchHistory() {
//    QDialog dialog(this);
//    dialog.setWindowTitle("Edit");

//    //Add the viewport to the scroll area
//    QScrollArea *scrollArea = new QScrollArea;
//    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

//    //Create a widget and set its layout as your new layout created above
//    QWidget *viewport = new QWidget(&dialog);
//    scrollArea->setWidget(viewport);
//    scrollArea->setWidgetResizable(true);

//    QFormLayout* form = new QFormLayout(viewport);
//    viewport->setLayout(form);

//    QFormLayout *dialog_layout = new QFormLayout(&dialog);
//    dialog.setLayout(dialog_layout);
//    dialog.layout()->addWidget(scrollArea);

//    History* historyTemp = currentVege->getHistoryObject(ui->historyList->currentRow());
//    string label =  "[Date Sold | " + historyTemp->getDateSold() + "] [company | " + historyTemp -> getCompany() + "]";
//    form -> addRow(new QLabel(QString::fromStdString(label)));
//    /* Price */
//    QLineEdit *priceLineEdit = new QLineEdit(&dialog);
//    string price = historyTemp -> getPrice();
//    priceLineEdit -> setText(QString::fromStdString(price));
//    QString label4 = QString("Price: ");
//    form->addRow(label4, priceLineEdit);

//    /* Button Box */
//    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
//                               Qt::Horizontal, &dialog);
//    form->addRow(&buttonBox);

//    dialog.window()->setFixedWidth(dialog.window()->sizeHint().width() + 100);

//    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
//    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

//    if (dialog.exec() == QDialog::Accepted ) {
//        currentVege->editHistoryPrice(ui->historyList->currentRow(), priceLineEdit->text().toUtf8().constData());
//        needSave = 1;
//        on_vegeList_itemClicked(ui->vegeList->currentItem());
//    }
//}

