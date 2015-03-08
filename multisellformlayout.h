#ifndef MULTISELLFORMLAYOUT_H
#define MULTISELLFORMLAYOUT_H

#include <QtWidgets>
#include <QObject>

#include "dialog.h"
#include "Inventory.h"
#include <set>
#include <vector>
#include <map>

class Dialog;

class MultiSellFormLayout:public QObject
{
Q_OBJECT
public:
    MultiSellFormLayout(int index, Dialog* d, Inventory* i, QFont f);
    ~MultiSellFormLayout();
    QFormLayout* getElement();
private:

    void updateRemainDrops();

    QFormLayout* mForm;
    Dialog* dialog;
    Inventory* mInventory;
    QFont font;
    int mVegIndex;
    int first;

    /* Contains all the remaining comboboxes*/
    vector<QComboBox*>* remainingDrops;

    /* Set of indexes from the actual remaining array */
    set<int>* selectedRemains;

    /* Array with each index corresponding to the actual index*/
    vector<int>* comboIndexToActual;




private slots:
    void addRemaining();
    void remainingDropChanged(int a);

};

#endif // MULTISELLFORMLAYOUT_H
