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

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    inventory(NULL),
    currentVege(NULL),
    needSave(0),
    font("Courier",-1,QFont::Bold,false)

{

    ui->setupUi(this);
    ui->vegeList->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->historyList->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->breakDown->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->returnList->setContextMenuPolicy(Qt::ActionsContextMenu);
    connect(ui->vegeList->itemDelegate(), SIGNAL(closeEditor(QWidget*, QAbstractItemDelegate::EndEditHint)),
            this, SLOT(ListWidgetEditEnd(QWidget*, QAbstractItemDelegate::EndEditHint)));
    inventory = new Inventory();
    createMenu();


    deleteVegeAction = new QAction(tr("&Delete"),this);
    connect(deleteVegeAction, SIGNAL(triggered()), this, SLOT(deleteVege()));
    ui->vegeList->addAction(deleteVegeAction);

    undoHistoryAction = new QAction(tr("&Undo"),this);
    connect(undoHistoryAction, SIGNAL(triggered()), this, SLOT(undoHistory()));
    ui->historyList->addAction(undoHistoryAction);

    deleteHistoryAction = new QAction(tr("&delete"),this);
    connect(deleteHistoryAction, SIGNAL(triggered()), this, SLOT(deleteHistory()));
    ui->historyList->addAction(deleteHistoryAction);

    QFont vFont = font;
    vFont.setPixelSize(18);
    ui->vegeList->setFont(vFont);
}

Dialog::~Dialog()
{
    delete ui;
}



void Dialog::createMenu()
{
    menuBar = new QMenuBar(this);
    fileMenu = new QMenu("File", this);
    editMenu = new QMenu("Edit", this);

    newAction = fileMenu->addAction(mTranslator.translate("New File").c_str());
    newAction->setShortcut(QKeySequence::New);
    loadAction = fileMenu->addAction(mTranslator.translate("Load File").c_str());
    loadAction->setShortcut(QKeySequence::Open);
    saveAction =fileMenu->addAction(mTranslator.translate("Save").c_str());;
    saveAction->setShortcut(QKeySequence::Save);
    saveAsAction = fileMenu->addAction(mTranslator.translate("Save As").c_str());
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    printAction = fileMenu->addAction(tr("印 Inventory"));
    printAction2 = fileMenu->addAction(tr("印 History"));
    fileMenu->addSeparator();
    translateMenu = fileMenu->addMenu(tr("Translate"));

    addPersonAction = editMenu->addAction(mTranslator.translate("Add Customer").c_str());
    addCompanyAction = editMenu->addAction(mTranslator.translate("Add Company").c_str());
    addVegetableAction = editMenu->addAction(mTranslator.translate("Add Vegetable").c_str());
    addUnitAction = editMenu->addAction(mTranslator.translate("Add Unit").c_str());
    removePersonAction = editMenu->addAction(mTranslator.translate("Remove Customer").c_str());
    removeCompanyAction = editMenu->addAction(mTranslator.translate("Remove Company").c_str());
    removeVegetableAction = editMenu->addAction(mTranslator.translate("Remove Vegetable").c_str());
    removeUnitAction = editMenu->addAction(mTranslator.translate("Remove Unit").c_str());
    changeToEnglishAction = translateMenu->addAction("English");
    changeToChineseAction = translateMenu->addAction("中文");


    menuBar->addMenu(fileMenu);
    menuBar->addMenu(editMenu);

    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(loadAction, SIGNAL(triggered()), this, SLOT(loadFile()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(printAction, SIGNAL(triggered()), this, SLOT(printInventory()));
    connect(printAction2, SIGNAL(triggered()), this, SLOT(printHistory()));
    connect(addVegetableAction, SIGNAL(triggered()), this, SLOT(addVegetable()));
    connect(addPersonAction, SIGNAL(triggered()), this, SLOT(addPerson()));
    connect(addCompanyAction, SIGNAL(triggered()), this, SLOT(addCompany()));
    connect(addUnitAction, SIGNAL(triggered()), this, SLOT(addUnit()));
    connect(changeToEnglishAction, SIGNAL(triggered()), this, SLOT(changeToEnglish()));
    connect(changeToChineseAction, SIGNAL(triggered()), this, SLOT(changeToChinese()));

    connect(removeVegetableAction, SIGNAL(triggered()), this, SLOT(removeVegetable()));
    connect(removePersonAction, SIGNAL(triggered()), this, SLOT(removePerson()));
    connect(removeCompanyAction, SIGNAL(triggered()), this, SLOT(removeCompany()));
    connect(removeUnitAction, SIGNAL(triggered()), this, SLOT(removeUnit()));

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
    ui ->changeTotal->setText(QString::number(currentVege->getTotalVeges()) + " " + QString::fromStdString(currentVege->getUnit()));

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
        dialog.setWindowTitle("買");
        // Use a layout allowing to have a label next to each field
        QFormLayout form(&dialog);

        // Add some text above the fields
        form.addRow(new QLabel("輸入進菜資料"));

        QLineEdit *lineEdit = new QLineEdit(&dialog);
        QString label = QString("買了多少？");
        form.addRow(label, lineEdit);

        QString label2 = QString("哪家公司的?");
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
        QString label4 = QString("幾號來的菜?");
        form.addRow(label4, date);


        QLineEdit *price = new QLineEdit(&dialog);
        QString label5 = QString("進價多少?");
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
                messageBox.critical(0,"錯誤","Not Valid!");
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
    if(currentVege){

        QDialog dialog(this);
        dialog.setWindowTitle("");
        // Use a layout allowing to have a label next to each field
        QFormLayout form(&dialog);

        // Add some text above the fields
        form.addRow(new QLabel("輸入賣菜資料"));



        QLineEdit *lineEdit = new QLineEdit(&dialog);
        QString label = QString("賣了多少？");
        form.addRow(label, lineEdit);


        QComboBox* customerDrop = new QComboBox(&dialog);
        for(int i = 0; i< inventory->getPersonNum(); i++){
            customerDrop->addItem(inventory->getPerson(i).c_str());
        }
        customerDrop->setFont(font);

        QString label2 = QString("賣给谁？");
        form.addRow(label2, customerDrop);

        QLineEdit * date = new QLineEdit(&dialog);
        time_t t = time(0);
        struct tm * now = localtime(&t);
        char buffer[128];
        sprintf(buffer, "%d/%d", now->tm_mon+1, now->tm_mday);
        date -> setText(QString::fromUtf8(buffer));
        QString label4 = QString("那天賣出");
        form.addRow(label4, date);

        QLineEdit *price = new QLineEdit(&dialog);
        QString label5 = QString("賣多少錢?");
        form.addRow(label5, price);

        QComboBox* remainingDrop = new QComboBox(&dialog);
        for(int i = 0; i< currentVege -> getRemainingNum(); i++){
            remainingDrop->addItem(currentVege->formatRemaining(i).c_str());
        }
        remainingDrop->setFont(font);

        form.addRow("你要賣那天的菜？", remainingDrop);

        QCheckBox checkBox("聯合賣",&dialog);
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
            if( !amount ){
                QMessageBox messageBox;
                messageBox.critical(0,"錯誤","Not Valid!");
                messageBox.setFixedSize(500,200);
                additionalSell(amount, customerDrop->currentIndex(), date->text(), price->text());
            }


            else if(amount > currentVege -> getTotalVeges() ){
                QMessageBox messageBox;
                messageBox.critical(0,"錯誤","不夠賣!");
                messageBox.setFixedSize(500,200);
                additionalSell(amount, customerDrop->currentIndex(), date->text(), price->text());
            }

            else if(!currentVege->sellVege(amount, customer,
                                      date->text().toUtf8().constData(),
                                      price->text().toUtf8().constData(), selection)){
                QMessageBox error;
                QMessageBox::StandardButton reply = error.critical(0,"警告","菜不足,繼續買另外一天的菜?",
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
                    messageBox.critical(0,"警告","聯合數字錯誤!");
                    messageBox.setFixedSize(500,200);
                    additionalSell(amount, customerDrop->currentIndex(), date->text(), price->text());
                }
            }
        }
    }
}

void Dialog::additionalSell(int amount, int cusIndex, QString dateB, QString priceB  ){
    QDialog dialog(this);
    dialog.setWindowTitle("賣");
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add some text above the fields
    form.addRow(new QLabel("輸入賣菜資料"));



    QLineEdit *lineEdit = new QLineEdit(&dialog);
    QString label = QString("賣了多少？");
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
    QString label2 = QString("賣给谁？");
    form.addRow(label2, customerDrop);

    QLineEdit * date = new QLineEdit(&dialog);
    date -> setText(dateB);
    QString label4 = QString("那天賣出");
    form.addRow(label4, date);

    QLineEdit *price = new QLineEdit(&dialog);
    QString label5 = QString("賣多少錢?");
    price ->setText(priceB);
    form.addRow(label5, price);

    QComboBox* remainingDrop = new QComboBox(&dialog);
    for(int i = 0; i< currentVege -> getRemainingNum(); i++){
        remainingDrop->addItem(currentVege->formatRemaining(i).c_str());
    }
    remainingDrop->setFont(font);

    form.addRow("你要賣那天的菜？", remainingDrop);

    QCheckBox checkBox("聯合賣",&dialog);
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
        if( !amount ){
            QMessageBox messageBox;
            messageBox.critical(0,"錯誤","Not Valid!");
            messageBox.setFixedSize(500,200);
            additionalSell(amount, customerDrop->currentIndex(), date->text(), price->text());
        }


        else if(amount > currentVege -> getTotalVeges() ){
            QMessageBox messageBox;
            messageBox.critical(0,"錯誤","不夠賣!");
            messageBox.setFixedSize(500,200);
            additionalSell(amount, customerDrop->currentIndex(), date->text(), price->text());
        }

        else if(!currentVege->sellVege(amount, customer,
                                  date->text().toUtf8().constData(),
                                  price->text().toUtf8().constData(), selection)){
            QMessageBox error;
            QMessageBox::StandardButton reply = error.critical(0,"警告","菜不足,繼續買另外一天的菜?",
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
                messageBox.critical(0,"警告","聯合數字錯誤!");
                messageBox.setFixedSize(500,200);
                additionalSell(amount, customerDrop->currentIndex(), date->text(), price->text());
            }
        }
    }
}

void Dialog::addVegetable(){
    QDialog dialog(this);
    dialog.setWindowTitle("加入新的菜名");
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add some text above the fields
    form.addRow(new QLabel("請輸入你要加的菜名"));

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
            messageBox.critical(0,"警告","沒單位!");
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
    QString text = QInputDialog::getText(this, tr("加入新公司"),
                      tr("請輸入你要加的新公司名字?"), QLineEdit::Normal,"", &ok);
    inventory->addCompany(text.toUtf8().constData());
        needSave = 1;
}

void Dialog::addPerson(){
    bool ok;
    QString text = QInputDialog::getText(this, tr("加入新客戶"),
                      tr("請輸入你要加的新客戶名字?"), QLineEdit::Normal,"", &ok);
    inventory->addPerson(text.toUtf8().constData());
        needSave = 1;
}

void Dialog::addUnit(){
    bool ok;
    QString text = QInputDialog::getText(this, tr("加入單位"),
                      tr("請輸入你要加的單位?"), QLineEdit::Normal,"", &ok);
    inventory->addUnit(text.toUtf8().constData());
        needSave = 1;
}

void Dialog::removeCompany(){
    QDialog dialog(this);
    dialog.setWindowTitle("刪除公司");
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add some text above the fields
    dialog.setWindowTitle("刪除公司");

    QComboBox* companyDrop = new QComboBox(&dialog);
        for(int i=0; i< inventory->getCompanyNum(); i++){
            companyDrop->addItem(inventory->getCompany(i).c_str());
    }
    companyDrop->setFont(font);
    form.addRow("你要刪掉哪一個公司？", companyDrop);


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
    QDialog dialog(this);
    dialog.setWindowTitle("刪除客戶");
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add some text above the fields
    dialog.setWindowTitle("刪除客戶");

    QComboBox* personDrop = new QComboBox(&dialog);
        for(int i=0; i< inventory->getPersonNum(); i++){
            personDrop->addItem(inventory->getPerson(i).c_str());
    }
    personDrop->setFont(font);

    form.addRow("你要刪掉哪一個客戶？", personDrop);

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

    QDialog dialog(this);
    dialog.setWindowTitle("刪掉單位");
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add some text above the fields
    dialog.setWindowTitle("刪掉單位");

    QComboBox* unitDrop = new QComboBox(&dialog);
        for(int i=0; i< inventory->getUnitNum(); i++){
            unitDrop->addItem(inventory->getUnit(i).c_str());
    }
    unitDrop->setFont(font);

    form.addRow("你要刪掉哪一個單位？", unitDrop);

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
    QDialog dialog(this);
    dialog.setWindowTitle("刪掉菜名");
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add some text above the fields
    dialog.setWindowTitle("刪掉菜名");

    QComboBox* vegeDrop = new QComboBox(&dialog);
        for(int i=0; i< inventory->getVegNum(); i++){
            vegeDrop->addItem(inventory->getVegetableByIndex(i)->getVegetablename().c_str());
    }
    vegeDrop->setFont(font);

    form.addRow("你要刪掉哪一個菜名？", vegeDrop);

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
    dialog1.setWindowTitle("列印資料");
    QFormLayout form(&dialog1);

    QLineEdit *lineEdit = new QLineEdit(&dialog1);
    QString label = QString("請輸入字體大小");
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
    dialog1.setWindowTitle("列印資料");
    QFormLayout form(&dialog1);

    QLineEdit *l2 = new QLineEdit(&dialog1);
    QString label2 = QString("你要列印哪一天的日期?");
    l2 -> setText(today.c_str());
    form.addRow(label2,l2);

    QLineEdit *lineEdit = new QLineEdit(&dialog1);
    QString label = QString("請輸入字體大小");
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
    form.addRow(new QLabel("你要幾家公司聯合? (最多4家)"));

    QLineEdit *lineEdit = new QLineEdit(&dialog);
    form.addRow( lineEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                           Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
    if (dialog.exec() == QDialog::Accepted) {
        return lineEdit->text().toInt();
    }
    return 0;
}

void Dialog::splitSell(int splits, int amount, int cusIndex, QString dateB, QString priceB  ){
    QDialog dialog(this);
    dialog.setWindowTitle("聯合賣");
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add some text above the fields
    form.addRow(new QLabel("輸入賣菜資料"));



    QLineEdit *lineEdit = new QLineEdit(&dialog);
    QString label = QString("賣了多少？");
    lineEdit -> setText(QString::number(amount));
    form.addRow(label, lineEdit);


    QComboBox* customerDrop = new QComboBox(&dialog);
    for(int i = 0; i< inventory->getPersonNum(); i++){
        customerDrop->addItem(inventory->getPerson(i).c_str());
    }

    customerDrop->setFont(font);
    customerDrop->setCurrentIndex(cusIndex);
    QString label2 = QString("賣给谁？");
    form.addRow(label2, customerDrop);

    QLineEdit * date = new QLineEdit(&dialog);
    date -> setText(dateB);
    QString label4 = QString("那天賣出");
    form.addRow(label4, date);

    QLineEdit *price = new QLineEdit(&dialog);
    QString label5 = QString("賣多少錢?");
    price ->setText(priceB);
    form.addRow(label5, price);

    form.addRow(new QLabel("你要賣那天的菜？"));
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
        if( !amount ){
            QMessageBox messageBox;
            messageBox.critical(0,"錯誤","Not Valid!");
            messageBox.setFixedSize(500,200);
            splitSell(splits, amount, customerDrop->currentIndex(), date->text(), price->text());
        }
        else if(amount != input1+input2+input3+input4){
            QMessageBox messageBox;
            messageBox.critical(0,"錯誤","輸入的數量不一樣!");
            messageBox.setFixedSize(500,200);
            splitSell(splits, amount, customerDrop->currentIndex(), date->text(), price->text());
        }
        else if(amount > currentVege -> getTotalVeges() ){
            QMessageBox messageBox;
            messageBox.critical(0,"錯誤","不夠賣!");
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
            messageBox.critical(0,"錯誤","輸入的數量太多!");
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

    string datafile1 = filename.toUtf8().constData();

    const char* datafile = datafile1.c_str();

    inventory->setFileName(datafile);
    fstream* fio = new fstream (datafile, ios :: out |ios::binary);
    fio->seekp(0, ios::beg);
    int temp = inventory->getCompanyNum();
    fio->write((const char *) &(temp), sizeof(int));
    temp = inventory->getPersonNum();
    fio->write((const char *) &(temp), sizeof(int));
    temp = inventory->getUnitNum();
    fio->write((const char *) &(temp), sizeof(int));
    temp = inventory->getVegNum();
    fio->write((const char *) &(temp), sizeof(int));
/*
    string temp2 = inventory->getFileName();
    char *a=new char[temp2.size()+1];
    a[temp2.size()]=0;
    int tempNum = temp2.size();
    fio->write((const char *) &(tempNum), sizeof(int));
    memcpy(a,temp2.c_str(),temp2.size());
    fio->write(a,strlen(a));*/


    for(int i = 0; i < inventory->getCompanyNum() ; i++){

        string temp3 = inventory->getCompany(i);
        char *a=new char[temp3.size()+1];
        a[temp3.size()]=0;
        int tempNum3 = temp3.size();
        fio->write((const char *) &(tempNum3), sizeof(int));
        memcpy(a,temp3.c_str(),temp3.size());
        fio->write(a,strlen(a));

    }

    for(int i = 0; i < inventory->getPersonNum() ; i++){

        string temp3 = inventory->getPerson(i);
        char *a=new char[temp3.size()+1];
        a[temp3.size()]=0;
        int tempNum3 = temp3.size();
        fio->write((const char *) &(tempNum3), sizeof(int));
        memcpy(a,temp3.c_str(),temp3.size());
        fio->write(a,strlen(a));
    }

    for(int i = 0; i < inventory->getUnitNum() ; i++){

        string temp3 = inventory->getUnit(i);
        char *a=new char[temp3.size()+1];
        a[temp3.size()]=0;
        int tempNum3 = temp3.size();
        fio->write((const char *) &(tempNum3), sizeof(int));
        memcpy(a,temp3.c_str(),temp3.size());
        fio->write(a,strlen(a));

    }


    for(int i = 0; i < inventory->getVegNum() ; i++){
        string temp3 = inventory->getVegetableByIndex(i)->getVegetablename();
        char *a=new char[temp3.size()+1];
        a[temp3.size()]=0;
        int tempNum3 = temp3.size();
        fio->write((const char *) &(tempNum3), sizeof(int));
        memcpy(a,temp3.c_str(),temp3.size());
        fio->write(a,strlen(a));



        int temp2 = inventory->getVegetableByIndex(i)->getTotalVeges();
        fio->write((const char *) &(temp2), sizeof(int));

        string temp4 = inventory->getVegetableByIndex(i)->getUnit() ;
        a=new char[temp4.size()+1];
        a[temp4.size()]=0;
        int tempNum4 = temp4.size();
        fio->write((const char *) &(tempNum4), sizeof(int));
        memcpy(a,temp4.c_str(),temp4.size());
        fio->write(a,strlen(a));

        temp2 = inventory->getVegetableByIndex(i)->getRemainingNum();
        fio->write((const char *) &(temp2), sizeof(int));
        temp2 =inventory->getVegetableByIndex(i)->getReturnNum();
        fio->write((const char *) &(temp2), sizeof(int));
        temp2 = inventory->getVegetableByIndex(i)->getHistoryNum();
        fio->write((const char *) &(temp2), sizeof(int));
        temp2 = inventory->getVegetableByIndex(i)->getTuiNum();
        fio->write((const char *) &(temp2), sizeof(int));
        string temp5 = inventory->getVegetableByIndex(i)->getMemo();
        a=new char[temp5.size()+1];
        a[temp5.size()]=0;
        int tempNum5 = temp5.size();
        fio->write((const char *) &(tempNum5), sizeof(int));
        memcpy(a,temp5.c_str(),temp5.size());
        fio->write(a,strlen(a));

        for( int j = 0; j< inventory->getVegetableByIndex(i)->getTuiNum(); j++){
            string temp = inventory->getVegetableByIndex(i)->getTuiObject(j)->getDatePurchased() ;
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            int tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));


            temp = inventory->getVegetableByIndex(i)->getTuiObject(j)->getCompany();
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));


            int temp2 = inventory->getVegetableByIndex(i)->getTuiObject(j)->getReturn() ;
            fio->write((const char *) &(temp2), sizeof(int));


            temp = inventory->getVegetableByIndex(i)->getTuiObject(j)->getDateReturned() ;
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));

        }

        for(int j = 0; j < inventory->getVegetableByIndex(i)->getRemainingNum() ; j++){
            string temp = inventory->getVegetableByIndex(i)->getRemainingObject(j)->getDate();
            char *a=new char[temp.size()+1];
            a[temp.size()]=0;
            int tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));



            temp = inventory->getVegetableByIndex(i)->getRemainingObject(j)->getCompany();
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));

            temp = inventory->getVegetableByIndex(i)->getRemainingObject(j)->getPrice();
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));

            int temp2 = inventory->getVegetableByIndex(i)->getRemainingObject(j)->getRemaining();
            fio->write((const char *) &(temp2 ), sizeof(int));
            temp2 = inventory->getVegetableByIndex(i)->getRemainingObject(j)->getReturn() ;
            fio->write((const char *) &(temp2), sizeof(int));
        }

        for(int j = 0; j < inventory->getVegetableByIndex(i)->getReturnNum() ; j++){
            string temp = inventory->getVegetableByIndex(i)->getReturnObject(j)->getDatePurchased() ;
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            int tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));



            temp = inventory->getVegetableByIndex(i)->getReturnObject(j)->getCompany();
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));



            int temp2 = inventory->getVegetableByIndex(i)->getReturnObject(j)->getReturn() ;
            fio->write((const char *) &(temp2), sizeof(int));
            temp = inventory->getVegetableByIndex(i)->getReturnObject(j)->getReturner() ;
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));


            temp = inventory->getVegetableByIndex(i)->getReturnObject(j)->getDateReturned() ;
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));

        }

        for(int j = 0; j < inventory->getVegetableByIndex(i)->getHistoryNum() ; j++){
            string temp = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getDateSold() ;
            char *a=new char[temp.size()+1];
            a[temp.size()]=0;
            int tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));


            temp = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getDatePurchased() ;
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));


            int temp2 = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getDifference();
            fio->write((const char *) &(temp2 ), sizeof(int));
            temp = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getCustomer() ;
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));


            temp = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getPrice();
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));


            temp = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getCompany();
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));


            temp = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getDateToCompare();
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));


            temp2 = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getReturned();
            fio->write((const char *) &(temp2 ), sizeof(int));
            temp2 = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getDumped() ;
            fio->write((const char *) &(temp2), sizeof(int));
            int temp21 = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getSplitNum();
            fio->write((const char *) &(temp21 ), sizeof(int));
            temp2 = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getChangeNum();
            fio->write((const char *) &(temp2 ), sizeof(int));
            temp2 = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getTui();
            fio->write((const char *) &(temp2 ), sizeof(int));
            for(int z = 0; z < temp21; z++){

                temp = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getDateSoldS(z) ;
                a=new char[temp.size()+1];
                a[temp.size()]=0;
                int tempNum = temp.size();
                fio->write((const char *) &(tempNum), sizeof(int));
                memcpy(a,temp.c_str(),temp.size());
                fio->write(a,strlen(a));


                temp = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getDatePurchasedS(z) ;
                a=new char[temp.size()+1];
                a[temp.size()]=0;
                tempNum = temp.size();
                fio->write((const char *) &(tempNum), sizeof(int));
                memcpy(a,temp.c_str(),temp.size());
                fio->write(a,strlen(a));


                int temp2 = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getDifferenceS(z);
                fio->write((const char *) &(temp2 ), sizeof(int));

                temp = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getCustomerS(z) ;
                a=new char[temp.size()+1];
                a[temp.size()]=0;
                tempNum = temp.size();
                fio->write((const char *) &(tempNum), sizeof(int));
                memcpy(a,temp.c_str(),temp.size());
                fio->write(a,strlen(a));


                temp = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getPriceS(z);
                a=new char[temp.size()+1];
                a[temp.size()]=0;
                tempNum = temp.size();
                fio->write((const char *) &(tempNum), sizeof(int));
                memcpy(a,temp.c_str(),temp.size());
                fio->write(a,strlen(a));


                temp = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getCompanyS(z);
                a=new char[temp.size()+1];
                a[temp.size()]=0;
                tempNum = temp.size();
                fio->write((const char *) &(tempNum), sizeof(int));
                memcpy(a,temp.c_str(),temp.size());
                fio->write(a,strlen(a));


                temp = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getDateToCompareS(z);
                a=new char[temp.size()+1];
                a[temp.size()]=0;
                tempNum = temp.size();
                fio->write((const char *) &(tempNum), sizeof(int));
                memcpy(a,temp.c_str(),temp.size());
                fio->write(a,strlen(a));

                temp2 = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getReturnedS(z);
                fio->write((const char *) &(temp2 ), sizeof(int));
                temp2 = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getDumpedS(z) ;
                fio->write((const char *) &(temp2), sizeof(int));
                temp2 = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getSplitNumS(z);
                fio->write((const char *) &(temp2 ), sizeof(int));
                temp2 = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getChangeNumS(z);
                fio->write((const char *) &(temp2 ), sizeof(int));
                temp2 = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getTuiS(z);
                fio->write((const char *) &(temp2 ), sizeof(int));
            }

        }


    }
    fio->close();
    needSave = 0;
}

void Dialog:: newFile(){
    if(needSave)
        askSave();
    //clear everything
    inventory->remove();
  //  inventory = new Inventory();
    ui->historyList->clear();
    ui->breakDown->clear();
    ui->returnList->clear();
    ui->vegeList->clear();
    ui->Memo_2->clear();
    ui->changeTotal->clear();
    //ui->buyCheck->
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
        needSave=0;
    }
}

void Dialog::save(){

    if(!inventory->getFileName().compare("")){
        saveAs();
    }else{


    const char* datafile = inventory->getFileName().c_str();

    inventory->setFileName(datafile);
    fstream* fio = new fstream (datafile, ios :: out| ios::binary);
    fio->seekp(0, ios::beg);
    int temp = inventory->getCompanyNum();
    fio->write((const char *) &(temp), sizeof(int));
    temp = inventory->getPersonNum();
    fio->write((const char *) &(temp), sizeof(int));
    temp = inventory->getUnitNum();
    fio->write((const char *) &(temp), sizeof(int));
    temp = inventory->getVegNum();
    fio->write((const char *) &(temp), sizeof(int));
/*
    string temp2 = inventory->getFileName();
    char *a=new char[temp2.size()+1];
    a[temp2.size()]=0;
    int tempNum = temp2.size();
    fio->write((const char *) &(tempNum), sizeof(int));
    memcpy(a,temp2.c_str(),temp2.size());
    fio->write(a,strlen(a));*/


    for(int i = 0; i < inventory->getCompanyNum() ; i++){

        string temp3 = inventory->getCompany(i);
        char *a=new char[temp3.size()+1];
        a[temp3.size()]=0;
        int tempNum3 = temp3.size();
        fio->write((const char *) &(tempNum3), sizeof(int));
        memcpy(a,temp3.c_str(),temp3.size());
        fio->write(a,strlen(a));

    }

    for(int i = 0; i < inventory->getPersonNum() ; i++){

        string temp3 = inventory->getPerson(i);
        char *a=new char[temp3.size()+1];
        a[temp3.size()]=0;
        int tempNum3 = temp3.size();
        fio->write((const char *) &(tempNum3), sizeof(int));
        memcpy(a,temp3.c_str(),temp3.size());
        fio->write(a,strlen(a));
    }

    for(int i = 0; i < inventory->getUnitNum() ; i++){

        string temp3 = inventory->getUnit(i);
        char *a=new char[temp3.size()+1];
        a[temp3.size()]=0;
        int tempNum3 = temp3.size();
        fio->write((const char *) &(tempNum3), sizeof(int));
        memcpy(a,temp3.c_str(),temp3.size());
        fio->write(a,strlen(a));

    }


    for(int i = 0; i < inventory->getVegNum() ; i++){
        string temp3 = inventory->getVegetableByIndex(i)->getVegetablename();
        char *a=new char[temp3.size()+1];
        a[temp3.size()]=0;
        int tempNum3 = temp3.size();
        fio->write((const char *) &(tempNum3), sizeof(int));
        memcpy(a,temp3.c_str(),temp3.size());
        fio->write(a,strlen(a));



        int temp2 = inventory->getVegetableByIndex(i)->getTotalVeges();
        fio->write((const char *) &(temp2), sizeof(int));

        string temp4 = inventory->getVegetableByIndex(i)->getUnit() ;
        a=new char[temp4.size()+1];
        a[temp4.size()]=0;
        int tempNum4 = temp4.size();
        fio->write((const char *) &(tempNum4), sizeof(int));
        memcpy(a,temp4.c_str(),temp4.size());
        fio->write(a,strlen(a));

        temp2 = inventory->getVegetableByIndex(i)->getRemainingNum();
        fio->write((const char *) &(temp2), sizeof(int));
        temp2 =inventory->getVegetableByIndex(i)->getReturnNum();
        fio->write((const char *) &(temp2), sizeof(int));
        temp2 = inventory->getVegetableByIndex(i)->getHistoryNum();
        fio->write((const char *) &(temp2), sizeof(int));
        temp2 = inventory->getVegetableByIndex(i)->getTuiNum();
        fio->write((const char *) &(temp2), sizeof(int));
        string temp5 = inventory->getVegetableByIndex(i)->getMemo();
        a=new char[temp5.size()+1];
        a[temp5.size()]=0;
        int tempNum5 = temp5.size();
        fio->write((const char *) &(tempNum5), sizeof(int));
        memcpy(a,temp5.c_str(),temp5.size());
        fio->write(a,strlen(a));

        for( int j = 0; j< inventory->getVegetableByIndex(i)->getTuiNum(); j++){
            string temp = inventory->getVegetableByIndex(i)->getTuiObject(j)->getDatePurchased() ;
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            int tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));


            temp = inventory->getVegetableByIndex(i)->getTuiObject(j)->getCompany();
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));


            int temp2 = inventory->getVegetableByIndex(i)->getTuiObject(j)->getReturn() ;
            fio->write((const char *) &(temp2), sizeof(int));


            temp = inventory->getVegetableByIndex(i)->getTuiObject(j)->getDateReturned() ;
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));

        }

        for(int j = 0; j < inventory->getVegetableByIndex(i)->getRemainingNum() ; j++){
            string temp = inventory->getVegetableByIndex(i)->getRemainingObject(j)->getDate();
            char *a=new char[temp.size()+1];
            a[temp.size()]=0;
            int tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));



            temp = inventory->getVegetableByIndex(i)->getRemainingObject(j)->getCompany();
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));

            temp = inventory->getVegetableByIndex(i)->getRemainingObject(j)->getPrice();
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));

            int temp2 = inventory->getVegetableByIndex(i)->getRemainingObject(j)->getRemaining();
            fio->write((const char *) &(temp2 ), sizeof(int));
            temp2 = inventory->getVegetableByIndex(i)->getRemainingObject(j)->getReturn() ;
            fio->write((const char *) &(temp2), sizeof(int));
        }

        for(int j = 0; j < inventory->getVegetableByIndex(i)->getReturnNum() ; j++){
            string temp = inventory->getVegetableByIndex(i)->getReturnObject(j)->getDatePurchased() ;
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            int tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));



            temp = inventory->getVegetableByIndex(i)->getReturnObject(j)->getCompany();
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));



            int temp2 = inventory->getVegetableByIndex(i)->getReturnObject(j)->getReturn() ;
            fio->write((const char *) &(temp2), sizeof(int));
            temp = inventory->getVegetableByIndex(i)->getReturnObject(j)->getReturner() ;
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));


            temp = inventory->getVegetableByIndex(i)->getReturnObject(j)->getDateReturned() ;
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));

        }

        for(int j = 0; j < inventory->getVegetableByIndex(i)->getHistoryNum() ; j++){
            string temp = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getDateSold() ;
            char *a=new char[temp.size()+1];
            a[temp.size()]=0;
            int tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));


            temp = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getDatePurchased() ;
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));


            int temp2 = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getDifference();
            fio->write((const char *) &(temp2 ), sizeof(int));
            temp = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getCustomer() ;
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));


            temp = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getPrice();
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));


            temp = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getCompany();
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));


            temp = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getDateToCompare();
            a=new char[temp.size()+1];
            a[temp.size()]=0;
            tempNum = temp.size();
            fio->write((const char *) &(tempNum), sizeof(int));
            memcpy(a,temp.c_str(),temp.size());
            fio->write(a,strlen(a));


            temp2 = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getReturned();
            fio->write((const char *) &(temp2 ), sizeof(int));
            temp2 = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getDumped() ;
            fio->write((const char *) &(temp2), sizeof(int));
            int temp21 = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getSplitNum();
            fio->write((const char *) &(temp21 ), sizeof(int));
            temp2 = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getChangeNum();
            fio->write((const char *) &(temp2 ), sizeof(int));
            temp2 = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getTui();
            fio->write((const char *) &(temp2 ), sizeof(int));
            for(int z = 0; z < temp21; z++){

                temp = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getDateSoldS(z) ;
                a=new char[temp.size()+1];
                a[temp.size()]=0;
                int tempNum = temp.size();
                fio->write((const char *) &(tempNum), sizeof(int));
                memcpy(a,temp.c_str(),temp.size());
                fio->write(a,strlen(a));


                temp = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getDatePurchasedS(z) ;
                a=new char[temp.size()+1];
                a[temp.size()]=0;
                tempNum = temp.size();
                fio->write((const char *) &(tempNum), sizeof(int));
                memcpy(a,temp.c_str(),temp.size());
                fio->write(a,strlen(a));


                int temp2 = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getDifferenceS(z);
                fio->write((const char *) &(temp2 ), sizeof(int));

                temp = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getCustomerS(z) ;
                a=new char[temp.size()+1];
                a[temp.size()]=0;
                tempNum = temp.size();
                fio->write((const char *) &(tempNum), sizeof(int));
                memcpy(a,temp.c_str(),temp.size());
                fio->write(a,strlen(a));


                temp = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getPriceS(z);
                a=new char[temp.size()+1];
                a[temp.size()]=0;
                tempNum = temp.size();
                fio->write((const char *) &(tempNum), sizeof(int));
                memcpy(a,temp.c_str(),temp.size());
                fio->write(a,strlen(a));


                temp = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getCompanyS(z);
                a=new char[temp.size()+1];
                a[temp.size()]=0;
                tempNum = temp.size();
                fio->write((const char *) &(tempNum), sizeof(int));
                memcpy(a,temp.c_str(),temp.size());
                fio->write(a,strlen(a));


                temp = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getDateToCompareS(z);
                a=new char[temp.size()+1];
                a[temp.size()]=0;
                tempNum = temp.size();
                fio->write((const char *) &(tempNum), sizeof(int));
                memcpy(a,temp.c_str(),temp.size());
                fio->write(a,strlen(a));

                temp2 = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getReturnedS(z);
                fio->write((const char *) &(temp2 ), sizeof(int));
                temp2 = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getDumpedS(z) ;
                fio->write((const char *) &(temp2), sizeof(int));
                temp2 = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getSplitNumS(z);
                fio->write((const char *) &(temp2 ), sizeof(int));
                temp2 = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getChangeNumS(z);
                fio->write((const char *) &(temp2 ), sizeof(int));
                temp2 = inventory->getVegetableByIndex(i)->getHistoryObject(j)->getTuiS(z);
                fio->write((const char *) &(temp2 ), sizeof(int));
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
        dialog.setWindowTitle("退還給公司的菜");
        // Use a layout allowing to have a label next to each field
        QFormLayout form(&dialog);

        // Add some text above the fields
        form.addRow(new QLabel("輸入退菜資料"));



        QLineEdit *lineEdit = new QLineEdit(&dialog);
        QString label = QString("退幾箱？");
        form.addRow(label, lineEdit);


        QComboBox* customerDrop = new QComboBox(&dialog);
        for(int i = 0; i< inventory->getPersonNum(); i++){
            customerDrop->addItem(inventory->getPerson(i).c_str());
        }
        customerDrop->setFont(font);
        QString label3 = QString("誰退回來的？");
        form.addRow(label3, customerDrop);

        QLineEdit * date = new QLineEdit(&dialog);
        time_t t = time(0);
        struct tm * now = localtime(&t);
        char buffer[128];
        sprintf(buffer, "%d/%d", now->tm_mon+1, now->tm_mday);
        date -> setText(QString::fromUtf8(buffer));
        QString label4 = QString("这是哪天退回來的");
        form.addRow(label4, date);

        QComboBox* companyDrop = new QComboBox(&dialog);
            for(int i=0; i< inventory->getCompanyNum(); i++){
                companyDrop->addItem(inventory->getCompany(i).c_str());
        }
        companyDrop->setFont(font);

        form.addRow("这是哪家公司的菜？", companyDrop);


        QLineEdit *lineEdit2 = new QLineEdit(&dialog);
        QString label2 = QString("这是哪天買的菜？");
        form.addRow(label2, lineEdit2);

        QCheckBox checkBox("退還給農場的菜",&dialog);
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
            if( !amount ){
                QMessageBox messageBox;
                messageBox.critical(0,"錯誤","Not Valid!");
                messageBox.setFixedSize(500,200);
             //   additionalReturn(amount, customerDrop->currentIndex(), date->text(), companyDrop->currentIndex(),
               //                  dateBought);
            }else{
                currentVege->returnThis(dateReturned, amount, returner, company, dateBought);
            }

        }else if (result == QDialog::Rejected){
            if(checkBox.isChecked()){

                QDialog dialog(this);
                dialog.setWindowTitle("退還給農場的菜");
                // Use a layout allowing to have a label next to each field
                QFormLayout form(&dialog);

                // Add some text above the fields
                form.addRow(new QLabel("輸入退菜資料"));


                QComboBox* remainingDrop = new QComboBox(&dialog);
                for(int i = 0; i< currentVege -> getRemainingNum(); i++){
                    remainingDrop->addItem(currentVege->formatRemaining(i).c_str());
                }
                remainingDrop->setFont(font);

                form.addRow("你要退那天的菜？", remainingDrop);

                QLineEdit *lineEdit = new QLineEdit(&dialog);
                QString label = QString("退幾箱？");
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
                    if( !amount ){
                        QMessageBox messageBox;
                        messageBox.critical(0,"錯誤","Not Valid!");
                        messageBox.setFixedSize(500,200);
                    }else if(!currentVege->returnTo(amount,today,selection)){
                        QMessageBox error;
                        error.critical(0,"警告","不夠菜退!");
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
    QMessageBox::StandardButton reply = error.critical(0,"警告","你要儲存嗎?",
                            QMessageBox::No|QMessageBox::Yes);
    error.setFixedSize(500,200);
    if (reply == QMessageBox::Yes) {
        save();
    }
    needSave = 0;
}

void Dialog::on_pushButton_clicked()
{
    if(currentVege){

        QDialog dialog(this);
        // Use a layout allowing to have a label next to each field
        QFormLayout form(&dialog);

        // Add some text above the fields
        dialog.setWindowTitle("倒");

        QComboBox* remainingDrop = new QComboBox(&dialog);
        for(int i = 0; i< currentVege -> getRemainingNum(); i++){
            remainingDrop->addItem(currentVege->formatRemaining(i).c_str());
        }
        remainingDrop->setFont(font);

        form.addRow("你要倒那天的菜？", remainingDrop);

        QLineEdit *lineEdit = new QLineEdit(&dialog);
        form.addRow(new QLabel("倒幾箱？"));
        form.addRow(lineEdit);


        QCheckBox checkBox("全部",&dialog);
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
            if( !amount ){
                QMessageBox messageBox;
                messageBox.critical(0,"錯誤","Not Valid!");
                messageBox.setFixedSize(500,200);
            }

            else if(!currentVege->dumpVege(amount, today, selection)){
                QMessageBox error;
                error.critical(0,"警告","不夠數量倒!");
                error.setFixedSize(500,200);

            }
        }else if (result == QDialog::Rejected){
            if(checkBox.isChecked()){
                QDialog dialog(this);
                // Use a layout allowing to have a label next to each field
                QFormLayout form(&dialog);

                // Add some text above the fields
                form.addRow(new QLabel("你確定你要倒掉全部嗎?"));

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

void Dialog::on_pushButton_2_clicked()
{

    QDialog dialog(this);

    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add some text above the fields
    dialog.setWindowTitle("刪掉歷史");


    QLineEdit *lineEdit = new QLineEdit(&dialog);
    form.addRow(new QLabel("刪掉多少？"));
    form.addRow(lineEdit);


    QCheckBox checkBox("全部",&dialog);
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
        if( !amount ){
            QMessageBox messageBox;
            messageBox.critical(0,"錯誤","Not Valid!");
            messageBox.setFixedSize(500,200);
        }

        else if(!currentVege->clearHist(amount)){
            QMessageBox error;
            error.critical(0,"警告","不夠數量刪除!");
            error.setFixedSize(500,200);

        }
    }else if (result == QDialog::Rejected){
        if(checkBox.isChecked()){
            QDialog dialog(this);
            // Use a layout allowing to have a label next to each field
            QFormLayout form(&dialog);

            // Add some text above the fields
            form.addRow(new QLabel("你確定你要刪掉全部嗎?"));

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

    QList<QListWidgetItem *> removeThis = ui->vegeList->
            findItems (ui->vegeList->currentItem()->text().toUtf8().constData(), Qt::MatchExactly);

    delete removeThis[0];
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
        if(!(ui->sellCheck->isChecked() && ui->buyCheck->isChecked() && ui->dumpCheck->isChecked() && ui->returnCheck->isChecked() && ui->tuiCheck->isChecked())){
                QMessageBox messageBox;
                messageBox.critical(0,"錯誤","你要打勾所有的選項才能undo!");
                messageBox.setFixedSize(500,200);
        }else{
            currentVege->deleteHistory( ui->historyList->currentRow());
                needSave = 1;
            /*QList<QListWidgetItem *> removeThis = ui->historyList->
                    findItems (ui->historyList->currentItem()->text().toUtf8().constData(), Qt::MatchExactly);

            delete removeThis[0];*/
            on_vegeList_itemPressed(ui->vegeList->currentItem());
        }
    }
}

void Dialog:: undoHistory(){
    if(currentVege&&currentVege->getHistoryNum()){
        if(!(ui->sellCheck->isChecked() && ui->buyCheck->isChecked() && ui->dumpCheck->isChecked() && ui->returnCheck->isChecked() && ui->tuiCheck->isChecked())){
            QMessageBox messageBox;
            messageBox.critical(0,"錯誤","你要打勾所有的選項才能undo!");
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

void Dialog::on_pushButton_3_clicked()
{
    //clear return
    ui->returnToFarm->clear();
    currentVege->clearTui();
}

void Dialog::changeToEnglish(){
    mTranslator.changeLanguage(ENGLISH);
    changeMenuLanguage();
}

void Dialog::changeToChinese(){
    mTranslator.changeLanguage(CHINESE);
    changeMenuLanguage();
}

void Dialog::changeMenuLanguage(){
    newAction->setText(mTranslator.translate("New File").c_str());
    loadAction ->setText(mTranslator.translate("Load File").c_str());
    saveAction ->setText(mTranslator.translate("Save").c_str());;
    saveAsAction ->setText(mTranslator.translate("Save As").c_str());

    addPersonAction->setText(mTranslator.translate("Add Customer").c_str());
    addCompanyAction ->setText(mTranslator.translate("Add Company").c_str());
    addVegetableAction ->setText(mTranslator.translate("Add Vegetable").c_str());
    addUnitAction ->setText(mTranslator.translate("Add Unit").c_str());
    removePersonAction ->setText(mTranslator.translate("Remove Customer").c_str());
    removeCompanyAction ->setText(mTranslator.translate("Remove Company").c_str());
    removeVegetableAction->setText(mTranslator.translate("Remove Vegetable").c_str());
    removeUnitAction->setText(mTranslator.translate("Remove Unit").c_str());

    ui->Buy->setText(mTranslator.translate("Buy").c_str());
    ui->Sell->setText(mTranslator.translate("Sell").c_str());
    ui->VegieList->setText(mTranslator.translate("Vegetables").c_str());
    ui->Total->setText(mTranslator.translate("History").c_str());
    ui->Inventory->setText(mTranslator.translate("Breakdown").c_str());
    ui->Return->setText(mTranslator.translate("Return").c_str());
    ui->dumpButton->setText(mTranslator.translate("Dump").c_str());
    ui->pushButton_3->setText(mTranslator.translate("Clear Memo").c_str());
    ui->pushButton_2->setText(mTranslator.translate("Clear History").c_str());
    ui->tuiCheck->setText(mTranslator.translate("Return to Company").c_str());


    ui->buyCheck->setText(mTranslator.translate("Buy").c_str());
    ui->sellCheck->setText(mTranslator.translate("Sell").c_str());
    ui->dumpCheck->setText(mTranslator.translate("Dump").c_str());
    ui->returnCheck->setText(mTranslator.translate("Return").c_str());
    ui->Returns->setText(mTranslator.translate("Returns").c_str());
}





