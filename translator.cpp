#include "translator.h"

Translator::Translator(){
    translator.insert(pair<string, string>("New File", "開新檔案"));
}

Translator::~Translator(){

}

string Translator::translate(string input, int language){
    if(language == ENGLISH)
        return input;

    return translator.find(input) -> second;
}
