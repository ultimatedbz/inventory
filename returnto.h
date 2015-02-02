#ifndef RETURNTO_H
#define RETURNTO_H

/*
 * returnto.h
 */



#include <string>
using namespace std;

class ReturnTo {
public:
  ReturnTo();

  ~ReturnTo();

  void tui(string dr, int amount, string bc, string dB);
  void updateReturn(int amount, string c);
  int getReturn();
  string getDatePurchased();
  string getCompany();
  string getDateReturned();
  string formatReturn(string unit);
  void load(fstream* fio);

private:
  string dayPurchased;
  string company;
  int returnNum;
  string dayReturned;
};


#endif // Return_H
