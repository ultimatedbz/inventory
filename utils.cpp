#include "utils.h"
#include <iomanip>
#include <sstream>

Utils::Utils()
{

}

string Utils::doubleToString(double theDouble) {
    stringstream stream;
    stream << fixed << setprecision(2) << theDouble;
    return stream.str();
}

string Utils::twoDecimals(string theString) {
    stringstream stream;
    stream << fixed << setprecision(2) << stod(theString);
    return stream.str();
}
