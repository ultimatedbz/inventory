#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "Inventory.h"
#include "translator.h"
#include "imenubar.h"
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

    void newFile();
    void loadFile();
    void save();
    void saveAs();
    void changeToEnglish();
    void changeToChinese();

    void addVegetable();
    void addCompany();
    void addPerson();
    void addUnit();
    void deleteVege();
    void removeCompany();
    void removePerson();
    void removeVegetable();
    void removeUnit();
    void printInventory();
    void printHistory();
    void writeInt(int temp, fstream* fio);
    void writeString(string temp, fstream* fio);


    void changeLanguage();
    void printI(QPrinter* printer);
    void printH(QPrinter* printer);

    Translator* mTranslator;

public slots:

    void on_Buy_clicked();
    void on_Sell_clicked();


    void additionalSell(int amount, int cusIndex, QString dateB, QString priceB);
    int querySplits();
    void splitSell(int splits, int amount, int cusIndex, QString dateB, QString priceB);
    int sameDay(int i1, int i2, int i3, int i4);

    void deleteHistory();
    void undoHistory();

    void on_Memo_2_textChanged();
    static int compareCompany(const void *a, const void *b);
    static int compareCustomer(const void *a, const void *b);
    void on_Return_clicked();
    void closeEvent(QCloseEvent *event);
    void askSave();

    void on_vegeList_itemPressed(QListWidgetItem *item);


    void ListWidgetEditEnd(QWidget *editor, QAbstractItemDelegate::EndEditHint hint);


    void slot1();
    void on_dumpButton_clicked();

private slots:
    void on_clearHistoryButton_clicked();

    void on_clearReturnButton_clicked();

private:
    Ui::Dialog *ui;
    Inventory* inventory;
    IMenuBar *menuBar;
    Vegetable *currentVege;
    int needSave;
    QFont font;
    int refresh;

};

#endif // DIALOG_H
