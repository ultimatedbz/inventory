#ifndef SEARCHRESULTS_H
#define SEARCHRESULTS_H

#include <QObject>
#include <QtWidgets>
#include <set>
#include <vector>
#include "dialog.h"

class Inventory;

using namespace std;

class SearchResultsController : public QObject
{
  Q_OBJECT
public:
  SearchResultsController(Inventory* inventory,
                      QFormLayout* fo, QFont font,
                      QScrollArea* sa,
                      QDialog* d, QLineEdit* dateTextField, QComboBox* companyDrop, QComboBox* customerDrop, Dialog* md);

  ~SearchResultsController();
signals:

public slots:
  void showSearchResults();

private:

  void updateVegeDrops();

  QFont font;
  QFormLayout* form;
  Inventory* mInventory;
  QScrollArea* scrollArea;
  QDialog* dialog;
  QLineEdit* dateTextField;
  QComboBox* companyDrop;
  QComboBox* customerDrop;
  Dialog* mainDialog;
};


#endif // SEARCHRESULTS_H
