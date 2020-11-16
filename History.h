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
  void buy(double amount, string bc, string date, string price);
  void sell(double amount, string customer, string date, string price, string db, string company, int change);
  void dump(double amount, string date, string bd, string company, int change);
  void returnn (string dR , double amount, string returner, string company,string dB);
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
  void tui(double amount, string date, string dayPurchased, string comp, int change);
  int getTui();
  string price;

private:
  string type;
  // The date sold. For tui, it's the day "tui'd"
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
  string removeYear(string word);
};

#endif // History_H
