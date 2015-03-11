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
    int getDifferenceS(int z);
  string getHistory(string unit);
  string getCustomer();
    string getCustomerS(int z);
  string getDateSold();
    string getDateSoldS(int z);
  string getDateToCompare();
    string getDateToCompareS(int z);
  string getDatePurchased();
    string getDatePurchasedS(int z);
  string getPrice();
    string getPriceS(int z);
  string getCompany();
    string getCompanyS(int z);

    int getReturnedS(int z);
    int getDumpedS(int z);
  int getReturned();
  int getDumped();
  void load(fstream* fio);
  string printFormat();
  void deleteHistory();
  int getChangeNum();
  int getChangeNumS(int z);
  void tui(int amount, string date, string bd, string comp, int change);
  int getTui();
  int getTuiS(int z);
private:
  string daySold;
  string dayPurchased;
  int difference;
  string customer;
  string price;
  string company;
  int returned;
  int dumped;
  string dateToCompare;
  int changeNum;
  int mTui;
  string padding(string word);
};

#endif // History_H
