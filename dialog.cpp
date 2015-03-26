#include "dialog.h"


Dialog::Dialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Dialog),
  inventory(new Inventory()),
  currentVege(NULL),
  needSave(0),
  font("Courier",-1,QFont::Bold,false),
  mTranslator(new Translator()),
  mAbbreviator(new Abbreviation()),
  menuBar(new IMenuBar(this,mTranslator))
{
  ui->setupUi(this);

  #ifndef Q_OS_WIN32
    ui->label_4->hide();
  #endif

  ui->multiSellButton->hide();
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

  dumpVegeAction = new QAction(mTranslator->translate("倒").c_str(), this);
  connect(dumpVegeAction, SIGNAL(triggered()), this, SLOT(dumpVege()));
  ui->breakDown->addAction(dumpVegeAction);

  undoHistoryAction = new QAction(tr("&Undo"),this);
  connect(undoHistoryAction, SIGNAL(triggered()), this, SLOT(undoHistory()));
  ui->historyList->addAction(undoHistoryAction);

  deleteHistoryAction = new QAction(tr("&Delete"),this);
  connect(deleteHistoryAction, SIGNAL(triggered()), this, SLOT(deleteHistory()));
  ui->historyList->addAction(deleteHistoryAction);

  /*Edit Vegetable Name */
  connect(ui->vegeList->itemDelegate(), SIGNAL(closeEditor(QWidget*, QAbstractItemDelegate::EndEditHint)),
          this, SLOT(ListWidgetEditEnd(QWidget*, QAbstractItemDelegate::EndEditHint)));


  QFont vFont = font;
  vFont.setPixelSize(18);
  ui->vegeList->setFont(vFont);
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
  if(!currentVege)
    return;

    QDialog dialog(this);
    dialog.setWindowTitle("multiple buy");

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

    /* Date */
    QLineEdit *date = new QLineEdit(&dialog);
    time_t t = time(0);
    struct tm * now = localtime(&t);
    char buffer[128];
    sprintf(buffer, "%d/%d", now->tm_mon+1, now->tm_mday);
    date -> setText(QString::fromUtf8(buffer));
    QString label4 = QString("Date");
    form->addRow(label4, date);

    MultiBuyController* multiBuyController = new MultiBuyController(
          inventory->getVegetableIndex(currentVege->getVegetablename()),
          inventory, form, font, scrollArea, &dialog);

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
              atoi(amount.c_str()), company, date->text().toStdString(),
              price);
      }
      on_vegeList_itemClicked(ui->vegeList->currentItem());
    }
}

void Dialog::on_Sell_clicked()
{
  if(!currentVege)
    return;

  if(inventory->numberOfNonEmptyVeges()){
    QDialog dialog(this);
    dialog.setWindowTitle("multiple sell");

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

    /* Generate form items that appear once*/

    /* Customer Drop */
    QComboBox* customerDrop = new QComboBox(&dialog);
    for(int i = 0; i< inventory->getPersonNum(); i++){
      customerDrop->addItem(inventory->getPerson(i).c_str());
    }

    customerDrop->setFont(font);

    QString label2 = QString("Customer");
    form->addRow(label2, customerDrop);

    /* Date */
    QLineEdit *date = new QLineEdit(&dialog);
    time_t t = time(0);
    struct tm * now = localtime(&t);
    char buffer[128];
    sprintf(buffer, "%d/%d", now->tm_mon+1, now->tm_mday);
    date -> setText(QString::fromUtf8(buffer));
    QString label4 = QString("Date");
    form->addRow(label4, date);

    MultiSellController* multiSellController = new MultiSellController(
          inventory, form, font, scrollArea, &dialog);

    /* Button Box */
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                           Qt::Horizontal, &dialog);

    dialog_layout->addRow(&buttonBox);
    QToolButton *tb = new QToolButton();
    tb->setText("+");


    QToolButton *tb1 = new QToolButton();
    tb1->setText("-");

    QHBoxLayout* hLay = new QHBoxLayout();
    hLay->addWidget(tb);
    hLay->addWidget(tb1);
    hLay->addWidget(&buttonBox);
    dialog_layout->addRow(hLay);

    dialog.window()->setFixedWidth(dialog.window()->sizeHint().width() + 100);

    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
    QObject::connect(tb, SIGNAL(clicked()),multiSellController, SLOT(addElement()));
    QObject::connect(tb1, SIGNAL(clicked()),multiSellController, SLOT(subtractElement()));
    if (dialog.exec() == QDialog::Accepted ) {
      for( int i = 0; i < multiSellController->getElementNum(); i++){
        int vegeIndex = multiSellController->
                            getActualVegeIndex(i);
        string customer = customerDrop->currentText().toUtf8().constData();
        string amountString = multiSellController->
                            getAmounts(i);
        string price = multiSellController->getPrice(i);
        vector<int> remainIndexes = multiSellController->
                            getActualRemainingIndexes(i);

        stringstream ss(amountString);
        vector<string> amounts;

        while (ss.good())
        {
            string substr;
            getline(ss, substr, ',');
            amounts.push_back(substr);
        }

        if(amounts.size() != remainIndexes.size())
          continue;

        vector<pair<int, int> > indexToPrice = vector<pair<int,int> >();

        /* Link up remainindexes to amounts */
        for(int j = 0; j < amounts.size(); j++){
          indexToPrice.push_back(
                make_pair(remainIndexes[amounts.size() - 1 - j],
                atoi(amounts[j].c_str())));
        }
        /* Sort the remainIndexes so that they are sold from greatest to least */
        sort(indexToPrice.begin(),indexToPrice.end(),myobject);

        /* Sell the remainIndexes, greatest to least */
        for(vector<pair<int,int> >::iterator it = indexToPrice.begin(); it != indexToPrice.end(); it++){

          inventory->getVegetableByIndex(vegeIndex)->sellVege(
                (*it).second, customer, date->text().toStdString(),
                price, (*it).first);
        }

        /*for(int j = 0; j < amounts.size(); j++){
          inventory->getVegetableByIndex(vegeIndex)->sellVege(
                atoi(amounts[j].c_str()), customer, date->text().toStdString(),
                price, remainIndexes[amounts.size() - 1 - j]);
        }*/
      }
      on_vegeList_itemClicked(ui->vegeList->currentItem());
    }
    /* Deallocate memory*/

  }else{
      QMessageBox messageBox;
      messageBox.critical(0,"警告","No Vegetables to Sell");
      messageBox.setFixedSize(500,200);
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
    dialog.setWindowTitle(mTranslator->translate("加入新客戶").c_str());

    QFormLayout* form = new QFormLayout(&dialog);

    QLineEdit* companyEdit = new QLineEdit(&dialog);
    QLineEdit* abbreviationEdit = new QLineEdit(&dialog);

    form->addRow(QString(mTranslator->translate("請輸入你要加的新公司名字?").c_str()), companyEdit);
    form->addRow(QString("Abbreviation"), abbreviationEdit);

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
                           ->translate("請輸入你要加的新客戶名字?").c_str());

    QFormLayout* form = new QFormLayout(&dialog);

    QLineEdit* customerEdit = new QLineEdit(&dialog);
    QLineEdit* abbreviationEdit = new QLineEdit(&dialog);

    form->addRow(QString( mTranslator ->translate("你要刪掉哪一個客戶？").c_str()), customerEdit);
    form->addRow(QString("Abbreviation"), abbreviationEdit);

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
            on_vegeList_itemClicked(ui->vegeList->currentItem());
        }

}

void Dialog::printInventory(){
  QPrinter printer;
  QPrintPreviewDialog preview (&printer, this);
  connect( &preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printI(QPrinter*)) );
  preview.exec();
}

void Dialog::printTransactions(){
  QPrinter printer;
  QPrintPreviewDialog preview (&printer, this);
  connect( &preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printT(QPrinter*)) );
  preview.exec();
}

void Dialog::printHistory(){
  QPrinter printer;
  QPrintPreviewDialog preview (&printer, this);
  connect( &preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printH(QPrinter*)) );
  preview.exec();

}

void Dialog::printT(QPrinter* printer){

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
      double lineHeight = painter.fontMetrics().height();
      int topMargin = 3 * lineHeight;
      QString leftText= "";
      QString rightText="";
      QString * currentText = &leftText;
      painter.drawLine(500,40,500,1250);
      painter.drawText(850,100,100,lineHeight,Qt::AlignRight|Qt::AlignTop, QString(today.c_str()) );
      int lineCount = 0;
      int column = 0;
      for(int i = 0; i < inventory->getVegNum(); i++){
          if(inventory->getVegetableByIndex(i)->getRemainingNum() || 
                  inventory->getVegetableByIndex(i)->hasInteraction()){
              if( lineHeight * (lineCount + (2 + inventory->getVegetableByIndex(i)->getRemainingNum())) > 1250  ){
                  if(currentText == &leftText){
                      currentText = &rightText;
                      lineCount = 0;
                      column = 1;
                      painter.drawLine(500, topMargin, 500, 1250);
                  }else{
                      painter.drawText(10,topMargin,390,1250,Qt::AlignLeft|Qt::AlignTop, leftText );
                      painter.drawText(460, topMargin,390,1250,Qt::AlignLeft|Qt::AlignTop, rightText );
                      leftText= "";
                      rightText="";
                      printer->newPage();

                      painter.drawLine(500,topMargin,500,1250);

                      painter.drawText(850,100,100,lineHeight,Qt::AlignRight|Qt::AlignTop, QString(today.c_str()) );
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

          *currentText = *currentText + QString(inventory->getVegetableByIndex(i)
                          ->formatTransaction(*mAbbreviator).c_str());

          lineCount += 2; // For Vegetable name and Total
          int additionalLines = inventory->
              getVegetableByIndex(i)->getMTransNum();
          lineCount += additionalLines; // Lines from transactions
          painter.drawLine(500 * column,lineHeight * (lineCount) + topMargin,
                          500 * (column + 1),lineHeight * (lineCount) + topMargin);

      }else{
              if( lineHeight * (lineCount + 1) > 1250  ){
                  if(currentText == &leftText){
                      currentText = &rightText;
                      lineCount = 0;
                      column = 1;
                      painter.drawLine(500,topMargin,500,1250);
                  }else{
                      painter.drawText(10,topMargin,390,1250,Qt::AlignLeft|Qt::AlignTop, leftText );
                      painter.drawText(460,topMargin,390,1250,Qt::AlignLeft|Qt::AlignTop, rightText );
                      leftText= "";
                      rightText="";
                      printer->newPage();

                      painter.drawLine(500,topMargin,500,1250);

                      painter.drawText(600,topMargin,100,lineHeight,Qt::AlignRight|Qt::AlignTop, QString(today.c_str()) );
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
              painter.drawLine(500 * column,lineHeight * (lineCount) + topMargin,
                              500 * (column + 1),lineHeight * (lineCount) + topMargin);

      }

    }
   painter.drawText(10,topMargin,390,1250,Qt::AlignLeft|Qt::AlignTop, leftText );
   painter.drawText(460,topMargin,390,1250,Qt::AlignLeft|Qt::AlignTop, rightText );
   painter.end();
  }
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
        double lineHeight = painter.fontMetrics().height();

        QString title = "test";

        int textWidth = painter.window().width();
        int maxHeight = painter.window().height();

        QRect titleRect = painter.boundingRect(0, 0, textWidth, maxHeight,
                                                Qt::TextWordWrap, title);


        QString leftText= "";
        QString rightText="";
        QString * currentText = &leftText;
        painter.drawLine(500,40,500,1250);
        painter.drawText(850,100,100,lineHeight,Qt::AlignRight|Qt::AlignTop, QString(today.c_str()) );
        int lineCount = 0;
        int column = 0;
        for(int i = 0; i<inventory->getVegNum(); i++){
            if(inventory->getVegetableByIndex(i)->getRemainingNum()){
                if( lineHeight * (lineCount + (2 + inventory->getVegetableByIndex(i)->getRemainingNum())) > 1250  ){
                    if(currentText == &leftText){
                        currentText = &rightText;
                        lineCount = 0;
                        column = 1;
                        painter.drawLine(500,40,500,1250);
                    }else{
                        painter.drawText(10,40+lineHeight,390,1250,Qt::AlignLeft|Qt::AlignTop, leftText );
                        painter.drawText(460,40+lineHeight,390,1250,Qt::AlignLeft|Qt::AlignTop, rightText );
                        leftText= "";
                        rightText="";
                        printer->newPage();

                        painter.drawLine(500,40,500,1250);

                        painter.drawText(550,40 + lineHeight,100,lineHeight,Qt::AlignRight|Qt::AlignTop, QString(today.c_str()) );
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
                painter.drawLine(500 * column,lineHeight * (lineCount +1) + 40,
                                500 *(column + 1),lineHeight * (1+lineCount)+40);

        }else{
                if( lineHeight * (lineCount + 1) > 1250  ){
                    if(currentText == &leftText){
                        currentText = &rightText;
                        lineCount = 0;
                        column = 1;
                        painter.drawLine(500,40,500,1250);
                    }else{
                        painter.drawText(10,40+lineHeight,390,1250,Qt::AlignLeft|Qt::AlignTop, leftText );
                        painter.drawText(460,40+lineHeight,390,1250,Qt::AlignLeft|Qt::AlignTop, rightText );
                        leftText= "";
                        rightText="";
                        printer->newPage();

                        painter.drawLine(500,40,500,1250);

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
                painter.drawLine(500 * column,lineHeight * (lineCount + 1) + 40,
                                500 * (column + 1),lineHeight * (lineCount + 1) + 40);

        }

    }
   painter.drawText(10,40+lineHeight,390,1250,Qt::AlignLeft|Qt::AlignTop, leftText );
   painter.drawText(460,40+lineHeight,390,1250,Qt::AlignLeft|Qt::AlignTop, rightText );
   painter.end();
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
        on_vegeList_itemClicked(ui->vegeList->currentItem());
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
        painter.drawLine(900,40,450,1250);
        painter.drawLine(1000,40,450,1250);
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

               for(int j = 0; j<index; j++){
                   if(temp[j].getCustomer().compare("\t") && !temp[j].getReturned()
                           && !temp[j].getDumped()){
                       sell[sellNum] = temp[j];
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
    if(filename != NULL){
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
            on_vegeList_itemClicked(ui->vegeList->item(0));
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
        writeInt(mAbbreviator->keyCount(),fio);
        qDebug()<<mAbbreviator->keyCount();
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
        on_vegeList_itemClicked(ui->vegeList->currentItem());
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
        on_vegeList_itemClicked(ui->vegeList->currentItem());
    }
}

void Dialog:: ListWidgetEditEnd(QWidget *editor, QAbstractItemDelegate::EndEditHint hint){


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
        currentVege = NULL;
        ui->historyList->clear();
        ui->breakDown->clear();
        ui->returnList->clear();
        ui->Memo_2->clear();
        ui->changeTotal->clear();
    }else{
       on_vegeList_itemClicked(ui->vegeList->currentItem());
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
            on_vegeList_itemClicked(ui->vegeList->currentItem());
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

void Dialog::on_vegeList_currentRowChanged(int currentRow)
{
    on_vegeList_itemClicked(ui->vegeList->currentItem());
}

void Dialog::showPreferences() {
  //mPreferencesPanel->show();
  //mPreferencesPanel->activateWindow();
  //mPreferencesPanel->raise();
}

int Dialog::queryVeges(){
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



