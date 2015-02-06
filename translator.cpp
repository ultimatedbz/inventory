#include "translator.h"

Translator::Translator(){
    /* Menus */

    translator.insert(pair<string, string>("開新檔案", "New File"));
    translator.insert(pair<string, string>("開啟舊檔", "Load File" ));
    translator.insert(pair<string, string>("儲存檔案", "Save" ));
    translator.insert(pair<string, string>("另存新檔", "Save As" ));
    translator.insert(pair<string, string>("加入新客戶", "Add Customer" ));
    translator.insert(pair<string, string>("加入新公司", "Add Company" ));
    translator.insert(pair<string, string>("加入新的菜名", "Add Vegetable" ));
    translator.insert(pair<string, string>("加入單位", "Add Unit" ));
    translator.insert(pair<string, string>("刪除客戶", "Delete Customer" ));
    translator.insert(pair<string, string>("刪除公司", "Delete Company" ));
    translator.insert(pair<string, string>("刪掉菜名", "Delete Vegetable" ));
    translator.insert(pair<string, string>("刪掉單位", "Delete Unit" ));
    translator.insert(pair<string, string>("印 Inventory", "Print Inventory" ));
    translator.insert(pair<string, string>("印 History", "Print History" ));

    /* User Interface */
    translator.insert(pair<string, string>("買", "Buy" ));
    translator.insert(pair<string, string>("賣", "Sell" ));
    translator.insert(pair<string, string>("菜名", "Vegetable List" ));
    translator.insert(pair<string, string>("庫存表", "Breakdown" ));
    translator.insert(pair<string, string>("倒", "Dump" ));
    translator.insert(pair<string, string>("退貨", "Return" ));
    translator.insert(pair<string, string>("歷史", "History" ));
    translator.insert(pair<string, string>("退貨單", "Returns" ));
    translator.insert(pair<string, string>("清掉退貨單", "Clear Return" ));
    translator.insert(pair<string, string>("清掉歷史", "Clear History" ));
    translator.insert(pair<string, string>("退給公司", "Return to Company" ));
    translator.insert(pair<string, string>("退給農場", "Customer Returned" ));


    /* Menu Popups */
    translator.insert(pair<string, string>("請輸入你要加的新公司名字?", "Company" ));
    translator.insert(pair<string, string>("請輸入你要加的新客戶名字?", "Customer" ));
    translator.insert(pair<string, string>("請輸入你要加的單位?", "Unit" ));
    translator.insert(pair<string, string>("請輸入你要加的菜名", "Vegetable" ));

    translator.insert(pair<string, string>("你要賣那天的菜？", "Date" ));
    translator.insert(pair<string, string>("你要刪掉哪一個公司？", "Company" ));
    translator.insert(pair<string, string>("你要刪掉哪一個客戶？", "Customer" ));
    translator.insert(pair<string, string>("你要刪掉哪一個單位？", "Unit" ));
    translator.insert(pair<string, string>("沒單位!", "No Units!" ));
    translator.insert(pair<string, string>("警告", "Warning" ));
    translator.insert(pair<string, string>("你要刪掉哪一個菜名？", "Vegetable" ));
    translator.insert(pair<string, string>("請輸入字體大小", "Font Size"));
    translator.insert(pair<string, string>("你要列印哪一天的日期?", "Date" ));

    /* On Buy Click */
    translator.insert(pair<string, string>("輸入進菜資料", "Buying Information" ));
    translator.insert(pair<string, string>("買了多少?", "Amount" ));
    translator.insert(pair<string, string>("哪家公司的?", "Company" ));
    translator.insert(pair<string, string>("幾號來的菜?", "Date" ));
    translator.insert(pair<string, string>("進價多少?", "Price" ));
    translator.insert(pair<string, string>("錯誤", "Error" ));

    /* On Sell Click */
    translator.insert(pair<string, string>("輸入賣菜資料", "Selling Information" ));
    translator.insert(pair<string, string>("賣了多少？", "Amount Sold" ));
    translator.insert(pair<string, string>("不夠賣!", "Not enough to sell!" ));
    translator.insert(pair<string, string>("賣给谁？", "Customer" ));
    translator.insert(pair<string, string>("那天賣出", "Date" ));
    translator.insert(pair<string, string>("賣多少錢?", "賣了多少？" ));
    translator.insert(pair<string, string>("你要賣那天的菜？", "In Stock" ));
    translator.insert(pair<string, string>("聯合賣", "Split" ));
    translator.insert(pair<string, string>("賣多少錢?", "Price" ));
    translator.insert(pair<string, string>("", "" ));




    translator.insert(pair<string, string>("", "" ));
    translator.insert(pair<string, string>("", "" ));
    translator.insert(pair<string, string>("", "" ));
    translator.insert(pair<string, string>("", "" ));
    translator.insert(pair<string, string>("", "" ));
    translator.insert(pair<string, string>("", "" ));
    translator.insert(pair<string, string>("", "" ));
    translator.insert(pair<string, string>("", "" ));
    translator.insert(pair<string, string>("", "" ));
    translator.insert(pair<string, string>("", "" ));
    translator.insert(pair<string, string>("", "" ));





    currentLanguage = CHINESE;
}

Translator::~Translator(){

}

string Translator::translate(string input){
    if(currentLanguage == CHINESE)
        return input;

    return translator.find(input) -> second;
}

void Translator::changeLanguage(int i){
    currentLanguage = i;
}
