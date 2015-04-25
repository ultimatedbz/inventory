#ifndef MULTISELLCONTROLLER_H
#define MULTISELLCONTROLLER_H

#include <QObject>
#include <QtWidgets>
#include <set>
#include <vector>

#include "translator.h"
class MultiSellElement;
class Inventory;

using namespace std;

class MultiSellController : public QObject
{
  Q_OBJECT
public:
  MultiSellController(Inventory* inventory,
                      QFormLayout* fo, QFont font,
                      QScrollArea* sa,
                      QDialog* d, Translator*);
  ~MultiSellController();
  int getActualVegeIndex(int i);
  string getAmounts(int i);
  vector<int> getActualRemainingIndexes(int i);
  string getPrice(int i);
  int getElementNum();
signals:

public slots:
  void vegeDropChanged( int newIndex );
  void addElement();
  void subtractElement();

private:

  void updateVegeDrops();

  QFont font;
  QFormLayout* form;
  Inventory* mInventory;
  QScrollArea* scrollArea;
  QDialog* dialog;
  Translator* mTranslator;

  /* Contains all the multiSellElements */
  vector<MultiSellElement*>* formArray;

  /* Set of indexes from the actual vege array */
  set<int>* selectedVeges;

  /* Array with each index corresponding to the actual index */
  vector<vector<int> >* comboIndexToActual;
};

#endif // MULTISELLCONTROLLER_H
