#ifndef IMENUBAR
#define IMENUBAR

#include <QMenuBar>
//#include "dialog.h"
#include "translator.h"

class IMenuBar : public QMenuBar {
    Q_OBJECT

public:
    explicit IMenuBar(QFont f, QWidget* parent = NULL, Translator* trans = NULL);
    Translator* mTranslator;
    QFont font;
    void changeLanguage();
public slots:
    void newFile();
    void loadFile();
    void save();
    void saveAs();
    void printInventory();
    void printHistory();
    void printTransactions();
    void changeToEnglish();
    void changeToChinese();
    void changeToDarkMode();
    void changeToLightMode();
    void showPreferences();

    void addVegetable();
    void addCompany();
    void addPerson();
    void addUnit();
    void removeCompany();
    void removePerson();
    void removeVegetable();
    void removeUnit();



private:
    QAction *undoAction;
    QAction *undoHistoryAction;
    QAction *exitAction;
    QAction *printAction;
    QAction *printAction2;
    QAction *printAction3;
    QAction *newAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *loadAction;
    QAction *addPersonAction;
    QAction *addCompanyAction;
    QAction *addVegetableAction;
    QAction *addUnitAction;
    QAction *removePersonAction;
    QAction *removeCompanyAction;
    QAction *removeVegetableAction;
    QAction *removeUnitAction;
    QAction *deleteVegeAction;
    QAction *deleteHistoryAction;
    QAction *deleteRemainingAction;
    QAction *deleteReturnAction;
    QAction *changeToChineseAction;
    QAction *changeToEnglishAction;
    QAction *mShowPreferencesAction;
    QAction *changeToDarkModeAction;
    QAction *changeToLightModeAction;
    QWidget* parent;
};

#endif // IMENUBAR

