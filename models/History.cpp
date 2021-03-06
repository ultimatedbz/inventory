/*
 * History.cpp
 */

#include <sstream>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <QDebug>
#include <string>
#include <cstdlib>

using namespace std;
#include "History.h"

/**
 * Initializes and returns a newly allocated scientific data set.
 *
 * @param     cfg                 Data set configuration.
 */


History::History()
  :
    daySold("\t"),
    dayPurchased("\t"),
    difference(0),
    customer("\t"),
    price("\t"),
    company("\t"),
    returned(0),
    dumped(0),
    dateToCompare("\t"),
    changeNum(0),
    mTui(0),
    type(""),
    utils(new Utils())
    
      {
      
      } 

History::~History(){}


string History::getHistory(string unit){
  char buffer [200];
  int temp = 0;
  for (int i = abs(difference); i > 0; i/=10) {
      temp++;
  }

  // 0 means they're equal lol. If the price is either equal to empty or tab, then use "--"
  if(!price.compare("") || !price.compare("\t"))
      price = "--";
  string Price = "$" + price;
  string day = daySold.compare("\t") ? daySold : dayPurchased;
  if (returned) {
      string temp2 = "退給公司";
      sprintf(buffer,"%8s%7d%6s%s%13s%s%13s%s%15s%10s",
                    daySold.c_str(),
                    difference,
                    unit.c_str(),
                    padding(customer).c_str(),
                    customer.c_str(),
                    padding(company).c_str(),
                    company.c_str(),
                    padding(temp2).c_str(),
                    temp2.c_str(),
                    dayPurchased.c_str());
  }else if (dumped) {
      string temp2 = "倒";
      sprintf(buffer,"%8s%7d%6s%13s%s%13s%s%15s%10s",
                    daySold.c_str(),
                    difference,
                    unit.c_str(),
                    " ",
                    padding(company).c_str(),
                    company.c_str(),
                    padding(temp2).c_str(),
                    temp2.c_str(),
                    dayPurchased.c_str());
  } else if (mTui) {
      string temp2 = "退給農場";
      sprintf(buffer,"%8s%7d%6s%13s%s%13s%s%15s%10s",
                    daySold.c_str(),
                    difference,
                    unit.c_str(),
                    " ",
                    padding(company).c_str(),
                    company.c_str(),
                    padding(temp2).c_str(),
                    temp2.c_str(),
                    dayPurchased.c_str());
  } else if(customer.compare("\t")) {
    // This is for sell (compare == 1 means not equal)

      // but why for buy, is daypurchased not shown at the end?
    sprintf(buffer,"%8s%7d%6s%s%13s%s%13s%15s%10s",
          day.c_str(),
          difference,
          unit.c_str(),
          padding(customer).c_str(),
          customer.c_str(),
          padding(company).c_str(),
          company.c_str(),
          Price.c_str(),
          dayPurchased.c_str());
  } else {
      // This is for buy. Day is day purchased, and we don't have a second date.
    sprintf(buffer,"%8s%7d%6s%s%26s%15s",
           day.c_str(),
            difference,
            unit.c_str(),
            padding(company).c_str(),
            company.c_str(),
            Price.c_str());
  }

  return buffer;
}

string History::printFormat() {
    char buffer [128];
    int temp = 0;
    for(int i = abs(difference); i > 0; i/=10){
        temp++;
    }

    // Checks if there is a price
    if (!price.compare("") || !price.compare("\t"))
        price = "--";

   if (returned) {
        char d [200];
        sprintf(d,"+%d",difference);
        sprintf(buffer,"%4s%s%15s%s%15s%6s",
                d,
                padding(customer).c_str(),
                customer.c_str(),
                padding(company).c_str(),
                company.c_str(),
               removeYear(dayPurchased).c_str());
    } else if (dumped) {
        sprintf(buffer,"%4d%s%15s%s%15s%6s",
                      difference,
                      padding(customer).c_str(),
                      customer.c_str(),
                      padding(company).c_str(),
                      company.c_str(),
                      removeYear(dayPurchased).c_str());
    } else if (mTui) {
        sprintf(buffer,"%4d%s%15s%s%15s%6s",
                difference,
                padding(customer).c_str(),
                customer.c_str(),
                padding(company).c_str(),
                company.c_str(),
                removeYear(dayPurchased).c_str());
    // Buy and Sell
    } else if (customer.compare("\t")) {
        if(!price.compare("") || !price.compare("\t"))
            price = "--";
        string Price = utils->hasEnding(price, ".00") ? "$" + price.substr(0, price.size() - 3) :  "$" + price;
        sprintf(buffer,"%4d%s%15s%s%15s%6s%4s",
            difference,
            padding(customer).c_str(),
            customer.c_str(),
            padding(company).c_str(),
            company.c_str(),
            removeYear(dayPurchased).c_str(),
            Price.c_str());
    } else {
        if(!price.compare("") || !price.compare("\t"))
            price = "--";
        string Price = utils->hasEnding(price, ".00") ? "$" + price.substr(0, price.size() - 3) :  "$" + price;
        char d [200];
        sprintf(d,"+%d",difference);
        sprintf(buffer,"%4s%24s%9s",
              d,
              company.c_str(),
              Price.c_str());
    }

    return buffer;
}

string History::getCustomer() {
    return customer;
}

string History::getCompany() {
    return company;
}

// The change in units
int History::getDifference() {
  return difference;
}

int History::getDumped() {
  return dumped;
}

int History::getReturned() {
  return returned;
}

int History::getTui() {
    return mTui;
}

string History::getPrice() {
    return price;
}

string History::getDatePurchased() {
    return dayPurchased;
}

string History::getDateSold() {
    return daySold;
}

string History::getDateToCompare() {
    return dateToCompare;
}

void History::buy(int amount, string bc, string date, string p) {
  difference = amount;
  company = bc;
  dayPurchased = date;
  dateToCompare = date;
  type = "Buy";

  if (p.compare("") && p.compare("\t")) {
      price = utils -> twoDecimals(p);
  } else {
      price = p;
  }
}

void History::sell(int amount, string b, string date, string p, string dp,string c , int change) {
  difference = amount *(-1);
  customer = b;
  daySold = date;
  dateToCompare = date;
  dayPurchased = dp;
  company = c;
  changeNum = change;
  type = "Sell";

  if (p.compare("") && p.compare("\t")) {
      price = utils -> twoDecimals(p);
  } else {
      price = p;
  }
}

void History::dump(int amount, string date, string bd, string comp, int change) {
    difference = amount *(-1);
    daySold = date;
    dumped = 1;
    dayPurchased = bd;
    dateToCompare = date;
    company = comp;
    changeNum = change;
    type = "Dump";
}

// Customer returned to Sam
void History::returnn(string dR, int amount, string returner, string c, string dB) {
    //day Purchased is day vegetables were bought
    dayPurchased = dB;
    //day Sold is day vegetables were returned
    daySold = dR;
    customer = returner;
    company =c;
    returned = 1;
    difference = amount;
    dateToCompare = dR;
    type = "Return";
}

// Return to farm
// - paramenter date: The day that the "tui" happened
void History::tui(int amount, string dateTui, string datePurchased, string comp, int change) {
    difference = amount *(-1);
    daySold = dateTui;
    mTui = 1;
    dayPurchased = datePurchased;
    dateToCompare = dateTui;
    company = comp;
    changeNum = change;
    type = "Tui";
}


void History::load(fstream* fio) {
    char temp[200];
    int tempNum;
    fio->read((char *) &(tempNum),sizeof(int));
    fio->read(temp, tempNum);
    temp[tempNum]='\0';
    string str(temp);
    daySold = str;


    char temp2[200];
    int tempNum2;
    fio->read((char *) &(tempNum2),sizeof(int));
    fio->read(temp2, tempNum2);
    temp2[tempNum2]='\0';
    string str2(temp2);
    dayPurchased = str2;

    fio->read(( char *) &(difference ), sizeof(int));

    char temp3[200];
    int tempNum3;
    fio->read((char *) &(tempNum3),sizeof(int));
    fio->read(temp3, tempNum3);
    temp3[tempNum3]='\0';
    string str3(temp3);
    customer = str3;


    char temp4[200];
    int tempNum4;
    fio->read((char *) &(tempNum4),sizeof(int));
    fio->read(temp4, tempNum4);
    temp4[tempNum4]='\0';
    string str4(temp4);
    price = str4;


    char temp5[200];
    int tempNum5;
    fio->read((char *) &(tempNum5),sizeof(int));
    fio->read(temp5, tempNum5);
    temp5[tempNum5]='\0';
    string str5(temp5);
    company = str5;
    char temp6[200];
    int tempNum6;
    fio->read((char *) &(tempNum6),sizeof(int));
    fio->read(temp6, tempNum6);
    temp6[tempNum6]='\0';
    string str6(temp6);
    dateToCompare = str6;
    fio->read(( char *) &(returned ), sizeof(int));
    fio->read(( char *) &(dumped), sizeof(int));
    fio->read(( char *) &(changeNum), sizeof(int));
    fio->read(( char *) &(mTui), sizeof(int));

    char temp7[200];
    int tempNum7;
    fio->read((char *) &(tempNum7),sizeof(int));
    fio->read(temp7, tempNum7);
    temp7[tempNum7]='\0';
    string str7(temp7);
    type = str7;
}

void History::deleteHistory() {
}

int History::getChangeNum() {
    return changeNum;
}

string History:: getType(){
  return type;
}

string History:: padding( string word) {
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

string History:: removeYear(string word) {
    int slashCount = 0;
    string ret = "";
    for(int i = 0; i < word.size(); i ++) {
        if (word[i] == '/') {
          slashCount++;
          if (slashCount == 2) {
              break;
          }
        }

        ret += word[i];
    }

    return ret;
}
