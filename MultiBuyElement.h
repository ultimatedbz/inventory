#ifndef MULTIBUYELEMENT_H
#define MULTIBUYELEMENT_H

#include <QtWidgets>
#include <QObject>
#include <set>
#include <vector>
#include "MultiBuyController.h"
#include "Inventory.h"
#include "scrolleater.h"
#include "translator.h"

using namespace std;

class MultiBuyElement:public QObject
{
public:
    MultiBuyElement(int index, MultiBuyController* c, Inventory* i,
        QFont f, set<int>* selectedVeges,
        vector<int>& vegeIndexToActual, Translator*);
    ~MultiBuyElement();
    QFormLayout* getElement();
    QComboBox* getVegeDrop();
    void updateVegeDrops(set<int> selectedVeges,
                         vector<int>& actualIndexes);
    string getAmount();
    string getPrice();
private:
    QFormLayout* mForm;
    MultiBuyController* controller;
    Inventory* mInventory;
    QFont font;
    QComboBox* vegeDrop;
    QLineEdit* amount;
    QLineEdit* price;
    ScrollEater* scrollEat;
};

#endif // MULTIBUYELEMENT_H
