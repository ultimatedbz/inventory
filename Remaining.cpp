
/*
 * Remaining.cpp
 */

#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;
#include "Remaining.h"

/**
 * Initializes and returns a newly allocated scientific data set.
 *
 * @param     cfg                 Data set configuration.
 */


Remaining::Remaining()
  :
    dayPurchased("\t"),
    company("\t"),
    remaining(0),
    returnNum(0),
    price("\t")

      {

      }

Remaining::~Remaining(){}


string Remaining::getCompany(){
    return company;
}

void Remaining::buy(int amount, string bc, string date, string p){

  company = bc;
  dayPurchased = date;
  remaining = amount;
  price = p;

}
void Remaining::returned(int amount, string bc, string date){
    company = bc;
    dayPurchased = date;
    remaining = amount;
    returnNum = amount;
}

string Remaining::getDate(){
    return dayPurchased;
}

string Remaining::getPrice(){
    return price;
}

int Remaining::getRemaining(){
   return remaining;
}

int Remaining:: getReturn(){
    return returnNum;
}

string Remaining::formatRemaining(){


      char buffer [128];
    sprintf(buffer,"%5s%12s%5d",
            dayPurchased.c_str(),
            company.c_str(),
            remaining);
 return buffer;
}
string Remaining::formatRemaining2(string unit){
    if(!price.compare("") || !price.compare("\t"))
        price = "--";
    string Price = "$" + price;
      char buffer [128];
    sprintf(buffer,"%5s%11d%6s%20s%7s",
            dayPurchased.c_str(),
            remaining,
            unit.c_str(),
            company.c_str(),
            Price.c_str());
 return buffer;
}
string Remaining::formatRemaining3(){
    if(!price.compare("") || !price.compare("\t"))
        price = "--";
    string Price = "$" + price;
    char buffer [128];
    sprintf(buffer,"%5s%5d%10s%4s",
           dayPurchased.c_str(),
           remaining,
           company.c_str(),
            Price.c_str());
 return buffer;
}
void Remaining:: updateRemaining(int amount){
    remaining += amount;
}

void Remaining:: updateRemainingWithRet(int amount){
    remaining += amount;
    returnNum = ((returnNum + amount) < 0)? 0 : returnNum + amount;

}


void Remaining::load(fstream* fio){
    char temp[200];
    int tempNum;
    fio->read((char *) &(tempNum),sizeof(int));
    fio->read(temp, tempNum);
    temp[tempNum]='\0';
    string str(temp);
    dayPurchased = str;

    char temp2[200];
    int tempNum2;
    fio->read((char *) &(tempNum2),sizeof(int));
    fio->read(temp2, tempNum2);
    temp2[tempNum2]='\0';
    string str2(temp2);
    company = str2;

    char temp3[200];
    int tempNum3;
    fio->read((char *) &(tempNum3),sizeof(int));
    fio->read(temp3, tempNum3);
    temp3[tempNum3]='\0';
    string str3(temp3);
    price = str3;

    fio->read(( char *) &(remaining ), sizeof(int));
    fio->read(( char *) &(returnNum), sizeof(int));
}
