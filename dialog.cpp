#include "dialog.h"
#include "ui_dialog.h"
#include "IConst.h"
#include <qlistwidget.h>
#include <Qtcore>
#include <QtGui>
#include <QMessageBox>
#include <QInputDialog>
#include <QListWidgetItem>
#include <QPrintPreviewWidget>
#include <QtWidgets>
#include <ctime>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QPrintPreviewDialog>
#include <QCheckBox>
#include <fstream>
#include <stdlib.h>
#include <QCloseEvent>
#include <QFontMetrics>
#include <QAbstractItemDelegate>
#include <QContextMenuEvent>
#include <QSplitter>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    inventory(NULL),
    currentVege(NULL),
    needSave(0),
    font("Courier",-1,QFont::Bold,false)

{

    ui->setupUi(this);

    #ifndef Q_OS_WIN32
        ui->label_4->hide();
    #endif

    ui->pushButton->hide();
    QList<int> sizes = ui->splitter_4->sizes();
    sizes.replace(0,this->width() * 1/4);
    sizes.replace(1,this->width() * 3/4);
    ui->splitter_4->setSizes(sizes);

    QList<int> sizes2 = ui->splitter_4->sizes();
    sizes2.replace(0,this->width() * 3/4 * 5/8);
    sizes2.replace(1,this->width() * 3/4 * 3/8);
    ui->splitter_3->setSizes(sizes2);

    ui->vegeList->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->historyList->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->breakDown->setContextMenuPolicy(Qt::ActionsContextMenu);

    deleteVegeAction = new QAction(tr("&Delete"),this);
    connect(deleteVegeAction, SIGNAL(triggered()), this, SLOT(deleteVege()));
    ui->vegeList->addAction(deleteVegeAction);

    undoHistoryAction = new QAction(tr("&Undo"),this);
    connect(undoHistoryAction, SIGNAL(triggered()), this, SLOT(undoHistory()));
    ui->historyList->addAction(undoHistoryAction);

    deleteHistoryAction = new QAction(tr("&Delete"),this);
    connect(deleteHistoryAction, SIGNAL(triggered()), this, SLOT(deleteHistory()));
    ui->historyList->addAction(deleteHistoryAction);

    /*Edit Vegetable Name */
    connect(ui->vegeList->itemDelegate(), SIGNAL(closeEditor(QWidget*, QAbstractItemDelegate::EndEditHint)),
            this, SLOT(ListWidgetEditEnd(QWidget*, QAbstractItemDelegate::EndEditHint)));

    inventory = new Inventory();
    mTranslator = new Translator();
    menuBar = new IMenuBar(this, mTranslator);

    QFont vFont = font;
    vFont.setPixelSize(18);
    ui->vegeList->setFont(vFont);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_vegeList_itemPressed(QListWidgetItem *item)
{
    ui->historyList->clear();
    ui->breakDown->clear();
    ui->returnList->clear();
    ui->returnToFarm->clear();

    QString text = item -> text();
    string vegeName = text.toStdString();
    currentVege = inventory->getVegetable(vegeName);
    int historyNum = currentVege->getHistoryNum();

    int index = 0;
    for(int i = 0; i < historyNum; i++){
        if((currentVege->getHistoryObject(i)->getTui() && ui->tuiCheck->isChecked())              ||
                (currentVege->getHistoryObject(i)->getDumped() && ui->dumpCheck->isChecked())     ||
                (currentVege->getHistoryObject(i)->getReturned() && ui->returnCheck->isChecked())   ||
                ((currentVege->getHistoryObject(i)->getCustomer().compare("\t") &&ui->sellCheck->isChecked())
                        &&
                        !(currentVege->getHistoryObject(i)->getTui() || currentVege->getHistoryObject(i)->getDumped()||
                          currentVege->getHistoryObject(i)->getReturned())     )       ||
                (ui->buyCheck->isChecked() && !(currentVege->getHistoryObject(i)->getTui() ||
                            currentVege->getHistoryObject(i)->getReturned() || currentVege->getHistoryObject(i)->getCustomer().compare("\t")
                            || currentVege->getHistoryObject(i)->getDumped()))     ){

            ui->historyList->addItem(QString::fromStdString(currentVege->viewHistory(i)));
            ui->historyList->item(index)->setFont(font);
            if(currentVege->getHistoryObject(i)->getReturned() ||
                    currentVege->getHistoryObject(i)->getDumped()||currentVege->getHistoryObject(i)->getTui()){
                font.setItalic(true);
                ui->historyList->item(index)->setFont(font);
                font.setItalic(false);
            }
            index++;
        }
    }

    ui ->changeTotal->setText(QString::number(currentVege->getTotalVeges()) +
                              " " + QString::fromStdString(currentVege->getUnit()));
    for(int i = 0; i< currentVege -> getRemainingNum() ; i++){
            ui->breakDown->addItem(currentVege->formatRemaining2(i).c_str());
            ui->breakDown->item(i)->setFont(font);
    }
    ui ->Memo_2->setText(currentVege->getMemo().c_str());
    for(int i = 0; i< currentVege->getReturnNum(); i++){
        ui->returnList->addItem(currentVege->formatReturn(i).c_str());
        ui->returnList->item(i)->setFont(font);
    }
    for(int i = 0; i< currentVege->getTuiNum(); i++){
        ui->returnToFarm->addItem(currentVege->formatTui(i).c_str());
        ui->returnToFarm->item(i)->setFont(font);
    }
    needSave = 1;
}


void Dialog::on_Buy_clicked()
{


    if(currentVege){

        QDialog dialog(this);
        dialog.setWindowTitle(mTranslator ->translate("買").c_str());
        // Use a layout allowing to have a label next to each field
        QFormLayout form(&dialog);

        // Add some text above the fields
        form.addRow(new QLabel(mTranslator->translate("輸入進菜資料").c_str()));

        QLineEdit *lineEdit = new QLineEdit(&dialog);
        QString label = QString(mTranslator->translate("買了多少?").c_str());
        form.addRow(label, lineEdit);

        QString label2 = QString(mTranslator->translate("哪家公司的?").c_str());
        QComboBox* companyDrop = new QComboBox(&dialog);
        for(int i = 0; i< inventory->getCompanyNum(); i++)
            companyDrop->addItem(inventory->getCompany(i).c_str());
        companyDrop->setFont(font);
        form.addRow(label2, companyDrop);

        QLineEdit *date = new QLineEdit(&dialog);
        time_t t = time(0);
        struct tm * now = localtime(&t);
        char buffer[128];
        sprintf(buffer, "%d/%d", now->tm_mon+1, now->tm_mday);
        date -> setText(QString::fromUtf8(buffer));
        QString label4 = QString(mTranslator->translate("幾號來的菜?").c_str());
        form.addRow(label4, date);


        QLineEdit *price = new QLineEdit(&dialog);
        QString label5 = QString(mTranslator->translate("進價多少?").c_str());
        form.addRow(label5, price);


        // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
        form.addRow(&buttonBox);
        QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
        QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

       // Show the dialog as modal
       if (dialog.exec() == QDialog::Accepted) {

            int amount = lineEdit->text().toInt();
            string buyerCompany = companyDrop->currentText().toUtf8().constData();
            on_vegeList_itemPressed(ui->vegeList->currentItem());

            if(!currentVege->buyVege(amount,buyerCompany,
                                     date->text().toUtf8().constData(),
                                     price->text().toUtf8().constData())){

                QMessageBox messageBox;
                messageBox.critical(0,mTranslator->translate("錯誤").c_str(),"Not Valid!");
                messageBox.setFixedSize(500,200);
            }

            else{
                on_vegeList_itemPressed(ui->vegeList->currentItem());

            }
       }
    }
}

void Dialog::on_Sell_clicked()
{
    if(currentVege && currentVege->getRemainingNum()){

        QDialog dialog(this);
        dialog.setWindowTitle(mTranslator->translate("賣").c_str());
        // Use a layout allowing to have a label next to each field
        QFormLayout form(&dialog);

        // Add some text above the fields
        form.addRow(new QLabel(mTranslator->translate("輸入賣菜資料").c_str()));



        QLineEdit *lineEdit = new QLineEdit(&dialog);
        QString label = QString(mTranslator->translate("賣了多少？").c_str());
        form.addRow(label, lineEdit);


        QComboBox* customerDrop = new QComboBox(&dialog);
        for(int i = 0; i< inventory->getPersonNum(); i++){
            customerDrop->addItem(inventory->getPerson(i).c_str());
        }
        customerDrop->setFont(font);

        QString label2 = QString(mTranslator->translate("賣给谁？").c_str());
        form.addRow(label2, customerDrop);

        QLineEdit * date = new QLineEdit(&dialog);
        time_t t = time(0);
        struct tm * now = localtime(&t);
        char buffer[128];
        sprintf(buffer, "%d/%d", now->tm_mon+1, now->tm_mday);
        date -> setText(QString::fromUtf8(buffer));
        QString label4 = QString(mTranslator->translate("那天賣出").c_str());
        form.addRow(label4, date);

        QLineEdit *price = new QLineEdit(&dialog);
        QString label5 = QString(mTranslator->translate("賣多少錢?").c_str());
        form.addRow(label5, price);

        QComboBox* remainingDrop = new QComboBox(&dialog);
        for(int i = 0; i< currentVege -> getRemainingNum(); i++){
            remainingDrop->addItem(currentVege->formatRemaining(i).c_str());
        }
        remainingDrop->setFont(font);

        form.addRow(mTranslator->translate("你要賣那天的菜？").c_str(), remainingDrop);

        QCheckBox checkBox(mTranslator->translate("聯合賣").c_str(),&dialog);
        form.addRow(&checkBox);

        // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
        form.addRow(&buttonBox);
        QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
        QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
        QObject::connect(&checkBox,SIGNAL(toggled(bool)), &dialog, SLOT(reject()));

        int result = dialog.exec();
        int amount = lineEdit->text().toInt();
        int selection = remainingDrop->currentIndex();
        string customer = customerDrop->currentText().toUtf8().constData();

       // Show the dialog as modal
        if ( result == QDialog::Accepted) {
            if( amount <= 0 ){
                QMessageBox messageBox;
                messageBox.critical(0,mTranslator->translate("錯誤").c_str(),"Not Valid!");
                messageBox.setFixedSize(500,200);
                additionalSell(amount, customerDrop->currentIndex(), date->text(), price->text());
            }


            else if(amount > currentVege -> getTotalVeges() ){
                QMessageBox messageBox;
                messageBox.critical(0,mTranslator->translate("錯誤").c_str(),
                                    mTranslator->translate("不夠賣!").c_str());
                messageBox.setFixedSize(500,200);
                additionalSell(amount, customerDrop->currentIndex(), date->text(), price->text());
            }

            else if(!currentVege->sellVege(amount, customer,
                                      date->text().toUtf8().constData(),
                                      price->text().toUtf8().constData(), selection)){
                QMessageBox error;
                QMessageBox::StandardButton reply = error.critical(0,"警告"
                           , mTranslator->translate("菜不足,繼續買另外一天的菜?").c_str(),
                           QMessageBox::No|QMessageBox::Yes);
                error.setFixedSize(500,200);
                if (reply == QMessageBox::Yes) {
                    int remaining = currentVege->getRemaining(selection);
                    currentVege -> sellVege( remaining,
                                            customer,
                                            date->text().toUtf8().constData(),
                                            price->text().toUtf8().constData(),
                                             selection);
                    on_vegeList_itemPressed(ui->vegeList->currentItem());

                    additionalSell(amount - remaining, customerDrop->currentIndex(), date->text(), price->text());
                }
            }
            on_vegeList_itemPressed(ui->vegeList->currentItem());
        }else if (result == QDialog::Rejected){
            if(checkBox.isChecked()){
                int splits = querySplits();
                if(splits > 0)
                    splitSell(splits, amount, customerDrop->currentIndex(), date->text(), price->text());
                else{
                    QMessageBox messageBox;
                    messageBox.critical(0,"警告",mTranslator->translate("聯合數字錯誤!").c_str());
                    messageBox.setFixedSize(500,200);
                    additionalSell(amount, customerDrop->currentIndex(), date->text(), price->text());
                }
            }
        }
    }
}

void Dialog::additionalSell(int amount, int cusIndex, QString dateB, QString priceB  ){
    QDialog dialog(this);
    dialog.setWindowTitle(mTranslator->translate("賣").c_str());
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add some text above the fields
    form.addRow(new QLabel(mTranslator->translate("輸入賣菜資料").c_str()));



    QLineEdit *lineEdit = new QLineEdit(&dialog);
    QString label = QString(mTranslator->translate("賣了多少？").c_str());
    lineEdit -> setText(QString::number(amount));
    form.addRow(label, lineEdit);


    QComboBox* customerDrop = new QComboBox(&dialog);
    for(int i = 0; i< inventory->getPersonNum(); i++){
        customerDrop->addItem(inventory->getPerson(i).c_str());
    }

    for(int i=0; i< inventory->getCompanyNum(); i++){
        customerDrop->addItem(inventory->getCompany(i).c_str());
    }
    customerDrop->setFont(font);
    customerDrop->setCurrentIndex(cusIndex);
    QString label2 = QString(mTranslator->translate("賣给谁？").c_str());
    form.addRow(label2, customerDrop);

    QLineEdit * date = new QLineEdit(&dialog);
    date -> setText(dateB);
    QString label4 = QString(mTranslator->translate("那天賣出").c_str());
    form.addRow(label4, date);

    QLineEdit *price = new QLineEdit(&dialog);
    QString label5 = QString(mTranslator->translate("賣多少錢?").c_str());
    price ->setText(priceB);
    form.addRow(label5, price);

    QComboBox* remainingDrop = new QComboBox(&dialog);
    for(int i = 0; i< currentVege -> getRemainingNum(); i++){
        remainingDrop->addItem(currentVege->formatRemaining(i).c_str());
    }
    remainingDrop->setFont(font);

    form.addRow(mTranslator->translate("你要賣那天的菜？").c_str(), remainingDrop);

    QCheckBox checkBox(mTranslator->translate("聯合賣").c_str(),&dialog);
    form.addRow(&checkBox);

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                           Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
    QObject::connect(&checkBox,SIGNAL(toggled(bool)), &dialog, SLOT(reject()));

    int result = dialog.exec();
    amount = lineEdit->text().toInt();
    int selection = remainingDrop->currentIndex();
    string customer = customerDrop->currentText().toUtf8().constData();

   // Show the dialog as modal
    if ( result == QDialog::Accepted) {
        if( amount <= 0 ){
            QMessageBox messageBox;
            messageBox.critical(0,"錯誤","Not Valid!");
            messageBox.setFixedSize(500,200);
            additionalSell(amount, customerDrop->currentIndex(), date->text(), price->text());
        }


        else if(amount > currentVege -> getTotalVeges() ){
            QMessageBox messageBox;
            messageBox.critical(0,"錯誤",mTranslator->translate("不夠賣!").c_str());
            messageBox.setFixedSize(500,200);
            additionalSell(amount, customerDrop->currentIndex(), date->text(), price->text());
        }

        else if(!currentVege->sellVege(amount, customer,
                                  date->text().toUtf8().constData(),
                                  price->text().toUtf8().constData(), selection)){
            QMessageBox error;
            QMessageBox::StandardButton reply = error.critical(0,"警告",
                                    mTranslator->translate("菜不足,繼續買另外一天的菜?").c_str(),
                                    QMessageBox::No|QMessageBox::Yes);
            error.setFixedSize(500,200);
            if (reply == QMessageBox::Yes) {
                int remaining = currentVege->getRemaining(selection);
                currentVege -> sellVege( remaining,
                                        customer,
                                        date->text().toUtf8().constData(),
                                        price->text().toUtf8().constData(),
                                         selection);
                on_vegeList_itemPressed(ui->vegeList->currentItem());

                additionalSell(amount - remaining, customerDrop->currentIndex(), date->text(), price->text());
            }
        }
        on_vegeList_itemPressed(ui->vegeList->currentItem());
    }else if (result == QDialog::Rejected){
        if(checkBox.isChecked()){
            int splits = querySplits();
            if(splits > 0)
                splitSell(splits, amount, customerDrop->currentIndex(), date->text(), price->text());
            else{
                QMessageBox messageBox;
                messageBox.critical(0,"警告",mTranslator
                                    ->translate("聯合數字錯誤!").c_str());
                messageBox.setFixedSize(500,200);
                additionalSell(amount, customerDrop->currentIndex(), date->text(), price->text());
            }
        }
    }
}

void Dialog::addVegetable(){
    QDialog dialog(this);
    dialog.setWindowTitle(mTranslator ->translate("加入新的菜名").c_str());
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);
    form.addRow(new QLabel(mTranslator ->translate("請輸入你要加的菜名").c_str()));

    QLineEdit *lineEdit = new QLineEdit(&dialog);


    QComboBox* unitDrop = new QComboBox(&dialog);
    for(int i = 0; i< inventory->getUnitNum(); i++){
        unitDrop->addItem(inventory->getUnit(i).c_str());
    }
    unitDrop->setFont(font);
    form.addRow(lineEdit, unitDrop);

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                           Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Show the dialog as modal
    if (dialog.exec() ==QDialog::Accepted) {

        if(!inventory->getUnitNum()){

            QMessageBox messageBox;
            messageBox.critical(0,mTranslator ->translate("警告").c_str(),
                                mTranslator ->translate("沒單位!").c_str());
            messageBox.setFixedSize(500,200);
        }else if(inventory->addVegetable(lineEdit->text().toUtf8().constData(),
                                   unitDrop->currentText().toUtf8().constData())){
            ui->vegeList->clear();
            for(int i = 0; i < inventory->getVegNum(); i++){
                ui->vegeList->addItem(inventory->getVegetableByIndex(i)->getVegetablename().c_str());
                ui->vegeList->item(i)->setFlags (ui->vegeList->item(i)->flags()| Qt::ItemIsEditable);
            }
            if(inventory->getVegNum() == 1){
                ui->vegeList->item(0)->setSelected(true);
                ui->vegeList->setCurrentRow(0);
                on_vegeList_itemPressed(ui->vegeList->item(0));
            }else{
                int temp = inventory->getVegetableIndex(currentVege->getVegetablename());
                ui->vegeList->item(temp)->setSelected(true);
                ui->vegeList->setCurrentRow(temp);
                on_vegeList_itemPressed(ui->vegeList->item(temp));
            }
        }


     }

}

void Dialog::slot1(){
    if(currentVege)
        on_vegeList_itemPressed(ui->vegeList->currentItem());
}

void Dialog::addCompany(){
    bool ok;
    QString text = QInputDialog::getText(this, mTranslator
                                         ->translate("加入新客戶").c_str(),
                      mTranslator ->translate("請輸入你要加的新公司名字?").c_str(),
                                         QLineEdit::Normal,"", &ok);
    if(ok)
        inventory->addCompany(text.toUtf8().constData());
    needSave = 1;
}

void Dialog::addPerson(){
    bool ok;
    QString text = QInputDialog::getText(this, mTranslator
                                         ->translate("請輸入你要加的新客戶名字?").c_str(),
                      mTranslator ->translate("你要刪掉哪一個客戶？").c_str(),
                                         QLineEdit::Normal,"", &ok);
    if(ok)
        inventory->addPerson(text.toUtf8().constData());
        needSave = 1;
}

void Dialog::addUnit(){
    bool ok;
    QString text = QInputDialog::getText(this, mTranslator ->translate("刪掉單位").c_str(),
                      mTranslator ->translate("你要刪掉哪一個單位？").c_str(), QLineEdit::Normal,"", &ok);
    if(ok)
        inventory->addUnit(text.toUtf8().constData());
    needSave = 1;
}

void Dialog::removeCompany(){
    if(!inventory->getCompanyNum())
        return;
    QDialog dialog(this);
    dialog.setWindowTitle(mTranslator ->translate("刪除公司").c_str());
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    QComboBox* companyDrop = new QComboBox(&dialog);
        for(int i=0; i< inventory->getCompanyNum(); i++){
            companyDrop->addItem(inventory->getCompany(i).c_str());
    }
    companyDrop->setFont(font);
    form.addRow(mTranslator ->translate("你要刪掉哪一個公司？").c_str(), companyDrop);


    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                           Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));


   // Show the dialog as modal
    if (  dialog.exec()== QDialog::Accepted) {
        inventory->removeCompany(companyDrop->currentText().toUtf8().constData());
            needSave = 1;
    }
}

void Dialog::removePerson(){
    if(!inventory->getPersonNum())
        return;
    QDialog dialog(this);
    dialog.setWindowTitle(mTranslator ->translate("刪除客戶").c_str());
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    QComboBox* personDrop = new QComboBox(&dialog);
        for(int i=0; i< inventory->getPersonNum(); i++){
            personDrop->addItem(inventory->getPerson(i).c_str());
    }
    personDrop->setFont(font);

    form.addRow(mTranslator ->translate("你要刪掉哪一個客戶？").c_str(), personDrop);

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                           Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));


   // Show the dialog as modal
    if (  dialog.exec()== QDialog::Accepted) {
        inventory->removePerson(personDrop->currentText().toUtf8().constData());
            needSave = 1;
    }

}

void Dialog::removeUnit(){
    if(!inventory->getUnitNum())
        return;
    QDialog dialog(this);
    dialog.setWindowTitle(mTranslator ->translate("刪掉單位").c_str());
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    QComboBox* unitDrop = new QComboBox(&dialog);
        for(int i=0; i< inventory->getUnitNum(); i++){
            unitDrop->addItem(inventory->getUnit(i).c_str());
    }
    unitDrop->setFont(font);

    form.addRow(mTranslator ->translate("你要刪掉哪一個單位？").c_str(), unitDrop);

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                           Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));


   // Show the dialog as modal
    if (  dialog.exec()== QDialog::Accepted) {
        inventory->removeUnit(unitDrop->currentText().toUtf8().constData());
            needSave = 1;
    }

}

void Dialog::removeVegetable(){
    if(!inventory->getVegNum())
        return;
    QDialog dialog(this);
    dialog.setWindowTitle(mTranslator ->translate("刪掉菜名").c_str());
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    QComboBox* vegeDrop = new QComboBox(&dialog);
        for(int i=0; i< inventory->getVegNum(); i++){
            vegeDrop->addItem(inventory->getVegetableByIndex(i)->getVegetablename().c_str());
    }
    vegeDrop->setFont(font);

    form.addRow(mTranslator ->translate("請輸入你要加的菜名").c_str(), vegeDrop);

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                           Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));


   // Show the dialog as modal
    if (  dialog.exec()== QDialog::Accepted) {
        inventory->removeVegetable(vegeDrop->currentText().toUtf8().constData());
            needSave = 1;



        QList<QListWidgetItem *> removeThis = ui->vegeList->
                findItems ( vegeDrop->currentText().toUtf8().constData(), Qt::MatchExactly);

        delete removeThis[0];
        if(inventory->getVegNum() == 0){
            currentVege = NULL;
            ui->historyList->clear();
            ui->breakDown->clear();
            ui->returnList->clear();
            ui->Memo_2->clear();
            ui->changeTotal->clear();
        }
    }else{
            on_vegeList_itemPressed(ui->vegeList->currentItem());
        }

}

void Dialog::printInventory(){
        QPrinter printer;
        QPrintPreviewDialog preview (&printer, this);
        connect( &preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printI(QPrinter*)) );
        preview.exec();

}

void Dialog::printHistory(){
        QPrinter printer;
        QPrintPreviewDialog preview (&printer, this);
        connect( &preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printH(QPrinter*)) );
        preview.exec();

}

void Dialog::printI(QPrinter* printer){

    int amount;
    time_t t = time(0);
    struct tm * now = localtime(&t);
    char buffer[128];
    sprintf(buffer, "%d/%d", now->tm_mon+1, now->tm_mday);
    string today = buffer;

    QDialog dialog1(this);
    dialog1.setWindowTitle(mTranslator ->translate("印 Inventory").c_str());
    QFormLayout form(&dialog1);

    QLineEdit *lineEdit = new QLineEdit(&dialog1);
    QString label = QString(mTranslator ->translate("請輸入字體大小").c_str());
    lineEdit -> setText("20");
    form.addRow(label, lineEdit);


    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                           Qt::Horizontal, &dialog1);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog1, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog1, SLOT(reject()));

    if (dialog1.exec() == QDialog::Accepted) {
        amount = lineEdit->text().toInt();
        QPainter painter;
        QFont font("Courier",-1,QFont::Bold,false);

        painter.begin(printer);
        font.setPixelSize(amount);
        painter.setFont(font);
        QFontMetrics metric(font);
        int lineHeight = metric.height();

        QString leftText= "";
        QString rightText="";
        QString * currentText = &leftText;
        painter.drawLine(450,40,450,1250);
        painter.drawText(600,40,100,lineHeight,Qt::AlignRight|Qt::AlignTop, QString(today.c_str()) );
        int lineCount = 0;
        int column = 0;
        for(int i = 0; i<inventory->getVegNum(); i++){
            if(inventory->getVegetableByIndex(i)->getRemainingNum()){
                if( lineHeight * (lineCount + (2 + inventory->getVegetableByIndex(i)->getRemainingNum())) > 1250  ){
                    if(currentText == &leftText){
                        currentText = &rightText;
                        lineCount = 0;
                        column = 1;
                        painter.drawLine(450,40,450,1250);
                    }else{
                        painter.drawText(10,40+lineHeight,390,1250,Qt::AlignLeft|Qt::AlignTop, leftText );
                        painter.drawText(460,40+lineHeight,390,1250,Qt::AlignLeft|Qt::AlignTop, rightText );
                        leftText= "";
                        rightText="";
                        printer->newPage();

                        painter.drawLine(450,40,450,1250);

                        painter.drawText(600,40 + lineHeight,100,lineHeight,Qt::AlignRight|Qt::AlignTop, QString(today.c_str()) );
                        currentText = &leftText;
                        column = 0;
                        lineCount = 0;
                }
            }

                *currentText = *currentText + QString(inventory->
                                   getVegetableByIndex(i)->
                                   getVegetablename().c_str()) + ":  "+ "\n";
                *currentText = *currentText +"Total: " + QString::number(inventory->getVegetableByIndex(i)
                               ->getTotalVeges())+ " " + QString(inventory->getVegetableByIndex(i)->
                                                            getUnit().c_str()) +"\n";
                for(int j =0; j < inventory->getVegetableByIndex(i)
                                    ->getRemainingNum(); j++){
                    *currentText = *currentText + QString(inventory->getVegetableByIndex(i)
                                    ->formatRemaining3(j).c_str()) +"\n";
                    lineCount++;
                }

                lineCount += 2;
                painter.drawLine(450 * column,lineHeight * (lineCount +1) + 40,
                                450 *(column + 1),lineHeight * (1+lineCount)+40);

        }else{
                if( lineHeight * (lineCount + 1) > 1250  ){
                    if(currentText == &leftText){
                        currentText = &rightText;
                        lineCount = 0;
                        column = 1;
                        painter.drawLine(450,40,450,1250);
                    }else{
                        painter.drawText(10,40+lineHeight,390,1250,Qt::AlignLeft|Qt::AlignTop, leftText );
                        painter.drawText(460,40+lineHeight,390,1250,Qt::AlignLeft|Qt::AlignTop, rightText );
                        leftText= "";
                        rightText="";
                        printer->newPage();

                        painter.drawLine(450,40,450,1250);

                        painter.drawText(600,40 + lineHeight,100,lineHeight,Qt::AlignRight|Qt::AlignTop, QString(today.c_str()) );
                        currentText = &leftText;
                        column = 0;
                        lineCount = 0;
                }
            }

                *currentText = *currentText + QString(inventory->
                                   getVegetableByIndex(i)->
                                   getVegetablename().c_str()) + ":  "+ QString::number(inventory->getVegetableByIndex(i)
                               ->getTotalVeges())+ " " + QString(inventory->getVegetableByIndex(i)->
                                                            getUnit().c_str()) +"\n";
                lineCount += 1;
                painter.drawLine(450 * column,lineHeight * (lineCount + 1) + 40,
                                450 *(column + 1),lineHeight * (lineCount + 1)+40);

        }

    }
   painter.drawText(10,40+lineHeight,390,1250,Qt::AlignLeft|Qt::AlignTop, leftText );
   painter.drawText(460,40+lineHeight,390,1250,Qt::AlignLeft|Qt::AlignTop, rightText );
   painter.end();
  }
}

int Dialog::compareCompany(const void * a, const void * b){

    History* A = (History *)a;
    History* B= (History *)b;
    const char* cus1 = A->getCompany().c_str();
    const char* cus2 = B->getCompany().c_str();

    return strcmp(cus1,cus2);
}

int Dialog::compareCustomer(const void * a, const void * b){

    History* A = (History *)a;
    History* B= (History *)b;
    const char* cus1 = A->getCustomer().c_str();
    const char* cus2 = B->getCustomer().c_str();

    return strcmp(cus1,cus2);
}

void Dialog::printH(QPrinter * printer){

    //qdialog what day do you want to print? have a drop down box with all the available days


    string dayComp;
    int amount;
    time_t t = time(0);
    struct tm * now = localtime(&t);
    char buffer[128];
    sprintf(buffer, "%d/%d", now->tm_mon+1, now->tm_mday);
    string today = buffer;

    QDialog dialog1(this);
    dialog1.setWindowTitle(mTranslator ->translate("印 History").c_str());
    QFormLayout form(&dialog1);

    QLineEdit *l2 = new QLineEdit(&dialog1);
    QString label2 = QString(mTranslator ->translate("你要列印哪一天的日期?").c_str());
    l2 -> setText(today.c_str());
    form.addRow(label2,l2);

    QLineEdit *lineEdit = new QLineEdit(&dialog1);
    QString label = QString(mTranslator ->translate("請輸入字體大小").c_str());
    lineEdit -> setText("20");
    form.addRow(label, lineEdit);


    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                           Qt::Horizontal, &dialog1);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog1, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog1, SLOT(reject()));

    if (dialog1.exec() == QDialog::Accepted) {
        amount = lineEdit->text().toInt();
        dayComp = l2->text().toUtf8().constData();
        QPainter painter;
        QFont font("Courier",-1,QFont::Bold,false);

        painter.begin(printer);
        font.setPixelSize(amount);
        painter.setFont(font);
        QFontMetrics metric(font);
        int lineHeight = metric.height();
        QString leftText= "";
        QString rightText="";
        QString * currentText = &leftText;
        int lineCount = 0;
        int column = 0;
        painter.drawLine(450,40,450,1250);
        painter.drawText(600,40,100,lineHeight,Qt::AlignRight|Qt::AlignTop, QString(dayComp.c_str()) );

        for(int i = 0; i<inventory->getVegNum(); i++){
            if(inventory->getVegetableByIndex(i)->getHistoryNum()){

               //make array of all histories that were from today using date to compare
               int historyNum = inventory->getVegetableByIndex(i)->getHistoryNum();
               History* temp = new History[historyNum];
               int index = 0;
               for(int j = 0; j< historyNum; j++){
                  if(!(inventory->getVegetableByIndex(i)->getHistoryObject(j)->getDateToCompare()
                           .compare(dayComp))){
                        temp[index] = *inventory->getVegetableByIndex(i)->getHistoryObject(j);
                        index++;
                   }
               }
               int sellNum = 0; int buyNum = 0; int returnNum = 0; int dumpNum = 0; int tuiNum = 0;
               //make 4 arrays, one for each type
               History* sell = new History[historyNum];
               History* buy = new History[historyNum];
               History* returnn = new History [historyNum];
               History* dump = new History [historyNum];
               History* tui = new History [historyNum];

               int splitNum = 0;
               for(int j = 0; j<index; j++){
                   if(temp[j].getCustomer().compare("\t") && !temp[j].getReturned()
                           && !temp[j].getDumped()){
                       sell[sellNum] = temp[j];
                       if(sell[sellNum].getSplitNum())
                           splitNum += sell[sellNum].getSplitNum()-1;
                       sellNum++;
                   }
                   //sort by customer
                   qsort(sell, sellNum, sizeof(History), compareCustomer);
               }

               for(int j = 0; j<index; j++){
                   if(temp[j].getReturned()){
                       returnn[returnNum] = temp[j];
                       returnNum++;
                   }
               }
               for(int j = 0; j<index; j++){
                   if(temp[j].getDumped()){
                       dump[dumpNum] = temp[j];
                       dumpNum++;
                   }
               }
               for(int j = 0; j<index; j++){
                   if(temp[j].getTui()){
                       tui[tuiNum] = temp[j];
                       tuiNum++;
                   }
               }
               for(int j = 0; j<index; j++){
                   if(!temp[j].getCustomer().compare("\t") && !temp[j].getReturned()
                           && !temp[j].getDumped()){
                       buy[buyNum] = temp[j];
                       buyNum++;
                   }
                   //sort by company
                   qsort(buy, buyNum, sizeof(History), compareCompany);
               }
                if(sellNum || buyNum || returnNum || dumpNum ||tuiNum){
                    //checks if the next block to print fits on the page
                    int first = 0;
                    if(buyNum)
                        first = buyNum;
                    else if(sellNum)
                        first = sellNum;
                    else if(returnNum)
                        first = returnNum;
                    else if(dumpNum)
                        first = dumpNum;
                    else
                        first = tuiNum;

                    if( lineHeight * (lineCount + 2 + first) > 1250  ){
                        if(currentText == &leftText){
                            currentText = &rightText;
                            lineCount = 0;
                            column = 1;
                        }else{
                            lineCount = 0;
                            column = 0;
                            painter.drawText(20,40+lineHeight,380,1250,Qt::AlignLeft|Qt::AlignTop, leftText );
                            painter.drawText(460,40+lineHeight,380,1250,Qt::AlignLeft|Qt::AlignTop, rightText );
                            leftText= "";
                            rightText="";
                            printer->newPage();
                            painter.drawLine(450,40,450,1250);
                            painter.drawText(600,40,100,lineHeight,Qt::AlignRight|Qt::AlignTop, dayComp.c_str() );
                            currentText = &leftText;
                        }
                    }

                    *currentText = *currentText + QString(inventory->
                                   getVegetableByIndex(i)->
                                   getVegetablename().c_str()) + ":  "+
                    QString(inventory->getVegetableByIndex(i)->
                                    getUnit().c_str()) +"\n";
                    lineCount++;

                }


                if(buyNum){
                    painter.drawLine(450 * column + 60,lineHeight *( lineCount+1) + 40,
                                450 *(column + 1),lineHeight * ( lineCount+1) + 40);
                    *currentText = *currentText + "   買:\n";
                    lineCount++;
                    painter.drawLine(450 * column + 60,lineHeight *( lineCount+1) + 40,
                                450 *(column + 1),lineHeight * ( lineCount+1) + 40);
                    for(int j =0; j < buyNum; j++){
                        *currentText = *currentText + QString(buy[j].printFormat().c_str()) +"\n";
                        lineCount++;
                    }

                }
                if(sellNum){
                    if( lineHeight * (lineCount + 1 + sellNum + splitNum) + 20 >1250  ){
                        if(currentText == &leftText){
                            currentText = &rightText;
                            lineCount = 0;
                            column = 1;
                        }else{
                            lineCount = 0;
                            column = 0;
                            painter.drawText(20,40,380,1250,Qt::AlignLeft|Qt::AlignTop, leftText );
                            painter.drawText(460,40,380,1250,Qt::AlignLeft|Qt::AlignTop, rightText );
                            leftText= "";
                            rightText="";
                            printer->newPage();

                            painter.drawLine(450,40,450,1250);
                            painter.drawText(600,40,100,lineHeight,Qt::AlignRight|Qt::AlignTop, QString(dayComp.c_str()) );
                            currentText = &leftText;
                        }
                    }
                    painter.drawLine(450 * column + 60,lineHeight *( lineCount+1) + 40,
                                450 *(column + 1),lineHeight * ( lineCount+1) + 40);
                    *currentText = *currentText + "   賣:\n";
                    lineCount++;
                    painter.drawLine(450 * column + 60,lineHeight *( lineCount+1) + 40,
                                450 *(column + 1),lineHeight * ( lineCount+1) + 40);
                    for(int j =0; j < sellNum; j++){
                        *currentText = *currentText + QString(sell[j].printFormat().c_str()) +"\n";
                        lineCount++;
                        if(sell[j].getSplitNum()){
                            lineCount += sell[j].getSplitNum()-1;
                        }
                    }

                }

                if(returnNum){

                    if( lineHeight * (lineCount + 1 + returnNum) + 20 >1090  ){
                        if(currentText == &leftText){
                            column = 1;
                            currentText = &rightText;
                            lineCount = 0;
                        }else{
                            lineCount = 0;
                            column = 0;
                            painter.drawText(20,40,400,1250,Qt::AlignLeft|Qt::AlignTop, leftText );
                            painter.drawText(460,40,380,1250,Qt::AlignLeft|Qt::AlignTop, rightText );
                            leftText= "";
                            rightText="";
                            printer->newPage();

                            painter.drawLine(450,40,450,1250);
                            painter.drawText(600,40,100,lineHeight,Qt::AlignRight|Qt::AlignTop, QString(dayComp.c_str()) );
                            currentText = &leftText;
                        }
                    }
                    painter.drawLine(450 * column + 60,lineHeight *( lineCount+1) + 40,
                                450 *(column + 1),lineHeight * ( lineCount+1) + 40);
                    *currentText = *currentText + "   退給公司 :\n";
                    lineCount++;
                    painter.drawLine(450 * column + 60,lineHeight *( lineCount+1) + 40,
                                450 *(column + 1),lineHeight * ( lineCount+1) + 40);
                    for(int j =0; j < returnNum; j++){
                        *currentText = *currentText + QString(returnn[j].printFormat().c_str()) +"\n";
                        lineCount++;

                    }


                }
                if(dumpNum){
                    if( lineHeight * (lineCount + 1 + dumpNum) + 20 >1090  ){
                        if(currentText == &leftText){
                            column = 1;
                            currentText = &rightText;
                            lineCount = 0;
                        }else{
                            lineCount = 0;
                            column = 0;
                            painter.drawText(20,40,400,1250,Qt::AlignLeft|Qt::AlignTop, leftText );
                            painter.drawText(460,40,380,1250,Qt::AlignLeft|Qt::AlignTop, rightText );
                            leftText= "";
                            rightText="";
                            printer->newPage();

                            painter.drawLine(450,40,450,1250);
                            painter.drawText(600,40,100,lineHeight,Qt::AlignRight|Qt::AlignTop, QString(dayComp.c_str()) );
                            currentText = &leftText;
                        }
                    }
                        painter.drawLine(450 * column + 60,lineHeight *( lineCount+1) + 40,
                                450 *(column + 1),lineHeight * ( lineCount+1) + 40);
                        *currentText = *currentText + "   倒:\n";
                        lineCount++;
                        painter.drawLine(450 * column + 60,lineHeight *( lineCount+1) + 40,
                                    450 *(column + 1),lineHeight * ( lineCount+1) + 40);
                        for(int j =0; j < dumpNum; j++){
                            *currentText = *currentText + QString(dump[j].printFormat().c_str()) +"\n";
                            lineCount++;
                        }

               }
                if(tuiNum){
                    if( lineHeight * (lineCount + 1 + tuiNum) + 20 >1090  ){
                        if(currentText == &leftText){
                            column = 1;
                            currentText = &rightText;
                            lineCount = 0;
                        }else{
                            lineCount = 0;
                            column = 0;
                            painter.drawText(20,40,400,1250,Qt::AlignLeft|Qt::AlignTop, leftText );
                            painter.drawText(460,40,380,1250,Qt::AlignLeft|Qt::AlignTop, rightText );

                            leftText= "";
                            rightText="";
                            printer->newPage();

                            painter.drawLine(450,40,450,1250);
                            painter.drawText(600,40,100,lineHeight,Qt::AlignRight|Qt::AlignTop, QString(dayComp.c_str()) );
                            currentText = &leftText;
                        }
                    }
                        painter.drawLine(450 * column + 60,lineHeight *( lineCount+1) + 40,
                                    450 *(column + 1),lineHeight * ( lineCount+1) + 40);
                        *currentText = *currentText + "   退給農場:\n";
                        lineCount++;
                        painter.drawLine(450 * column + 60,lineHeight *( lineCount+1) + 40,
                                    450 *(column + 1),lineHeight * ( lineCount+1) + 40);
                        for(int j =0; j < tuiNum; j++){
                            *currentText = *currentText + QString(tui[j].printFormat().c_str()) +"\n";
                            lineCount++;
                        }


               }
                painter.drawLine(450 * column ,lineHeight * (1+lineCount) +40,
                                450 *(column + 1),lineHeight * (lineCount+1)+40);
            }
        }

        painter.drawText(20,40+lineHeight,380,1250,Qt::AlignLeft|Qt::AlignTop, leftText );
        painter.drawText(460,40+lineHeight,380,1250,Qt::AlignLeft|Qt::AlignTop, rightText );
        painter.end();

    }
}

int Dialog::querySplits(){
    QDialog dialog(this);
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add some text above the fields
    form.addRow(new QLabel(mTranslator->translate("你要幾家公司聯合? (最多4家)").c_str()));

    QLineEdit *lineEdit = new QLineEdit(&dialog);
    form.addRow( lineEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                           Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
    if (dialog.exec() == QDialog::Accepted ) {
        return lineEdit->text().toInt();
    }
    return 0;
}

void Dialog::splitSell(int splits, int amount, int cusIndex, QString dateB, QString priceB  ){
    QDialog dialog(this);
    dialog.setWindowTitle(mTranslator->translate("聯合賣").c_str());
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add some text above the fields
    form.addRow(new QLabel(mTranslator->translate("輸入賣菜資料").c_str()));


    QLineEdit *lineEdit = new QLineEdit(&dialog);
    QString label = QString(mTranslator->translate("賣了多少？").c_str());
    lineEdit -> setText(QString::number(amount));
    form.addRow(label, lineEdit);


    QComboBox* customerDrop = new QComboBox(&dialog);
    for(int i = 0; i< inventory->getPersonNum(); i++){
        customerDrop->addItem(inventory->getPerson(i).c_str());
    }

    customerDrop->setFont(font);
    customerDrop->setCurrentIndex(cusIndex);
    QString label2 = QString(mTranslator->translate("賣给谁？").c_str());
    form.addRow(label2, customerDrop);

    QLineEdit * date = new QLineEdit(&dialog);
    date -> setText(dateB);
    QString label4 = QString(mTranslator->translate("那天賣出").c_str());
    form.addRow(label4, date);

    QLineEdit *price = new QLineEdit(&dialog);
    QString label5 = QString(mTranslator->translate("賣多少錢?").c_str());
    price ->setText(priceB);
    form.addRow(label5, price);

    form.addRow(new QLabel(mTranslator->translate("你要賣那天的菜？").c_str()));
    QComboBox* remainingDrop = new QComboBox(&dialog);
    QLineEdit *lineEdit1 = new QLineEdit(&dialog);
    QLineEdit *lineEdit2;
    QComboBox* remainingDrop2;
    QLineEdit *lineEdit3;
    QComboBox* remainingDrop3;
    QLineEdit *lineEdit4;
    QComboBox* remainingDrop4;
    int selection1=-1;
    int selection2=-1;
    int selection3=-1;
    int selection4=-1;

    int input1=0, input2 = 0, input3=0, input4 =0;

    for(int i = 0; i< currentVege -> getRemainingNum(); i++){
        remainingDrop->addItem(currentVege->formatRemaining(i).c_str());
    }
    remainingDrop->setFont(font);
        form.addRow(remainingDrop, lineEdit1);
        QObject::connect(remainingDrop, SIGNAL(activated(int)), lineEdit1, SLOT(setFocus()));
    if(splits >=2){
            lineEdit2 = new QLineEdit(&dialog);
            remainingDrop2 = new QComboBox(&dialog);
            for(int i = 0; i< currentVege -> getRemainingNum(); i++){
                remainingDrop2->addItem(currentVege->formatRemaining(i).c_str());
            }
            remainingDrop2->setFont(font);
            remainingDrop2->setCurrentIndex(1);
            form.addRow(remainingDrop2,lineEdit2);
            QObject::connect(remainingDrop2, SIGNAL(activated(int)), lineEdit2, SLOT(setFocus()));
    }

    if(splits >=3){
            lineEdit3 = new QLineEdit(&dialog);
            remainingDrop3 = new QComboBox(&dialog);
            for(int i = 0; i< currentVege -> getRemainingNum(); i++){
                remainingDrop3->addItem(currentVege->formatRemaining(i).c_str());
            }
            remainingDrop3->setFont(font);
            remainingDrop3->setCurrentIndex(2);
            form.addRow(remainingDrop3, lineEdit3);
            QObject::connect(remainingDrop3, SIGNAL(activated(int)), lineEdit3, SLOT(setFocus()));
    }
    if(splits >=4){
            lineEdit4 = new QLineEdit(&dialog);
            remainingDrop4 = new QComboBox(&dialog);
            for(int i = 0; i< currentVege -> getRemainingNum(); i++){
                remainingDrop4->addItem(currentVege->formatRemaining(i).c_str());
            }
            remainingDrop4->setFont(font);
            remainingDrop4->setCurrentIndex(3);
            form.addRow(remainingDrop4, lineEdit4);
            QObject::connect(remainingDrop4, SIGNAL(activated(int)), lineEdit4, SLOT(setFocus()));
}


    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                           Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    int result = dialog.exec();
    amount = lineEdit->text().toInt();
    input1= lineEdit1->text().toInt();
    selection1 = remainingDrop -> currentIndex();
    if(splits >= 2 ){
        input2= lineEdit2->text().toInt();
        selection2 = remainingDrop2 -> currentIndex();
    }
    if(splits >=3 ){
        input3= lineEdit3->text().toInt();
        selection3 = remainingDrop3 -> currentIndex();
    }
    if(splits >= 4){
        input4= lineEdit4->text().toInt();
        selection4 = remainingDrop4 -> currentIndex();
    }

    string customer = customerDrop->currentText().toUtf8().constData();

   // Show the dialog as modal
    if ( result == QDialog::Accepted) {
        if( amount <= 0 ){
            QMessageBox messageBox;
            messageBox.critical(0,"錯誤","Not Valid!");
            messageBox.setFixedSize(500,200);
            splitSell(splits, amount, customerDrop->currentIndex(), date->text(), price->text());
        }
        else if(amount != input1+input2+input3+input4){
            QMessageBox messageBox;
            messageBox.critical(0,"錯誤",mTranslator->translate("輸入的數量不一樣!").c_str());
            messageBox.setFixedSize(500,200);
            splitSell(splits, amount, customerDrop->currentIndex(), date->text(), price->text());
        }
        else if(amount > currentVege -> getTotalVeges() ){
            QMessageBox messageBox;
            messageBox.critical(0,"錯誤",mTranslator->translate("不夠賣!").c_str());
            messageBox.setFixedSize(500,200);
            splitSell(splits, amount, customerDrop->currentIndex(), date->text(), price->text());
        }
        else if(sameDay(selection1,selection2,selection3,selection4)){
            QMessageBox messageBox;
            messageBox.critical(0,"錯誤","Selling from same day!");
            messageBox.setFixedSize(500,200);
            splitSell(splits, amount, customerDrop->currentIndex(), date->text(), price->text());
        }
        else if((input1 > currentVege->getRemaining(selection1))||
                (input2 &&
                input2 > currentVege->getRemaining(selection2))||
                (input3 &&
                input3 > currentVege->getRemaining(selection3))||
                (input4 &&
                input4> currentVege->getRemaining(selection4))){
            QMessageBox messageBox;
            messageBox.critical(0,"錯誤",mTranslator
                                ->translate("輸入的數量太多!").c_str());
            messageBox.setFixedSize(500,200);
            splitSell(splits, amount, customerDrop->currentIndex(), date->text(), price->text());
        }else if((input1 <= 0)||
                (input2 &&
                input2 <= 0)||
                (input3 &&
                input3 <= 0)||
                (input4 &&
                input4 <= 0)){
            QMessageBox messageBox;
            messageBox.critical(0,"錯誤",mTranslator
                                ->translate("輸入的數量太多!").c_str());
            messageBox.setFixedSize(500,200);
            splitSell(splits, amount, customerDrop->currentIndex(), date->text(), price->text());
        }else{
            currentVege->splitSell(splits, input1, input2, input3, input4, date->text().toUtf8().constData(),
                               customer, price->text().toUtf8().constData(), selection1, selection2, selection3,selection4);

        }
        on_vegeList_itemPressed(ui->vegeList->currentItem());
    }
}

int Dialog:: sameDay(int i1, int i2, int i3, int i4){

    if(i1 > -1)
        if(i1==i2 || i1==i3 || i1==i4)
            return 1;
    if(i2 > -1)
        if(i2==i3 || i2==i4)
            return 1;
    if(i3 > -1)
        if(i3==i4)
            return 1;
    return 0;
}

void Dialog::saveAs(){

    QWidget *activeWindow = QApplication::activeWindow();
    QString filename = QFileDialog::getSaveFileName(activeWindow,
        "Save File As", "", "Datafile files (*.datafile);;All files (*)");
    string datafile = filename.toUtf8().constData();
    if(datafile != ""){
        inventory->setFileName(datafile.c_str());
        save();
    }
}

void Dialog::newFile(){

    if(needSave)
        askSave();
    //clear everything
    inventory->remove();
    ui->historyList->clear();
    ui->breakDown->clear();
    ui->returnList->clear();
    ui->vegeList->clear();
    ui->Memo_2->clear();
    ui->changeTotal->clear();

    currentVege = NULL;
 }

void Dialog::loadFile(){
    if(needSave)
        askSave();
    QWidget *activeWindow = QApplication::activeWindow();
    QString filename = QFileDialog::getOpenFileName(activeWindow,
        "Load File", "", "Datafile files (*.datafile);;All files (*)");
    if(filename != NULL){
        needSave = 0;
        newFile();
        string datafile1 = filename.toUtf8().constData();
        const char* datafile = datafile1.c_str();
        fstream* fio = new fstream (datafile, ios :: in| ios::binary);
        fio->seekg(0, ios::beg);
        inventory->load(fio);
        fio->close();
        inventory->setFileName(filename.toUtf8().constData() );
        if(inventory->getVegNum()){
            for(int i = 0; i<inventory->getVegNum(); i++){
                ui->vegeList->addItem(QString::fromStdString(inventory->getVegetableByIndex(i)->getVegetablename()));
                ui->vegeList->item(i)->setFlags (ui->vegeList->item(i)->flags()| Qt::ItemIsEditable);
            }
            QFont vFont = font;
            vFont.setPixelSize(18);
            ui->vegeList->setFont(vFont);
            ui->vegeList->item(0)->setSelected(true);
            ui->vegeList->setCurrentRow(0);
            on_vegeList_itemPressed(ui->vegeList->item(0));
        }
        needSave = 0;
    }
}

void Dialog::save(){

    if(!inventory->getFileName().compare("")){
        saveAs();
    }else{
        fstream* fio = new fstream (inventory->getFileName().c_str(),
                                    ios :: out |ios::binary);
        fio->seekp(0, ios::beg);
        writeInt(inventory->getCompanyNum(),fio);
        writeInt(inventory->getPersonNum(),fio);
        writeInt(inventory->getUnitNum(),fio);
        writeInt(inventory->getVegNum(),fio);
        for(int i = 0; i < inventory->getCompanyNum() ; i++){
            writeString( inventory->getCompany(i), fio);
        }

        for(int i = 0; i < inventory->getPersonNum() ; i++){
            writeString( inventory->getPerson(i) , fio);
        }

        for(int i = 0; i < inventory->getUnitNum() ; i++){
            writeString( inventory->getUnit(i), fio);
        }
        for(int i = 0; i < inventory->getVegNum() ; i++){
            writeString(inventory->getVegetableByIndex(i)
                        ->getVegetablename(), fio);
            writeInt( inventory->getVegetableByIndex(i)->getTotalVeges(), fio);
            writeString( inventory->getVegetableByIndex(i)->getUnit(), fio);
            writeInt( inventory->getVegetableByIndex(i)->getRemainingNum(), fio);
            writeInt( inventory->getVegetableByIndex(i)->getReturnNum(), fio);
            writeInt( inventory->getVegetableByIndex(i)->getHistoryNum(), fio);
            writeInt( inventory->getVegetableByIndex(i)->getTuiNum(), fio);
            writeString( inventory->getVegetableByIndex(i)->getMemo(), fio);

            for( int j = 0; j< inventory->getVegetableByIndex(i)->getTuiNum(); j++){
                writeString( inventory->getVegetableByIndex(i)
                             ->getTuiObject(j)->getDatePurchased(), fio);
                writeString( inventory->getVegetableByIndex(i)
                             ->getTuiObject(j)->getCompany(), fio);
                writeInt( inventory->getVegetableByIndex(i)
                          ->getTuiObject(j)->getReturn(), fio);
                writeString( inventory->getVegetableByIndex(i)
                             ->getTuiObject(j)->getDateReturned(), fio);
            }

            for(int j = 0; j < inventory->getVegetableByIndex(i)
                                                        ->getRemainingNum() ; j++){
                writeString(inventory->getVegetableByIndex(i)
                            ->getRemainingObject(j)->getDate(), fio);
                writeString(inventory->getVegetableByIndex(i)
                            ->getRemainingObject(j)->getCompany(), fio);
                writeString(inventory->getVegetableByIndex(i)
                            ->getRemainingObject(j)->getPrice(), fio);
                writeInt(inventory->getVegetableByIndex(i)
                         ->getRemainingObject(j)->getRemaining(), fio);
                writeInt(inventory->getVegetableByIndex(i)
                         ->getRemainingObject(j)->getReturn(), fio);
            }

            for(int j = 0; j < inventory->getVegetableByIndex(i)
                                                ->getReturnNum() ; j++){
                writeString(inventory->getVegetableByIndex(i)
                            ->getReturnObject(j)->getDatePurchased(), fio);
                writeString(inventory->getVegetableByIndex(i)
                            ->getReturnObject(j)->getCompany(), fio);
                writeInt(inventory->getVegetableByIndex(i)
                         ->getReturnObject(j)->getReturn(), fio);
                writeString(inventory->getVegetableByIndex(i)
                            ->getReturnObject(j)->getReturner(), fio);
                writeString(inventory->getVegetableByIndex(i)
                            ->getReturnObject(j)->getDateReturned(), fio);
            }

            for(int j = 0; j < inventory->getVegetableByIndex(i)->getHistoryNum() ; j++){
                writeString(inventory->getVegetableByIndex(i)
                            ->getHistoryObject(j)->getDateSold(), fio);
                writeString(inventory->getVegetableByIndex(i)
                            ->getHistoryObject(j)->getDatePurchased(),fio);
                writeInt(inventory->getVegetableByIndex(i)
                         ->getHistoryObject(j)->getDifference(), fio);
                writeString(inventory->getVegetableByIndex(i)
                            ->getHistoryObject(j)->getCustomer(), fio);
                writeString(inventory->getVegetableByIndex(i)
                            ->getHistoryObject(j)->getPrice(), fio);
                writeString(inventory->getVegetableByIndex(i)
                            ->getHistoryObject(j)->getCompany(), fio);
                writeString(inventory->getVegetableByIndex(i)
                            ->getHistoryObject(j)->getDateToCompare(), fio);
                writeInt(inventory->getVegetableByIndex(i)
                         ->getHistoryObject(j)->getReturned(), fio);
                writeInt(inventory->getVegetableByIndex(i)
                         ->getHistoryObject(j)->getDumped(), fio);
                writeInt(inventory->getVegetableByIndex(i)
                         ->getHistoryObject(j)->getSplitNum(), fio);
                writeInt(inventory->getVegetableByIndex(i)
                         ->getHistoryObject(j)->getChangeNum(), fio);
                writeInt(inventory->getVegetableByIndex(i)
                         ->getHistoryObject(j)->getTui(), fio);
                for(int z = 0; z < inventory->getVegetableByIndex(i)
                                    ->getHistoryObject(j)->getSplitNum(); z++){
                    writeString(inventory->getVegetableByIndex(i)
                                ->getHistoryObject(j)->getDateSoldS(z), fio);
                    writeString(inventory->getVegetableByIndex(i)
                                ->getHistoryObject(j)->getDatePurchasedS(z), fio);
                    writeInt(inventory->getVegetableByIndex(i)
                             ->getHistoryObject(j)->getDifferenceS(z), fio);
                    writeString(inventory->getVegetableByIndex(i)
                                ->getHistoryObject(j)->getCustomerS(z), fio);
                    writeString(inventory->getVegetableByIndex(i)
                                ->getHistoryObject(j)->getPriceS(z), fio);
                    writeString(inventory->getVegetableByIndex(i)
                                ->getHistoryObject(j)->getCompanyS(z), fio);
                    writeString(inventory->getVegetableByIndex(i)
                                ->getHistoryObject(j)->getDateToCompareS(z), fio);
                    writeInt(inventory->getVegetableByIndex(i)
                             ->getHistoryObject(j)->getReturnedS(z), fio);
                    writeInt(inventory->getVegetableByIndex(i)->getHistoryObject(j)
                             ->getDumpedS(z),fio) ;
                    writeInt(inventory->getVegetableByIndex(i)
                            ->getHistoryObject(j)->getSplitNumS(z), fio);
                    writeInt(inventory->getVegetableByIndex(i)->getHistoryObject(j)
                             ->getChangeNumS(z), fio);
                    writeInt(inventory->getVegetableByIndex(i)
                             ->getHistoryObject(j)->getTuiS(z), fio);
                }

            }
        }
        fio->close();
        needSave = 0;
    }
}

void Dialog::on_Memo_2_textChanged()
{
    if(inventory->getVegNum())
        currentVege->updateMemo(ui->Memo_2->toPlainText().toUtf8().constData());
}

void Dialog::on_Return_clicked()
{
    if(currentVege){
        QDialog dialog(this);
        dialog.setWindowTitle(mTranslator->translate("退還給公司的菜").c_str());
        // Use a layout allowing to have a label next to each field
        QFormLayout form(&dialog);

        // Add some text above the fields
        form.addRow(new QLabel(mTranslator->translate("輸入退菜資料").c_str()));



        QLineEdit *lineEdit = new QLineEdit(&dialog);
        QString label = QString( mTranslator->translate("退幾箱？").c_str());
        form.addRow(label, lineEdit);


        QComboBox* customerDrop = new QComboBox(&dialog);
        for(int i = 0; i< inventory->getPersonNum(); i++){
            customerDrop->addItem(inventory->getPerson(i).c_str());
        }
        customerDrop->setFont(font);
        QString label3 = QString(mTranslator->translate("誰退回來的？").c_str());
        form.addRow(label3, customerDrop);

        QLineEdit * date = new QLineEdit(&dialog);
        time_t t = time(0);
        struct tm * now = localtime(&t);
        char buffer[128];
        sprintf(buffer, "%d/%d", now->tm_mon+1, now->tm_mday);
        date -> setText(QString::fromUtf8(buffer));
        QString label4 = QString(mTranslator->translate("这是哪天退回來的").c_str());
        form.addRow(label4, date);

        QComboBox* companyDrop = new QComboBox(&dialog);
            for(int i=0; i< inventory->getCompanyNum(); i++){
                companyDrop->addItem(inventory->getCompany(i).c_str());
        }
        companyDrop->setFont(font);

        form.addRow(mTranslator->translate("这是哪家公司的菜？").c_str(), companyDrop);


        QLineEdit *lineEdit2 = new QLineEdit(&dialog);
        QString label2 = QString(mTranslator->translate("这是哪天買的菜？").c_str());
        form.addRow(label2, lineEdit2);

        QCheckBox checkBox(mTranslator->translate("退還給農場的菜").c_str(),&dialog);
        form.addRow(&checkBox);

        // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
        form.addRow(&buttonBox);
        QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
        QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
        QObject::connect(&checkBox,SIGNAL(toggled(bool)), &dialog, SLOT(reject()));

        int result = dialog.exec();
        int amount = lineEdit->text().toInt();
        string dateBought = lineEdit2->text().toUtf8().constData();
        string returner = customerDrop->currentText().toUtf8().constData();
        string company = companyDrop->currentText().toUtf8().constData();
        string dateReturned = date->text().toUtf8().constData();
       // Show the dialog as modal
        if ( result == QDialog::Accepted) {
            if( amount <= 0 ){
                QMessageBox messageBox;
                messageBox.critical(0,"錯誤","Not Valid!");
                messageBox.setFixedSize(500,200);
            }else{
                currentVege->returnThis(dateReturned, amount, returner, company, dateBought);
            }

        }else if (result == QDialog::Rejected){
            if(checkBox.isChecked()){

                QDialog dialog(this);
                dialog.setWindowTitle(mTranslator->translate("退還給農場的菜").c_str());
                // Use a layout allowing to have a label next to each field
                QFormLayout form(&dialog);

                // Add some text above the fields
                form.addRow(new QLabel(mTranslator->translate("輸入退菜資料").c_str()));


                QComboBox* remainingDrop = new QComboBox(&dialog);
                for(int i = 0; i< currentVege -> getRemainingNum(); i++){
                    remainingDrop->addItem(currentVege->formatRemaining(i).c_str());
                }
                remainingDrop->setFont(font);

                form.addRow(mTranslator->translate("你要退那天的菜？").c_str(), remainingDrop);

                QLineEdit *lineEdit = new QLineEdit(&dialog);
                QString label = QString(mTranslator->translate("退幾箱？").c_str());
                form.addRow(label, lineEdit);



                // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
                QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                       Qt::Horizontal, &dialog);
                form.addRow(&buttonBox);
                QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
                QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

                int result = dialog.exec();
                int amount = lineEdit->text().toInt();
                int selection = remainingDrop->currentIndex();

                time_t t = time(0);
                struct tm * now = localtime(&t);
                char today[128];
                sprintf(today, "%d/%d", now->tm_mon+1, now->tm_mday);

               // Show the dialog as modal
                if ( result == QDialog::Accepted) {
                    if( amount <= 0 ){
                        QMessageBox messageBox;
                        messageBox.critical(0,"錯誤","Not Valid!");
                        messageBox.setFixedSize(500,200);
                    }else if(!currentVege->returnTo(amount,today,selection)){
                        QMessageBox error;
                        error.critical(0,"警告",mTranslator
                                       ->translate("不夠菜退!").c_str());
                        error.setFixedSize(500,200);
                    }

                }
            }
        }
        on_vegeList_itemPressed(ui->vegeList->currentItem());
    }
}

void Dialog::closeEvent(QCloseEvent *event) {
    event = NULL;
    if (needSave)
        askSave();
}

void Dialog:: askSave(){
    QMessageBox error;
    QMessageBox::StandardButton reply = error.critical(0,"警告",
                            mTranslator->translate("你要儲存嗎?").c_str(),
                            QMessageBox::No|QMessageBox::Yes);
    error.setFixedSize(500,200);
    if (reply == QMessageBox::Yes) {
        save();
    }
    needSave = 0;
}

void Dialog::on_dumpButton_clicked()
{
    if(currentVege){

        QDialog dialog(this);
        // Use a layout allowing to have a label next to each field
        QFormLayout form(&dialog);

        // Add some text above the fields
        dialog.setWindowTitle(mTranslator->translate("倒").c_str());

        QComboBox* remainingDrop = new QComboBox(&dialog);
        for(int i = 0; i< currentVege -> getRemainingNum(); i++){
            remainingDrop->addItem(currentVege->formatRemaining(i).c_str());
        }
        remainingDrop->setFont(font);

        form.addRow(mTranslator->translate("你要倒那天的菜？").c_str(), remainingDrop);

        QLineEdit *lineEdit = new QLineEdit(&dialog);
        form.addRow(new QLabel(mTranslator->translate("倒幾箱？").c_str()));
        form.addRow(lineEdit);


        QCheckBox checkBox(mTranslator->translate("全部").c_str(),&dialog);
        form.addRow(&checkBox);


        // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
        form.addRow(&buttonBox);
        QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
        QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
        QObject::connect(&checkBox,SIGNAL(toggled(bool)), &dialog, SLOT(reject()));

        int result = dialog.exec();
        int amount = lineEdit->text().toInt();
        int selection = remainingDrop->currentIndex();

        time_t t = time(0);
        struct tm * now = localtime(&t);
        char today[128];
        sprintf(today, "%d/%d", now->tm_mon+1, now->tm_mday);

       // Show the dialog as modal
        if ( result == QDialog::Accepted) {
            if( amount <= 0){
                QMessageBox messageBox;
                messageBox.critical(0,"錯誤","Not Valid!");
                messageBox.setFixedSize(500,200);
            }

            else if(!currentVege->dumpVege(amount, today, selection)){
                QMessageBox error;
                error.critical(0,"警告",mTranslator->translate("不夠數量倒!").c_str());
                error.setFixedSize(500,200);

            }
        }else if (result == QDialog::Rejected){
            if(checkBox.isChecked()){
                QDialog dialog(this);
                // Use a layout allowing to have a label next to each field
                QFormLayout form(&dialog);

                // Add some text above the fields
                form.addRow(new QLabel(mTranslator
                                       ->translate("你確定你要倒掉全部嗎?").c_str()));

                QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                       Qt::Horizontal, &dialog);
                form.addRow(&buttonBox);
                QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
                QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
                if (dialog.exec() == QDialog::Accepted) {
                    amount = currentVege->getRemaining(selection);
                    currentVege->dumpVege(amount, today, selection);
                }
            }
        }
        on_vegeList_itemPressed(ui->vegeList->currentItem());
    }
}

void Dialog:: ListWidgetEditEnd(QWidget *editor, QAbstractItemDelegate::EndEditHint hint){


        QString NewValue = reinterpret_cast<QLineEdit*>(editor)->text();
        currentVege->setVegetableName(NewValue.toUtf8().constData());
        ui->vegeList->clear();
        inventory->sortVeges();

        for(int i = 0; i < inventory->getVegNum(); i++){
            ui->vegeList->addItem(inventory->getVegetableByIndex(i)->getVegetablename().c_str());
            ui->vegeList->item(i)->setFlags (ui->vegeList->item(i)->flags()| Qt::ItemIsEditable);

        }
        if(inventory->getVegNum() == 1){
            ui->vegeList->item(0)->setSelected(true);
            ui->vegeList->setCurrentRow(0);
            on_vegeList_itemPressed(ui->vegeList->item(0));
        }else{
            int temp = inventory->getVegetableIndex(NewValue.toUtf8().constData());
            ui->vegeList->item(temp)->setSelected(true);
            ui->vegeList->setCurrentRow(temp);
            on_vegeList_itemPressed(ui->vegeList->item(temp));
        }

}

void Dialog:: deleteVege(){
    inventory->removeVegetable( ui->vegeList->currentItem()->text().toUtf8().constData());
    needSave = 1;
    qDeleteAll(ui->vegeList->selectedItems());


    if(inventory->getVegNum() == 0){
        currentVege = NULL;
        ui->historyList->clear();
        ui->breakDown->clear();
        ui->returnList->clear();
        ui->Memo_2->clear();
        ui->changeTotal->clear();
    }else{
       on_vegeList_itemPressed(ui->vegeList->currentItem());
    }

}

void Dialog:: deleteHistory(){
    if(currentVege && currentVege->getHistoryNum()){
        if(!(ui->sellCheck->isChecked() && ui->buyCheck->isChecked()
             && ui->dumpCheck->isChecked() && ui->returnCheck->isChecked()
             && ui->tuiCheck->isChecked())){
                QMessageBox messageBox;
                messageBox.critical(0,"錯誤",
                            mTranslator->translate("你要打勾所有的選項才能undo!").c_str());
                messageBox.setFixedSize(500,200);
        }else{
            currentVege->deleteHistory( ui->historyList->currentRow());
                needSave = 1;
            on_vegeList_itemPressed(ui->vegeList->currentItem());
        }
    }
}

void Dialog:: undoHistory(){
    if(currentVege&&currentVege->getHistoryNum()){
        if(!(ui->sellCheck->isChecked() && ui->buyCheck->isChecked() && ui->dumpCheck->isChecked() && ui->returnCheck->isChecked() && ui->tuiCheck->isChecked())){
            QMessageBox messageBox;
            messageBox.critical(0,"錯誤",
                           mTranslator->translate("你要打勾所有的選項才能undo!").c_str());
            messageBox.setFixedSize(500,200);
        }else{
            int amount = 0;
            string dP, company, customer, dS;
            amount = currentVege->getHistoryObject(ui->historyList->currentRow())->getDifference();
            dP =  currentVege->getHistoryObject(ui->historyList->currentRow())->getDatePurchased();
            company = currentVege->getHistoryObject(ui->historyList->currentRow())->getCompany();
            int returnNum = currentVege->getHistoryObject(ui->historyList->currentRow())->getChangeNum();
            customer = currentVege->getHistoryObject(ui->historyList->currentRow())->getCustomer();
            dS = currentVege->getHistoryObject(ui->historyList->currentRow())->getDateSold();

            if(currentVege->getHistoryObject(ui->historyList->currentRow())->getDumped()){
                qDebug()<<"dumped";
                currentVege->restock(amount, dP, company, returnNum);

            }else if(currentVege->getHistoryObject(ui->historyList->currentRow())->getTui()){
                qDebug()<<"Tui";
                currentVege->restock(amount, dP, company, returnNum);
                currentVege->reTui(dS, amount, dP, company);
            }else if(currentVege->getHistoryObject(ui->historyList->currentRow())->getReturned()){
                qDebug()<<"return";
                currentVege->undoRetOrBuy(amount, dP, company, dS, customer);
            }
            else if(currentVege->getHistoryObject(ui->historyList->currentRow())->getCustomer().compare("\t")){ //if sold
                if( currentVege->getHistoryObject(ui->historyList->currentRow())->getSplitNum()){
                    qDebug()<<"split sell";
                    for(int z =0; z< currentVege->getHistoryObject(ui->historyList->currentRow())->getSplitNum(); z++){
                        currentVege->restock(currentVege->getHistoryObject(ui->historyList->currentRow())->getDifferenceS(z),
                                             currentVege->getHistoryObject(ui->historyList->currentRow())->getDatePurchasedS(z),
                                             currentVege->getHistoryObject(ui->historyList->currentRow())->getCompanyS(z),
                                             currentVege->getHistoryObject(ui->historyList->currentRow())->getChangeNumS(z));
                    }
                }else{
                    qDebug()<<"sell";
                    currentVege->restock(amount, dP,company,returnNum);
                }
            }
            else{
                qDebug()<<"buy";
                currentVege->undoRetOrBuy(amount, dP, company, dS, customer);
            }

            currentVege->deleteHistory( ui->historyList->currentRow());
                needSave = 1;

            QList<QListWidgetItem *> removeThis = ui->historyList->
                    findItems (ui->historyList->currentItem()->text().toUtf8().constData(), Qt::MatchExactly);

            delete removeThis[0];
            on_vegeList_itemPressed(ui->vegeList->currentItem());
        }
    }
}

void Dialog::changeToEnglish(){
    mTranslator ->changeLanguage(ENGLISH);
    changeLanguage();
}

void Dialog::changeToChinese(){
    mTranslator ->changeLanguage(CHINESE);
    changeLanguage();
}

void Dialog::changeLanguage(){

    menuBar->changeLanguage();

    ui->Buy->setText((mTranslator ->translate("買")).c_str());
    ui->Sell->setText((mTranslator ->translate("賣")).c_str());
    ui->VegieList->setText((mTranslator ->translate("菜名") + ":").c_str());
    ui->Total->setText((mTranslator ->translate("歷史") + ":").c_str());
    ui->Inventory->setText((mTranslator ->translate("庫存表") + ":").c_str());
    ui->Return->setText(mTranslator ->translate("退貨").c_str());
    ui->dumpButton->setText(mTranslator ->translate("倒").c_str());
    ui->clearReturnButton->setText(mTranslator ->translate("清掉退貨單").c_str());
    ui->clearHistoryButton->setText(mTranslator ->translate("清掉歷史").c_str());
    ui->Return->setText(mTranslator ->translate("退貨").c_str());

    ui->tuiCheck->setText(mTranslator ->translate("退給公司").c_str());
    ui->buyCheck->setText(mTranslator ->translate("買").c_str());
    ui->sellCheck->setText(mTranslator ->translate("賣").c_str());
    ui->dumpCheck->setText(mTranslator ->translate("倒").c_str());
    ui->returnCheck->setText(mTranslator ->translate("退給農場").c_str());

    ui->Returns->setText((mTranslator ->translate("退貨單") + ":").c_str());
}

void Dialog::writeInt(int temp, fstream *fio){
    fio->write((const char *) &(temp), sizeof(int));
}

void Dialog::writeString(string temp, fstream* fio){
    char* a=new char[temp.size()+1];
    a[temp.size()]=0;
    int tempNum = temp.size();
    fio->write((const char *) &(tempNum), sizeof(int));
    memcpy(a,temp.c_str(),temp.size());
    fio->write(a,strlen(a));
}

void Dialog::on_clearHistoryButton_clicked()
{
    if(currentVege){
        QDialog dialog(this);

        // Use a layout allowing to have a label next to each field
        QFormLayout form(&dialog);

        // Add some text above the fields
        dialog.setWindowTitle(mTranslator->translate("刪掉歷史").c_str());


        QLineEdit *lineEdit = new QLineEdit(&dialog);
        form.addRow(new QLabel(mTranslator->translate("刪掉多少？").c_str()));
        form.addRow(lineEdit);


        QCheckBox checkBox(mTranslator->translate("全部").c_str(),&dialog);
        form.addRow(&checkBox);


        // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
        form.addRow(&buttonBox);
        QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
        QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
        QObject::connect(&checkBox,SIGNAL(toggled(bool)), &dialog, SLOT(reject()));

        int result = dialog.exec();
        int amount = lineEdit->text().toInt();


       // Show the dialog as modal
        if ( result == QDialog::Accepted) {
            if( amount <= 0 ){
                QMessageBox messageBox;
                messageBox.critical(0,"錯誤","Not Valid!");
                messageBox.setFixedSize(500,200);
            }

            else if(!currentVege->clearHist(amount)){
                QMessageBox error;
                error.critical(0,"警告",mTranslator->translate("不夠數量刪除!").c_str());
                error.setFixedSize(500,200);

            }
        }else if (result == QDialog::Rejected){
            if(checkBox.isChecked()){
                QDialog dialog(this);
                // Use a layout allowing to have a label next to each field
                QFormLayout form(&dialog);

                // Add some text above the fields
                form.addRow(new QLabel(mTranslator->translate("你確定你要刪掉全部嗎?").c_str()));

                QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                       Qt::Horizontal, &dialog);
                form.addRow(&buttonBox);
                QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
                QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
                if (dialog.exec() == QDialog::Accepted) {
                    amount = currentVege->getHistoryNum();
                    currentVege->clearHist(amount);
                }
            }
        }

        on_vegeList_itemPressed(ui->vegeList->currentItem());
    }
}

void Dialog::on_clearReturnButton_clicked()
{
    if(currentVege){
        //clear return
        ui->returnToFarm->clear();
        currentVege->clearTui();
    }
}
