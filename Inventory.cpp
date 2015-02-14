/*
 * Inventory.cpp
 */

#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <QDebug>
#include <fstream>
using namespace std;
#include "Return.h"
#include "returnto.h"
#include "History.h"
#include "Remaining.h"
#include "Vegetable.h"
#include "Inventory.h"
/**
 * Initializes and returns a newly allocated scientific data set.
 *
 * @param     cfg                 Data set configuration.
 */


Inventory::Inventory()
    :vegetableNum(0),
     companyNum(0),
     personNum(0),
     unitNum(0),
     vegeArray(NULL),
     companyArray(NULL),
     unitArray(NULL),
     personArray(NULL),

     fileName("")
      {
      
      } 
Inventory:: ~Inventory(void){

}
int Inventory::addVegetable(string name, string unit){
    for( int i = 0; i < vegetableNum; i++){
        if(!vegeArray[i].getVegetablename().compare(name)){
            return 0;
        }
    }
  Vegetable newVege(name, unit);
  if( !vegeArray )
      vegeArray = new Vegetable[100];
  vegeArray[vegetableNum] = newVege;
  vegetableNum++;
  qsort(vegeArray,vegetableNum, sizeof(Vegetable), compareV);
  return 1;
}

void Inventory::sortVeges(){
      qsort(vegeArray,vegetableNum, sizeof(Vegetable), compareV);
}

int Inventory::compareV(const void * a, const void *b){
    Vegetable* A = (Vegetable *)a;
    Vegetable* B = (Vegetable *)b;

    const int result = strcmp(A->getVegetablename().c_str(),B->getVegetablename().c_str());
    return result;
}
int Inventory::compareS(const void * a, const void *b){
    string* A = (string *)a;
    string* B = (string *)b;

    const int result = strcmp(A->c_str(),B->c_str());
    return result;
}
int Inventory::addCompany(string name){

    for( int i = 0; i < companyNum; i++){
        if(!companyArray[i].compare(name)){
            return 0;
        }
    }

  string newString = name;
  if( !companyArray )
      companyArray = new string[100];
  companyArray[companyNum] = newString;
  companyNum++;
  qsort(companyArray, companyNum, sizeof(string), compareS);
  return 1;
}

int Inventory::addUnit(string name){

    for( int i = 0; i < unitNum; i++){
        if(!unitArray[i].compare(name)){
            return 0;
        }
    }

  string newString = name;
  if( !unitArray )
      unitArray = new string[100];
  unitArray[unitNum] = newString;
  unitNum++;
  return 1;
}

int Inventory::addPerson(string name){
    for( int i = 0; i < personNum; i++){
        if(!personArray[i].compare(name)){
            return 0;
        }
    }
    string newString = name;
    if( !personArray )
        personArray = new string[100];
    personArray[personNum] = newString;
    personNum++;
    qsort(personArray, personNum, sizeof(string), compareS);
    return 1;
}

int Inventory::getVegNum(){
  return vegetableNum;
}

int Inventory::getPersonNum(){
  return personNum;
}

int Inventory::getCompanyNum(){
  return companyNum;
}

int Inventory::getUnitNum(){
  return unitNum;
}

Vegetable* Inventory::getVegetable(string name){
  for( int i = 0; i < vegetableNum; i++){
    if(!vegeArray[i].getVegetablename().compare(name)){
      return &vegeArray[i];
    }
  }
  return NULL;
}

int Inventory::getVegetableIndex(string name){
  for( int i = 0; i < vegetableNum; i++){
    if(!vegeArray[i].getVegetablename().compare(name)){
      return i;
    }
  }
  return -1;
}

Vegetable* Inventory::getVegetableByIndex(int index){

      return &vegeArray[index];

}

string Inventory::getPerson(int index){
    return personArray[index];

}


string Inventory::getCompany(int index){
    return companyArray[index];

}

string Inventory::getUnit(int index){
    return unitArray[index];
}


int Inventory::checkIfInInventory(string name){
  for( int i = 0; i < vegetableNum; i++){
    if(!vegeArray[i].getVegetablename().compare(name))
      return 1;
  }
  return 0;
}

void Inventory::viewHistory(string name){
  for( int i = 0; i < vegetableNum; i++){
    if(!vegeArray[i].getVegetablename().compare(name)){

    }
  }
}

void Inventory::removeCompany(string name){
  for( int i = 0; i < companyNum; i++){
    if(!companyArray[i].compare(name)){
      for( int j = i; j < companyNum - 1; j++){
        companyArray[j] = companyArray[j+1];
      }
      companyArray[companyNum - 1] = "\t";
      companyNum--;
    }
  }
  if(!companyNum)
      companyArray = NULL;
}

void Inventory::removePerson(string name){
  for( int i = 0; i < personNum; i++){
    if(!personArray[i].compare(name)){
      for( int j = i; j < personNum - 1; j++){
        personArray[j] = personArray[j+1];
      }
      personArray[personNum - 1] = "\t";
      personNum--;
    }
  }
  if(!personNum)
      personArray = NULL;
}

void Inventory::removeVegetable(string name){
  for( int i = 0; i < vegetableNum; i++){
    if(!vegeArray[i].getVegetablename().compare(name)){
      for( int j = i; j < vegetableNum - 1; j++){
        vegeArray[j] = vegeArray[j + 1];
      }
      //vegeArray[vegetableNum - 1] = NULL;
      vegetableNum--;
    }
  }


  if(!vegetableNum)
      vegeArray = NULL;
}

void Inventory::removeUnit(string name){
  for( int i = 0; i < unitNum; i++){
    if(!unitArray[i].compare(name)){
      for( int j = i; j < unitNum - 1; j++){
        unitArray[j] = unitArray[j + 1];
      }
      unitArray[unitNum - 1 ] = "\t";
      unitNum--;
    }
  }
  if(!unitNum)
      unitArray = NULL;
}

string Inventory::getFileName(){
    return fileName;
}

void Inventory::setFileName(string name){
    fileName = name;
}

void Inventory::remove(){
    if(companyArray){
        delete[] companyArray;
        companyArray = NULL;
    }
    if(unitArray){

        delete[] unitArray;
        unitArray = NULL;
    }
    if(personArray){

        delete[] personArray;
        personArray = NULL;
    }
   for(int i = 0; i< vegetableNum; i++){
        vegeArray[i].remove();

    }
   if(vegeArray)
        delete[] vegeArray;
   vegeArray = NULL;
   vegetableNum = companyNum = personNum = unitNum = 0;
  // delete this;
}


void Inventory::load(fstream* fio){
    fio->read(( char *) &(companyNum), sizeof(int));
    fio->read(( char *) &(personNum), sizeof(int));
    fio->read(( char *) &(unitNum), sizeof(int));
    fio->read(( char *) &(vegetableNum), sizeof(int));


    if(companyNum)
        companyArray = new string[100];
    for(int i = 0; i < companyNum; i++){
        char temp2[200];
        int tempNum2;
        fio->read((char *) &(tempNum2),sizeof(int));
        fio->read(temp2, tempNum2);
        temp2[tempNum2] = '\0';
        string str(temp2);
        companyArray[i] = str;
    }
    if(personNum)
        personArray = new string[100];

    for(int i = 0; i < personNum ; i++){
        char temp2[200];
        int tempNum2;
        fio->read((char *) &(tempNum2),sizeof(int));
        fio->read(temp2, tempNum2);
        temp2[tempNum2] = '\0';
        string str(temp2);
        personArray[i] = str;

    }

    if(unitNum)
        unitArray = new string[100];

    for(int i = 0; i < unitNum ; i++){
        char temp2[200];
        int tempNum2;
        fio->read((char *) &(tempNum2),sizeof(int));
        fio->read(temp2, tempNum2);
        temp2[tempNum2] = '\0';

        string str(temp2);
        unitArray[i] = str;
    }
    if(vegetableNum)
        vegeArray = new Vegetable[100];
    for(int i = 0; i < vegetableNum; i++){
        Vegetable temp;
        temp.load(fio);
        vegeArray[i] = temp;
    }
}











