#include "dialog.h"
#include "utils.h"
#include <iostream>
using namespace std;

void Dialog::on_Buy_clicked() {
    if(!currentVege)
        return;

    QDialog dialog(this);
    dialog.setWindowTitle(mTranslator ->translate("買").c_str());

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

    QString label2 = QString(mTranslator->translate("哪家公司的?").c_str());
    form->addRow(label2, companyDrop);

    /* Date */
    QLineEdit *date = new QLineEdit(&dialog);
    time_t t = time(nullptr);
    struct tm * now = localtime(&t);
    char buffer[128];
    sprintf(buffer, "%d/%d/%d", now->tm_mon+1, now->tm_mday, now->tm_year - 100);
    date -> setText(QString::fromUtf8(buffer));
    QString label4 = QString(mTranslator->translate("幾號來的菜?").c_str());
    form->addRow(label4, date);

    MultiBuyController* multiBuyController = new MultiBuyController(
                inventory->getVegetableIndex(currentVege->getVegetablename()),
                inventory, form, font, scrollArea, &dialog, mTranslator);

    /* Button Box */
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    QToolButton *tb = new QToolButton();
    tb->setText("+");


    QToolButton *tb1 = new QToolButton();
    tb1->setText("-");

    QHBoxLayout* hLay = new QHBoxLayout();
    hLay->addWidget(tb);
    hLay->addWidget(tb1);
    hLay->addWidget(&buttonBox);
    dialog_layout->addRow(hLay);

    // dialog_layout->set
    dialog.window()->setFixedWidth(dialog.window()->sizeHint().width() + 100);

    QObject::connect(tb, SIGNAL(clicked()),multiBuyController, SLOT(addElement()));
    QObject::connect(tb1, SIGNAL(clicked()),multiBuyController, SLOT(subtractElement()));
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    if (dialog.exec() == QDialog::Accepted ) {
        for( int i = 0; i < multiBuyController->getElementNum(); i++){
            int vegeIndex = multiBuyController->
                    getActualVegeIndex(i);
            string company = companyDrop->currentText().toUtf8().constData();
            string amount = multiBuyController->
                    getAmount(i);
            string price = multiBuyController->getPrice(i);
            inventory->getVegetableByIndex(vegeIndex)->buyVege(
                        stoi(amount), company, date->text().toStdString(),
                        price);
        }

        on_vegeList_itemClicked(ui->vegeList->currentItem());
    }
}

