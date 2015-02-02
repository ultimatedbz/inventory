#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "Inventory.h"
#include <QListWidgetItem>
#include <iostream>
#include <QPrinter>

QT_BEGIN_NAMESPACE
class QAction;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QMenuBar;
class QPushButton;
class QTextEdit;
class QComboBox;
QT_END_NAMESPACE

namespace Ui {
class Dialog;

}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void createMenu();
private slots:

    void on_Buy_clicked();
    void on_Sell_clicked();
    void addVegetable();
    void addCompany();
    void addPerson();
    void addUnit();
    void removeCompany();
    void removePerson();
    void removeVegetable();
    void removeUnit();
    void printInventory();
    void printHistory();
    void additionalSell(int amount, int cusIndex, QString dateB, QString priceB);
    int querySplits();
    void splitSell(int splits, int amount, int cusIndex, QString dateB, QString priceB);
    int sameDay(int i1, int i2, int i3, int i4);
    void save();
    void saveAs();
    void newFile();
    void loadFile();
    void on_Memo_2_textChanged();
    static int compareCompany(const void *a, const void *b);
    static int compareCustomer(const void *a, const void *b);
    void on_Return_clicked();

    void on_pushButton_clicked();
    void closeEvent(QCloseEvent *event);
    void askSave();

    void on_pushButton_2_clicked();
    void printI(QPrinter* printer);
    void printH(QPrinter* printer);

    void on_vegeList_itemPressed(QListWidgetItem *item);


    void ListWidgetEditEnd(QWidget *editor, QAbstractItemDelegate::EndEditHint hint);

    void deleteVege();
    void deleteHistory();
    void undoHistory();
    void slot1();

    void on_pushButton_3_clicked();

private:
    Ui::Dialog *ui;
    Inventory* inventory;
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *editMenu;
    QAction *undoAction;
    QAction *undoHistoryAction;
    QAction *exitAction;
    QAction *printAction;
    QAction *printAction2;
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
    Vegetable *currentVege;
    int needSave;
    QFont font;
    int refresh;
};

#endif // DIALOG_H
