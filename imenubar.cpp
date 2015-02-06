#include "imenubar.h"
#include <QApplication>
#include <QDebug>
#include "dialog.h"


IMenuBar::IMenuBar(QWidget* p, Translator* trans):
        QMenuBar(p),
        parent(p),
        mTranslator(trans){
    QMenu* fileMenu = addMenu("File");

    QMenu* editMenu = addMenu("Edit");

    newAction = fileMenu->addAction(mTranslator ->translate("New File").c_str());
    newAction->setShortcut(QKeySequence::New);
    loadAction = fileMenu->addAction(mTranslator ->translate("Load File").c_str());
    loadAction->setShortcut(QKeySequence::Open);
    saveAction =fileMenu->addAction(mTranslator ->translate("Save").c_str());;
    saveAction->setShortcut(QKeySequence::Save);
    saveAsAction = fileMenu->addAction(mTranslator ->translate("Save As").c_str());
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    printAction = fileMenu->addAction(mTranslator ->translate("Print Inventory").c_str());
    printAction2 = fileMenu->addAction(mTranslator ->translate("Print History").c_str());


    fileMenu->addSeparator();
    QMenu* translateMenu = fileMenu->addMenu(tr("Translate"));

    addPersonAction = editMenu->addAction(mTranslator ->translate("Add Customer").c_str());
    addCompanyAction = editMenu->addAction(mTranslator ->translate("Add Company").c_str());
    addVegetableAction = editMenu->addAction(mTranslator ->translate("Add Vegetable").c_str());
    addUnitAction = editMenu->addAction(mTranslator ->translate("Add Unit").c_str());
    removePersonAction = editMenu->addAction(mTranslator ->translate("Delete Customer").c_str());
    removeCompanyAction = editMenu->addAction(mTranslator ->translate("Delete Company").c_str());
    removeVegetableAction = editMenu->addAction(mTranslator ->translate("Delete Vegetable").c_str());
    removeUnitAction = editMenu->addAction(mTranslator ->translate("Delete Unit").c_str());
    changeToEnglishAction = translateMenu->addAction("English");
    changeToChineseAction = translateMenu->addAction("中文");



    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(loadAction, SIGNAL(triggered()), this, SLOT(loadFile()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(printAction, SIGNAL(triggered()), this, SLOT(printInventory()));
    connect(printAction2, SIGNAL(triggered()), this, SLOT(printHistory()));
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
    newAction->setText(mTranslator ->translate("New File").c_str());
    loadAction ->setText(mTranslator ->translate("Load File").c_str());
    saveAction ->setText(mTranslator ->translate("Save").c_str());;
    saveAsAction ->setText(mTranslator ->translate("Save As").c_str());

    addPersonAction->setText(mTranslator ->translate("Add Customer").c_str());
    addCompanyAction ->setText(mTranslator ->translate("Add Company").c_str());
    addVegetableAction ->setText(mTranslator ->translate("Add Vegetable").c_str());
    addUnitAction ->setText(mTranslator ->translate("Add Unit").c_str());
    removePersonAction ->setText(mTranslator ->translate("Delete Customer").c_str());
    removeCompanyAction ->setText(mTranslator ->translate("Delete Company").c_str());
    removeVegetableAction->setText(mTranslator ->translate("Delete Vegetable").c_str());
    removeUnitAction->setText(mTranslator ->translate("Delete Unit").c_str());
    printAction->setText(mTranslator ->translate("Print Inventory").c_str());
    printAction2->setText(mTranslator ->translate("Print History").c_str());
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

void IMenuBar:: changeToEnglish(){
    ((Dialog* ) parent) -> changeToEnglish();
}

void IMenuBar:: changeToChinese(){
    ((Dialog* ) parent) -> changeToChinese();
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















