#include "dialog.h"
#include "utils.h"
#include <iostream>
using namespace std;

void Dialog::printInventory(){
    QPrinter printer;
    QPrintPreviewDialog preview (&printer, this);
    connect( &preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printI(QPrinter*)) );
    preview.exec();
}

void Dialog::printTransactions(){
    QPrinter printer;
    QPrintPreviewDialog preview (&printer, this);
    connect( &preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printT(QPrinter*)) );
    preview.exec();
}

void Dialog::printHistory(){
    QPrinter printer;
    QPrintPreviewDialog preview (&printer, this);
    connect( &preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printH(QPrinter*)) );
    preview.exec();

}

/// Print
static int dateStartX = 600;
static int dateWidth = 100;
static int pageCenterX = 400;
static int leftPageStartX = 10;
static int rightPageStartX = pageCenterX + 10;
static int textWidth = 380;
static int pageHeight = 1000;

void Dialog::printT(QPrinter* printer){
    int amount;
    time_t t = time(nullptr);
    struct tm * now = localtime(&t);
    char buffer[128];
    sprintf(buffer, "%d/%d/%d", now->tm_mon+1, now->tm_mday, now->tm_year - 100);
    string today = buffer;

    QDialog dialog1(this);
    dialog1.setWindowTitle(mTranslator ->translate("印 記錄表").c_str());
    QFormLayout form(&dialog1);

    QLineEdit *lineEdit = new QLineEdit(&dialog1);
    QString label = QString(mTranslator ->translate("請輸入字體大小").c_str());
    lineEdit -> setText("14");
    form.addRow(label, lineEdit);


    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog1);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog1, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog1, SLOT(reject()));

    if (dialog1.exec() == QDialog::Accepted) {
        amount = lineEdit->text().toInt();
        QPainter painter;
        QFont font("Courier",-1,QFont::Bold,false);

        painter.begin(printer);

        font.setPixelSize(amount);
        painter.setFont(font);
        int lineHeight = painter.fontMetrics().height();
        int topMargin = 3 * lineHeight;
        QString leftText= "";
        QString rightText="";
        QString * currentText = &leftText;
        painter.drawLine(pageCenterX,topMargin,pageCenterX,pageHeight);
        painter.drawText(dateStartX,lineHeight,dateWidth,lineHeight,Qt::AlignRight|Qt::AlignTop, QString(today.c_str()) );
        int lineCount = 0;
        int column = 0;
        for(int i = 0; i < inventory->getVegNum(); i++){
            if(inventory->getVegetableByIndex(i)->getRemainingNum() ||
                    inventory->getVegetableByIndex(i)->hasInteraction()){

                /* Need to calculate first to see if line limit will be reached */
                inventory->getVegetableByIndex(i)->formatTransaction(*mAbbreviator);
                int additionalLines = inventory->
                        getVegetableByIndex(i)->getMTransNum();
                if( lineHeight * (lineCount + (2 + additionalLines)) + topMargin > pageHeight  ){
                    if(currentText == &leftText){
                        currentText = &rightText;
                        lineCount = 0;
                        column = 1;
                        painter.drawLine(pageCenterX, topMargin, pageCenterX, pageHeight);
                    }else{
                        painter.drawText(leftPageStartX,topMargin,textWidth,pageHeight,Qt::AlignLeft|Qt::AlignTop, leftText );
                        painter.drawText(rightPageStartX, topMargin,textWidth,pageHeight,Qt::AlignLeft|Qt::AlignTop, rightText );
                        leftText= "";
                        rightText="";
                        printer->newPage();

                        painter.drawLine(pageCenterX,topMargin,pageCenterX,pageHeight);

                        painter.drawText(dateStartX,lineHeight,dateWidth,lineHeight,Qt::AlignRight|Qt::AlignTop, QString(today.c_str()) );
                        currentText = &leftText;
                        column = 0;
                        lineCount = 0;
                    }
                }

                *currentText = *currentText + QString(inventory->
                                                      getVegetableByIndex(i)->
                                                      getVegetablename().c_str()) + ":  "+ "\n";
                *currentText = *currentText +"Total: " + QString::number(inventory->getVegetableByIndex(i)
                                                                         ->getTotalVeges())+ " " + QString(inventory->getVegetableByIndex(i)->
                                                                                                           getUnit().c_str()) +"\n";

                *currentText = *currentText + QString(inventory->getVegetableByIndex(i)
                                                      ->formatTransaction(*mAbbreviator).c_str());

                lineCount += 2;
                lineCount += additionalLines; // Lines from transactions
                painter.drawLine(pageCenterX * column,lineHeight * (lineCount) + topMargin,
                                 pageCenterX * (column + 1),lineHeight * (lineCount) + topMargin);

            }else{
                if( lineHeight * (lineCount + 1) + topMargin > pageHeight  ){
                    if(currentText == &leftText){
                        currentText = &rightText;
                        lineCount = 0;
                        column = 1;
                        painter.drawLine(pageCenterX,topMargin,pageCenterX,pageHeight);
                    }else{
                        painter.drawText(leftPageStartX,topMargin,textWidth,pageHeight,Qt::AlignLeft|Qt::AlignTop, leftText );
                        painter.drawText(rightPageStartX,topMargin,textWidth,pageHeight,Qt::AlignLeft|Qt::AlignTop, rightText );
                        leftText= "";
                        rightText="";
                        printer->newPage();

                        painter.drawLine(pageCenterX,topMargin,pageCenterX,pageHeight);

                        painter.drawText(dateStartX,lineHeight,dateWidth,lineHeight,Qt::AlignRight|Qt::AlignTop, QString(today.c_str()) );
                        currentText = &leftText;
                        column = 0;
                        lineCount = 0;
                    }
                }

                *currentText = *currentText + QString(inventory->
                                                      getVegetableByIndex(i)->
                                                      getVegetablename().c_str()) + ":  "+ QString::number(inventory->getVegetableByIndex(i)
                                                                                                           ->getTotalVeges())+ " " + QString(inventory->getVegetableByIndex(i)->
                                                                                                                                             getUnit().c_str()) +"\n";
                lineCount += 1;
                painter.drawLine(pageCenterX * column,lineHeight * (lineCount) + topMargin,
                                 pageCenterX * (column + 1),lineHeight * (lineCount) + topMargin);
            }
        }
        painter.drawText(leftPageStartX,topMargin,textWidth,pageHeight,Qt::AlignLeft|Qt::AlignTop, leftText );
        painter.drawText(rightPageStartX,topMargin,textWidth,pageHeight,Qt::AlignLeft|Qt::AlignTop, rightText );
        painter.end();
    }
}

void Dialog::printI(QPrinter* printer){
    int amount;
    time_t t = time(nullptr);
    struct tm * now = localtime(&t);
    char buffer[128];
    sprintf(buffer, "%d/%d/%d", now->tm_mon+1, now->tm_mday, now->tm_year - 100);
    string today = buffer;

    QDialog dialog1(this);
    dialog1.setWindowTitle("列印資料");
    QFormLayout form(&dialog1);

    QLineEdit *lineEdit = new QLineEdit(&dialog1);
    QString label = QString(mTranslator ->translate("請輸入字體大小").c_str());
    lineEdit -> setText("18");
    form.addRow(label, lineEdit);


    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog1);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog1, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog1, SLOT(reject()));

    if (dialog1.exec() == QDialog::Accepted) {
        amount = lineEdit->text().toInt();
        QPainter painter;
        QFont font("Courier",-1,QFont::Bold,false);

        painter.begin(printer);

        font.setPixelSize(amount);
        painter.setFont(font);
        QFontMetrics metric(font);
        int lineHeight = painter.fontMetrics().height();
        int topMargin = 3 * lineHeight;
        QString leftText= "";
        QString rightText="";
        QString* currentText = &leftText;
        painter.drawLine(pageCenterX,topMargin,pageCenterX,pageHeight);
        painter.drawText(dateStartX,lineHeight,dateWidth,lineHeight,Qt::AlignRight|Qt::AlignTop, QString(today.c_str()) );
        int lineCount = 0;
        int column = 0;
        for(int i = 0; i<inventory->getVegNum(); i++){
            if(inventory->getVegetableByIndex(i)->getRemainingNum()){
                int additionalLines = lineCount + (2 + inventory->getVegetableByIndex(i)->getRemainingNum());
                if (lineHeight * additionalLines + topMargin  > pageHeight) {
                    if(currentText == &leftText){
                        currentText = &rightText;
                        lineCount = 0;
                        column = 1;
                        painter.drawLine(pageCenterX, topMargin,pageCenterX, pageHeight);
                    }else{
                        painter.drawText(leftPageStartX,topMargin,textWidth,pageHeight,Qt::AlignLeft|Qt::AlignTop, leftText );
                        painter.drawText(rightPageStartX,topMargin,textWidth,pageHeight,Qt::AlignLeft|Qt::AlignTop, rightText );
                        leftText = "";
                        rightText = "";
                        printer->newPage();

                        painter.drawLine(pageCenterX,topMargin,pageCenterX,pageHeight);

                        painter.drawText(dateStartX,lineHeight,dateWidth,lineHeight,Qt::AlignRight|Qt::AlignTop, QString(today.c_str()) );
                        currentText = &leftText;
                        column = 0;
                        lineCount = 0;
                    }
                }

                painter.drawLine(pageCenterX * column, lineHeight * lineCount + topMargin,
                                 pageCenterX *(column + 1), lineHeight * lineCount + topMargin);

                *currentText = *currentText + QString(inventory->
                                                      getVegetableByIndex(i)->
                                                      getVegetablename().c_str()) + ":  "+ "\n";
                *currentText = *currentText +"Total: " + QString::number(inventory->getVegetableByIndex(i)
                                                                         ->getTotalVeges())+ " " + QString(inventory->getVegetableByIndex(i)->
                                                                                                           getUnit().c_str()) +"\n";
                for(int j =0; j < inventory->getVegetableByIndex(i)
                    ->getRemainingNum(); j++) {
                    // This prints the line item
                    *currentText = *currentText + QString(inventory->getVegetableByIndex(i)
                                                          -> formatRemaining3(j, *mAbbreviator).c_str()) +"\n";
                    lineCount++;
                }

                lineCount += 2;
/*                painter.drawLine(pageCenterX * column, lineHeight * lineCount + topMargin,
                                 pageCenterX *(column + 1), lineHeight * lineCount + topMargin);
*/
            }else{
                if( lineHeight * (lineCount + 1) + topMargin > pageHeight  ){
                    if(currentText == &leftText){
                        currentText = &rightText;
                        lineCount = 0;
                        column = 1;
                        painter.drawLine(pageCenterX,topMargin,pageCenterX,pageHeight);
                    }else{
                        painter.drawText(leftPageStartX, topMargin,rightPageStartX,pageHeight,Qt::AlignLeft|Qt::AlignTop, leftText );
                        painter.drawText(rightPageStartX, topMargin,rightPageStartX,pageHeight,Qt::AlignLeft|Qt::AlignTop, rightText );
                        leftText= "";
                        rightText="";
                        printer->newPage();

                        painter.drawLine(pageCenterX,topMargin,pageCenterX,pageHeight);

                        painter.drawText(dateStartX, lineHeight,dateWidth,lineHeight,Qt::AlignRight|Qt::AlignTop, QString(today.c_str()) );
                        currentText = &leftText;
                        column = 0;
                        lineCount = 0;
                    }
                }

                painter.drawLine(pageCenterX * column,lineHeight * lineCount + topMargin,
                                 pageCenterX * (column + 1),lineHeight * lineCount + topMargin);
                *currentText = *currentText + QString(inventory->
                                                      getVegetableByIndex(i)->
                                                      getVegetablename().c_str()) + ":  "+ QString::number(inventory->getVegetableByIndex(i)
                                                                                                           ->getTotalVeges())+ " " + QString(inventory->getVegetableByIndex(i)->
                                                                                                                                             getUnit().c_str()) +"\n";
                lineCount += 1;
            }

        }
        painter.drawText(leftPageStartX, topMargin, rightPageStartX,pageHeight,Qt::AlignLeft|Qt::AlignTop, leftText );
        painter.drawText(rightPageStartX, topMargin, rightPageStartX,pageHeight,Qt::AlignLeft|Qt::AlignTop, rightText );
        painter.end();
    }
}

void Dialog::printH(QPrinter * printer){

    //qdialog what day do you want to print? have a drop down box with all the available days
    string dayComp;
    int amount;
    time_t t = time(nullptr);
    struct tm * now = localtime(&t);
    char buffer[128];
    sprintf(buffer, "%d/%d/%d", now->tm_mon+1, now->tm_mday, now->tm_year - 100);
    string today = buffer;

    QDialog dialog1(this);
    dialog1.setWindowTitle(mTranslator ->translate("印 History").c_str());
    QFormLayout form(&dialog1);

    QLineEdit *l2 = new QLineEdit(&dialog1);
    QString label2 = QString(mTranslator ->translate("你要列印哪一天的日期?").c_str());
    l2 -> setText(today.c_str());
    form.addRow(label2,l2);

    QLineEdit *lineEdit = new QLineEdit(&dialog1);
    QString label = QString(mTranslator ->translate("請輸入字體大小").c_str());
    lineEdit -> setText("12");
    form.addRow(label, lineEdit);


    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog1);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog1, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog1, SLOT(reject()));

    if (dialog1.exec() == QDialog::Accepted) {
        amount = lineEdit->text().toInt();
        dayComp = l2->text().toUtf8().constData();
        QPainter painter;
        QFont font("Courier",-1,QFont::Bold,false);

        painter.begin(printer);

        font.setPixelSize(amount);
        painter.setFont(font);
        QFontMetrics metric(font);
        int lineHeight = metric.height();
        QString leftText= "";
        QString rightText="";
        QString * currentText = &leftText;
        int lineCount = 0;
        int topMargin = 3 * lineHeight;
        int column = 0;
        painter.drawLine(pageCenterX,topMargin,pageCenterX,pageHeight);
        painter.drawText(dateStartX,lineHeight,dateWidth,lineHeight,Qt::AlignRight|Qt::AlignTop, QString(dayComp.c_str()) );

        for(int i = 0; i<inventory->getVegNum(); i++){
            if(inventory->getVegetableByIndex(i)->getHistoryNum()){

                //make array of all histories that were from today using date to compare
                int historyNum = inventory->getVegetableByIndex(i)->getHistoryNum();
                History* temp = new History[historyNum];
                int index = 0;
                for(int j = 0; j< historyNum; j++){
                    if(!(inventory->getVegetableByIndex(i)->getHistoryObject(j)->getDateToCompare()
                         .compare(dayComp))){
                        temp[index] = *inventory->getVegetableByIndex(i)->getHistoryObject(j);
                        index++;
                    }
                }
                int sellNum = 0; int buyNum = 0; int returnNum = 0; int dumpNum = 0; int tuiNum = 0;
                //make 4 arrays, one for each type
                History* sell = new History[historyNum];
                History* buy = new History[historyNum];
                History* returnn = new History [historyNum];
                History* dump = new History [historyNum];
                History* tui = new History [historyNum];

                for (int j = 0; j<index; j++){
                    if(temp[j].getType() == "Sell"){
                        sell[sellNum] = temp[j];
                        sellNum++;
                    }

                    //sort by customer
                    sort(sell, sell + sellNum, compareCustomer);
                }

                for (int j = 0; j<index; j++){
                    if (temp[j].getType() == "Return"){
                        returnn[returnNum] = temp[j];
                        returnNum++;
                    }
                }

                for (int j = 0; j<index; j++){
                    if (temp[j].getType() == "Dump"){
                        dump[dumpNum] = temp[j];
                        dumpNum++;
                    }
                }

                for (int j = 0; j<index; j++){
                    if (temp[j].getType() == "Tui"){
                        tui[tuiNum] = temp[j];
                        tuiNum++;
                    }
                }

                for (int j = 0; j<index; j++){
                    if (temp[j].getType() == "Buy"){
                        buy[buyNum] = temp[j];
                        buyNum++;
                    }

                    //sort by company
                    sort(buy, buy + buyNum, compareCompany);

                }

                if(sellNum || buyNum || returnNum || dumpNum ||tuiNum){
                    //checks if the next block to print fits on the page
                    int first = 0;
                    if(buyNum)
                        first = buyNum;
                    else if(sellNum)
                        first = sellNum;
                    else if(returnNum)
                        first = returnNum;
                    else if(dumpNum)
                        first = dumpNum;
                    else
                        first = tuiNum;

                    if( lineHeight * (lineCount + 2 + first) + topMargin > pageHeight  ){
                        if(currentText == &leftText){
                            currentText = &rightText;
                            lineCount = 0;
                            column = 1;
                        }else{
                            lineCount = 0;
                            column = 0;
                            painter.drawText(leftPageStartX,topMargin,rightPageStartX,pageHeight,Qt::AlignLeft|Qt::AlignTop, leftText );
                            painter.drawText(rightPageStartX, topMargin, rightPageStartX,pageHeight,Qt::AlignLeft|Qt::AlignTop, rightText );
                            leftText= "";
                            rightText="";
                            printer->newPage();
                            painter.drawLine(pageCenterX,topMargin,pageCenterX,pageHeight);
                            painter.drawText(dateStartX, lineHeight,dateWidth,lineHeight,Qt::AlignRight|Qt::AlignTop, dayComp.c_str() );
                            currentText = &leftText;
                        }
                    }

                    *currentText = *currentText + QString(inventory->
                                                          getVegetableByIndex(i)->
                                                          getVegetablename().c_str()) + ":  "+
                            QString(inventory->getVegetableByIndex(i)->
                                    getUnit().c_str()) +"\n";
                    lineCount++;

                }


                if(buyNum){
                    painter.drawLine(pageCenterX * column + 60,lineHeight *( lineCount) + topMargin,
                                     pageCenterX *(column + 1),lineHeight * ( lineCount) + topMargin);
                    *currentText = *currentText + "   買:\n";
                    lineCount++;
                    painter.drawLine(pageCenterX * column + 60,lineHeight *( lineCount) + topMargin,
                                     pageCenterX *(column + 1),lineHeight * ( lineCount) + topMargin);
                    for(int j =0; j < buyNum; j++){
                        *currentText = *currentText + QString(buy[j].printFormat().c_str()) +"\n";
                        lineCount++;
                    }

                }
                if(sellNum){
                    if( lineHeight * (lineCount + sellNum) + topMargin > pageHeight  ){
                        if(currentText == &leftText){
                            currentText = &rightText;
                            lineCount = 0;
                            column = 1;
                        }else{
                            lineCount = 0;
                            column = 0;
                            painter.drawText(leftPageStartX,topMargin,rightPageStartX,pageHeight,Qt::AlignLeft|Qt::AlignTop, leftText );
                            painter.drawText(rightPageStartX,topMargin,rightPageStartX,pageHeight,Qt::AlignLeft|Qt::AlignTop, rightText );
                            leftText= "";
                            rightText="";
                            printer->newPage();

                            painter.drawLine(pageCenterX,topMargin,pageCenterX,pageHeight);
                            painter.drawText(dateStartX,lineHeight,dateWidth,lineHeight,Qt::AlignRight|Qt::AlignTop, QString(dayComp.c_str()) );
                            currentText = &leftText;
                        }
                    }
                    painter.drawLine(pageCenterX * column + 60,lineHeight *( lineCount) + topMargin,
                                     pageCenterX *(column + 1),lineHeight * ( lineCount) + topMargin);
                    *currentText = *currentText + "   賣:\n";
                    lineCount++;
                    painter.drawLine(pageCenterX * column + 60,lineHeight *( lineCount) + topMargin,
                                     pageCenterX *(column + 1),lineHeight * ( lineCount) + topMargin);
                    for(int j =0; j < sellNum; j++){
                        *currentText = *currentText + QString(sell[j].printFormat().c_str()) +"\n";
                        lineCount++;
                    }
                }

                if(returnNum){

                    if( lineHeight * (lineCount + returnNum) + topMargin > pageHeight  ){
                        if(currentText == &leftText){
                            column = 1;
                            currentText = &rightText;
                            lineCount = 0;
                        }else{
                            lineCount = 0;
                            column = 0;
                            painter.drawText(leftPageStartX,topMargin,rightPageStartX,pageHeight,Qt::AlignLeft|Qt::AlignTop, leftText );
                            painter.drawText(rightPageStartX,topMargin,rightPageStartX,pageHeight,Qt::AlignLeft|Qt::AlignTop, rightText );
                            leftText= "";
                            rightText="";
                            printer->newPage();

                            painter.drawLine(pageCenterX,topMargin,pageCenterX,pageHeight);
                            painter.drawText(dateStartX,lineHeight,dateWidth,lineHeight,Qt::AlignRight|Qt::AlignTop, QString(dayComp.c_str()) );
                            currentText = &leftText;
                        }
                    }
                    painter.drawLine(rightPageStartX * column + 60,lineHeight *( lineCount) + topMargin,
                                     rightPageStartX *(column + 1),lineHeight * ( lineCount) + topMargin);
                    *currentText = *currentText + "   退給公司 :\n";
                    lineCount++;
                    painter.drawLine(pageCenterX * column + 60,lineHeight *( lineCount) + topMargin,
                                     pageCenterX *(column + 1),lineHeight * ( lineCount) + topMargin);
                    for(int j =0; j < returnNum; j++){
                        *currentText = *currentText + QString(returnn[j].printFormat().c_str()) +"\n";
                        lineCount++;
                    }


                }

                if(dumpNum){
                    if( lineHeight * (lineCount + dumpNum) + topMargin > pageHeight  ){
                        if(currentText == &leftText){
                            column = 1;
                            currentText = &rightText;
                            lineCount = 0;
                        }else{
                            lineCount = 0;
                            column = 0;
                            painter.drawText(leftPageStartX,topMargin,rightPageStartX,pageHeight,Qt::AlignLeft|Qt::AlignTop, leftText );
                            painter.drawText(rightPageStartX,topMargin,rightPageStartX,pageHeight,Qt::AlignLeft|Qt::AlignTop, rightText );
                            leftText= "";
                            rightText="";
                            printer->newPage();

                            painter.drawLine(pageCenterX,topMargin,pageCenterX,pageHeight);
                            painter.drawText(rightPageStartX,topMargin,dateWidth,lineHeight,Qt::AlignRight|Qt::AlignTop, QString(dayComp.c_str()) );
                            currentText = &leftText;
                        }
                    }
                    painter.drawLine(pageCenterX * column + 60,lineHeight *( lineCount) + topMargin,
                                     pageCenterX *(column + 1),lineHeight * ( lineCount) + topMargin);
                    *currentText = *currentText + "   倒:\n";
                    lineCount++;
                    painter.drawLine(pageCenterX * column + 60,lineHeight *( lineCount) + topMargin,
                                     pageCenterX *(column + 1),lineHeight * ( lineCount) + topMargin);
                    for(int j =0; j < dumpNum; j++){
                        *currentText = *currentText + QString(dump[j].printFormat().c_str()) +"\n";
                        lineCount++;
                    }

                }

                if (tuiNum){
                    if (lineHeight * (lineCount + tuiNum) + topMargin > pageHeight  ){
                        if (currentText == &leftText){
                            column = 1;
                            currentText = &rightText;
                            lineCount = 0;
                        } else {
                            lineCount = 0;
                            column = 0;
                            painter.drawText(leftPageStartX,topMargin,rightPageStartX,pageHeight,Qt::AlignLeft|Qt::AlignTop, leftText );
                            painter.drawText(rightPageStartX,topMargin,rightPageStartX,pageHeight,Qt::AlignLeft|Qt::AlignTop, rightText );

                            leftText= "";
                            rightText="";
                            printer->newPage();

                            painter.drawLine(pageCenterX,topMargin,pageCenterX,pageHeight);
                            painter.drawText(dateStartX, lineHeight,dateWidth,lineHeight,Qt::AlignRight|Qt::AlignTop, QString(dayComp.c_str()) );
                            currentText = &leftText;
                        }
                    }

                    painter.drawLine(pageCenterX * column + 60,lineHeight *( lineCount) + topMargin,
                                     pageCenterX *(column + 1),lineHeight * ( lineCount) + topMargin);
                    *currentText = *currentText + "   退給農場:\n";
                    lineCount++;
                    painter.drawLine(pageCenterX * column + 60,lineHeight *( lineCount) + topMargin,
                                     pageCenterX *(column + 1),lineHeight * ( lineCount) + topMargin);
                    for (int j =0; j < tuiNum; j++) {
                        *currentText = *currentText + QString(tui[j].printFormat().c_str()) +"\n";
                        lineCount++;
                    }
                }

                painter.drawLine(pageCenterX * column ,lineHeight * (lineCount) + topMargin,
                                 pageCenterX *(column + 1),lineHeight * (lineCount) + topMargin);
            }
        }

        painter.drawText(leftPageStartX, topMargin,rightPageStartX,pageHeight,Qt::AlignLeft|Qt::AlignTop, leftText );
        painter.drawText(rightPageStartX,topMargin,rightPageStartX,pageHeight,Qt::AlignLeft|Qt::AlignTop, rightText );
        painter.end();

    }
}
