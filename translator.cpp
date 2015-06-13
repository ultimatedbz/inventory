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
    translator.insert(pair<string, string>("印 History", "Print Today's Transactions" ));
    translator.insert(pair<string, string>("印記錄表", "Print History" ));


    /* User Interface */
    translator.insert(pair<string, string>("買", "Buy" ));
    translator.insert(pair<string, string>("賣", "Sell" ));
    translator.insert(pair<string, string>("菜名", "Vegetable List" ));
    translator.insert(pair<string, string>("菜名子", "Vegetable" ));
    translator.insert(pair<string, string>("庫存表", "Breakdown" ));
    translator.insert(pair<string, string>("倒", "Dump" ));
    translator.insert(pair<string, string>("退貨", "Return" ));
    translator.insert(pair<string, string>("歷史", "History" ));
    translator.insert(pair<string, string>("退貨單", "Returns" ));
    translator.insert(pair<string, string>("清掉退貨單", "Clear Return" ));
    translator.insert(pair<string, string>("清掉歷史", "Clear History" ));
    translator.insert(pair<string, string>("退給公司", "Return to Company" ));
    translator.insert(pair<string, string>("退給農場", "Customer Returned" ));
    translator.insert(pair<string, string>("簡寫", "Abbreviation" ));

    /* Menu Popups */
    translator.insert(pair<string, string>("請輸入你要加的新公司名字", "Company" ));
    translator.insert(pair<string, string>("請輸入你要加的新客戶名字", "Customer" ));
    translator.insert(pair<string, string>("請輸入你要加的單位", "Unit" ));
    translator.insert(pair<string, string>("請輸入你要加的菜名", "Vegetable" ));

    translator.insert(pair<string, string>("你要刪掉哪一個公司？", "Company" ));
    translator.insert(pair<string, string>("你要刪掉哪一個客戶？", "Customer" ));
    translator.insert(pair<string, string>("你要刪掉哪一個單位？", "Unit" ));
    translator.insert(pair<string, string>("你要刪掉哪一個菜名？", "Vegetable" ));
    translator.insert(pair<string, string>("沒單位!", "No Units!" ));
    translator.insert(pair<string, string>("警告", "Warning" ));
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
    translator.insert(pair<string, string>("不夠賣!", "Not Enough to Sell!" ));
    translator.insert(pair<string, string>("賣给谁？", "Customer" ));
    translator.insert(pair<string, string>("那天賣出", "Date" ));
    translator.insert(pair<string, string>("你要賣那天的菜？", "In Stock" ));
    translator.insert(pair<string, string>("聯合賣", "Split" ));
    translator.insert(pair<string, string>("賣多少錢?", "Price" ));
    translator.insert(pair<string, string>("菜不足,繼續買另外一天的菜?",
                   "Not Enough Vegetables, Sell Remaining on Another Day?" ));
    translator.insert(pair<string, string>("聯合數字錯誤!", "Not Valid" ));
    translator.insert(pair<string, string>("不夠賣!", "Not Enough to Sell" ));

    /* Query Splits */
    translator.insert(pair<string, string>("你要幾家公司聯合? (最多4家)",
                                           "How Many Splits? (Max 4)" ));
    translator.insert(pair<string, string>("聯合賣", "Split Sell" ));
    translator.insert(pair<string, string>("輸入的數量不一樣!", "Numbers Do Not Sum Up to Total!" ));
    translator.insert(pair<string, string>("輸入的數量太多!", "Not Enough to Sell!" ));

    /* On Return Click */
    translator.insert(pair<string, string>("退還給公司的菜", "Returned to Us" ));
    translator.insert(pair<string, string>("退幾箱？", "Amount" ));
    translator.insert(pair<string, string>("誰退回來的？", "Customer" ));
    translator.insert(pair<string, string>("这是哪天退回來的", "Date" ));
    translator.insert(pair<string, string>("这是哪家公司的菜？", "Company" ));
    translator.insert(pair<string, string>("这是哪天買的菜？", "Original Date of Vegetable" ));
    translator.insert(pair<string, string>("退還給農場的菜", "Return to Company" ));
    translator.insert(pair<string, string>("輸入退菜資料", "Return Information" ));
    translator.insert(pair<string, string>("你要退那天的菜？", "Return Which Item?" ));
    translator.insert(pair<string, string>("不夠菜退!", "Not Enough to Return!" ));


    translator.insert(pair<string, string>("你要儲存嗎?", "Do You Want To Save?"));

    /* Dump */
    translator.insert(pair<string, string>("倒", "Dump" ));
    translator.insert(pair<string, string>("你要倒那天的菜？", "In Stock" ));
    translator.insert(pair<string, string>("倒幾箱？", "Amount" ));
    translator.insert(pair<string, string>("全部", "All" ));
    translator.insert(pair<string, string>("不夠數量倒!", "Not Enough to Dump!" ));
    translator.insert(pair<string, string>("你確定你要倒掉全部嗎?",
                                           "Are You Sure You Want to Dump All?" ));

    /* Delete/Undo History */
    translator.insert(pair<string, string>("你要打勾所有的選項才能undo!",
                                "You Must Check All Boxes to Undo!" ));


    translator.insert(pair<string, string>("你確定你要刪掉全部嗎?",
                                           "Are You Sure You Want to Delete All?" ));
    translator.insert(pair<string, string>("刪掉歷史", "Delete History" ));
    translator.insert(pair<string, string>("刪掉多少？", "Delete How Many?" ));
    translator.insert(pair<string, string>("不夠數量刪除!", "Not Enough to Delete!" ));

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
