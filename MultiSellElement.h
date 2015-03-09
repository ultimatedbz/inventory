#ifndef MULTISELLELEMENT_H
#define MULTISELLELEMENT_H

#include <QtWidgets>
#include <QObject>

#include "Inventory.h"
#include "MultiSellController.h"
#include <set>
#include <vector>


class MultiSellElement:public QObject
{
Q_OBJECT
public:
    MultiSellElement(int index, MultiSellController* c, Inventory* i, QFont f,
                     set<int>* selectedVeges,
                     vector<int>& vegeIndexToActual);
    ~MultiSellElement();
    QFormLayout* getElement();
    QComboBox* getVegeDrop();
    void updateVegeDrops(set<int> selectedVeges,
                         vector<int>& actualIndexes);
private:

    void updateRemainDrops();

    QFormLayout* mForm;
    MultiSellController* controller;
    Inventory* mInventory;
    QFont font;
    int mVegIndex;
    int first;
    QComboBox* vegeDrop;
    QHBoxLayout* hLay;

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

#endif// MULTISELLELEMENT_H
