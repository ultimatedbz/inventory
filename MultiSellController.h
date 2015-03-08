#ifndef MULTISELLCONTROLLER_H
#define MULTISELLCONTROLLER_H

#include <QObject>
#include <QtWidgets>
#include "Inventory.h"
#include "MultiSellElement.h"
#include <set>
#include <vector>

class MultiSellController : public QObject
{
  Q_OBJECT
public:
  MultiSellController(int queryNum,
                      Inventory* inventory, QFormLayout& form, QFont font);
  ~MultiSellController();

signals:

public slots:
  void vegeDropChanged( int newIndex );

private:

  /* Contains all the vegeDrops */
  vector<QComboBox*>* vegeDrops;

  /* Set of indexes from the actual vege array */
  set<int>* selectedVeges;

  /* Array with each index corresponding to the actual index */
  vector<int>* comboIndexToActual;
};

#endif // MULTISELLCONTROLLER_H
