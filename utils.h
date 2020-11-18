#ifndef UTILS_H
#define UTILS_H

#include <string>

using namespace std;

class Utils
{
public:
    Utils();
    string doubleToString(double theDouble);
    string twoDecimals(string theString);
    bool hasEnding (string const &fullString, string const &ending);
};

#endif // UTILS_H
