#ifndef MULTISELLFORMLAYOUT_H
#define MULTISELLFORMLAYOUT_H

#include <QtWidgets>
#include <QObject>
#include "Inventory.h"F

class MultiSellFormLayout:public QObject
{
Q_OBJECT
public:
    MultiSellFormLayout(int queryNum, QDialog* d, Inventory* i, QFont f);
    ~MultiSellFormLayout();
    QFormLayout* getMSElement(int i);
private:
    QFormLayout* mFormArray;
    QDialog* dialog;
    Inventory* mInventory;
    QFont font;

public slots:
    void addRemaining();

};

#endif // MULTISELLFORMLAYOUT_H
