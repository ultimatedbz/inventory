#include "imenubar.h"
#include <QApplication>
#include <QDebug>
#include "dialog.h"


IMenuBar::IMenuBar(QFont f, QWidget* p, Translator* trans):
        QMenuBar(p),
        font(f),
        mTranslator(trans),
        parent(p) {

    setFixedHeight(40);
    setFixedWidth(2000);
    QMenu* fileMenu = addMenu("File");
    fileMenu->setFont(font);
    QMenu* editMenu = addMenu("Edit");
    editMenu->setFont(font);

    newAction = fileMenu->addAction(mTranslator ->translate("開新檔案").c_str());
    newAction->setShortcut(QKeySequence::New);
    newAction->setFont(font);
    loadAction = fileMenu->addAction(mTranslator ->translate("開啟舊檔").c_str());
    loadAction->setShortcut(QKeySequence::Open);
    loadAction->setFont(font);
    saveAction =fileMenu->addAction(mTranslator ->translate("儲存檔案").c_str());;
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setFont(font);
    saveAsAction = fileMenu->addAction(mTranslator ->translate("另存新檔").c_str());
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    saveAsAction->setFont(font);
    printAction = fileMenu->addAction(mTranslator ->translate("印 Inventory").c_str());
    printAction->setFont(font);
    printAction2 = fileMenu->addAction(mTranslator ->translate("印 History").c_str());
    printAction2->setFont(font);
    printAction3 = fileMenu->addAction(mTranslator ->translate("印 記錄表").c_str());
    printAction3->setFont(font);


    fileMenu->addSeparator();
    QMenu* translateMenu = fileMenu->addMenu(tr("Translate"));
    QMenu* appearanceMenu = fileMenu->addMenu(tr("Appearance"));

    addPersonAction = editMenu->addAction(mTranslator ->translate("加入新客戶").c_str());
    addPersonAction->setFont(font);
    addCompanyAction = editMenu->addAction(mTranslator ->translate("加入新公司").c_str());
    addCompanyAction->setFont(font);
    addVegetableAction = editMenu->addAction(mTranslator ->translate("加入新的菜名").c_str());
    addVegetableAction->setFont(font);
    addUnitAction = editMenu->addAction(mTranslator ->translate("加入單位").c_str());
    addUnitAction->setFont(font);
    removePersonAction = editMenu->addAction(mTranslator ->translate("刪除客戶").c_str());
    removePersonAction->setFont(font);
    removeCompanyAction = editMenu->addAction(mTranslator ->translate("刪除公司").c_str());
    removeCompanyAction->setFont(font);
    removeVegetableAction = editMenu->addAction(mTranslator ->translate("刪掉菜名").c_str());
    removeVegetableAction->setFont(font);
    removeUnitAction = editMenu->addAction(mTranslator ->translate("刪掉單位").c_str());
    removeUnitAction->setFont(font);
    changeToEnglishAction = translateMenu->addAction("English");
    changeToEnglishAction->setFont(font);
    changeToChineseAction = translateMenu->addAction("中文");
    changeToChineseAction->setFont(font);
    changeToDarkModeAction = appearanceMenu->addAction("Dark Mode");
    changeToDarkModeAction->setFont(font);
    changeToLightModeAction = appearanceMenu->addAction("Light Mode");
    changeToLightModeAction->setFont(font);

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
    connect(changeToDarkModeAction, SIGNAL(triggered()), this, SLOT(changeToDarkMode()));
    connect(changeToLightModeAction, SIGNAL(triggered()), this, SLOT(changeToLightMode()));

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
    printAction3->setText(mTranslator ->translate("印 記錄表").c_str());
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

void IMenuBar:: changeToDarkMode(){
    ((Dialog* ) parent) -> changeToDarkMode();
}

void IMenuBar:: changeToLightMode(){
    ((Dialog* ) parent) -> changeToLightMode();
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
















