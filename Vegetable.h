#ifndef VEGETABLE_H
#define VEGETABLE_H
/*
 * Vegetable.h
 */

#include <string>
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
  int remainExist(string company, string date);
  void updateRemaining(int i, int amount);
  void updateReturnInRemaining(int exist, int amount);
  void updateMemo(string m);
  void addReturnToRemaining(int amount);
  string getUnit();
  string viewHistory(int i);
  string formatTui(int i);
  string formatRemaining(int i);
  string formatRemaining2(int i);
  string formatRemaining3(int i);
  string formatReturn(int i);
  const string getVegetablename() const;
  void returnThis(string dateBought, int a, string rer, string co, string dateB);
  int returnExist(string company, string date);
  static int compareH(const void *a, const void *b);
  static int compareR(const void *a, const void *b);
  static int compareRet(const void *a, const void *b);
  void load(fstream* fio);
  int clearHist(int num);
  void splitSell(int splits,int  input1,int input2,int input3, int input4,string date,
                  string customer, string price, int selection1,int selection2, int selection3, int selection4);
   void setVegetableName(string name);
   void deleteHistory (int index);
   int restock(int amount, string dp, string company, int retNum);
   int undoRetOrBuy(int amount, string dP,string company,string dS,string customer);
   int returnExistCompany(string company, string date);
   void reTui(string dS, int amount, string dP, string company);
   ReturnTo* getTuiObject(int index);
   void clearTui();
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
};

#endif // Vegetable_H
