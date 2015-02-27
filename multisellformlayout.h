#ifndef MULTISELLFORMLAYOUT_H
#define MULTISELLFORMLAYOUT_H

#include <QtWidgets>
#include <QObject>
#include "Inventory.h"
#include <set>
#include <vector>
#include <map>

class MultiSellFormLayout:public QObject
{
Q_OBJECT
public:
    MultiSellFormLayout(int index, QDialog* d, Inventory* i, QFont f);
    ~MultiSellFormLayout();
    QFormLayout* getElement();
private:
    QFormLayout* mForm;
    QDialog* dialog;
    Inventory* mInventory;
    QFont font;
    int first;

    /* Set of indexes from the actual remaining array */
    set<int>* selectedRemains;

    int mVegIndex;
    void updateRemainDrops();
    vector<QComboBox*>* remainingDrops; // contains all remainingcomboboxes

    /* Array with each index corresponding to the actual index
     **/
    vector<int>* comboIndexToActual;

private slots:
    void addRemaining();
    void remainingDropChanged(int a);

};

#endif // MULTISELLFORMLAYOUT_H
