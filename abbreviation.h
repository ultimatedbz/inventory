#ifndef ABBREVIATION_H
#define ABBREVIATION_H

#include <map>
#include <string>

using namespace std;

class Abbreviation
{
public:
    Abbreviation();
    ~Abbreviation();
    string shrink(string input);
    void add(string key, string value);
    void remove(string input);
    int keyCount();
    map<string, string> getAbbs();
private:
    map<string, string> abbreviations;
};

#endif // ABBREVIATION_H
