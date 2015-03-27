#include "imenubar.h"
#include <QApplication>
#include <QDebug>
#include "dialog.h"


IMenuBar::IMenuBar(QWidget* p, Translator* trans):
        QMenuBar(p),
        parent(p),
        mTranslator(trans){

    setFixedHeight(25);
    setFixedWidth(2000);
    QMenu* fileMenu = addMenu("File");

    QMenu* editMenu = addMenu("Edit");

    newAction = fileMenu->addAction(mTranslator ->translate("開新檔案").c_str());
    newAction->setShortcut(QKeySequence::New);
    loadAction = fileMenu->addAction(mTranslator ->translate("開啟舊檔").c_str());
    loadAction->setShortcut(QKeySequence::Open);
    saveAction =fileMenu->addAction(mTranslator ->translate("儲存檔案").c_str());;
    saveAction->setShortcut(QKeySequence::Save);
    saveAsAction = fileMenu->addAction(mTranslator ->translate("另存新檔").c_str());
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    printAction = fileMenu->addAction(mTranslator ->translate("印 Inventory").c_str());
    printAction3 = fileMenu->addAction(mTranslator ->translate("印記錄表").c_str());
    printAction2 = fileMenu->addAction(mTranslator ->translate("印 History").c_str());


    fileMenu->addSeparator();
    QMenu* translateMenu = fileMenu->addMenu(tr("Translate"));

    addPersonAction = editMenu->addAction(mTranslator ->translate("加入新客戶").c_str());
    addCompanyAction = editMenu->addAction(mTranslator ->translate("加入新公司").c_str());
    addVegetableAction = editMenu->addAction(mTranslator ->translate("加入新的菜名").c_str());
    addUnitAction = editMenu->addAction(mTranslator ->translate("加入單位").c_str());
    removePersonAction = editMenu->addAction(mTranslator ->translate("刪除客戶").c_str());
    removeCompanyAction = editMenu->addAction(mTranslator ->translate("刪除公司").c_str());
    removeVegetableAction = editMenu->addAction(mTranslator ->translate("刪掉菜名").c_str());
    removeUnitAction = editMenu->addAction(mTranslator ->translate("刪掉單位").c_str());
    changeToEnglishAction = translateMenu->addAction("English");
    changeToChineseAction = translateMenu->addAction("中文");

    mShowPreferencesAction = new QAction(tr("&Preferences"), this);
    mShowPreferencesAction->setShortcuts(QKeySequence::Preferences);
    QObject::connect(mShowPreferencesAction, SIGNAL(triggered()),
                     this, SLOT(showPreferences()));


    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(loadAction, SIGNAL(triggered()), this, SLOT(loadFile()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(printAction, SIGNAL(triggered()), this, SLOT(printInventory()));
    connect(printAction2, SIGNAL(triggered()), this, SLOT(printHistory()));
    connect(printAction3, SIGNAL(triggered()), this, SLOT(printTransactions()));
    connect(changeToEnglishAction, SIGNAL(triggered()), this, SLOT(changeToEnglish()));
    connect(changeToChineseAction, SIGNAL(triggered()), this, SLOT(changeToChinese()));

    connect(addVegetableAction, SIGNAL(triggered()), this, SLOT(addVegetable()));
    connect(addPersonAction, SIGNAL(triggered()), this, SLOT(addPerson()));
    connect(addCompanyAction, SIGNAL(triggered()), this, SLOT(addCompany()));
    connect(addUnitAction, SIGNAL(triggered()), this, SLOT(addUnit()));
    connect(removeVegetableAction, SIGNAL(triggered()), this, SLOT(removeVegetable()));
    connect(removePersonAction, SIGNAL(triggered()), this, SLOT(removePerson()));
    connect(removeCompanyAction, SIGNAL(triggered()), this, SLOT(removeCompany()));
    connect(removeUnitAction, SIGNAL(triggered()), this, SLOT(removeUnit()));
}

void IMenuBar:: changeLanguage(){
    newAction->setText(mTranslator ->translate("開新檔案").c_str());
    loadAction ->setText(mTranslator ->translate("開啟舊檔").c_str());
    saveAction ->setText(mTranslator ->translate("儲存檔案").c_str());;
    saveAsAction ->setText(mTranslator ->translate("另存新檔").c_str());
    addPersonAction->setText(mTranslator ->translate("加入新客戶").c_str());
    addCompanyAction ->setText(mTranslator ->translate("加入新公司").c_str());
    addVegetableAction ->setText(mTranslator ->translate("加入新的菜名").c_str());
    addUnitAction ->setText(mTranslator ->translate("加入單位").c_str());
    removePersonAction ->setText(mTranslator ->translate("刪除客戶").c_str());
    removeCompanyAction ->setText(mTranslator ->translate("刪除公司").c_str());
    removeVegetableAction->setText(mTranslator ->translate("刪掉菜名").c_str());
    removeUnitAction->setText(mTranslator ->translate("刪掉單位").c_str());
    printAction->setText(mTranslator ->translate("印 Inventory").c_str());
    printAction2->setText(mTranslator ->translate("印 History").c_str());
}

/* File */

void IMenuBar:: newFile(){
    ((Dialog* ) parent) -> newFile();
}

void IMenuBar:: loadFile(){
    ((Dialog* ) parent) -> loadFile();
}

void IMenuBar:: save(){
    ((Dialog* ) parent) -> save();
}

void IMenuBar:: saveAs(){
    ((Dialog* ) parent) -> saveAs();
}

void IMenuBar:: printInventory(){
    ((Dialog* ) parent) -> printInventory();
}

void IMenuBar:: printHistory(){
    ((Dialog* ) parent) -> printHistory();
}

void IMenuBar:: printTransactions(){
    ((Dialog* ) parent) -> printTransactions();
}

void IMenuBar:: changeToEnglish(){
    ((Dialog* ) parent) -> changeToEnglish();
}

void IMenuBar:: changeToChinese(){
    ((Dialog* ) parent) -> changeToChinese();
}

void IMenuBar::showPreferences(){
    ((Dialog* ) parent) -> showPreferences();
}

/* Edit */

void IMenuBar:: addVegetable(){
    ((Dialog* ) parent) -> addVegetable();
}


void IMenuBar:: addCompany(){
    ((Dialog* ) parent) -> addCompany();
}

void IMenuBar:: addPerson(){
    ((Dialog* ) parent) -> addPerson();
}

void IMenuBar:: addUnit(){
    ((Dialog* ) parent) -> addUnit();
}

void IMenuBar:: removeCompany(){
    ((Dialog* ) parent) -> removeCompany();
}

void IMenuBar:: removePerson(){
    ((Dialog* ) parent) -> removePerson();
}

void IMenuBar:: removeVegetable(){
    ((Dialog* ) parent) -> removeVegetable();
}

void IMenuBar:: removeUnit(){
    ((Dialog* ) parent) -> removeUnit();
}
















