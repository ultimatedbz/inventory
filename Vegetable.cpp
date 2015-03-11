/*
 * Vegetable.cpp
 */
#include <QDebug>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <QDebug>
using namespace std;
#include "returnto.h"
#include "Return.h"
#include "History.h"
#include "Remaining.h"
#include "Vegetable.h"


/**
 * Initializes and returns a newly allocated scientific data set.
 *
 * @param     cfg                 Data set configuration.
 */

Vegetable::Vegetable()  :
    vegetableName(""),
    historyArray(NULL),
    totalVeges(0),
    historyNum(0),
    unit(""),
    remainingArray(NULL),
    remainingNum(0),
    returnArray(NULL),
    returnNum(0),
    memo(""),
    tuiNum(0),
    tuiArray(NULL){}

Vegetable::Vegetable(string name, string u)
  : vegetableName(name),
    historyArray(NULL),
    totalVeges(0),
    historyNum(0),
    unit(u),
    remainingArray(NULL),
    remainingNum(0),
    returnArray(NULL),
    returnNum(0),
    memo(""),
    tuiNum(0),
    tuiArray(NULL)
      {
      } 
Vegetable::~Vegetable(){
}
const std::string Vegetable:: getVegetablename() const{
  return vegetableName;
}

string Vegetable::viewHistory(int i ){
    return historyArray[i].getHistory(unit);

}
int Vegetable::getTuiNum(){
    return tuiNum;
}

int Vegetable::getHistoryNum(){
  return historyNum;
}
int Vegetable::getRemainingNum(){
  return remainingNum;
}

int Vegetable::getReturnNum(){
    return returnNum;
}
int Vegetable::buyVege(int amount, string bc, string date, string price){

  if(amount < 1)
      return 0;
  if( historyArray==NULL){
    historyArray = new History[100];
  }
  if( !remainingArray)
      remainingArray = new Remaining[100];


  if( historyNum == 100){
      for(int i = 0; i < historyNum -1; i++){
          historyArray[i] = historyArray[i+1];
      }
      historyNum--;
  }

  History newHist;
  newHist.buy(amount, bc, date, price);
  historyArray[historyNum] = newHist;
  historyNum++;
  int exist = remainExist(bc, date);
  if(exist > -1)
      updateRemaining(exist, amount);
  else{
    Remaining newRem;
    newRem.buy(amount, bc, date, price);
    remainingArray[remainingNum] = newRem;
    remainingNum++;
  }
  totalVeges += newHist.getDifference();
  qsort(remainingArray, remainingNum, sizeof(Remaining), compareR);
  return 1;
}

int Vegetable::remainExist(string company, string date){
    for(int i = 0; i < remainingNum; i++){
        if(company == remainingArray[i].getCompany() &&
                date == remainingArray[i].getDate())
            return i;
    }
    return -1;
}
int Vegetable::compareH(const void * a, const void * b){
    History* A = (History *)a;
    History* B= (History *)b;
    const char* date1 = A->getDateToCompare().c_str();
    const char* date2 = B->getDateToCompare().c_str();
    int day1,month1;
    int day2,month2;

    sscanf(date1, "%d/%d", &month1, &day1);
    sscanf(date2, "%d/%d", &month2, &day2);

    if(month1 - month2 > 5){
        month2 += 12;
    }else if (month2 - month1 > 5){
        month1 += 12;
    }

    if (month1 == month2 ){
        if(day1 > day2)
            return 1;
        else if(day1 < day2)
            return -1;
        else
            return 0;
    } else if (month1 > month2){
        return 1;
    } else
        return -1;

    return 0;
}

int Vegetable::compareR(const void * a, const void *b){
    Remaining* A = (Remaining*)a;
    Remaining* B= (Remaining *)b;

    const int result = strcmp(A->getCompany().c_str(),B->getCompany().c_str());
    if(result)
       return result;

    const char* date1 = ((Remaining *)a)->getDate().c_str();
    const char* date2 = ((Remaining *)b)->getDate().c_str();
    int day1,month1;
    int day2,month2;

    sscanf(date1, "%d/%d", &month1, &day1);
    sscanf(date2, "%d/%d", &month2, &day2);

    if(month1 - month2 > 5){
        month2 += 12;
    }else if (month2 - month1 > 5){
        month1 += 12;
    }

    if (month1 == month2 ){
        if(day1 > day2)
            return 1;
        else if(day1 < day2)
            return -1;
        else
            return 0;
    } else if (month1 > month2){
        return 1;
    } else
        return -1;

    return 0;
}
int Vegetable::compareRet(const void * a, const void *b){
    Return* A = (Return*)a;
    Return* B = (Return *)b;

    const int result = strcmp(A->getCompany().c_str(),B->getCompany().c_str());
    if(result)
       return result;

    const char* date1 = ((Return *)a)->getDatePurchased().c_str();
    const char* date2 = ((Return *)b)->getDatePurchased().c_str();
    int day1,month1;
    int day2,month2;

    sscanf(date1, "%d/%d", &month1, &day1);
    sscanf(date2, "%d/%d", &month2, &day2);

    if(month1 - month2 > 5){
        month2 += 12;
    }else if (month2 - month1 > 5){
        month1 += 12;
    }

    if (month1 == month2 ){
        if(day1 > day2)
            return 1;
        else if(day1 < day2)
            return -1;
        else
            return 0;
    } else if (month1 > month2){
        return 1;
    } else
        return -1;

    return 0;
}

void Vegetable::setVegetableName(string name){
    vegetableName = name;
}

int Vegetable::sellVege(int amount, string customer, string date, string price, int selection){

    int returnChange = 0;
    string dp = remainingArray[selection].getDate();
    string bc = remainingArray[selection].getCompany();

    if( historyArray==NULL){
      historyArray = new History[100];
    }

    if( historyNum == 100){
        for(int i = 0; i < historyNum -1; i++){
            historyArray[i] = historyArray[i+1];
        }
        historyNum--;
    }

  if(getRemaining(selection) - amount < 0)
      return 0;

  /* Update the Breakdown */
  remainingArray[selection].updateRemaining((-1) * amount);

  if (remainingArray[selection].getReturn()){
      /* Change returns */
      returnChange = min(amount,remainingArray[selection].getReturn());

      while(returnChange){
          int temp = returnExistCompany( remainingArray[selection].getCompany(),
                                  remainingArray[selection].getDate());
          int portion = returnArray[temp].getReturn();

          int deduct = min(portion, returnChange);
          returnArray[temp].updateReturn((-1)*deduct,customer);
          if(temp >-1 && returnArray[temp].getReturn() == 0){
                for(int i=temp; i < returnNum - 1; i++){
                    returnArray[temp]= returnArray[temp+1];
                }
                returnNum--;
          }
          returnChange -= deduct;

      }

  }

    //needs to be last or else selection will get messed up
    if(remainingArray[selection].getRemaining() == 0){
        if(remainingNum !=1){
            for(int i=selection; i < remainingNum -1; i++){
                remainingArray[i]= remainingArray[1+i];
            }
        }
     remainingNum--;
  }
  History newHist;
  newHist.sell(amount, customer, date, price, dp,bc, returnChange);
  historyArray[historyNum] = newHist;
  historyNum++;
  totalVeges += newHist.getDifference();
  return 1;
}

int Vegetable::getTotalVeges(){
    return totalVeges;
}

string Vegetable::getUnit(){
    return unit;
}

string Vegetable::formatTui(int i){
    return tuiArray[i].formatReturn(unit);
}

string Vegetable::formatRemaining(int i){
    return remainingArray[i].formatRemaining();
}

string Vegetable::formatRemaining2(int i){
    return remainingArray[i].formatRemaining2(unit);
}

string Vegetable::formatRemaining3(int i){
    return remainingArray[i].formatRemaining3();
}

string Vegetable::formatReturn(int i){
    return returnArray[i].formatReturn(unit);
}

int Vegetable::getRemaining(int i){
    return remainingArray[i].getRemaining();
}


void Vegetable::updateRemaining(int i, int amount){
    remainingArray[i].updateRemaining(amount);
}

void Vegetable::returnThis(string dateReturned, int amount, string returner, string  company, string dateBought){

    if( !remainingArray)
        remainingArray = new Remaining[100];

    if( historyArray==NULL){
      historyArray = new History[100];
    }

    if( historyNum == 100){
        for(int i = 0; i < historyNum -1; i++){
            historyArray[i] = historyArray[i+1];
        }
        historyNum--;
    }


    if( !returnArray)
        returnArray = new Return[100];

    //check if remaining already exists

    int exist = remainExist(company, dateBought);
    if(exist > -1)
        updateReturnInRemaining(exist, amount);
    else{
        Remaining newRem;
        newRem.returned(amount, company, dateBought);
        remainingArray[remainingNum] = newRem;
        remainingNum++;
    }


    History newHist;
    newHist.returnn(dateReturned, amount, returner, company,dateBought);
    historyArray[historyNum] = newHist;
    historyNum++;

    //check if return exists yet

    exist = returnExist(returner, dateBought);
    if(exist > -1)
        returnArray[exist]. updateReturn(amount, dateReturned);
    else{
        Return newRet;
        newRet.add(dateReturned, amount, returner, company,dateBought);
        returnArray[returnNum] = newRet;
        returnNum++;

    }
  totalVeges += newHist. getDifference();
    qsort(returnArray, returnNum, sizeof(Return), compareRet);
    qsort(remainingArray, remainingNum, sizeof(Remaining), compareR);
}

void Vegetable::updateReturnInRemaining(int i, int amount){
    remainingArray[i].updateRemainingWithRet(amount);
}

int Vegetable::returnExist(string returner, string date){
    for(int i = 0; i < returnNum; i++){
        if(returner == returnArray[i].getReturner() &&
                date == returnArray[i].getDatePurchased())
            return i;
    }
    return -1;
}
int Vegetable::returnExistCompany(string company, string date){
    for(int i = 0; i < returnNum; i++){
        if(company == returnArray[i].getCompany() &&
                date == returnArray[i].getDatePurchased())
            return i;
    }
    return -1;
}

int Vegetable::dumpVege(int amount, string date, int selection){
    string dp = remainingArray[selection].getDate();
    string bc = remainingArray[selection].getCompany();
    int returnChange = 0;
    if( historyArray==NULL){
      historyArray = new History[100];
    }

    if( historyNum == 100){
        for(int i = 0; i < historyNum -1; i++){
            historyArray[i] = historyArray[i+1];
        }
        historyNum--;
    }

  if(getRemaining(selection) - amount < 0)
      return 0;


  if (remainingArray[selection].getReturn()){

      returnChange = min(amount,remainingArray[selection].getReturn());
      remainingArray[selection].updateRemainingWithRet((-1) * amount);

  }
  else
      remainingArray[selection].updateRemaining((-1) * amount);

  //change individual returns
  int left = returnChange;

  while(left){

      int temp = returnExistCompany(remainingArray[selection].getCompany(),
                              remainingArray[selection].getDate());
      int portion = returnArray[temp].getReturn();

      int deduct = min(portion, left);
      returnArray[temp].updateReturn((-1)*deduct,remainingArray[selection].getCompany());

      if(temp >-1 && returnArray[temp].getReturn() == 0){
            for(int i=temp; i < returnNum; i++){
                returnArray[temp]= returnArray[temp+1];
            }
            returnNum--;
      }
      left -= deduct;

  }


//needs to be last or else selection will get messed up
  if(remainingArray[selection].getRemaining() == 0){
      for(int i=selection; i < remainingNum; i++){
          remainingArray[i]= remainingArray[i+1];
      }
      remainingNum--;
  }

  History newHist;
  newHist.dump(amount, date,dp,
               bc, returnChange);
  historyArray[historyNum] = newHist;
  historyNum++;

  totalVeges += newHist.getDifference();
  return 1;
}


History* Vegetable::getHistoryObject(int index){
    return &historyArray[index];
}

Return* Vegetable::getReturnObject(int index){
    return &returnArray[index];
}

string Vegetable::getMemo(){
    return memo;
}

void Vegetable::updateMemo(string m){

    memo = m;
}

void Vegetable::remove(){
    if(historyArray){
        for(int z = 0; z < historyNum; z++){
            historyArray[z].deleteHistory();
        }

        delete[] historyArray;
    }
    historyArray = NULL;
    if(remainingArray)
        delete[] remainingArray;
    remainingArray = NULL;
    if(returnArray)
        delete[] returnArray;
    returnArray = NULL;
    if(tuiArray)
        delete[] tuiArray;
    tuiArray = NULL;
}

int Vegetable::clearHist(int num){
    if (historyNum < num)
        return 0;
    else if(historyNum == num){
        if(historyArray)
            delete[] historyArray;
        historyArray = NULL;
        historyNum = 0;
        return 1;
    }else{
        for(int i = 0; i < historyNum - num; i++){
            historyArray[i] = historyArray[num+i];
        }
        historyNum = historyNum - num;
        return 1;
    }
}

void Vegetable::clearTui(){
    if(tuiNum<0)
        return;
    else{
        delete[]tuiArray;
        tuiArray = NULL;
        tuiNum = 0;
    }
}

void Vegetable::load(fstream* fio){
    char temp[200];
    int tempNum;
    fio->read((char *) &(tempNum),sizeof(int));
    fio->read(temp, tempNum);
    temp[tempNum]='\0';
    string str(temp);
    vegetableName = str;
    fio->read(( char *) &(totalVeges), sizeof(int));
    char temp2[200];
    int tempNum2;
    fio->read((char *) &(tempNum2),sizeof(int));
    fio->read(temp2, tempNum2);
    temp2[tempNum2]='\0';
    string str2(temp2);
    unit = str2;

    fio->read(( char *) &(remainingNum), sizeof(int));
    fio->read(( char *) &(returnNum), sizeof(int));
    fio->read(( char *) &(historyNum), sizeof(int));
    fio->read(( char *) &(tuiNum), sizeof(int));

    char temp3[BUFSIZ];
    int tempNum3;
    fio->read((char *) &(tempNum3),sizeof(int));

    if(tempNum3){

        fio->read(temp3, tempNum3);
        temp3[tempNum3]='\0';
        string str3(temp3);
        memo = str3;
    }

    if(tuiNum)
        tuiArray = new ReturnTo[100];
    for(int i = 0; i<tuiNum; i++){
        ReturnTo temp;
        temp.load(fio);
        tuiArray[i]=temp;
    }
    if(remainingNum)
        remainingArray = new Remaining[100];
    for(int i = 0; i<remainingNum; i++){
        Remaining temp;
        temp.load(fio);
        remainingArray[i]=temp;
    }

    if(returnNum)
        returnArray = new Return[100];
    for(int i = 0; i<returnNum; i++){
        Return temp;
        temp.load(fio);
        returnArray[i]=temp;
    }
    if(historyNum)
        historyArray = new History[100];
    for(int i = 0; i<historyNum; i++){
        History temp;
        temp.load(fio);
        historyArray[i]=temp;
    }

}

Remaining* Vegetable::getRemainingObject(int index){
    return &remainingArray[index];
}

ReturnTo* Vegetable::getTuiObject(int index){
    return &tuiArray[index];
}

void Vegetable::deleteHistory(int index){
    historyArray[index].deleteHistory();
    for(int i = index; i < historyNum - 1; i++){
        historyArray[i] = historyArray[1 + i];
    }
    historyNum--;
}

int Vegetable::restock(int amount, string dP, string bc, int retNum){
    if(totalVeges + (-1)*amount < 0){
        return 0;
    }
    if( !remainingArray)
        remainingArray = new Remaining[100];

    if( !returnArray)
        returnArray = new Return[100];

    //checks if already in inventory breakdown
    int exist = remainExist(bc, dP);
    if(exist > -1)
        updateRemaining(exist,(-1)* amount);
    else{
      Remaining newRem;
      newRem.buy((-1)*amount - retNum, bc, dP, "\t");
      if(retNum)
          newRem.returned(retNum, bc, dP);
      remainingArray[remainingNum] = newRem;
      remainingNum++;
    }
    //check if return exists yet

    if(retNum){
        exist = returnExistCompany(bc, dP);
        if(exist > -1)
            returnArray[exist]. updateReturn(retNum, "    ");
        else{
            Return newRet;
            newRet.add("    ", retNum, "       ", bc, dP);
            returnArray[returnNum] = newRet;
            returnNum++;
        }
    }
    totalVeges += (-1)*amount;
    qsort(remainingArray, remainingNum, sizeof(Remaining), compareR);
return 1;
}

int Vegetable::undoRetOrBuy(int amount, string dP,string company,string dR,string customer){
    if(totalVeges + (-1)*amount < 0){
        return 0;
    }

    int selection = remainExist(company,dP);
    if(company.compare("\t")){//return
      if (remainingArray[selection].getReturn()){
          remainingArray[selection].updateRemainingWithRet((-1) * amount);
          returnArray[returnExistCompany( remainingArray[selection].getCompany(),
                remainingArray[selection].getDate())].updateReturn((-1)*amount,dR);
      }
      else
          remainingArray[selection].updateRemaining((-1) * amount);

      int temp = returnExist( customer,
                              remainingArray[selection].getDate());
      if(temp >-1 && returnArray[temp].getReturn() == 0){
          for(int i=temp; i < returnNum; i++){
              returnArray[temp]= returnArray[temp+1];
          }
          returnNum--;
      }


        //needs to be last or else selection will get messed up
        if(remainingArray[selection].getRemaining() == 0){
            if(remainingNum !=1){
                for(int i=selection; i < remainingNum; i++){
                remainingArray[i]= remainingArray[1+i];
                }
         }
         remainingNum--;
      }
    }else{//buy
          remainingArray[selection].updateRemaining((-1) * amount);

        //needs to be last or else selection will get messed up
        if(remainingArray[selection].getRemaining() == 0){
            if(remainingNum !=1){
                for(int i=selection; i < remainingNum; i++){
                remainingArray[i]= remainingArray[1+i];
                }
         }
         remainingNum--;
      }
    }
    totalVeges-=amount;
    return 1;
}


int Vegetable::returnTo(int amount, string date, int selection){
    string dp = remainingArray[selection].getDate();
    string bc = remainingArray[selection].getCompany();
    int returnChange = 0;
    if( historyArray==NULL){
      historyArray = new History[100];
    }

    if(tuiArray == NULL)
        tuiArray = new ReturnTo[100];

    if( historyNum == 100){
        for(int i = 0; i < historyNum -1; i++){
            historyArray[i] = historyArray[i+1];
        }
        historyNum--;
    }

  if(getRemaining(selection) - amount < 0)
      return 0;


  if (remainingArray[selection].getReturn()){

      returnChange = min(amount,remainingArray[selection].getReturn());
      remainingArray[selection].updateRemainingWithRet((-1) * amount);

  }
  else
      remainingArray[selection].updateRemaining((-1) * amount);

  //change individual returns
  int left = returnChange;

  while(left){

      int temp = returnExistCompany(remainingArray[selection].getCompany(),
                              remainingArray[selection].getDate());
      int portion = returnArray[temp].getReturn();

      int deduct = min(portion, left);
      returnArray[temp].updateReturn((-1)*deduct,remainingArray[selection].getCompany());

      if(temp >-1 && returnArray[temp].getReturn() == 0){
            for(int i=temp; i < returnNum; i++){
                returnArray[temp]= returnArray[temp+1];
            }
            returnNum--;
      }
      left -= deduct;

  }


//needs to be last or else selection will get messed up
  if(remainingArray[selection].getRemaining() == 0){
      for(int i=selection; i < remainingNum; i++){
          remainingArray[i]= remainingArray[i+1];
      }
      remainingNum--;
  }

  History newHist;
  newHist.tui(amount, date,dp,
               bc, returnChange);
  historyArray[historyNum] = newHist;
  historyNum++;
  totalVeges += newHist.getDifference();

  for(int i=0; i<tuiNum; i++){
      if(tuiArray[i].getCompany()==remainingArray[i].getCompany() &&
                        tuiArray[i].getDatePurchased()==remainingArray[selection].getDate()){
          tuiArray[i].updateReturn(amount, date);
          return 1;
      }
  }
   ReturnTo newTui;
   newTui.tui(date,amount,bc,dp);
   tuiArray[tuiNum] = newTui;
   tuiNum++;

  return 1;
}

void Vegetable::reTui(string dateS, int amount, string dP, string company){

    for(int i=0; i<tuiNum; i++){
        if(tuiArray[i].getCompany()==company &&  tuiArray[i].getDatePurchased()==dP){
            tuiArray[i].updateReturn(amount, dateS);

            if(tuiArray[i].getReturn() ==0){
                for(int j = i ; j < tuiNum-1; j++){
                    tuiArray[j] = tuiArray[j+1];
                }
                tuiNum--;
            }
        }
    }
}









