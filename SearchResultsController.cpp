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

        vector<pair<History*, int>> matchingDates = vector<pair<History*, int>>();
        for (int j = 0; j < vegetable->getHistoryNum(); j++) {
            History* history = vegetable->getHistoryObject(j);
            if (history->getDateToCompare() == date) {
                matchingDates.push_back(make_pair(history, j));
            }
        }

        vector<pair<History*, int>> matchingCompany = vector<pair<History*, int>>();
        if (company != "") {
            for (unsigned int j = 0; j < matchingDates.size(); j++) {
                pair<History*, int> history = matchingDates[j];
                if (company != "" && company == history.first->getCompany()) {
                    matchingCompany.push_back(history);
                }
            }
        } else {
            matchingCompany = matchingDates;
        }

        vector<pair<History*, int>> matchingCustomer = vector<pair<History*, int>>();
        if (customer != "") {
            for (unsigned int j = 0; j < matchingCompany.size(); j++) {
                pair<History*, int> history = matchingCompany[j];
                if (customer != "" && customer == history.first->getCustomer()) {
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
        SearchHistoryList* historyList = new SearchHistoryList(vegetable, mainDialog, matchingCustomer);
        historyList->resize(100, 60);
        historyList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        anyLayout->addWidget(historyList);


        CollapsibleSection* collapsibleSection = new CollapsibleSection(QString(vegetable->getVegetablename().c_str()), 300, nullptr);
        collapsibleSection->setContentLayout(*anyLayout);
        form->addRow(collapsibleSection);

        dialog->adjustSize();

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