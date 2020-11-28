#include "dialog.h"
#include "utils.h"
#include "collapsiblesection.h"
#include "searchResultsController.h"
#include "searchhistorylist.h"

SearchResultsController::SearchResultsController(Inventory* inventory,
                                                 QFormLayout* fo, QFont f,
                                                 QScrollArea* sa,
                                                 QDialog* d, QLineEdit* dateTextFieldd, QComboBox* companyDropp, QComboBox* customerDropp, Dialog* md):
  font(f),
  form(fo),
  mInventory(inventory),
  scrollArea(sa),
  dialog(d),
  dateTextField(dateTextFieldd),
  companyDrop(companyDropp),
  customerDrop(customerDropp),
  mainDialog(md)
{
    calculateButtons = vector<QPushButton*>();
    searchHistoryLists = vector<SearchHistoryList*>();
}

SearchResultsController::~SearchResultsController()
{

}

void SearchResultsController:: showSearchResults() {
    string company = companyDrop->currentText().toUtf8().constData();
    string customer = customerDrop->currentText().toUtf8().constData();
    string date = dateTextField->text().toStdString();

    if (date == "" || (company == "" && customer == "")) {
        QMessageBox messageBox;
        messageBox.critical(nullptr,"Error","You need to set company or customer!");
        messageBox.setFixedSize(500,200);
        return;
    }

    // Clear all results
    while (form->rowCount() > 4) {
        form->removeRow(4);
    }

    bool hasResults = false;
    for (int i = 0; i < mInventory->getVegNum(); i++) {
        Vegetable* vegetable = mInventory->getVegetableByIndex(i);
        vector<pair<History*, int>> histories = vector<pair<History*, int>>();

        for (int j = 0; j < vegetable->getHistoryNum(); j++) {
            histories.push_back(make_pair(vegetable->getHistoryObject(j), j));
        }

        // Filter by Customer first
        vector<pair<History*, int>> matchingCustomer = vector<pair<History*, int>>();
        if (customer != "") {
            for (unsigned int j = 0; j < histories.size(); j++) {
                pair<History*, int> history = histories[j];
                if (customer == history.first->getCustomer()) {
                    matchingCustomer.push_back(history);
                }
            }
        } else {
            matchingCustomer = histories;
        }

        // Filter by Company
        vector<pair<History*, int>> matchingCompany = vector<pair<History*, int>>();
        if (company != "") {
            for (unsigned int j = 0; j < matchingCustomer.size(); j++) {
                pair<History*, int> history = matchingCustomer[j];
                if (company == history.first->getCompany()) {
                    matchingCompany.push_back(history);
                }
            }
        } else {
            matchingCompany = matchingCustomer;
        }

        // Filter by Date, but choose date based on transaction type
        vector<pair<History*, int>> matchingDates = vector<pair<History*, int>>();
        for (unsigned int j = 0; j < matchingCompany.size(); j++) {
            pair<History*, int> history = matchingCompany[j];

            if (history.first->getType() == "Buy") {
                if (date == history.first->getDatePurchased()) {
                    matchingDates.push_back(history);
                }
            } else if (history.first->getType() == "Sell" || history.first->getType() == "Tui" || history.first->getType() == "Return" || history.first->getType() == "Dump") {
                // If searched a customer, check against date sold
                if (date == history.first->getDateSold() && customer != "") {
                    matchingDates.push_back(history);
                // If searched a company, check against date purchased
                } else if (date == history.first->getDatePurchased() && company != "") {
                    matchingDates.push_back(history);
                }
            }
        }

        if (matchingDates.size() == 0) {
            continue;
        } else {
            hasResults = true;
        }

        // Adds the history item here
        auto * anyLayout = new QVBoxLayout();
        SearchHistoryList* historyList = new SearchHistoryList(vegetable, mainDialog, matchingDates, date, company);
        historyList->setBaseSize(700, 100);
        historyList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        anyLayout->addWidget(historyList);
        searchHistoryLists.push_back(historyList);

        QPushButton* calculateButton = new QPushButton();
        calculateButton->setText("Calculate");
        anyLayout->addWidget(calculateButton);
        calculateButtons.push_back(calculateButton);
        QObject::connect(calculateButton, SIGNAL(clicked()),this, SLOT(calculateHistory()));

        CollapsibleSection* collapsibleSection = new CollapsibleSection(QString(vegetable->getVegetablename().c_str()), 300, nullptr);
        collapsibleSection->setContentLayout(*anyLayout);
        collapsibleSection->setBaseSize(700,100);
        form->addRow(collapsibleSection);

        historyList->setContextMenuPolicy(Qt::ActionsContextMenu);

        QAction* editHistoryAction = new QAction(tr("&Edit"), this);
        connect(editHistoryAction, SIGNAL(triggered()), historyList, SLOT(editSearchHistory()));
        historyList->addAction(editHistoryAction);
    }

    if (!hasResults) {
        QMessageBox messageBox;
        messageBox.critical(nullptr,"Error","No results!");
        messageBox.setFixedSize(500,200);
        return;
    }
}

void SearchResultsController:: calculateHistory() {
    string company = companyDrop->currentText().toUtf8().constData();
    string customer = customerDrop->currentText().toUtf8().constData();
    if (company != "" && customer != "") {
        QMessageBox messageBox;
        messageBox.critical(nullptr,"Error","Cannot calculate if you search both company and customer!");
        messageBox.setFixedSize(500,200);
        return;
    }

    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender()); // retrieve the button you have clicked
    for (int i = 0; i < calculateButtons.size(); i++) {
        if (buttonSender == calculateButtons[i]) {
            searchHistoryLists[i]->calculateHistory();
        }
    }
}
