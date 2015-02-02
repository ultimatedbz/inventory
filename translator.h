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
    string translate(string input, int language);

private:
    int currentLanguage;
    map<string, string> translator;
};

#endif // TRANSLATOR

