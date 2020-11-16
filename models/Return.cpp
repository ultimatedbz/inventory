
/*
 * Return.cpp
 */

#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <qDebug>

using namespace std;
#include "Return.h"

/**
 * Initializes and returns a newly allocated scientific data set.
 *
 * @param     cfg                 Data set configuration.
 */


Return::Return()
  :
    dayPurchased("\t"),
    company("\t"),
    returnNum(0),
    returner("\t"),
    dayReturned("\t")
      {

      }

Return::~Return(){}


string Return::getCompany(){
    return company;
}

void Return::add(string dateRet, double amount, string ret, string bc, string dateB){

  company = bc;
  dayPurchased = dateB;
  returnNum = amount;
  returner = ret;
  dayReturned = dateRet;

}


string Return::getDatePurchased(){
    return dayPurchased;
}

string Return::getDateReturned(){
    return dayReturned;
}

string Return::getReturner(){
    return returner;
}


int Return::getReturn(){
    return returnNum;
}


string Return::formatReturn(string unit) {
    char buffer [128];
    int temp = 0;
    for(int i = abs(returnNum); i > 0; i/=10){
        temp++;
    }
      sprintf(buffer,"%8s%7d%6s%s%13s%s%13s%9s",
            dayReturned.c_str(),
            returnNum,
            unit.c_str(),
            padding(returner).c_str(),
            returner.c_str(),
            padding(company).c_str(),
            company.c_str(),
            dayPurchased.c_str());
      return buffer;

}

void Return:: updateReturn(double amount, string newDateReturned) {
    returnNum = ((returnNum + amount) < 0)? 0 : returnNum + amount;
    if (newDateReturned != "") {
        dayReturned = newDateReturned;
    }
}

void Return::load(fstream* fio){
    char temp[200];
    int tempNum;
    fio->read((char *) &(tempNum),sizeof(int));
    fio->read(temp, tempNum);
    temp[tempNum]='\0';
    string str(temp);
    dayPurchased= str;

    char temp2[200];
    int tempNum2;
    fio->read((char *) &(tempNum2),sizeof(int));
    fio->read(temp2, tempNum2);
    temp2[tempNum2]='\0';
    string str2(temp2);
    company = str2;

    fio->read(( char *) &(returnNum), sizeof(int));

    char temp3[200];
    int tempNum3;
    fio->read((char *) &(tempNum3),sizeof(int));
    fio->read(temp3, tempNum3);
    temp3[tempNum3]='\0';
    string str3(temp3);
    returner = str3;

    char temp4[200];
    int tempNum4;
    fio->read((char *) &(tempNum4),sizeof(int));
    fio->read(temp4, tempNum4);
    temp4[tempNum4]='\0';
    string str4(temp4);
    dayReturned = str4;

}

string Return:: padding( string word){
    string product = "";
    int num = 0;
    for(int i = 0; i < word.size(); i ++){
        char x = word[i];
        if( x < 0 && (x & 0x40)){
            product = product + " ";
            num++;
            if(num == 2){
                num = 0;
                product = product + " ";
            }
        }
    }
    return product;
}
