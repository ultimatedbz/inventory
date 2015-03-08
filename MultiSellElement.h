#ifndef MULTISELLELEMENT_H
#define MULTISELLELEMENT_H

#include <QtWidgets>
#include <QObject>

#include "dialog.h"
#include "Inventory.h"
#include <set>
#include <vector>

class MultiSellController;

class MultiSellElement:public QObject
{
Q_OBJECT
public:
    MultiSellElement(int index, MultiSellController* d, Inventory* i, QFont f);
    ~MultiSellElement();
    QFormLayout* getElement();
    QComboBox* getVegeDrop();
private:

    void updateRemainDrops();

    QFormLayout* mForm;
    MultiSellController* controller;
    Inventory* mInventory;
    QFont font;
    int mVegIndex;
    int first;
    QComboBox* vegeDrop;

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
