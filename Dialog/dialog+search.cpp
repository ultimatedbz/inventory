#include "dialog.h"
#include "utils.h"
#include "collapsiblesection.h"
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

    /* Button Box */
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok, Qt::Horizontal, &dialog);

    dialog_layout->addRow(&buttonBox);

    QHBoxLayout* hLay = new QHBoxLayout();
    hLay->addWidget(&buttonBox);
    dialog_layout->addRow(hLay);

    dialog.window()->setFixedWidth(dialog.window()->sizeHint().width() + 100);

    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));

    if (dialog.exec() == QDialog::Accepted ) {
        string company = companyDrop->currentText().toUtf8().constData();
        string customer = customerDrop->currentText().toUtf8().constData();
        string date = dateTextField->text().toStdString();

        showSearchResults(date, company, customer);
    }
}

void Dialog:: showSearchResults(string date, string company, string customer) {
    if (date == "" || (company == "" && customer == "")) {
        QMessageBox messageBox;
        messageBox.critical(nullptr,"Error","You need to set date, and company / customer");
        messageBox.setFixedSize(500,200);
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Search results");

    //Add the viewport to the scroll area
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    //Create a widget and set its layout as your new layout created above
    QWidget *viewport = new QWidget(&dialog);
    scrollArea->setWidget(viewport);
    scrollArea->setWidgetResizable(true);

    QFormLayout* form = new QFormLayout(viewport);
    viewport->setLayout(form);

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
        for (unsigned int j = 0; j < matchingDates.size(); j++) {
            History* history = matchingDates[j];
            if (company != "" && company == history->getCompany()) {
                matchingCompany.push_back(history);
            }
        }

        vector<History*> matchingCustomer = vector<History*>();
        for (unsigned int j = 0; j < matchingCompany.size(); j++) {
            History* history = matchingCompany[j];
            if (customer != "" && customer == history->getCustomer()) {
                matchingCustomer.push_back(history);
            }
        }

        qDebug() << matchingCustomer.size();

        auto * anyLayout = new QVBoxLayout();
        QToolButton *testButton = new QToolButton();
        testButton->setText("test");
        anyLayout->addWidget(testButton);

        CollapsibleSection* collapsibleSection = new CollapsibleSection(QString(vegetable->getVegetablename().c_str()), 300, 0);
        collapsibleSection->setContentLayout(*anyLayout);
        form->addRow(collapsibleSection);
    }
}
