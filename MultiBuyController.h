#ifndef MULTIBUYCONTROLLER_H
#define MULTIBUYCONTROLLER_H

#include <QObject>
#include <QtWidgets>
#include <set>
#include <vector>
#include "Inventory.h"
#include "translator.h"

class MultiBuyElement;

using namespace std;

class MultiBuyController : public QObject
{
  Q_OBJECT
public:
    MultiBuyController(int currentVI, Inventory* inventory, QFormLayout* fo,
                       QFont font, QScrollArea* sa, QDialog* d, Translator*);
    ~MultiBuyController();
    int getActualVegeIndex(int i);
    string getAmount(double i);
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
    vector<MultiBuyElement*>* formArray;

    /* Set of indexes from the actual vege array */
    set<int>* selectedVeges;

    /* Array with each index corresponding to the actual index */
    vector<vector<int> >* comboIndexToActual;
};

#endif // MULTIBUYCONTROLLER_H
