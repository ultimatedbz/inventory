#ifndef REMAINING_H
#define REMAINING_H

/*
 * Remaining.h
 */

#include <string>
#include "abbreviation.h"
using namespace std;

class Remaining {
public:
  Remaining();

  ~Remaining();

  void removeRemaining();
  void buy(double amount, string bc, string date, string p);
  void updateRemaining(double amount);
  void updateRemainingWithRet(double amount);
  int getRemaining();
  double getReturn();
  string getPrice();
  string getDate();
  string getCompany();
  string formatRemaining();
  string formatRemaining2(string unit);
  string formatRemaining3(Abbreviation abb);
  void returned(double amount, string bc, string date);
  void load(fstream* fio);

private:
  string dayPurchased;
  string company;
  int remaining;
  int returnNum;
  string price;
  string padding(string word);
  string removeYear(string word);
};


#endif // REMAINING_H
