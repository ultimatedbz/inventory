#ifndef SEARCHHISTORYLIST_H
#define SEARCHHISTORYLIST_H

#include <QObject>
#include <QtWidgets>
#include <QListWidget>
#include "Vegetable.h"
#include "dialog.h"
#include "History.h"

using namespace std;

class SearchHistoryList : public QListWidget
{
    Q_OBJECT
public:
    SearchHistoryList(Vegetable* v, Dialog* d, vector<pair<History*, int>> hp, string dayPurchased, string company);
    Vegetable* vegetable;
    Dialog* dialog;
    vector<pair<History*, int>> historyPairs;
    void calculateHistory();
    // Used for calculate
    string datePurchased;
    string company;

public slots:
    void editSearchHistory();
};

#endif // SEARCHHISTORYLIST_H
