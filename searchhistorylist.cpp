#include "searchhistorylist.h"
#include <QDialog>
#include <QScrollArea>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include "History.h"

SearchHistoryList::SearchHistoryList(Vegetable* v, Dialog* d, vector<pair<History*, int>> hp, string dayPurchased, string comp)
{
    vegetable = v;
    dialog = d;
    // Vector of Pairs of History and original index
    historyPairs = hp;
    datePurchased = dayPurchased;
    company = comp;

    for (unsigned int i = 0; i < historyPairs.size(); i++) {
        this->addItem(QString::fromStdString(historyPairs[i].first->getHistory(vegetable->getUnit())));

        if (historyPairs[i].first->getReturned() || historyPairs[i].first->getDumped()|| historyPairs[i].first->getTui()) {
            d->font.setItalic(true);
        }

        this->item(i)->setFont(d->font);
        d->font.setItalic(false);
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
    string label =  "";

    if (historyTemp->getType() == "Buy") {
        label = "[Date Purchased | " + historyTemp->getDatePurchased() + "] [company | " + historyTemp -> getCompany() + "]";
    } else if (historyTemp->getType() == "Sell") {
        label = "[Date Sold | " + historyTemp->getDateSold() + "] [company | " + historyTemp -> getCompany() + "]";
    } else if (historyTemp->getType() == "Dump") {
        label = "[Date Dumped | " + historyTemp->getDateSold() + "] [company | " + historyTemp -> getCompany() + "]";
    } else if (historyTemp->getType() == "Return") {
        label = "[Date Returned | " + historyTemp->getDateSold() + "] [company | " + historyTemp -> getCompany() + "]";
    } else if (historyTemp->getType() == "Tui") {
        label = "[Date RT to Company | " + historyTemp->getDateSold() + "] [company | " + historyTemp -> getCompany() + "]";
    }

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
        this->item(this->currentRow())->setText(QString::fromStdString(historyTemp->getHistory(vegetable->getUnit())));
    }
}

void SearchHistoryList::calculateHistory()
{
    if (company == "") {
        QMessageBox messageBox;
        messageBox.critical(nullptr,"Error","Can only calculate if you searched with a company!");
        messageBox.setFixedSize(500,200);
        return;
    }

    string firstLine = "Date | " + datePurchased + "\n";
    string secondLine = "Company | " + company + "\n";

    string breakdownLine = "\nBreakdown | ";
    double revenue = 0;
    int units = 0;
    bool firstBreakDown = true;
    string noPrices = "";
    string returnedLine = "";
    string tuiDate = "";

    int totalBoxes = 0;
    int totalSold = 0;

    string dumps = "";
    string tuis = "";

    string buyPrice = "";
    bool oneBuyPrice = true;

    Utils* utils = new Utils();
    for (int i = 0; i < historyPairs.size(); i++) {
        History* temp = historyPairs[i].first;
        if (temp -> getDatePurchased() == datePurchased && temp -> getCompany() == company) {
            if (temp -> getType() == "Sell") {
                if (temp -> getPrice() == "--") {
                    noPrices += "No Price: " + to_string(temp -> getDifference() * -1) + " " + temp->getPrice() + "  " + temp->getCustomer() + " " + temp -> getDateSold() + "\n";
                    totalSold += temp -> getDifference() * -1;
                    continue;
                }

                if (firstBreakDown) {
                    firstBreakDown = false;
                } else {
                    breakdownLine += " + ";
                }

                units += temp -> getDifference() * -1;
                totalSold += temp -> getDifference() * -1;
                revenue += stod(temp -> getPrice()) * temp -> getDifference() * -1;
                breakdownLine += "$" + temp -> getPrice() + " * " + to_string(temp -> getDifference() * -1);
            } else if (temp -> getType() == "Tui") {
                tuis += "RT to farm: " + to_string(temp -> getDifference() * -1) + " " + vegetable->getUnit() + " " + temp -> getDateSold() + "\n";
                totalSold += temp -> getDifference() * -1;
            } else if (temp -> getType() == "Dump") {
                dumps += "Dumped: " + to_string(temp -> getDifference() * -1) + " " + vegetable->getUnit() + " " + temp -> getDateSold() + "\n";
                totalSold += temp -> getDifference() * -1;
            } else if (temp -> getType() == "Buy") {
                totalBoxes += temp -> getDifference();
                if (buyPrice == "" || buyPrice == temp->getPrice()) {
                    buyPrice = temp->getPrice();
                } else {
                    oneBuyPrice = false;
                    buyPrice = "Multiple: $" + buyPrice +" and $" + temp->getPrice();
                }
            } else if (temp -> getType() == "Return") {
                // No access to original price here.
                //units += temp -> getDifference() * -1;
                //revenue += stoi(temp -> getPrice()) * temp -> getDifference() * -1;
                totalSold -= temp -> getDifference();
                returnedLine += "    " + to_string(temp -> getDifference()) + " " + vegetable -> getUnit() + " " + temp -> getCustomer() + " " + temp -> getDateSold() + "\n";
            }
        }
    }

    breakdownLine += "\n";
    string boxesLine = "Total Boxes (Buy needs to be in history) | " + to_string(totalBoxes) + "\n";

    string totalPaidToCompany = "";
    if (oneBuyPrice) {
        if (buyPrice == "--") {
            totalPaidToCompany = "--";
        } else {
            string companyPriceBreakdown = "    <---    " + to_string(totalBoxes) + " " + "Boxes X $" + buyPrice;
            totalPaidToCompany = utils->doubleToString(stod(buyPrice) * totalBoxes) + companyPriceBreakdown;
        }
    } else {
        totalPaidToCompany = buyPrice;
    }

    string companyPriceLine = "Company Price | $" + totalPaidToCompany +"\n";

    string thirdLine = "Revenue | $" + utils -> doubleToString(revenue) + "\n";
    string fourthLine = "Units sold | " + to_string(units) + " "+ vegetable->getUnit() +"\n";

    int remaining = 0;

    for (int i = 0; i < vegetable -> getRemainingNum(); i++) {
        Remaining* temp = vegetable -> getRemainingObject(i);
        if (temp -> getDate() == datePurchased && temp -> getCompany() == company) {
            remaining = temp -> getRemaining();
            break;
        }
    }

    string fifthLine = "Boxes remaining | " + to_string(remaining) + "\n";

    if (returnedLine != "") {
        returnedLine = "\nManual Deduction: \n\nCustomer RT| \n" + returnedLine;
    }

    string totalSoldLine = "Total sold | " + to_string(totalSold) + " "+ vegetable->getUnit() +"\n";

    // Launch popup
    QDialog popup(this);
    popup.setWindowTitle("Calculate");

    QTextEdit* memo = new QTextEdit();
    memo->setText(QString::fromStdString(firstLine + secondLine + boxesLine + companyPriceLine + thirdLine + fourthLine + totalSoldLine + fifthLine + breakdownLine + "\n" + noPrices + dumps + tuis + returnedLine));

    QVBoxLayout *dialog_layout = new QVBoxLayout(&popup);
    popup.setLayout(dialog_layout);
    popup.layout()->addWidget(memo);
    popup.layout()->setMargin(0);
    memo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    popup.setBaseSize(400, 400);

    popup.exec();
}
