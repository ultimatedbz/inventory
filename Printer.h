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

#ifndef PRINTER
#define PRINTER


class Printer
{
public:
    Printer();
    void printHistory(QPrinter * printer);
    void printInventory(QPrinter*);
    void printTransactions(QPrinter*);
};

#endif // PRINTER
