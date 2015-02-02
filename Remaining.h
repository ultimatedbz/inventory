#ifndef REMAINING_H
#define REMAINING_H

/*
 * Remaining.h
 */

#include <string>
using namespace std;

class Remaining {
public:
  Remaining();

  ~Remaining();

  void removeRemaining();
  void buy(int amount, string bc, string date, string p);
  void updateRemaining(int amount);
  void updateRemainingWithRet(int amount);
  int getRemaining();
  int getReturn();
  string getPrice();
  string getDate();
  string getCompany();
  string formatRemaining();
  string formatRemaining2(string unit);
  string formatRemaining3();
  void returned(int amount, string bc, string date);
  void load(fstream* fio);

private:
  string dayPurchased;
  string company;
  int remaining;
  int returnNum;
  string price;
};


#endif // REMAINING_H
