#ifndef INVENTORY_H
#define INVENTORY_H
/*
 * Inventory.h
 */

#include <string>
using namespace std;
#include "returnto.h"
#include "Return.h"
#include "History.h"
#include "Remaining.h"
#include "Vegetable.h"
#include "Inventory.h"

class Inventory {
public:
  /**
   * Initializes and returns a newly allocated scientific data set.
   *
   * @param   cfg                 Data set configuration.
   */
  Inventory();

  /**
   * Deallocates scientific data set.
   */
  ~Inventory();

  int addVegetable(string name, string unit);

  int addCompany(string name);

  int addPerson(string name);

  int addUnit(string name);

  int getVegNum();

  int getCompanyNum();

  int getPersonNum();

  int getUnitNum();

  int getMemoNum();

  Vegetable* getVegetableByIndex(int index);

  Vegetable* getVegetable(string name);

  int getVegetableIndex(string name);

  string getUnit(int index);

  string getCompany(int index);

  string getPerson(int index);

  string getFileName();

  void remove();

  void removeVegetable(string name);

  void removePerson(string name);

  void removeUnit(string name);

  void removeCompany(string name);

  int checkIfInInventory(string name);

  void viewHistory(string name);

  void setFileName(string datafile);

  void sortVeges();

  void load(fstream* fio);
  static int compareV(const void * a, const void *b);
  static int compareS(const void * a, const void *b);



private:
  int vegetableNum;
  int companyNum;
  int personNum;
  int unitNum;
  Vegetable* vegeArray;
  string* companyArray;
  string* unitArray;
  string* personArray;
  string fileName;

};

#endif //INVENTORY_H
