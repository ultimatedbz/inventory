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
    splitNum(0),
    splitArray(NULL),
    changeNum(0),
    mTui(0)
    
      {
      
      } 

History::~History(){}


string History::getHistory(string unit){
  char buffer [200];
  int temp = 0;
  for(int i = abs(difference); i > 0; i/=10){
      temp++;
  }
  if(!price.compare("") || !price.compare("\t"))
      price = "--";
  string Price = "$" + price;
  string day = daySold.compare("\t") ? daySold : dayPurchased;
  if(splitNum){
        string final= "";
        for(int i = 0; i<splitNum; i++){
            sprintf(buffer,"%5s%7d%6s%13s%13s%15s%7s",
                  day.c_str(),
                  splitArray[i].getDifference(),
                  unit.c_str(),
                  customer.c_str(),
                  splitArray[i].getCompany().c_str(),
                  Price.c_str(),
                  splitArray[i].getDatePurchased().c_str());
            if(i == splitNum - 1)
                final= final + string(buffer);
            else
                final= final + string(buffer) + "\n";
        }
        return final;
  }
  else if (returned){
      string temp2 = "退給公司";
      sprintf(buffer,"%5s%7d%6s%13s%13s%15s%7s",
                    daySold.c_str(),
                    difference,
                    unit.c_str(),
                    customer.c_str(),
                    company.c_str(),
                    temp2.c_str(),
                    dayPurchased.c_str());
  }else if (dumped){
      string temp2 = "倒";
      sprintf(buffer,"%5s%7d%6s%13s%13s%15s%7s",
                    daySold.c_str(),
                    difference,
                    unit.c_str(),
                    " ",
                    company.c_str(),
                    temp2.c_str(),
                    dayPurchased.c_str());
  }else if (mTui){
      string temp2 = "退給農場";
      sprintf(buffer,"%5s%7d%6s%13s%13s%15s%7s",
                    daySold.c_str(),
                    difference,
                    unit.c_str(),
                    " ",
                    company.c_str(),
                    temp2.c_str(),
                    dayPurchased.c_str());
  }else if(customer.compare("\t"))
    sprintf(buffer,"%5s%7d%6s%13s%13s%15s%7s",
          day.c_str(),
          difference,
          unit.c_str(),
          customer.c_str(),
          company.c_str(),
          Price.c_str(),
          dayPurchased.c_str());
  else
    sprintf(buffer,"%5s%7d%6s%26s%15s",
           day.c_str(),
            difference,
            unit.c_str(),
            company.c_str(),
            Price.c_str());

  return buffer;
}

string History::printFormat(){

    char buffer [128];
    int temp = 0;
    for(int i = abs(difference); i > 0; i/=10){
        temp++;
    }
    if(!price.compare("") || !price.compare("\t"))
        price = "--";

    if(splitNum){
          string final= "";
          for(int i = 0; i < splitNum; i++){
              int temp2 = 0;
              for(int p = abs(splitArray[i].getDifference()); p > 0; p/=10){
                  temp2++;
              }
               sprintf(buffer,"%4d%12s%12s%5s",
                    splitArray[i].getDifference(),
                    customer.c_str(),
                    splitArray[i].getCompany().c_str(),
                    splitArray[i].getDatePurchased().c_str());
              if(i == splitNum - 1)
                  final= final + string(buffer);
              else
                  final= final + string(buffer) + "\n";
          }
          return final;
    }
    else if (returned){
        char d [200];
        sprintf(d,"+%d",difference);

        sprintf(buffer,"%4s%12s%12s%5s",
                d,
                customer.c_str(),
                company.c_str(),
                dayPurchased.c_str());
    }else if (dumped){
        sprintf(buffer,"%4d%12s%12s%5s",
                      difference,
                      customer.c_str(),
                      company.c_str(),
                      dayPurchased.c_str());
    }else if(mTui){
        sprintf(buffer,"%4d%12s%12s%5s",
                difference,
                customer.c_str(),
                company.c_str(),
                dayPurchased.c_str());
    }else if(customer.compare("\t")){
        if(!price.compare("") || !price.compare("\t"))
            price = "--";
        string Price = "$" + price;
        sprintf(buffer,"%4d%12s%12s%5s%4s",
            difference,
            customer.c_str(),
            company.c_str(),
            dayPurchased.c_str(),
            Price.c_str());
    }else{
        if(!price.compare("") || !price.compare("\t"))
            price = "--";
        string Price = "$" + price;
        char d [200];
        sprintf(d,"+%d",difference);
        sprintf(buffer,"%4s%24s%9s",
              d,
              company.c_str(),
              Price.c_str());
    }
    return buffer;
}

string History::getCustomer(){
    return customer;
}

string History::getCompany(){
    return company;
}

int History::getDifference(){
  return difference;
}

int History::getDumped(){
  return dumped;
}

int History::getReturned(){
  return returned;
}

int History::getTui(){
    return mTui;
}

string History::getPrice(){
    return price;
}

string History::getDatePurchased(){
    return dayPurchased;
}

string History::getDateSold(){
    return daySold;
}

string History::getDateToCompare(){
    return dateToCompare;
}

void History::buy(int amount, string bc, string date, string p){

  difference = amount;
  company = bc;
  dayPurchased = date;
  price = p;
  dateToCompare = date;
  //customer is set as the company
}

void History::sell(int amount, string b, string date, string p, string dp,string c , int change){

  difference = amount *(-1);
  customer = b;
  daySold = date;
  price = p;
  dateToCompare = date;
  dayPurchased = dp;
  company = c;
  changeNum = change;
}

void History:: splitSell(int spl, int input1,int  input2,int  input3,int  input4, string dateS,string cust,
               string pr, string d1, string d2, string d3, string d4, string c1, string c2, string c3, string c4,
                         int ch1, int ch2, int ch3, int ch4){
    difference = (-1)*(input1 + input2 + input3 + input4);
    daySold = dateS;
    dateToCompare = dateS;
    customer = cust;
    company = "meh";
    price = pr;
    int i = 0;
    int input[4];
    string day[4];
    string comp[4];
    int change[4];

    if(input1){
        input[0] = input1;
        day[0]= d1;
        comp[0] = c1;
        change[0]= ch1;
        i++;
    }
    if(spl > 1 && input2){
        input[i] = input2;
        day[i] = d2;
        comp[i] = c2;
        change[i] = ch2;
        i++;
    }
    if(spl > 2 && input3){
        input[i] = input3;
        day[i] = d3;
        comp[i] = c3;
        change[i] = ch3;
        i++;
    }
    if(spl > 3 && input4){
        input[i] = input4;
        day[i] = d4;
        comp[i] = c4;
        change[i] = ch4;
        i++;
    }
    splitNum = i;

    splitArray = new History[i];
    for(int j = 0; j < i; j++){
        History newHist;
        newHist.sell(input[j], cust, dateS, pr, day[j], comp[j],change[j]);
        splitArray[j] = newHist;
    }
}

void History::dump(int amount, string date, string bd, string comp, int change){
    difference = amount *(-1);
    daySold = date;
    dumped = 1;
    dayPurchased=bd;
    dateToCompare = date;
    company = comp;
    changeNum = change;
}

void History::returnn(string dR, int amount, string returner, string c, string dB){
    //day Purchased is day vegetables were bought
    dayPurchased = dB;
    //day Sold is day vegetables were returned
    daySold = dR;
    customer = returner;
    company =c;
    returned = 1;
    difference = amount;
    dateToCompare = dR;
}



void History::load(fstream* fio){
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
    fio->read(( char *) &(splitNum), sizeof(int));
    fio->read(( char *) &(changeNum), sizeof(int));
    fio->read(( char *) &(mTui), sizeof(int));
    if(splitNum)
        splitArray = new History[splitNum];
    for(int z = 0; z < splitNum; z++){
        splitArray[z].load(fio);
    }
}

int History::getSplitNum(){
    return splitNum;
}


int History:: getDifferenceS(int z){
   return splitArray[z].getDifference();
}
int History:: getSplitNumS(int z){
   return splitArray[z].getSplitNum();
}
int History:: getDumpedS(int z){
   return splitArray[z].getDumped();
}
int History:: getReturnedS(int z){
   return splitArray[z].getReturned();
}
string History::getCustomerS(int z){
    return splitArray[z].getCustomer();
}

string History::getDateSoldS(int z){
   return splitArray[z].getDateSold();
}

string History::getDateToCompareS(int z){
   return splitArray[z].getDateToCompare();
}

string History::getDatePurchasedS(int z){
   return splitArray[z].getDatePurchased();
}

string History::getPriceS(int z){
   return splitArray[z].getPrice();
}

string History::getCompanyS(int z){
   return splitArray[z].getCompany();
}

void History::deleteHistory(){
    if(splitArray)
        delete[] splitArray;
}

int History::getChangeNum(){
    return changeNum;
}

int History::getChangeNumS(int z){
    return splitArray[z].getChangeNum();
}

int History::getTuiS(int z){
    return splitArray[z].getTui();
}


void History::tui(int amount, string date, string bd, string comp, int change){
    difference = amount *(-1);
    daySold = date;
    mTui = 1;
    dayPurchased=bd;
    dateToCompare = date;
    company = comp;
    changeNum = change;
}


