#ifndef MULTISELLFORMLAYOUT_H
#define MULTISELLFORMLAYOUT_H

#include <QtWidgets>
#include <QObject>
#include "Inventory.h"F

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

public slots:
    void addRemaining();

};

#endif // MULTISELLFORMLAYOUT_H
