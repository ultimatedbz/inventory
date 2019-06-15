#ifndef HISTORY_H
#define HISTORY_H
/*
 * History.h
 */

#include <string>
using namespace std;

class History {
public:
  History();

  ~History();

  void removeHist();
  void buy(int amount, string bc, string date, string price);
  void sell(int amount, string customer, string date, string price, string db, string company, int change);
  void dump(int amount, string date, string bd, string company, int change);
  void returnn (string dR , int amount, string returner, string company,string dB);
  void updateCompany(string company);
  void updateCompany(int victim, int source);
  int getDifference();
  string getHistory(string unit);
  string getCustomer();
  string getDateSold();
  string getDateToCompare();
  string getDatePurchased();
  string getPrice();
  string getCompany();
  string getType();

  int getReturned();
  int getDumped();
  void load(fstream* fio);
  string printFormat();
  void deleteHistory();
  int getChangeNum();
  void tui(int amount, string date, string bd, string comp, int change);
  int getTui();
  string price;

private:
  string type;
  string daySold;
  string dayPurchased;
  int difference;
  string customer;
  string company;
  int returned;
  int dumped;
  string dateToCompare;
  int changeNum;
  int mTui;
  string padding(string word);
};

#endif // History_H
