#ifndef MULTIBUYCONTROLLER_H
#define MULTIBUYCONTROLLER_H

#include <QObject>
#include <QtWidgets>
#include <set>
#include <vector>
#include "Inventory.h"

class MultiBuyElement;

using namespace std;

class MultiBuyController : public QObject
{
  Q_OBJECT
public:
    MultiBuyController(Inventory* inventory, QFormLayout* fo, QFont font);
    ~MultiBuyController();
    int getActualVegeIndex(int i);
    string getAmount(int i);
    string getPrice(int i);
  signals:

  public slots:
    void vegeDropChanged( int newIndex );

  private:

    void updateVegeDrops();

    int elementNum;
    QFont font;
    QFormLayout* form;
    Inventory* mInventory;

    /* Contains all the multiSellElements */
    vector<MultiBuyElement*> formArray;

    /* Set of indexes from the actual vege array */
    set<int>* selectedVeges;

    /* Array with each index corresponding to the actual index */
    vector<vector<int> >* comboIndexToActual;
};

#endif // MULTIBUYCONTROLLER_H
