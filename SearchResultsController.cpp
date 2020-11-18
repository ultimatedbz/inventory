#include "dialog.h"
#include "utils.h"
#include "collapsiblesection.h"
#include "SearchResultsController.h"

SearchResultsController::SearchResultsController(Inventory* inventory,
                                         QFormLayout* fo,
                              QFont f, QScrollArea* sa, QDialog* d, QLineEdit* dateTextFieldd, QComboBox* companyDropp, QComboBox* customerDropp):
  mInventory(inventory),
  font(f),
  form(fo),
  scrollArea(sa),
  dialog(d),
  dateTextField(dateTextFieldd),
  companyDrop(companyDropp),
  customerDrop(customerDropp)
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
    for (int i = 0; i < mInventory->getVegNum(); i++) {
        Vegetable* vegetable = mInventory->getVegetableByIndex(i);

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

        QAction* editHistoryAction = new QAction(tr("&Edit"), this);
        connect(editHistoryAction, SIGNAL(triggered()), this, SLOT(editHistory()));
        historyList->addAction(editHistoryAction);
    }

    if (!hasResults) {
        QMessageBox messageBox;
        messageBox.critical(nullptr,"Error","No results!");
        messageBox.setFixedSize(500,200);
        return;
    }

    dialog->show();
}
