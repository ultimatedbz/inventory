#include "translator.h"

Translator::Translator(){
    /* Menus */

    translator.insert(pair<string, string>("New File", "開新檔案"));
    translator.insert(pair<string, string>("Load File", "開啟舊檔" ));
    translator.insert(pair<string, string>("Save", "儲存檔案" ));
    translator.insert(pair<string, string>("Save As", "另存新檔" ));
    translator.insert(pair<string, string>("Add Customer", "加入新客戶" ));
    translator.insert(pair<string, string>("Add Company", "加入新公司" ));
    translator.insert(pair<string, string>("Add Vegetable", "加入新的菜名" ));
    translator.insert(pair<string, string>("Add Unit", "加入單位" ));
    translator.insert(pair<string, string>("Delete Customer", "刪除客戶" ));
    translator.insert(pair<string, string>("Delete Company", "刪除公司" ));
    translator.insert(pair<string, string>("Delete Vegetable", "刪掉菜名" ));
    translator.insert(pair<string, string>("Delete Unit", "刪掉單位" ));

    /* User Interface */
    translator.insert(pair<string, string>("Buy", "買" ));
    translator.insert(pair<string, string>("Sell", "賣" ));
    translator.insert(pair<string, string>("Vegetables", "菜名" ));
    translator.insert(pair<string, string>("Breakdown", "庫存表" ));
    translator.insert(pair<string, string>("Inventory", "庫存表" ));
    translator.insert(pair<string, string>("Dump", "倒" ));
    translator.insert(pair<string, string>("Return", "退貨" ));
    translator.insert(pair<string, string>("History", "歷史" ));
    translator.insert(pair<string, string>("Returns", "退貨單" ));
    translator.insert(pair<string, string>("Clear Memo", "清掉退貨單" ));
    translator.insert(pair<string, string>("Clear History", "清掉歷史" ));
    translator.insert(pair<string, string>("Return to Us", "退給公司" ));
    translator.insert(pair<string, string>("Return to Company", "退給農場" ));
    translator.insert(pair<string, string>("Clear Returns", "清掉退貨單" ));
    translator.insert(pair<string, string>("Print Inventory", "印 Inventory" ));
    translator.insert(pair<string, string>("Print History", "印 History" ));

    /* Menu Popups */
    translator.insert(pair<string, string>("Company Name", "請輸入你要加的新公司名字?" ));
    translator.insert(pair<string, string>("Customer Name", "請輸入你要加的新客戶名字?" ));
    translator.insert(pair<string, string>("Unit Name", "請輸入你要加的單位?" ));
    translator.insert(pair<string, string>("Vegetable Name", "請輸入你要加的菜名" ));

    translator.insert(pair<string, string>("Which Day?", "你要賣那天的菜？" ));
    translator.insert(pair<string, string>("Which Company?", "你要刪掉哪一個公司？" ));
    translator.insert(pair<string, string>("Which Customer?", "你要刪掉哪一個客戶？" ));
    translator.insert(pair<string, string>("Which Unit?", "你要刪掉哪一個單位？" ));
    translator.insert(pair<string, string>("No Units!", "沒單位!" ));
    translator.insert(pair<string, string>("Warning", "警告" ));
    translator.insert(pair<string, string>("Which Vegetable?", "你要刪掉哪一個菜名？" ));
    translator.insert(pair<string, string>("Font Size", "請輸入字體大小"));
    translator.insert(pair<string, string>("Print Which Day?", "你要列印哪一天的日期?" ));

    translator.insert(pair<string, string>("", "聯合賣" ));
    translator.insert(pair<string, string>("", "錯誤" ));
    translator.insert(pair<string, string>("Not enough to sell!", "不夠賣!" ));
    translator.insert(pair<string, string>("", "買了多少？" ));
    translator.insert(pair<string, string>("", "哪家公司的?" ));
    translator.insert(pair<string, string>("", "幾號來的菜?" ));
    translator.insert(pair<string, string>("", "進價多少?" ));
    translator.insert(pair<string, string>("", "輸入賣菜資料" ));
    translator.insert(pair<string, string>("", "賣了多少？" ));
    translator.insert(pair<string, string>("", "賣给谁？" ));
    translator.insert(pair<string, string>("", "那天賣出" ));
    translator.insert(pair<string, string>("", "賣多少錢?" ));
    translator.insert(pair<string, string>("", "你要賣那天的菜？" ));

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
    if(currentLanguage == ENGLISH)
        return input;

    return translator.find(input) -> second;
}

void Translator::changeLanguage(int i){
    currentLanguage = i;
}
