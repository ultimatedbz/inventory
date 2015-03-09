#ifndef MULTISELLCONTROLLER_H
#define MULTISELLCONTROLLER_H

#include <QObject>
#include <QtWidgets>

#include "Inventory.h"
#include <set>
#include <vector>

class MultiSellElement;

class MultiSellController : public QObject
{
  Q_OBJECT
public:
  MultiSellController(int queryNum,
                      Inventory* inventory, QFormLayout* fo, QFont font);
  ~MultiSellController();

signals:

public slots:
  void vegeDropChanged( int newIndex );

private:

  void updateVegeDrops();

  QFont font;
  QFormLayout* form;
  Inventory* mInventory;

  /* Contains all the multiSellElements */
  MultiSellElement** formArray;

  /* Set of indexes from the actual vege array */
  set<int>* selectedVeges;

  /* Array with each index corresponding to the actual index */
  vector<int>* comboIndexToActual;
};

#endif // MULTISELLCONTROLLER_H
