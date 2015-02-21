#ifndef MULTISELLFORMLAYOUT_H
#define MULTISELLFORMLAYOUT_H

#include <QtWidgets>
#include "Inventory.h"


class MultiSellFormLayout
{
public:
    MultiSellFormLayout(int queryNum, QDialog* d, Inventory* i, QFont f);
    ~MultiSellFormLayout();
    QFormLayout* getMSElement(int i);
private:
    QFormLayout* mFormArray;
    QDialog* dialog;
    Inventory* mInventory;
    QFont font;

private slots:
    void addRemaining();

};

#endif // MULTISELLFORMLAYOUT_H
