#ifndef SEARCHRESULTS_H
#define SEARCHRESULTS_H

#include <QObject>
#include <QtWidgets>
#include <set>
#include <vector>

class MultiSellElement;
class Inventory;

using namespace std;

class SearchResultsController : public QObject
{
  Q_OBJECT
public:
  SearchResultsController(Inventory* inventory,
                      QFormLayout* fo, QFont font,
                      QScrollArea* sa,
                      QDialog* d, QLineEdit* dateTextField, QComboBox* companyDrop, QComboBox* customerDrop);
  ~SearchResultsController();
signals:

public slots:
  void showSearchResults();
  void editSearchHistory();

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
};


#endif // SEARCHRESULTS_H
