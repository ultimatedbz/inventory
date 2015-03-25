#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "Inventory.h"
#include "translator.h"
#include "imenubar.h"
#include "ipreferencespanel.h"
#include "MultiSellElement.h"
#include "MultiSellController.h"
#include "MultiBuyElement.h"
#include "ui_dialog.h"
#include "IConst.h"
#include "abbreviation.h"
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
#include <QListWidgetItem>
#include <iostream>
#include <QPrinter>
#include <sstream>

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
    void showPreferences();

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
    void printTransactions();
    void writeInt(int temp, fstream* fio);
    void writeString(string temp, fstream* fio);
    int readInt(fstream* fio);
    string readString(fstream* fio);

    void changeLanguage();



public slots:

    void on_Buy_clicked();
    void on_Sell_clicked();
    void printI(QPrinter* printer);
    void printH(QPrinter* printer);
    void printT(QPrinter* printer);

    void deleteHistory();
    void undoHistory();
    void deleteVege();

    void on_Memo_2_textChanged();
    static int compareCompany(const void *a, const void *b);
    static int compareCustomer(const void *a, const void *b);
    void on_Return_clicked();
    void closeEvent(QCloseEvent *event);
    void askSave();

    void on_vegeList_itemClicked(QListWidgetItem *item);


    void ListWidgetEditEnd(QWidget *editor, QAbstractItemDelegate::EndEditHint hint);


    void slot1();



private slots:
    void on_clearHistoryButton_clicked();
    void on_dumpButton_clicked();
    void on_clearReturnButton_clicked();
    void dumpVege();

    void on_vegeList_currentRowChanged(int currentRow);

private:
    Ui::Dialog *ui;
    Inventory* inventory;
    IPreferencesPanel *mPreferencesPanel;

    Vegetable *currentVege;
    int needSave;
    QFont font;
    int refresh;

    QAction* deleteVegeAction;
    QAction* undoHistoryAction;
    QAction* deleteHistoryAction;
    QAction* dumpVegeAction;

    Translator* mTranslator;
    Abbreviation* mAbbreviator;
    IMenuBar *menuBar;

    int queryVeges();
    struct myclass {
      bool operator() (pair<int,int> i, pair<int,int> j) {
        return (i.first>j.first);
      }
    } myobject;


};

#endif // DIALOG_H
