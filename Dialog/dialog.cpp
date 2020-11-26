#include "dialog.h"
#include "utils.h"
#include <iostream>
using namespace std;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    needSave(0),
    inventory(new Inventory()),
    currentVege(nullptr),
    font("Courier",-1,QFont::Bold,false),
    mTranslator(new Translator()),
    mAbbreviator(new Abbreviation()),
    menuBar(new IMenuBar(this,mTranslator)),
    utils(new Utils())
{
    ui->setupUi(this);

#ifndef Q_OS_WIN32
    ui->label_4->hide();
#endif

    ui->multiSellButton->hide();
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

    dumpVegeAction = new QAction(mTranslator->translate("倒").c_str(), this);
    connect(dumpVegeAction, SIGNAL(triggered()), this, SLOT(dumpVege()));
    ui->breakDown->addAction(dumpVegeAction);

    undoHistoryAction = new QAction(tr("&Undo"),this);
    connect(undoHistoryAction, SIGNAL(triggered()), this, SLOT(undoHistory()));
    ui->historyList->addAction(undoHistoryAction);

    deleteHistoryAction = new QAction(tr("&Delete"),this);
    connect(deleteHistoryAction, SIGNAL(triggered()), this, SLOT(deleteHistory()));
    ui->historyList->addAction(deleteHistoryAction);

    editHistoryAction = new QAction(tr("&Edit"), this);
    connect(editHistoryAction, SIGNAL(triggered()), this, SLOT(editHistory()));
    ui->historyList->addAction(editHistoryAction);


    /*Edit Vegetable Name */
    connect(ui->vegeList->itemDelegate(), SIGNAL(closeEditor(QWidget*, QAbstractItemDelegate::EndEditHint)),
            this, SLOT(ListWidgetEditEnd(QWidget*, QAbstractItemDelegate::EndEditHint)));


    QFont vFont = font;
    vFont.setPixelSize(18);
    ui->vegeList->setFont(vFont);

    if(mTranslator->currentLanguage == CHINESE) {
        changeToChinese();
    } else {
        changeToEnglish();
    }
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_vegeList_itemClicked(QListWidgetItem *item)
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

    // Add items to history. All these checks are for filtering
    for(int i = 0; i < historyNum; i++) {
        bool hasTui = (currentVege->getHistoryObject(i)->getTui() && ui->tuiCheck->isChecked());
        bool hasDump = (currentVege->getHistoryObject(i)->getDumped() && ui->dumpCheck->isChecked());
        bool hasHistory = (currentVege->getHistoryObject(i)->getReturned() && ui->returnCheck->isChecked());
        bool hasSell = ((currentVege->getHistoryObject(i)->getCustomer().compare("\t") && ui->sellCheck->isChecked())
                        &&
                        !(currentVege->getHistoryObject(i)->getTui() || currentVege->getHistoryObject(i)->getDumped()||
                          currentVege->getHistoryObject(i)->getReturned()));
        bool hasBuy = (ui->buyCheck->isChecked() && !(currentVege->getHistoryObject(i)->getTui() ||
                                                      currentVege->getHistoryObject(i)->getReturned() || currentVege->getHistoryObject(i)->getCustomer().compare("\t")
                                                      || currentVege->getHistoryObject(i)->getDumped()));
        if (hasTui || hasDump || hasHistory || hasSell || hasBuy) {
            ui->historyList->addItem(QString::fromStdString(currentVege->viewHistory(i)));
            ui->historyList->item(index)->setFont(font);
            if (currentVege->getHistoryObject(i)->getReturned() ||
                    currentVege->getHistoryObject(i)->getDumped()||currentVege->getHistoryObject(i)->getTui()){
                font.setItalic(true);
                ui->historyList->item(index)->setFont(font);
                font.setItalic(false);
            }

            index++;
        }
    }

    // Scroll history list down
    ui->historyList->verticalScrollBar() ->setValue(ui -> historyList -> verticalScrollBar() -> maximum());

    // Change text of total units. i.e. 32 Boxes
    ui ->changeTotal->setText(QString::number(currentVege->getTotalVeges()) +
                              " " + QString::fromStdString(currentVege->getUnit()));

    // The remaining count of this vegetable, broken down by seller
    for(int i = 0; i< currentVege -> getRemainingNum() ; i++){
        ui->breakDown->addItem(currentVege->formatRemaining2(i).c_str());
        ui->breakDown->item(i)->setFont(font);
    }

    // The memo for this vegetable
    ui ->Memo_2->setText(currentVege->getMemo().c_str());

    // Lays out the vegetables that have been returned by customers
    for(int i = 0; i< currentVege->getReturnNum(); i++) {
        ui->returnList->addItem(currentVege->formatReturn(i).c_str());
        ui->returnList->item(i)->setFont(font);
    }

    // Lays out the vegetables that will be returned to the farm
    for(int i = 0; i< currentVege->getTuiNum(); i++) {
        ui->returnToFarm->addItem(currentVege->formatTui(i).c_str());
        ui->returnToFarm->item(i)->setFont(font);
    }

    needSave = 1;
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
            messageBox.critical(nullptr,mTranslator ->translate("警告").c_str(),
                                mTranslator ->translate("沒單位!").c_str());
            messageBox.setFixedSize(500,200);
        }else if(inventory->addVegetable(lineEdit->text().toUtf8().constData(),
                                         unitDrop->currentText().toUtf8().constData())){
            ui->vegeList->blockSignals(true);
            ui->vegeList->clear();
            ui->vegeList->blockSignals(false);

            for(int i = 0; i < inventory->getVegNum(); i++){
                ui->vegeList->addItem(inventory->getVegetableByIndex(i)->getVegetablename().c_str());
                ui->vegeList->item(i)->setFlags (ui->vegeList->item(i)->flags()| Qt::ItemIsEditable);
            }
            if(inventory->getVegNum() == 1){
                ui->vegeList->item(0)->setSelected(true);
                ui->vegeList->setCurrentRow(0);
                on_vegeList_itemClicked(ui->vegeList->item(0));
            }else{
                int temp = inventory->getVegetableIndex(currentVege->getVegetablename());
                ui->vegeList->item(temp)->setSelected(true);
                ui->vegeList->setCurrentRow(temp);
                on_vegeList_itemClicked(ui->vegeList->item(temp));
            }
        }
    }
}

void Dialog::slot1(){
    if(currentVege)
        on_vegeList_itemClicked(ui->vegeList->currentItem());
}

void Dialog::addCompany(){
    QDialog dialog(this);
    dialog.setWindowTitle(mTranslator->translate("加入新公司").c_str());

    QFormLayout* form = new QFormLayout(&dialog);

    QLineEdit* companyEdit = new QLineEdit(&dialog);
    QLineEdit* abbreviationEdit = new QLineEdit(&dialog);

    form->addRow(QString(mTranslator->translate("請輸入你要加的新公司名字").c_str()), companyEdit);
    form->addRow(QString(mTranslator ->translate("簡寫").c_str()), abbreviationEdit);

    /* Button Box */
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);

    form->addRow(&buttonBox);

    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));

    if (dialog.exec() == QDialog::Accepted ) {
        string temp = (abbreviationEdit->text().toUtf8().constData() == "")?
                    companyEdit->text().toUtf8().constData():
                    abbreviationEdit->text().toUtf8().constData();
        mAbbreviator->add(companyEdit->text().toUtf8().constData(),
                          temp);
        inventory->addCompany(companyEdit->text().toUtf8().constData());
    }

    needSave = 1;
}

void Dialog::addPerson(){

    QDialog dialog(this);
    dialog.setWindowTitle( mTranslator
                           ->translate("加入新客戶").c_str());

    QFormLayout* form = new QFormLayout(&dialog);

    QLineEdit* customerEdit = new QLineEdit(&dialog);
    QLineEdit* abbreviationEdit = new QLineEdit(&dialog);

    form->addRow(QString( mTranslator ->translate("請輸入你要加的新客戶名字").c_str()), customerEdit);
    form->addRow(QString(mTranslator ->translate("簡寫").c_str()), abbreviationEdit);

    /* Button Box */
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);

    form->addRow(&buttonBox);

    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));

    if (dialog.exec() == QDialog::Accepted ) {
        string temp = (abbreviationEdit->text().toUtf8().constData() == "")?
                    customerEdit->text().toUtf8().constData():
                    abbreviationEdit->text().toUtf8().constData();
        mAbbreviator->add(customerEdit->text().toUtf8().constData(),
                          temp);
        inventory->addPerson(customerEdit->text().toUtf8().constData());
    }

    needSave = 1;
}

void Dialog::addUnit(){
    bool ok;
    QString text = QInputDialog::getText(this, mTranslator ->translate("加入單位").c_str(),
                                         mTranslator ->translate("請輸入你要加的單位").c_str(), QLineEdit::Normal,"", &ok);
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
            currentVege = nullptr;
            ui->historyList->clear();
            ui->breakDown->clear();
            ui->returnList->clear();
            ui->Memo_2->clear();
            ui->changeTotal->clear();
        }
    }else{
        on_vegeList_itemClicked(ui->vegeList->currentItem());
    }

}


void Dialog::dumpVege(){
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
        remainingDrop->setCurrentIndex(ui->breakDown->currentRow());
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

        time_t t = time(nullptr);
        struct tm * now = localtime(&t);
        char today[128];
        sprintf(today, "%d/%d/%d", now->tm_mon+1, now->tm_mday, now->tm_year - 100);

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
        on_vegeList_itemClicked(ui->vegeList->currentItem());
    }
}

int Dialog::compareCompany(History a, History b) {
    return strcmp(a.getCompany().c_str(), b.getCompany().c_str()) < 0;
}

int Dialog::compareCustomer(History a, History b) {
    return strcmp(a.getCustomer().c_str(), b.getCustomer().c_str()) < 0;
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

    ui->vegeList->blockSignals(true);
    ui->vegeList->clear();
    ui->vegeList->blockSignals(false);

    ui->Memo_2->clear();
    ui->changeTotal->clear();\

    currentVege = NULL;
}

void Dialog::loadFile(){

    if(needSave)
        askSave();
    QWidget *activeWindow = QApplication::activeWindow();
    QString filename = QFileDialog::getOpenFileName(activeWindow,
                                                    "Load File", "", "Datafile files (*.datafile);;All files (*)");
    if (filename != NULL) {
        needSave = 0;
        newFile();
        setWindowTitle(filename.section("/",-1,-1));
        string datafile1 = filename.toUtf8().constData();
        const char* datafile = datafile1.c_str();
        fstream* fio = new fstream (datafile, ios :: in| ios::binary);

        fio->seekg(0, ios::beg);

        int abbs;
        fio->read(( char *) &(abbs), sizeof(int));
        for(int i = 0;i < abbs; i++){
            mAbbreviator->add(readString(fio), readString(fio));
        }

        inventory->load(fio);
        fio->close();

        inventory->setFileName(filename.toUtf8().constData() );
        if (inventory->getVegNum()) {
            for(int i = 0; i<inventory->getVegNum(); i++) {
                ui->vegeList->addItem(QString::fromStdString(inventory->getVegetableByIndex(i)->getVegetablename()));
                ui->vegeList->item(i)->setFlags (ui->vegeList->item(i)->flags()| Qt::ItemIsEditable);
            }

            QFont vFont = font;
            vFont.setPixelSize(18);
            ui->vegeList->setFont(vFont);
            ui->vegeList->item(0)->setSelected(true);
            ui->vegeList->setCurrentRow(0);
            on_vegeList_itemClicked(ui->vegeList->item(0));
        }

        needSave = 0;
    }
}

void Dialog::save(){

    if (!inventory->getFileName().compare("")) {
        saveAs();
    } else {
        fstream* fio = new fstream (inventory->getFileName().c_str(),
                                    ios :: out |ios::binary);
        fio->seekp(0, ios::beg);
        writeInt(mAbbreviator->keyCount(),fio);
        map<string, string> abbs = mAbbreviator->getAbbs();
        for(map<string,string>::iterator it = abbs.begin(); it !=abbs.end() ; it++){
            writeString( it->first, fio);
            writeString( it->second, fio);
        }
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
                         ->getHistoryObject(j)->getChangeNum(), fio);
                writeInt(inventory->getVegetableByIndex(i)
                         ->getHistoryObject(j)->getTui(), fio);
                writeString(inventory->getVegetableByIndex(i)
                            ->getHistoryObject(j)->getType(), fio);
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
        time_t t = time(nullptr);
        struct tm * now = localtime(&t);
        char buffer[128];
        sprintf(buffer, "%d/%d/%d", now->tm_mon+1, now->tm_mday, now->tm_year - 100);
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
                messageBox.critical(nullptr,"錯誤","Not Valid!");
                messageBox.setFixedSize(500,200);
            }else{
                currentVege->returnThis(dateReturned, amount, returner, company, dateBought);
            }

        } else if (result == QDialog::Rejected){
            // If checked, this means we are returning to farm
            if(checkBox.isChecked()){

                QDialog dialog(this);
                dialog.setWindowTitle(mTranslator->translate("退還給農場的菜").c_str());
                // Use a layout allowing to have a label next to each field
                QFormLayout form(&dialog);

                // Add some text above the fields
                form.addRow(new QLabel(mTranslator->translate("輸入退菜資料").c_str()));


                QComboBox* remainingDrop = new QComboBox(&dialog);
                for (int i = 0; i< currentVege -> getRemainingNum(); i++){
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

                time_t t = time(nullptr);
                struct tm * now = localtime(&t);
                char today[128];
                sprintf(today, "%d/%d/%d", now->tm_mon+1, now->tm_mday, now->tm_year - 100);

                // Show the dialog as modal
                if ( result == QDialog::Accepted) {
                    if ( amount <= 0 ) {
                        QMessageBox messageBox;
                        messageBox.critical(nullptr,"錯誤","Not Valid!");
                        messageBox.setFixedSize(500,200);
                    } else if(!currentVege->returnTo(amount,today,selection)) {
                        QMessageBox error;
                        error.critical(nullptr,"警告",mTranslator
                                       ->translate("不夠菜退!").c_str());
                        error.setFixedSize(500,200);
                    }

                }
            }
        }

        on_vegeList_itemClicked(ui->vegeList->currentItem());
    }
}

void Dialog::closeEvent(QCloseEvent *event) {
    event = nullptr;
    if (needSave)
        askSave();
}

void Dialog:: askSave(){
    QMessageBox error;
    QMessageBox::StandardButton reply = error.critical(nullptr,"警告",
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

        time_t t = time(nullptr);
        struct tm * now = localtime(&t);
        char today[128];
        sprintf(today, "%d/%d/%d", now->tm_mon+1, now->tm_mday, now->tm_year - 100);

        // Show the dialog as modal
        if ( result == QDialog::Accepted) {
            if( amount <= 0) {
                QMessageBox messageBox;
                messageBox.critical(nullptr,"錯誤","Not Valid!");
                messageBox.setFixedSize(500,200);
            }

            else if(!currentVege->dumpVege(amount, today, selection)){
                QMessageBox error;
                error.critical(nullptr,"警告",mTranslator->translate("不夠數量倒!").c_str());
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
        on_vegeList_itemClicked(ui->vegeList->currentItem());
    }
}

void Dialog:: ListWidgetEditEnd(QWidget *editor, QAbstractItemDelegate::EndEditHint /* unused */){
    QString NewValue = reinterpret_cast<QLineEdit*>(editor)->text();
    currentVege->setVegetableName(NewValue.toUtf8().constData());

    ui->vegeList->blockSignals(true);

    ui->vegeList->clear();
    inventory->sortVeges();

    for(int i = 0; i < inventory->getVegNum(); i++){
        ui->vegeList->addItem(inventory->getVegetableByIndex(i)->getVegetablename().c_str());
        ui->vegeList->item(i)->setFlags (ui->vegeList->item(i)->flags()| Qt::ItemIsEditable);

    }
    if(inventory->getVegNum() == 1){
        ui->vegeList->item(0)->setSelected(true);
        ui->vegeList->setCurrentRow(0);
        on_vegeList_itemClicked(ui->vegeList->item(0));
    }else{
        int temp = inventory->getVegetableIndex(NewValue.toUtf8().constData());
        ui->vegeList->item(temp)->setSelected(true);
        ui->vegeList->setCurrentRow(temp);
        on_vegeList_itemClicked(ui->vegeList->item(temp));
    }
    ui->vegeList->blockSignals(false);
}

void Dialog:: deleteVege(){
    if(!currentVege)
        return;
    inventory->removeVegetable( ui->vegeList->currentItem()->text().toUtf8().constData());
    needSave = 1;
    qDeleteAll(ui->vegeList->selectedItems());


    if(inventory->getVegNum() == 0){
        currentVege = nullptr;
        ui->historyList->clear();
        ui->breakDown->clear();
        ui->returnList->clear();
        ui->Memo_2->clear();
        ui->changeTotal->clear();
    }else{
        on_vegeList_itemClicked(ui->vegeList->currentItem());
    }
}

void Dialog:: editHistory(){
    if(currentVege && currentVege->getHistoryNum()){
        if(!(ui->sellCheck->isChecked() && ui->buyCheck->isChecked()
             && ui->dumpCheck->isChecked() && ui->returnCheck->isChecked()
             && ui->tuiCheck->isChecked())){
            QMessageBox messageBox;
            messageBox.critical(nullptr,"錯誤",
                                mTranslator->translate("你要打勾所有的選項才能edit!").c_str());
            messageBox.setFixedSize(500,200);
        } else {
            QDialog dialog(this);
            dialog.setWindowTitle("Edit");

            //Add the viewport to the scroll area
            QScrollArea *scrollArea = new QScrollArea;
            scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

            //Create a widget and set its layout as your new layout created above
            QWidget *viewport = new QWidget(&dialog);
            scrollArea->setWidget(viewport);
            scrollArea->setWidgetResizable(true);

            QFormLayout* form = new QFormLayout(viewport);
            viewport->setLayout(form);

            QFormLayout *dialog_layout = new QFormLayout(&dialog);
            dialog.setLayout(dialog_layout);
            dialog.layout()->addWidget(scrollArea);

            History* historyTemp = currentVege->getHistoryObject(ui->historyList->currentRow());
            string label =  "[Date Sold | " + historyTemp->getDateSold() + "] [company | " + historyTemp -> getCompany() + "]";
            form -> addRow(new QLabel(QString::fromStdString(label)));
            /* Price */
            QLineEdit *priceLineEdit = new QLineEdit(&dialog);
            string price = historyTemp -> getPrice();
            priceLineEdit -> setText(QString::fromStdString(price));
            QString label4 = QString("Price: ");
            form->addRow(label4, priceLineEdit);

            /* Button Box */
            QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                       Qt::Horizontal, &dialog);
            form->addRow(&buttonBox);

            dialog.window()->setFixedWidth(dialog.window()->sizeHint().width() + 100);

            QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
            QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

            if (dialog.exec() == QDialog::Accepted ) {
                currentVege->editHistoryPrice(ui->historyList->currentRow(), priceLineEdit->text().toUtf8().constData());
                needSave = 1;
                on_vegeList_itemClicked(ui->vegeList->currentItem());
            }
        }
    }
}

void Dialog:: deleteHistory(){
    if(currentVege && currentVege->getHistoryNum()){
        if(!(ui->sellCheck->isChecked() && ui->buyCheck->isChecked()
             && ui->dumpCheck->isChecked() && ui->returnCheck->isChecked()
             && ui->tuiCheck->isChecked())){
            QMessageBox messageBox;
            messageBox.critical(nullptr,"錯誤",
                                mTranslator->translate("你要打勾所有的選項才能undo!").c_str());
            messageBox.setFixedSize(500,200);
        }else{
            currentVege->deleteHistory( ui->historyList->currentRow());
            needSave = 1;
            on_vegeList_itemClicked(ui->vegeList->currentItem());
        }
    }
}

void Dialog:: undoHistory(){
    if(currentVege&&currentVege->getHistoryNum()){
        if(!(ui->sellCheck->isChecked() && ui->buyCheck->isChecked() && ui->dumpCheck->isChecked() && ui->returnCheck->isChecked() && ui->tuiCheck->isChecked())){
            QMessageBox messageBox;
            messageBox.critical(nullptr,"錯誤",
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

            string type = currentVege->getHistoryObject(ui->historyList->currentRow())->getType();
            if (type == "Dump"){
                currentVege->restock(amount, dP, company, returnNum);
            }else if (type == "Tui"){
                currentVege->restock(amount, dP, company, returnNum);
                currentVege->reTui(dS, amount, dP, company);
            }else if (type == "Return"){
                currentVege->undoRetOrBuy(amount, dP, company, dS, customer);
            }else if (type == "Sell"){
                currentVege->restock(amount, dP,company,returnNum);
            }else if (type == "Buy"){
                currentVege->undoRetOrBuy(amount, dP, company, dS, customer);
            }

            currentVege->deleteHistory( ui->historyList->currentRow());
            needSave = 1;

            QList<QListWidgetItem *> removeThis = ui->historyList->
                    findItems (ui->historyList->currentItem()->text().toUtf8().constData(), Qt::MatchExactly);

            delete removeThis[0];
            on_vegeList_itemClicked(ui->vegeList->currentItem());
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
    dumpVegeAction->setText(mTranslator ->translate("倒").c_str());
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

int Dialog::readInt(fstream* fio){
    int storeHere;
    fio->read(( char *) &(storeHere), sizeof(int));
    return storeHere;
}

string Dialog::readString(fstream* fio){
    char temp2[200];
    int tempNum2;
    fio->read((char *) &(tempNum2),sizeof(int));
    fio->read(temp2, tempNum2);
    temp2[tempNum2] = '\0';
    string str(temp2);
    return str;
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
                messageBox.critical(nullptr,"錯誤","Not Valid!");
                messageBox.setFixedSize(500,200);
            }

            else if(!currentVege->clearHist(amount)){
                QMessageBox error;
                error.critical(nullptr,"警告",mTranslator->translate("不夠數量刪除!").c_str());
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

        on_vegeList_itemClicked(ui->vegeList->currentItem());
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

void Dialog::on_vegeList_currentRowChanged(int /* unused */)
{
    on_vegeList_itemClicked(ui->vegeList->currentItem());
}

void Dialog::showPreferences() {
    //mPreferencesPanel->show();
    //mPreferencesPanel->activateWindow();
    //mPreferencesPanel->raise();
}

int Dialog::queryVeges() {
    QDialog dialog(this);
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add some text above the fields
    form.addRow(new QLabel("Sell How Many Kinds?"));

    QLineEdit *lineEdit = new QLineEdit(&dialog);
    form.addRow( lineEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
    if (dialog.exec() == QDialog::Accepted ) {
        return lineEdit->text().toInt();
    }else
        return -1;
}

void Dialog::on_CalculateSold_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Calculate");

    //Add the viewport to the scroll area
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    //Create a widget and set its layout as your new layout created above
    QWidget *viewport = new QWidget(&dialog);
    scrollArea->setWidget(viewport);
    scrollArea->setWidgetResizable(true);

    QFormLayout* form = new QFormLayout(viewport);
    viewport->setLayout(form);

    QFormLayout *dialog_layout = new QFormLayout(&dialog);
    dialog.setLayout(dialog_layout);
    dialog.layout()->addWidget(scrollArea);

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


    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form->addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    if (dialog.exec() == QDialog::Accepted && currentVege) {
        string datePurchased = date->text().toUtf8().constData();
        string firstLine = "Date | " + datePurchased + "\n";

        string company = companyDrop->currentText().toUtf8().constData();
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

        Utils* utils = new Utils();
        for (int i = 0; i < currentVege -> getHistoryNum(); i++) {
            History* temp = currentVege -> getHistoryObject(i);
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
                    tuis += "RT to farm: " + to_string(temp -> getDifference() * -1) + " " + currentVege->getUnit() + " " + temp -> getDateSold() + "\n";
                    totalSold += temp -> getDifference() * -1;
                } else if (temp -> getType() == "Dump") {
                    dumps += "Dumped: " + to_string(temp -> getDifference() * -1) + " " + currentVege->getUnit() + " " + temp -> getDateSold() + "\n";
                    totalSold += temp -> getDifference() * -1;
                } else if (temp -> getType() == "Buy") {
                    totalBoxes += temp -> getDifference();
                } else if (temp -> getType() == "Return") {
                    // No access to original price here.
                    //units += temp -> getDifference() * -1;
                    //revenue += stoi(temp -> getPrice()) * temp -> getDifference() * -1;
                    totalSold -= temp -> getDifference();
                    returnedLine += "    " + to_string(temp -> getDifference()) + " " + currentVege -> getUnit() + " " + temp -> getCustomer() + " " + temp -> getDateSold() + "\n";
                }
            }
        }

        breakdownLine += "\n";
        string boxesLine = "Total Boxes (Buy needs to be in history) | " + to_string(totalBoxes) + "\n";
        string thirdLine = "Revenue | $" + utils -> doubleToString(revenue) + "\n";
        string fourthLine = "Units sold | " + to_string(units) + " "+ currentVege->getUnit() +"\n";


        int remaining = 0;

        for (int i = 0; i < currentVege -> getRemainingNum(); i++) {
            Remaining* temp = currentVege -> getRemainingObject(i);
            if (temp -> getDate() == datePurchased && temp -> getCompany() == company) {
                remaining = temp -> getRemaining();
                break;
            }
        }

        string fifthLine = "Boxes remaining | " + to_string(remaining) + "\n";

        if (returnedLine != "") {
            returnedLine = "\nManual Deduction: \n\nCustomer RT| \n" + returnedLine;
        }

        string totalSoldLine = "Total sold | " + to_string(totalSold) + " "+ currentVege->getUnit() +"\n";

        ui ->Memo_2->setText(QString::fromStdString(firstLine + secondLine + boxesLine + thirdLine + fourthLine + totalSoldLine + fifthLine + breakdownLine + "\n" + noPrices + dumps + tuis + returnedLine));
    }
}
