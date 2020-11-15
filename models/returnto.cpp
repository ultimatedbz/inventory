
/*
 * ReturnTo.cpp
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
#include "ReturnTo.h"

/**
 * Initializes and returns a newly allocated scientific data set.
 *
 * @param     cfg                 Data set configuration.
 */


ReturnTo::ReturnTo()
  :
    dayPurchased("\t"),
    company("\t"),
    returnNum(0),
    dayReturned("\t")
      {

      }

ReturnTo::~ReturnTo(){}


string ReturnTo::getCompany(){
    return company;
}

void ReturnTo::tui(string dateRet, int amount, string bc, string dateB){

  company = bc;
  dayPurchased = dateB;
  returnNum = amount;
  dayReturned = dateRet;

}


string ReturnTo::getDatePurchased(){
    return dayPurchased;
}

string ReturnTo::getDateReturned(){
    return dayReturned;
}

int ReturnTo::getReturn(){
    return returnNum;
}


string ReturnTo::formatReturn(string unit){
    char buffer [128];
    int temp = 0;
    for(int i = abs(returnNum); i > 0; i/=10){
        temp++;
    }
      sprintf(buffer,"%5s%7d%6s%s%13s%9s",
            dayReturned.c_str(),
            returnNum,
            unit.c_str(),
            padding(company).c_str(),
            company.c_str(),
            dayPurchased.c_str());
      return buffer;

}

void ReturnTo:: updateReturn(int amount, string c){
    returnNum = ((returnNum + amount) < 0)? 0 : returnNum + amount;
    dayReturned = c;
}

void ReturnTo::load(fstream* fio){
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


    char temp4[200];
    int tempNum4;
    fio->read((char *) &(tempNum4),sizeof(int));
    fio->read(temp4, tempNum4);
    temp4[tempNum4]='\0';
    string str4(temp4);
    dayReturned = str4;

}

string ReturnTo:: padding( string word){
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
