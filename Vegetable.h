#ifndef VEGETABLE_H
#define VEGETABLE_H

#include "abbreviation.h"
#include "Remaining.h"
#include "Return.h"
#include "History.h"
#include "returnto.h"

#include <string>
#include <vector>
using namespace std;

class Vegetable {
public:
  /**
   * Initializes and returns a newly allocated scientific data set.
   *
   * @param   cfg                 Data set configuration.
   */
  Vegetable();
  ~Vegetable();
  Vegetable(string name, string u);

  /**
   * Deallocates scientific data set.
   */
  void remove();
  int buyVege(int amount, string bc, string date, string price);
  int sellVege(int amount, string costumer, string date, string price, int selection);
  int dumpVege(int amount, string date, int selection);
  int returnTo(int amount, string date, int selection);
  void returnThis(string dateReturned, int amount,
                  string returner, string company, string dateBought);

  int getHistoryNum();
  int getTotalVeges();
  int getRemaining(int i);
  int getRemainingIndex(int selection);
  int getRemainingNum();
  int getReturnNum();
  int getTuiNum();
  string getMemo();
  Remaining* getRemainingObject(int index);
  History* getHistoryObject(int index);
  Return* getReturnObject(int index);
  string getUnit();

  int remainExist(string company, string date);
  void updateRemaining(int i, int amount);
  void updateReturnInRemaining(int exist, int amount);
  void updateMemo(string m);
  void addReturnToRemaining(int amount);

  string viewHistory(int i);
  string formatTui(int i);
  string formatRemaining(int i);
  string formatRemaining2(int i);
  string formatRemaining3(int i, Abbreviation abb);
  string formatReturn(int i);
  const string getVegetablename() const;
  int returnExist(string company, string date);
  static int compareH(const void *a, const void *b);
  static bool compareR(Remaining a, Remaining b);
  static bool compareRet(Return a, Return b);
  void load(fstream* fio);
  int clearHist(int num);
   void setVegetableName(string name);
   void deleteHistory (int index);
   int restock(int amount, string dp, string company, int retNum);
   int undoRetOrBuy(int amount, string dP,string company,string dS,string customer);
   int returnExistCompany(string company, string date);
   void reTui(string dS, int amount, string dP, string company);
   ReturnTo* getTuiObject(int index);
   void clearTui();

   /* Transaction Printing */
   string formatTransaction(Abbreviation abb);
   void setUpTrans(Abbreviation abb);
   void transBuy(Abbreviation abb);
   void transSell(int amount, string datePurchased, string company, string customer,
                  Abbreviation abb);
   void transTui(int amount, string datePurchased, string company);
   void transReturn(int amount, string datePurchased, string company);
   void transDump(int amount, string datePurachased, string company);
   string transByIndex(int index);
   bool hasInteraction();
   int getTransNum();
   int getMTransNum();
   string padding(string word);
   vector<vector<string> > getTransactions();
   void editHistoryPrice(int, string);
private:
  string vegetableName;
  History* historyArray;
  int totalVeges;
  int historyNum;
  string unit;
  Remaining* remainingArray;
  int remainingNum;
  Return* returnArray;
  int returnNum;
  string memo;
  int tuiNum;
  ReturnTo* tuiArray;
  vector<vector<string> > transactions;
  int mTransNum;
};

#endif // Vegetable_H
