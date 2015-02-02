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
  int getSplitNum();
    int getSplitNumS(int z);
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
  void splitSell(int spl, int input1,int  input2,int  input3,int  input4, string dateS,string cust,
                 string pr, string d1, string d2, string d3, string d4, string c1, string c2, string c3, string c4,
                 int a, int b, int c, int d);
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
  int splitNum;
  History* splitArray;
  int changeNum;
  int mTui;
};

#endif // History_H
