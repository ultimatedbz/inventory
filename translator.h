#ifndef TRANSLATOR
#define TRANSLATOR

#include "IConst.h"
#include <string>
#include <map>

using namespace std;

class Translator{
public:
    Translator();
    ~Translator();
    string translate(string input);
    void setLanguage();
    void changeLanguage(int i);
    int currentLanguage;

private:
    map<string, string> translator;
};

#endif // TRANSLATOR

