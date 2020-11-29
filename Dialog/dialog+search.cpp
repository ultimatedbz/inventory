#include "dialog.h"
#include "utils.h"
#include "collapsiblesection.h"
#include "searchResultsController.h"
#include <iostream>
#include <QShortcut>
#include <QKeySequence>

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

    // The visible parts of the contents are rendered in the viewport
    QWidget *viewport = new QWidget(&dialog);
   // viewport->setSizePolicy(QSizePolicy::Expanding, QSizePolicy ::Expanding);

    scrollArea->setWidget(viewport);
    scrollArea->setWidgetResizable(true);
    //sscrollArea->setBaseSize(700, 800);

    QFormLayout* form = new QFormLayout(viewport);

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
    QToolButton *searchButton = new QToolButton();
    searchButton->setText("Search");

    QHBoxLayout* hLay = new QHBoxLayout();
    hLay->addWidget(searchButton);
    form->addRow(hLay);

    dialog.setLayout(new QVBoxLayout);
    dialog.layout()->addWidget(scrollArea);
    dialog.layout()->setMargin(0);

    SearchResultsController* searchResults = new SearchResultsController(inventory, form, font, scrollArea, &dialog, dateTextField, companyDrop, customerDrop, this);
    dialog.window()->setBaseSize(720,800);


    QObject::connect(searchButton, SIGNAL(clicked()),searchResults, SLOT(showSearchResults()));
    QObject::connect(dateTextField, SIGNAL(returnPressed()),searchResults, SLOT(showSearchResults()));

    QShortcut *shortcut = new QShortcut(Qt::Key_Return, &dialog);
    QObject::connect(shortcut, SIGNAL(activated()), searchResults, SLOT(showSearchResults()));

    QShortcut *shortcut2 = new QShortcut(Qt::Key_Enter, &dialog);
    QObject::connect(shortcut2, SIGNAL(activated()), searchResults, SLOT(showSearchResults()));

    dialog.exec();
}
