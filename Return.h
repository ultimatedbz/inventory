#ifndef Return_H
#define Return_H

/*
 * Return.h
 */

#include <string>
using namespace std;

class Return {
public:
  Return();

  ~Return();

  void add(string dr, int amount, string returner, string bc, string dB);
  void updateReturn(int amount, string c);
  int getReturn();
  string getDatePurchased();
  string getCompany();
  string getReturner();
  string getDateReturned();
  string formatReturn(string unit);
  void load(fstream* fio);

private:
  string dayPurchased;
  string company;
  int returnNum;
  string returner;
  string dayReturned;
};


#endif // Return_H
